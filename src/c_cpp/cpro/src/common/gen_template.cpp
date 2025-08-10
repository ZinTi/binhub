#include "gen_template.h"
#include <cstdio>
#include <cstring>
#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif

#include "proj_info.h"

extern const char* template_src_c;
extern const char* template_src_cpp;
extern const char* template_cmakelists_c;
extern const char* template_cmakelists_cpp;

int write_template_src(const std::string filename){
	FILE* fp = fopen(filename.c_str(), "w+");
	if (!strcmp(".c", strrchr(filename.c_str(), '.'))){
		fprintf(fp, "%s", template_src_c);
		// fputs("This is testing for fputs...\n", fp);
	} else if (!strcmp(".cpp", strrchr(filename.c_str(), '.'))){
		fprintf(fp, "%s", template_src_cpp);
	} else {
		binhub_cprintf((binhub_tcolor_t){CLR_BRT_RED, CLR_NONE}, "The file name extension cannot determine whether it is C or C++.");
		BINHUB_ENDLINE(1);
		exit(0);
	}

	fclose(fp);
	return 0;
}

int write_template_cmakelists(const std::string project_name, bool is_cpp_project){
	FILE* fp = fopen(std::string(project_name + "/CMakeLists.txt").c_str(), "w+");
	if (!is_cpp_project){
		fprintf(fp, "%s", template_src_c);
	} else {
		fprintf(fp, "%s", template_src_cpp);
	}

	fclose(fp);
	return 0;
}

const char* template_src_c = R"(/* template_src_c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>
#include <stdint.h>
#include <windows.h>
// #include <conio.h>

void greet(void);
int cputs(uint16_t color, const char* str, uint16_t line); // color/custom puts
int cprintf(uint16_t color, const char* format, ...); // color/custom printf
void chcolor(uint16_t color); // change console color
int chcp(unsigned int in_cp, unsigned int out_cp); // change codepage

int main(int argc, char* argv[]){
	chcp(65001, 65001);
	greet();
	chcp(0, 0);
	return 0;
}

void greet(void){
	cputs(0x02, "Hello, World!", 1);
}

int cputs(uint16_t color, const char* str, uint16_t line){
	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	uint16_t current_attr;
	if(GetConsoleScreenBufferInfo(h_console, &csbi)){
		current_attr = csbi.wAttributes;
	}else {
		puts("ERROR: Failed to get console screen buffer information.\n");
		current_attr = 0x07;
	}
	SetConsoleTextAttribute(h_console, color);
	puts(str);
	SetConsoleTextAttribute(h_console, current_attr);
	while(line--){
		putchar('\n');
	}
	return 0;
}

int cprintf(uint16_t color, const char* format, ...){
	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	uint16_t current_attr;
	if(GetConsoleScreenBufferInfo(h_console, &csbi)){
		current_attr = csbi.wAttributes;
	}else {
		puts("ERROR: Failed to get console screen buffer information.\n");
		current_attr = 0x07;
	}
	SetConsoleTextAttribute(h_console, color);
	va_list args;
	va_start(args, format);
	int ret = vprintf(format, args);
	va_end(args);
	SetConsoleTextAttribute(h_console, current_attr);
	return ret;
}

void chcolor(uint16_t color){
	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h_console, color);
}

int chcp(unsigned int in_cp, unsigned int out_cp){
	static unsigned int old_in_cp;
	static unsigned int old_out_cp;
	if(in_cp != 0){
		old_in_cp = GetConsoleCP();
		SetConsoleCP(in_cp);
	}else {
		SetConsoleCP(old_in_cp);
	}

	if(out_cp != 0){
		old_out_cp = GetConsoleOutputCP();
		SetConsoleOutputCP(out_cp);
	}else {
		SetConsoleOutputCP(old_out_cp);
	}
	return 0;
}

)";

const char* template_src_cpp = R"(/* template_src_cpp */
#include <iostream>
using namespace std;
void show_version(void);

int main(){
	show_version();
	// 输出Hello World!字符串
	cout << "Hello, world!" << endl;
	return 0;
}
void show_version(void){
	cout << "YourProject version 0.0.1 (compiled " << __TIME__ << ", " << __DATE__ << ")\n";
	cout << "Copyright (c) 2025 Your Name. All rights reserved. your@email.com\n" << endl;
}

)";

const char* template_cmakelists_c = R"(/* template_cmakelists_c */
cmake_minimum_required(VERSION 3.15)
project(YourProject)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)

)";

const char* template_cmakelists_cpp = R"(/* template_cmakelists_cpp */
cmake_minimum_required(VERSION 3.15)
project(YourProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)

)";
