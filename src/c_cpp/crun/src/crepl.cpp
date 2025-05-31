#include "crepl.h"
#include <bemapiset.h>
#include <cstdio>
#include "info.h"

#include "ProjectCommon.h"
#include "match_regex.h"
#include "prog.h"
#include "safe_exit.h"
#include "subprocess.h"

#define TEMP_SRC_FILENAME               "~temp.c"
#define CONFIGFILE			            "run.conf"
#define MaxLenStatements	            300
#define MaxCharSize			            1024

/* 这是全局变量：决定了函数RunOrNot()的定界符 */
int countBrace = 0, countBracket = 0, countParen = 0; // 落单的花括号、方括号、圆括号计数（不统计被单/双引号包围了的，已经\转义的不算单/双引号）
int OpenBraceCount = 0, CloseBraceCount = 0;		  // 左花括号计数、右花括号计数，二者相等
int OpenBracketCount = 0, CloseBracketCount = 0;	  // 左方括号计数、右方括号计数，二者相等
int OpenParenCount = 0, CloseParenCount = 0;		  // 左圆括号计数，右圆括号计数，二者相等

int determineType(const char* str){
	if (isMatch(str, R"(^\s*#\s*include\s*(<[^>]+>|"[^"]+"))")){ // 匹配 #include 预处理命令
		crun::ProjectCommon::ColorPrintf(0x09, "Lib: %s", str);
		putchar('\n');
		return 1;
	}
	else if (isMatch(str, R"(^\s*#\s*define\s+\w+(\s*\([^)]*\))?\s+.+$)")){ // 匹配 #define 宏定义
		crun::ProjectCommon::ColorPrintf(0x0D, "Macro: %s", str);
		putchar('\n');
		return 2;
	}
	else if (isMatch(str, R"(^\s*\w+(\s+\w+)?\s+\w+\s*\([^)]*\)\s*\{[^}]*\}$)")){ // 匹配 C语言函数的定义
		crun::ProjectCommon::ColorPrintf(0x0B, "Func: %s", str);
		putchar('\n');
		return 3;
	}
	else if (isMatch(str, R"(^\s*struct\s+\w+\s*\{[^}]*\}\s*;$)")){ // 匹配 C语言结构体的定义
		crun::ProjectCommon::ColorPrintf(0x0E, "Struct: %s", str);
		putchar('\n');
		return 4;
	}
	else if (isMatch(str, R"(^\s*union\s+\w+\s*\{[^}]*\}\s*;$)")){ // 匹配 C语言共用体的定义
		crun::ProjectCommon::ColorPrintf(0x0E, "Union: %s", str);
		putchar('\n');
		return 5;
	}
	else if (isMatch(str, R"(^\s*[^;]+;\s*$)")){ // 匹配 C语言普通语句
		return 6;
	}
	else{
		// printf("Invalid data!\n");
		return -1;
	}
}

