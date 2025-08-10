#include "repl.h"
#include <iostream>
#include <string>
#include <vector>
#include <bemapiset.h>
#include <cstdio>
#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif

#include "proj_info.h"
#include "match_regex.h"
#include "subprocess.h"

#define D_TMP_SRC_FILENAME    			"~temp.c"
#define CONFIG_FILENAME			        "cpro_repl.config"
#define MAX_LEN_STMTS	            	300
#define MAX_CHAR_SIZE			        1024


/*************** 全局变量与对象 *********************/

/* 全局变量：决定了函数run_or_not()的定界符 */
int count_brace = 0, countBracket = 0, countParen = 0; // 落单的花括号、方括号、圆括号计数（不统计被单/双引号包围了的，已经\转义的不算单/双引号）
int open_brace_count = 0, close_brace_count = 0;		  // 左花括号计数、右花括号计数，二者相等
int open_bracket_count = 0, close_bracket_count = 0;	  // 左方括号计数、右方括号计数，二者相等
int open_paren_count = 0, close_paren_count = 0;		  // 左圆括号计数，右圆括号计数，二者相等

int handle_cmd_repl(const int argc, char* argv[]){
	cpro::MemoryProg mem_prog;
	cpro::MakeProg make_prog;

	cpro::print_version();

	make_prog.set_src_fn(D_TMP_SRC_FILENAME);

	std::string config_path = cpro::proj_info.hcheck_env();
	if (config_path.empty()){
#ifdef _WIN32
	    config_path.append("\\conf\\");
	    config_path.append(CONFIG_FILENAME);
#else
	    config_path.append("/conf/");
        config_path.append(CONFIG_FILENAME);
#endif
    }
    // read_line(configpath, &mem_prog); // import cpro_repl_config.ini

    int s = 1; // 状态
    int lines = 0;								   // 行数
	auto* command_1 = static_cast<char*>(malloc(MAX_LEN_STMTS)); // 存储普通状态的字符串
	auto* f_s_u = static_cast<char*>(malloc(MAX_CHAR_SIZE));
	char aStatement[100];

	for (unsigned int cyclecounter = 0;; cyclecounter++){
		/* 这部分：决定命令提示符的样式 */
		if (s > 0){
			binhub_cprintf((binhub_tcolor_t){CLR_BRT_CYAN, CLR_NONE}, "C> ", 0);

			lines = 0;
			open_brace_count = close_brace_count = 0;
		}
		else if (s == -1 || s == -2 || s == -3){
			binhub_cprintf((binhub_tcolor_t){CLR_BRT_CYAN, CLR_NONE}, "%d| ", lines);
			for (int i = 0; i < (open_brace_count - close_brace_count); i++){
				putchar('\t');
			}
		} else {
			binhub_cprintf((binhub_tcolor_t){CLR_BRT_CYAN, CLR_NONE}, "C> ", 0);
		}

		fgets(command_1, sizeof(command_1), stdin); // 获取输入的字符串
		fflush(stdin);

		/* 判断是程序特殊指令，子函数内只判断不执行 */
		bool judgRet1 = is_special_command(command_1);
		if (judgRet1 == true){
			s = 1; // 是命令，执行完了
		} else {
			int judgRet2 = determine_type(command_1); /*判断是否是预处理命令 */
			if (judgRet2 == 1){
				mem_prog.include_lib(command_1);
				s = 1;
			}
			else if (judgRet2 == 2){
				mem_prog.define_macro(command_1);
				s = 1;
			}
			else if (judgRet2 == 3){
				mem_prog.define_func(command_1);
			}
			else if (judgRet2 == 4 || judgRet2 == 5){
				mem_prog.define_type(command_1);
			}
			else if (judgRet2 == 6){
				mem_prog.append_main_stmt(command_1);
				mem_prog.write_to_file(D_TMP_SRC_FILENAME);
				make_prog.compile_and_assemble();
				make_prog.link();
				cpro::Subprocess subprocess(make_prog.get_exe_fn(), NULL, false);
				subprocess.exec();
			}
			else { // 不是特殊指令，也不是预处理命令
				int olds = s;
				s = run_or_not(command_1);
				if (s == -1){ // 花括号未闭合
					if (olds == -1 || olds == -2 || olds == -3){
						strcat(f_s_u, command_1);
					}
					else {
						strcpy(f_s_u, command_1); // 开始定义函数
					}
				}
				else if (s == -2){ // 未写完的普通语句
					if (olds == -1 || olds == -2 || olds == -3){
						strcat(aStatement, command_1);
					}
					else {
						// aStatement[0] = '\0';
						strcpy(aStatement, command_1); // 开始写普通语句
					}
				}
				else if (s == -3){ // 未以分号结尾的结构体或共用体（联合体）
					if (olds == -1 || olds == -2 || olds == -3){
						strcat(f_s_u, command_1);
					}
					else {
						strcpy(f_s_u, command_1); // 开始定义结构体或联合体
					}
				}
				else if (s == 1){ // 普通语句写完了
					if (olds == -1 || olds == -2 || olds == -3){
						strcat(aStatement, command_1);
					}
					else {
						strcpy(aStatement, command_1);
					}
					mem_prog.append_main_stmt(aStatement);
					mem_prog.write_to_file(D_TMP_SRC_FILENAME);
					make_prog.compile_and_assemble();
					make_prog.link();
					Subprocess subprocess(make_prog.get_exe_fn(), NULL, false);
					subprocess.exec();
				}
				else if (s == 2){ // 结束函数、结构体、共用体的定义
					strcat(f_s_u, command_1);
					binhub_cprintf((binhub_tcolor_t){CLR_BRT_CYAN, CLR_NONE}, "%s\n", f_s_u);
					mem_prog.define_func(f_s_u);
				}
				else if (s == 3){ // 结束函数的定义（不是结构体或共用体）
					strcat(f_s_u, command_1);
					binhub_cprintf((binhub_tcolor_t){CLR_BRT_CYAN, CLR_NONE}, "%s\n", f_s_u);
					mem_prog.define_func(f_s_u);
				}
				else if (s == (-99)){
					binhub_cprintf((binhub_tcolor_t){CLR_BRT_RED, CLR_NONE}, "错误！出现落单的“}”"); BINHUB_ENDLINE(1);
				}
				else {
					// 预留
				}
			}
		} // 是特殊命令吗？

		if (s == -1 || s == -2 || s == -3){
			lines++;
		}
	}
	// free(command_1); // 释放内存


	return 0;
}

