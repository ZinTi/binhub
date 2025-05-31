// 系统库
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROJ_NAME			"BaseConvert"
#define PROJ_VER			"0.1"
#define PROJ_YEARS			"2023-2025"
#define AUTHOR				"Mr. ZENG Lai(Zin)"
#define CONTACT 			"Email: vip201@126.com"

// 函数原型
void DisplayVersion(char* path);
void DisplayHelp(char* path);
void convertBinary(char* binary);
void convertOctal(char* octal);
void convertDecimal(char* decimal);
void convertHexadecimal(char* hexadecimal);
void printBinary(long decimal);

int main(int argc, char* argv[]) {
    // 检查命令行参数
    if (argc < 2) {
        DisplayVersion(argv[0]);
        printf("Insufficient arguments. Use baseconvert.exe --help for usage.\n\n");
        return 1;
    }else if(argc > 3){
        printf("Too many arguments. Use baseconvert.exe --help for usage.\n\n");
        return 1;
    }

    // 处理命令行参数
    if (strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "--help") == 0) {
        DisplayHelp(argv[0]);
    } else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        DisplayVersion(argv[0]);
        
    } else if (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "--binary") == 0) {
        if (argc < 3) {
            printf("Error: Missing binary number for conversion.\n");
            return 1;
        }
        convertBinary(argv[2]);
    } else if (strcmp(argv[1], "-o") == 0 || strcmp(argv[1], "--octal") == 0) {
        if (argc < 3) {
            printf("Error: Missing octal number for conversion.\n");
            return 1;
        }
        convertOctal(argv[2]);
    } else if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "--decimal") == 0) {
        if (argc < 3) {
            printf("Error: Missing decimal number for conversion.\n");
            return 1;
        }
        convertDecimal(argv[2]);
    } else if (strcmp(argv[1], "-x") == 0 || strcmp(argv[1], "--hexadecimal") == 0) {
        if (argc < 3) {
            printf("Error: Missing hexadecimal number for conversion.\n");
            return 1;
        }
        convertHexadecimal(argv[2]);
    } else {
        // 默认情况，假设输入为十进制数
        printf("Invalid option: %s! Assume that the input is a decimal number.\n",argv[1]);
        convertDecimal(argv[1]);
    }

    return 0;
}

void DisplayVersion(char* path){
	printf("%s version %s (compiled %s, %s)\n", PROJ_NAME, PROJ_VER, __TIME__, __DATE__);
	printf("Copyleft (c) %s %s. Licensed under the GNU General Public License.\n%s\n\n", PROJ_YEARS, AUTHOR, CONTACT);
    printf("%s\n\n", path);
}

void DisplayHelp(char* path) {
    printf("Usage:\n\t%s [OPTIONS] NUMBER\n", path);
    printf("\tConverts between different number bases.\n\n");
    printf("Options:\n");
    printf("  -?, --help\t\tDisplay this help message.\n");
    printf("  -v, --version\t\tDisplay the version of the program.\n");
    printf("  -b, --binary\t\tConvert binary to octal, decimal, and hexadecimal.\n");
    printf("  -o, --octal\t\tConvert octal to binary, decimal, and hexadecimal.\n");
    printf("  -d, --decimal\t\tConvert decimal to binary, octal, and hexadecimal.\n");
    printf("  -x, --hexadecimal\tConvert hexadecimal to binary, octal, and decimal.\n");
}

// 将二进制转换为其他进制
void convertBinary(char* binary) {
    // 进行二进制合法性检查，这里简化为只包含0和1的合法二进制数
    for (int i = 0; i < strlen(binary); i++) {
        if (binary[i] != '0' && binary[i] != '1') {
            printf("Error: Invalid binary number.\n");
            exit(1);
        }
    }

    // 进行进制转换并输出
    long decimal = strtol(binary, NULL, 2);
    printf("BIN:\t%s\nOCT:\t%lo\nDEC:\t%ld\nHEX:\t%lX\n", binary, decimal, decimal, decimal);
}

// 将八进制转换为其他进制
void convertOctal(char* octal) {
    // 进行八进制合法性检查
    char* endptr;
    strtol(octal, &endptr, 8);
    if (*endptr != '\0') {
        printf("Error: Invalid octal number.\n");
        exit(1);
    }

    // 进行进制转换并输出
    long decimal = strtol(octal, NULL, 8);
    printBinary(decimal);
    printf("OCT:\t%s\nDEC:\t%ld\nHEX:\t%lX\n", octal, decimal, decimal);
}

// 将十进制转换为其他进制
void convertDecimal(char* decimal) {
    // 进行十进制合法性检查
    char* endptr;
    strtol(decimal, &endptr, 10);
    if (*endptr != '\0') {
        printf("Error: Invalid decimal number.\n");
        exit(1);
    }

    // 进行进制转换并输出
    long decimalValue = strtol(decimal, NULL, 10);
    printBinary(decimalValue);
    printf("OCT:\t%lo\nDEC:\t%s\nHEX:\t%lX\n", decimalValue, decimal, decimalValue);
}

// 将十六进制转换为其他进制
void convertHexadecimal(char* hexadecimal) {
    // 进行十六进制合法性检查
    char* endptr;
    strtol(hexadecimal, &endptr, 16);
    if (*endptr != '\0') {
        printf("Error: Invalid hexadecimal number.\n");
        exit(1);
    }

    // 进行进制转换并输出
    long decimal = strtol(hexadecimal, NULL, 16);
    printBinary(decimal);
    printf("OCT:\t%lo\nDEC:\t%ld\nHEX:\t%s\n", decimal, decimal, hexadecimal);
}

void printBinary(long decimal) {
    if (decimal == 0) {
        printf("BIN:\t0\n");
        return;
    }

    // 定义一个数组来存储二进制表示
    int binary[64];  // 假设长整型为64位
    int index = 0;

    // 将十进制数转换为二进制
    while (decimal > 0) {
        binary[index++] = decimal % 2;
        decimal /= 2;
    }

    // 从数组末尾开始打印二进制数
    printf("BIN:\t");
    for (int i = index - 1; i >= 0; i--) {
        printf("%d", binary[i]);
    }

    printf("\n");
}
