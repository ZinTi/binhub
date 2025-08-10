#include "help.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif

namespace cpro {

void print_manual(const char* command){
	if(!command){
	    std::cout << "CPro is a tool for managing C source code.\n\n";

	    std::cout << "Usage:\n\n";
	    std::cout << "\tcpro <command> [arguments]\n\n";
	    std::cout << "The commands are:\n\n";
	    std::cout << "\tenv\tprint CPro environment information.\n";
	    std::cout << "\thelp\tDisplay this help information.\n";
	    std::cout << "\tproj\tCreate a new CMake project.\n";
	    std::cout << "\trepl\tEnter the interactive mode.\n";
	    std::cout << "\trun\tCompile and run C program.\n";
	    std::cout << "\tscaffold\tGenerate a pair of header(.h) and implementation(.c/.cpp) files.\n";
	    std::cout << "\ttable\tPrint table.\n";
	    std::cout << "\tversion\tprint CPro version.\n";
	    std::cout << "\twinres\tGenerate a Windows res file.\n";
	    std::cout << std::endl;

    	std::cout << "Use `cpro help <command>` for more information about a command.\n\n";
	} else {
		if(!strcmp(command, "env")){
			std::cout << "Type `cpro env` to check the runtime environment." << std::endl;
		} else if(!strcmp(command, "help")){
			std::cout << "Type `cpro help <command>` for more information on <command>." << std::endl;
		} else if(!strcmp(command, "proj")){
			std::cout << "Type `cpro proj` to create a new CMake project." << std::endl;
		} else if(!strcmp(command, "repl")){
			print_repl_command();
		} else if(!strcmp(command, "run")){
			print_run_command();
		} else if(!strcmp(command, "scaffold")){
			print_scaffold_command();
		} else if(!strcmp(command, "table")){
			std::cout << "Type `cpro table <name>` to print table." << std::endl;
			std::cout << "<name>: ascii, priority" << std::endl;
		} else if(!strcmp(command, "version")){
			std::cout << "Type `cpro version` to print the version information." << std::endl;
		} else if(!strcmp(command, "winres")){
			print_winres_command();
		} else {
			// binhub_cerr("No usage information available for command '%s' or invalid command.", command);
			binhub_cerr("Command `%s` is not recognized.", command);
		}
	}
}

void print_env(void){
#ifdef _WIN32
    #define CMD_GCC_VERSION    "gcc -v"
    #define CMD_CMAKE_VERSION  "cmake --version"
#else
    #define CMD_GCC_VERSION    "gcc -v"
    #define CMD_CMAKE_VERSION  "cmake --version"
#endif

    binhub_whereami();    // 打印环境变量 BINHUB_HOME
    binhub_cprintf((binhub_tcolor_t){CLR_MAGENTA, CLR_NONE}, "gcc -v:"); BINHUB_ENDLINE(1);
    system(CMD_GCC_VERSION);
    binhub_cprintf((binhub_tcolor_t){CLR_MAGENTA, CLR_NONE}, "cmake --version:"); BINHUB_ENDLINE(1);
    system(CMD_CMAKE_VERSION);
}

void print_version(void){
    printf(PROJ_NAME " version " PROJ_VER " (compiled " __TIME__ ", " __DATE__ ")\n");
    printf("Copyleft (c) " PROJ_YEARS " " AUTHOR ". All rights reserved. " AUTHOR_CONTACT "\n\n");
    printf("Enter \".help\" for usage hints.\n");
}

void print_repl_command(void){
    printf("REPL mode commands are:\n\n");

    printf("List of all commands (case-insensitive):\n\n");

    std::cout << "(1) built-in command\n";
    printf("%-24s %s\n", ".exit", "Exit CREPL.");
    printf("%-24s %s\n", ".help", "Display this information.");
    printf("%-24s %s\n", ".quit", "Quit CREPL. Same as `.exit`.");
    printf("%-24s %s\n", ".renew", "Use a new environment.");
    printf("%-24s %s\n", ".system", "Execute a system shell command.");
    printf("%-24s %s\n", ".undo", "Undo the previous statement.\n");

    std::cout << "(2) file command\n";
    printf("%-24s %s\n", ".file", "Print all filenames of memory.");
    printf("%-24s %s\n", ".file <filename>", "Add header or source file in memory.");
    printf("%-24s %s\n", ".file <filename> -s", "Switch file.");
    printf("%-24s %s\n", ".file <filename> -d", "Delete header file in memory.\n");

    std::cout << "(3) statements command\n";
    printf("%-24s %s\n", ".inc", "Print included header.");
    printf("%-24s %s\n", ".inc <header>", "Include header, example: .inc <stdio.h> <stdlib.h>");
    printf("%-24s %s\n", ".inc <header> -d", "Delete included header.\n");

	printf("%-24s %s\n", ".macro -c", "Clear macro definition.");
    printf("%-24s %s\n", ".macro -e", "Edit macro definition.");
	printf("%-24s %s\n", ".macro -p", "Print all user-defined macros.");
    printf("%-24s %s\n", ".macro -w", "Write macro definition to file.\n");

	printf("%-24s %s\n", ".struct", "List all struct.");
	printf("%-24s %s\n", ".struct <tag>", "Edit struct body.");
	printf("%-24s %s\n", ".struct <tag> -d", "Delete struct definition.\n");
	printf("%-24s %s\n", ".struct <tag> -p", "Print struct definition.\n");

	printf("%-24s %s\n", ".union", "List all union.");
	printf("%-24s %s\n", ".union <tag>", "Edit union body.");
	printf("%-24s %s\n", ".union <tag> -d", "Delete union definition.\n");
	printf("%-24s %s\n", ".union <tag> -p", "Print union definition.\n");

}

// cpro run [OPTIONS] <sources>
void print_run_command(void){
    printf("Usage: cpro run [build options] <sources>\n\n");

    printf("The program only supports combined short options and does not support long options or separate short options.\n\n");
    printf("build options:\n");
    printf(" -a\tObtain the arguments and pass them to the subprocess.\n");
    printf(" -o\tRename the executable file.\n");
    printf(" -s\tDo not delete intermediate files.\n\n");

}

void print_scaffold_command(void){
	std::cout << "Usage: cpro scaffold -t{c|cpp} <filename>\n\n";

	std::cout << "Generate a pair of header(.h) and implementation(.c/.cpp) files.\n";
}

void print_winres_command(void){
	std::cout << "Usage: cpro winres <input> [output]\n\n";

	std::cout << "<input>:\tThe path or name of the Windows icon file in ICO format.\n";
	std::cout << "[output]:\tPlace the output into [output].";
}


};