int run_or_not(const char* str){
	const int closeBraceNotOpened = -99; // -C

	const int finiNormStat = 1; // C
	const int end_fsu_semi = 2;	// C
	const int end_f_nosemi = 3;	// C

	const int openBraceNotClosed = -1; //|
	const int unfiniNormStat = -2;	   //|
	const int end_su_nosemi = -3;	   //|

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
				open_brace_count++; // count_brace++;
			else if (*str == '}')
				close_brace_count++;
			else if (*str == '[')
				open_bracket_count++; // countBracket++;
			else if (*str == ']')
				close_bracket_count++;
			else if (*str == '(')
				open_paren_count++;
			else if (*str == ')')
				close_paren_count++;

			if (*str == ';' && *(str + 1) == '\0')
				isSemicolonLast = 1;
		}

		str++;
	}

	if (open_brace_count > close_brace_count){ // 在结构体/共用体/函数中  --> |
		return openBraceNotClosed;
	} else if (open_brace_count == close_brace_count){
		if (open_brace_count == 0){ // 在花括号之外，普通语句，请判断是否使用了分号
			if (isSemicolonLast == 1){ // 使用了分号 --> C
				return finiNormStat;
			} else { // 没有使用分号 --> |
				return unfiniNormStat;
			}
		} else { // 在花括号之外，刚结束花括号，请判断是否使用了分号
			if (isSemicolonLast == 1){ // 使用了分号 --> C
				return end_fsu_semi;
			} else { // 没有使用分号，请判断刚才结束定义的是函数，还是结构体/共用体
				if (open_paren_count != 0){ // 是函数，*******************************************判断条件有误
					return end_f_nosemi;
				} else { // 是结构体/共用体
					return end_su_nosemi;
				}
			}
		}
	} else { // open_brace_count < close_brace_count
		return closeBraceNotOpened;
	}
}

int determine_type(const char* str){
	if (){ // 匹配 #include 预处理命令
		binhub_cprintf((binhub_tcolor_t){CLR_BRT_BLUE, CLR_NONE}, "Lib: %s", str);
		putchar('\n');
		return 1;
	} else if (is_match(str, R"(^\s*#\s*define\s+\w+(\s*\([^)]*\))?\s+.+$)")){ // 匹配 #define 宏定义
		binhub_cprintf((binhub_tcolor_t){CLR_BRT_MAGENTA, CLR_NONE}, "Macro: %s", str);
		putchar('\n');
		return 2;
	} else if (is_match(str, R"(^\s*\w+(\s+\w+)?\s+\w+\s*\([^)]*\)\s*\{[^}]*\}$)")){ // 匹配 C语言函数的定义
		binhub_cprintf((binhub_tcolor_t){CLR_BRT_CYAN, CLR_NONE}, "Func: %s", str);
		putchar('\n');
		return 3;
	} else if (is_match(str, R"(^\s*struct\s+\w+\s*\{[^}]*\}\s*;$)")){ // 匹配 C语言结构体的定义
		binhub_cprintf((binhub_tcolor_t){CLR_BRT_YELLOW, CLR_NONE}, "Struct: %s", str);
		putchar('\n');
		return 4;
	} else if (is_match(str, R"(^\s*union\s+\w+\s*\{[^}]*\}\s*;$)")){ // 匹配 C语言共用体的定义
		binhub_cprintf((binhub_tcolor_t){CLR_BRT_YELLOW, CLR_NONE}, "Union: %s", str);
		putchar('\n');
		return 5;
	} else if (is_match(str, R"(^\s*[^;]+;\s*$)")){ // 匹配 C语言普通语句
		return 6;
	} else {
		// printf("Invalid data!\n");
		return -1;
	}
}