int CREPL(){
	run::prog::MemoryProg testProgM;
	run::prog::SrcToELF testProgN;
	crun::ProjectCommon::ChCP(65001, 65001);
	DisplayVersion();

	testProgN.setSrcFn(TEMP_SRC_FILENAME);

	std::string config_path = crun::ProjectCommon::checkEnv();
	if (config_path.empty()){
#ifdef _WIN32
	    config_path.append("\\conf\\");
	    config_path.append(CONFIGFILE);
#else
	    config_path.append("/conf/");
        config_path.append(CONFIGFILE);
#endif
    }
    // ReadLine(configpath, &testProgM); // import config.ini

    int s = 1; // 状态
    int lines = 0;								   // 行数
	auto* command_1 = static_cast<char*>(malloc(MaxLenStatements)); // 存储普通状态的字符串
	auto* FSU = static_cast<char*>(malloc(MaxCharSize));
	char aStatement[100];

	for (unsigned int cyclecounter = 0;; cyclecounter++){
		/* 这部分：决定命令提示符的样式 */
		if (s > 0){
			crun::ProjectCommon::ColorPuts(0x0B, "C> ", 0);

			lines = 0;
			OpenBraceCount = CloseBraceCount = 0;
		}
		else if (s == -1 || s == -2 || s == -3){
			crun::ProjectCommon::ColorPrintf(0x0B, "%d| ", lines);
			for (int i = 0; i < (OpenBraceCount - CloseBraceCount); i++){
				putchar('\t');
			}
		}
		else{
			crun::ProjectCommon::ColorPuts(0x0B, "C> ", 0);
		}

		fgets(command_1, sizeof(command_1), stdin); // 获取输入的字符串
		fflush(stdin);

		/* 判断是程序特殊指令，子函数内只判断不执行 */
		bool judgRet1 = IsItSpecialCommand(command_1);
		if (judgRet1 == true){
			s = 1; // 是命令，执行完了
		}
		else{
			int judgRet2 = determineType(command_1); /*判断是否是预处理命令 */
			if (judgRet2 == 1){
				testProgM.addInc(command_1);
				s = 1;
			}
			else if (judgRet2 == 2){
				testProgM.addDef(command_1);
				s = 1;
			}
			else if (judgRet2 == 3){
				testProgM.addFunc(command_1);
			}
			else if (judgRet2 == 4 || judgRet2 == 5){
				testProgM.addStructUnion(command_1);
			}
			else if (judgRet2 == 6){
				testProgM.addMainStatement(command_1);
				testProgM.writeToFile(TEMP_SRC_FILENAME);
				testProgN.CompileAssemble();
				testProgN.Link();
				ChildProcess* child = new ChildProcess(testProgN.getELF(), NULL, false);
				child->execSubprog();
				delete child;
			}
			else{ // 不是特殊指令，也不是预处理命令
				int olds = s;
				s = RunOrNot(command_1);
				if (s == -1){ // 花括号未闭合
					if (olds == -1 || olds == -2 || olds == -3){
						strcat(FSU, command_1);
					}
					else{
						strcpy(FSU, command_1); // 开始定义函数
					}
				}
				else if (s == -2){ // 未写完的普通语句
					if (olds == -1 || olds == -2 || olds == -3){
						strcat(aStatement, command_1);
					}
					else{
						// aStatement[0] = '\0';
						strcpy(aStatement, command_1); // 开始写普通语句
					}
				}
				else if (s == -3){ // 未以分号结尾的结构体或共用体（联合体）
					if (olds == -1 || olds == -2 || olds == -3){
						strcat(FSU, command_1);
					}
					else{
						strcpy(FSU, command_1); // 开始定义结构体或联合体
					}
				}
				else if (s == 1){ // 普通语句写完了
					if (olds == -1 || olds == -2 || olds == -3){
						strcat(aStatement, command_1);
					}
					else{
						strcpy(aStatement, command_1);
					}
					testProgM.addMainStatement(aStatement);
					testProgM.writeToFile(TEMP_SRC_FILENAME);
					testProgN.CompileAssemble();
					testProgN.Link();
					ChildProcess* child = new ChildProcess(testProgN.getELF(), NULL, false);
					child->execSubprog();
					delete child;
				}
				else if (s == 2){ // 结束函数、结构体、共用体的定义
					strcat(FSU, command_1);
					crun::ProjectCommon::ColorPrintf(0x0B, "%s\n", FSU);
					testProgM.addFunc(FSU);
				}
				else if (s == 3){ // 结束函数的定义（不是结构体或共用体）
					strcat(FSU, command_1);
					crun::ProjectCommon::ColorPrintf(0x0B, "%s\n", FSU);
					testProgM.addFunc(FSU);
				}
				else if (s == (-99)){
					crun::ProjectCommon::ColorPuts(0x8 | 0x4, "错误！出现落单的“}”", 1);
				}
				else{
					// 预留
				}
			}
		} // 是特殊命令吗？

		if (s == -1 || s == -2 || s == -3){
			lines++;
		}
	}
	// free(command_1); // 释放内存
	remove(TEMP_SRC_FILENAME);
	remove("~temp.exe");

	crun::ProjectCommon::ChCP(0, 0);
	return 0;
}

