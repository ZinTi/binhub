#include <cstdio>
#include <cstring>

#include "ProjectCommon.h"
#include "safe_exit.h"

int UseTemplate(const char* fileName){
	FILE* fp = nullptr;
	fp = fopen(fileName, "w+");
	if (!strcmp(".c", strrchr(fileName, '.'))){
		fprintf(fp, "#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include <math.h>\n#include <time.h>\n#include <stdarg.h>\n#include <windows.h>\n// #include <conio.h>\n\nvoid Hello(void);\nint ColorPuts(unsigned short color, const char* Str, unsigned short line);\nint ColorPrintf(unsigned short color, const char* format, ...);\nvoid ChColor(unsigned short color);\nint ChCP(unsigned int newInputCP, unsigned int newOutputCP);\n\nint main(int argc, char* argv[]){\n\tChCP(65001, 65001);\n\tHello();\n\tChCP(0, 0);\n\treturn 0;\n}\n\nvoid Hello(void){\n\tColorPuts(0x02, \"Hello, World!\", 1);\n}\n\nint ColorPuts(unsigned short color, const char* Str, unsigned short line){\n\tHANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);\n\tCONSOLE_SCREEN_BUFFER_INFO csbi;\n\tunsigned short currentAttr;\n\tif(GetConsoleScreenBufferInfo(hConsole, &csbi)){\n\t\tcurrentAttr = csbi.wAttributes;\n\t}else{\n\t\tputs(\"ERROR: Failed to get console screen buffer information.\\n\");\n\t\tcurrentAttr = 0x07;\n\t}\n\tSetConsoleTextAttribute(hConsole, color);\n\tputs(Str);\n\tSetConsoleTextAttribute(hConsole, currentAttr);\n\twhile(line--){\n\t\tputchar(\'\\n\');\n\t}\n\treturn 0;\n}\n\nint ColorPrintf(unsigned short color, const char* format, ...){\n\tHANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);\n\tCONSOLE_SCREEN_BUFFER_INFO csbi;\n\tunsigned short currentAttr;\n\tif(GetConsoleScreenBufferInfo(hConsole, &csbi)){\n\t\tcurrentAttr = csbi.wAttributes;\n\t}else{\n\t\tputs(\"ERROR: Failed to get console screen buffer information.\\n\");\n\t\tcurrentAttr = 0x07;\n\t}\n\tSetConsoleTextAttribute(hConsole, color);\n\tva_list args;\n\tva_start(args, format);\n\tint ret = vprintf(format, args);\n\tva_end(args);\n\tSetConsoleTextAttribute(hConsole, currentAttr);\n\treturn ret;\n}\n\nvoid ChColor(unsigned short color){\n\tHANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);\n\tSetConsoleTextAttribute(hConsole, color);\n}\n\nint ChCP(unsigned int newInputCP, unsigned int newOutputCP){\n\tstatic unsigned int oldInputCP;\n\tstatic unsigned int oldOutputCP;\n\tif(newInputCP != 0){\n\t\toldInputCP = GetConsoleCP();\n\t\tSetConsoleCP(newInputCP);\n\t}else{\n\t\tSetConsoleCP(oldInputCP);\n\t}\n\n\tif(newOutputCP != 0){\n\t\toldOutputCP = GetConsoleOutputCP();\n\t\tSetConsoleOutputCP(newOutputCP);\n\t}else{\n\t\tSetConsoleOutputCP(oldOutputCP);\n\t}\n\treturn 0;\n}\n");
		// fputs("This is testing for fputs...\n", fp);
	}
	else if (!strcmp(".cpp", strrchr(fileName, '.'))){
		fprintf(fp, "#include <iostream>\nusing namespace std;\nvoid ShowVersion(void);\n\nint main(){\n\tShowVersion();\n\t// 输出Hello World!字符串\n\tcout << \"Hello, world!\" << endl;\n\treturn 0;\n}\nvoid ShowVersion(void){\n\tcout << \"YourProjectName version 1.0 (compiled \" << __TIME__ << \", \" << __DATE__ << \")\" << endl;\n\tcout << \"Copyright (c) 2025 Zeng Lai. All rights reserved. vip201@126.com\\n\" << endl;\n}");
	}
	else{
		crun::ProjectCommon::ColorPuts(0x0C, "The file name extension cannot determine whether it is C or C++.", 1);
		SafeExit(0);
	}

	fclose(fp);
	return 0;
}
