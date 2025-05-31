// 系统库
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
	#include <windows.h>

	int ChCP(unsigned int newInputCP, unsigned int newOutputCP){
		static unsigned int oldInputCP;
		static unsigned int oldOutputCP;
		if(newInputCP != 0){ //
			oldInputCP = GetConsoleCP();
			SetConsoleCP(newInputCP);
		}else{
			SetConsoleCP(oldInputCP);
		}

		if(newOutputCP != 0){
			oldOutputCP = GetConsoleOutputCP();
			SetConsoleOutputCP(newOutputCP);
		}else{
			SetConsoleOutputCP(oldOutputCP);
		}
		return 0;
	}
#endif

#include "color.h"

// 项目相关宏定义
#define PROJ_NAME			"ASCII-Table"
#define PROJ_VER			"0.1"
#define PROJ_YEARS			"2023-2025"
#define AUTHOR				"Mr. ZENG Lai(Zin)"
#define CONTACT 			"Email: vip201@126.com"

// 颜色
TextColor COLOR_PROMPT_OK = {COLOR_BRIGHT_GREEN, COLOR_NONE};
TextColor COLOR_TABLE_QUERY = {COLOR_BRIGHT_CYAN, COLOR_NONE};
TextColor COLOR_UASGE = {COLOR_BRIGHT_RED, COLOR_NONE};
TextColor COLOR_SPACE = {COLOR_NONE, COLOR_BRIGHT_RED};

/**
 * @brief 打印项目名称、版本等信息
 */
void About(void);

/**
 * @brief 打印使用帮助
 */
void Usage(void);

/**
 * @brief 查询字符值
 * @param pStr IN 字符串
 */
void QueryCharaterValue(const char* pStr);

/**
 * @brief 打印ASCII表
 * @param t IN 选择要打印的表，可选 'c': 主要是控制字符, 'p': 主要是可见字符, 'e': 主要是扩展字符
 */
void ASCII_table(const char t);

int main(int argc,char* argv[]){
#ifdef _WIN32
	ChCP(65001, 65001); // 设置CodePage
#else
	// Linux不需要
#endif
	if(argc==1){
		About();
		Usage();

		ASCII_table('c');
		ASCII_table('p');
		ASCII_table('e');
	
	}else if(argc==2){
		const char* p = (const char*)argv[1];
		QueryCharaterValue(argv[1]);
	}else{
		printf("Too many parameters.\n");
	}
#ifdef _WIN32
	ChCP(0, 0); // 恢复原有CodePage
#else
	// Linux不需要
#endif
	return 0;
}

void About(void){
	printf("%s version %s (compiled %s, %s)\n", PROJ_NAME, PROJ_VER, __TIME__, __DATE__);
	printf("Copyleft (c) %s %s. Licensed under the GNU General Public License.\n%s\n\n", PROJ_YEARS, AUTHOR, CONTACT);
}

void Usage(void){
	ColorPrintf(COLOR_UASGE, "USAGE: ascii [characters]\n");
	ColorPrintf(COLOR_UASGE, "For example:\n");
	ColorPrintf(COLOR_UASGE, "    ascii 0Aaz#\n");
	ColorPrintf(COLOR_UASGE, "    ascii \"\'\"\n");
	ColorPrintf(COLOR_UASGE, "    ascii \'\"\'\n");
	ColorPrintf(COLOR_UASGE, "    ascii \'09 Az\'\n\n");
}

void QueryCharaterValue(const char* pStr){
	ColorPrintf(COLOR_TABLE_QUERY, "┏━━━━━┯━━━━━┯━━━━━┯━━━━━┓\n");
	ColorPrintf(COLOR_TABLE_QUERY, "┃ DEC │ OCT │ HEX │ Sym ┃\n");
	ColorPrintf(COLOR_TABLE_QUERY, "┠─────┼─────┼─────┼─────┨\n");

	for( int i=0 ; i<strlen(pStr) ; i++, pStr++ ){
		if(*pStr>31 && *pStr<127){
			ColorPrintf(COLOR_TABLE_QUERY, "┃%4d │ %03o │ x%02X │  %c  ┃\n", *pStr, *pStr, *pStr, *pStr);
			//putchar('\n');
		}else{
			ColorPrintf(COLOR_TABLE_QUERY, "┃%9X不属于[32,127)┃\n", *pStr);
			//putchar('\n');
		}
	}
	ColorPrintf(COLOR_TABLE_QUERY, "┗━━━━━┷━━━━━┷━━━━━┷━━━━━┛\n");
}