bool IsItSpecialCommand(const char* str){
	if (isMatch(str, R"(^\s*\.(exit|quit)\s*$)")) {
		crun::ProjectCommon::ColorPuts(0x0B, "Bye!", 1);
		SafeExit(0);
	}
	else if (isMatch(str, R"(^\s*\.undo\s*$)")) {
		printf("Undo...\n");
	}
	else if (isMatch(str, R"(^\s*\.(system|shell|cmd)\s{1,}.{1,}$)")) { // 执行系统命令
		char systemCmd[MaxCharSize];
		strcpy(systemCmd, str + 5);
		printf("CMD: %s...\n", systemCmd);
		system(systemCmd);
	}
	else if (isMatch(str, R"(^\s*\.renew\s*$)")){
		// remove( FULL_SRC );
		remove("~temp.exe");
		system("cls");
		printf("-- renew --\n");
    }
    else if (isMatch(str, R"(^\s*\.(help|man)\s*$)")){
		DisplayManual();
	}
	else if(isMatch(str, R"(^\s*\.(cls|clear)\s*$)")) {
		system("cls");
	}
	else if (isMatch(str, R"(^\s*\..{1,}$)")) {
		crun::ProjectCommon::ColorPuts(0x04, "Invalid command!", 1);
	}
	else if (isMatch(str, R"(^\s*$)")) { // 输入的都是空格
	}
	else{
		return false;
	}
	return true;
}

int RunOrNot(const char* str){
	const int closeBraceNotOpened = -99; // -C

	const int finiNormStat = 1; // C
	const int endFSU_Semi = 2;	// C
	const int endF_NoSemi = 3;	// C

	const int openBraceNotClosed = -1; //|
	const int unfiniNormStat = -2;	   //|
	const int endSU_NoSemi = -3;	   //|

	int isPoundFirst = 0, isSemicolonLast = 0; // #为首？ ;为尾？
	int isInQuotes = 0, isInSingleQuotes = 0;

	while (*str != '\0'){
		if (*str == '\"' && !isInSingleQuotes){
			isInQuotes = !isInQuotes;
		}
		else if (*str == '\'' && !isInQuotes){
			isInSingleQuotes = !isInSingleQuotes;
		}

		if (!isInQuotes && !isInSingleQuotes){
			if (*str == '{')
				OpenBraceCount++; // countBrace++;
			else if (*str == '}')
				CloseBraceCount++;
			else if (*str == '[')
				OpenBracketCount++; // countBracket++;
			else if (*str == ']')
				CloseBracketCount++;
			else if (*str == '(')
				OpenParenCount++;
			else if (*str == ')')
				CloseParenCount++;

			if (*str == ';' && *(str + 1) == '\0')
				isSemicolonLast = 1;
		}

		str++;
	}

	if (OpenBraceCount > CloseBraceCount){ // 在结构体/共用体/函数中  --> |
		return openBraceNotClosed;
	}
	else if (OpenBraceCount == CloseBraceCount){
		if (OpenBraceCount == 0){ // 在花括号之外，普通语句，请判断是否使用了分号
			if (isSemicolonLast == 1){ // 使用了分号 --> C
				return finiNormStat;
			}
			else{ // 没有使用分号 --> |
				return unfiniNormStat;
			}
		}
		else{ // 在花括号之外，刚结束花括号，请判断是否使用了分号
			if (isSemicolonLast == 1){ // 使用了分号 --> C
				return endFSU_Semi;
			}
			else{ // 没有使用分号，请判断刚才结束定义的是函数，还是结构体/共用体
				if (OpenParenCount != 0){ // 是函数，*******************************************判断条件有误
					return endF_NoSemi;
				}
				else{ // 是结构体/共用体
					return endSU_NoSemi;
				}
			}
		}
	}
	else{ // OpenBraceCount < CloseBraceCount
		return closeBraceNotOpened;
	}
}