void ASCII_table(const char t){
	if(t == 'c'){
		typedef struct CtrlCharX{
			short int v;
			char c1[4];
			char c2[3];
			char c3[3];
			char des[28];
		}CTRLCHARX;

		CTRLCHARX Part1[32] = {
			{0,"NUL","  ","\\0","Null character"},
			{1,"SOH","^A","  ","Start of Heading"},
			{2,"STX","^B","  ","Start of Text"},
			{3,"ETX","^C","  ","End of Text"},
			{4,"EOT","^D","  ","End of Transmission"},
			{5,"ENQ","^E","  ","Enquiry"},
			{6,"ACK","^F","  ","Acknowledge"},
			{7,"BEL","^G","\\a","Bell, Alert"},
			{8,"BS","^H","\\b","Backspace"},
			{9,"HT","^I","\\t","Horizontal Tab"},
			{10,"LF","^J","\\n","Line Feed"},
			{11,"VT","^K","\\v","Vertical Tabulation"},
			{12,"FF","^L","\\f","Form Feed"},
			{13,"CR","^M","\\r","Carriage Return"},
			{14,"SO","^N","  ","Shift Out"},
			{15,"SI","^O","  ","Shift In"},
			{16,"DLE","^P","  ","Data Link Escape"},
			{17,"DC1","^Q","  ","Device Control One (XON)"},
			{18,"DC2","^R","  ","Device Control Two"},
			{19,"DC3","^S","  ","Device Control Three (XOFF)"},
			{20,"DC4","^T","  ","Device Control Four"},
			{21,"NAK","^U","  ","Negative Acknowledge"},
			{22,"SYN","^V","  ","Synchronous Idle"},
			{23,"ETB","^W","  ","End of Transmission Block"},
			{24,"CAN","^X","  ","Cancel"},
			{25,"EM","^Y","  ","End of medium"},
			{26,"SUB","^Z","  ","Substitute"},
			{27,"ESC","^[","\\e","Escape"},
			{28,"FS","^\\","  ","File Separator"},
			{29,"GS","^]","  ","Group Separator"},
			{30,"RS","^^","  ","Record Separator"},
			{31,"US","^_","  ","Unit Separator"}
		};

		ColorPrintf(COLOR_PROMPT_OK, "-- ASCII control characters (character code 0~31) --\n");
		ColorPrintf(COLOR_PROMPT_OK, "┏━━━━━┯━━━━━┯━━━━━┯━━━━━━━━┯━━━━┯━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		ColorPrintf(COLOR_PROMPT_OK, "┃ DEC │ OCT │ HEX │ Symbol │ \\  │         Description         ┃\n");
		ColorPrintf(COLOR_PROMPT_OK, "┠─────┼─────┼─────┼────────┼────┼─────────────────────────────┨\n");
		for(int i=0 ; i<32 ; i++){
			ColorPrintf(COLOR_PROMPT_OK, "┃%4d │ %#03o │ x%02X │%4s %2s │%3s │ %-28s┃\n", Part1[i].v, Part1[i].v, Part1[i].v, Part1[i].c1, Part1[i].c2, Part1[i].c3, Part1[i].des);
			//putchar('\n');
		}
		ColorPrintf(COLOR_PROMPT_OK, "┗━━━━━┷━━━━━┷━━━━━┷━━━━━━━━┷━━━━┷━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
		puts("The first 32 characters in the ASCII-table are unprintable control codes and are used to control peripherals such as printers.\n");
	
	}else if(t == 'p'){
		ColorPrintf(COLOR_PROMPT_OK, "-- ASCII printable characters (character code 32~127) --\n");
		ColorPrintf(COLOR_PROMPT_OK, "┏━━━━━┯━━━━━┯━━━━━┯━━━━━╥━━━━━┯━━━━━┯━━━━━┯━━━━━┓\n");
		ColorPrintf(COLOR_PROMPT_OK, "┃ DEC │ OCT │ HEX │ Sym ║ DEC │ OCT │ HEX │ Sym ┃\n");
		ColorPrintf(COLOR_PROMPT_OK, "┠─────┼─────┼─────┼─────╫─────┼─────┼─────┼─────┨\n");

		ColorPrintf(COLOR_PROMPT_OK, "┃  32 │ 040 │ x20 │  ");
		ColorPrintf(COLOR_SPACE, " ");
		ColorPrintf(COLOR_PROMPT_OK, "  ║  80 │ 120 │ x50 │  %c  ┃\n", 80);
		//putchar('\n');

		for(short int i=33,j=81;i<79;i++,j++){
			ColorPrintf(COLOR_PROMPT_OK, "┃%4d │ %03o │ x%X │  %c  ║%4d │ %03o │ x%X │  %c  ┃\n",i,i,i,i,j,j,j,j);
			//putchar('\n');
		}

		ColorPrintf(COLOR_PROMPT_OK, "┃  79 │ 117 │ x4F │  O  ║ 127 │ 177 │ x7F │ DEL ┃\n");
		ColorPrintf(COLOR_PROMPT_OK, "┗━━━━━┷━━━━━┷━━━━━┷━━━━━╨━━━━━┷━━━━━┷━━━━━┷━━━━━┛\n");
		puts("Codes 32-127 are common for all the different variations of the ASCII table, they are called printable characters, represent letters, digits, punctuation marks, and a few miscellaneous symbols. You will find almost every character on your keyboard. Character 127 represents the command DEL.\n");

	}else if(t == 'e'){
		ColorPrintf(COLOR_PROMPT_OK, "-- The extended ASCII codes (character code 128~255) --\n");
		ColorPrintf(COLOR_PROMPT_OK, "Omitted here.\n\n");
	}else{
		puts("ASCII_table: Invalid Option.");
	}
}