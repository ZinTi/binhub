#include <cstdarg>
#include <cstdio>
#include "ProjectCommon.h"

void PrintError(const char* location, const char* descriptionFormat, ...){
    va_list args;
    va_start(args, descriptionFormat);
    crun::ProjectCommon::ColorPrintf(0x04, "%s: ", location);
    crun::ProjectCommon::ColorPrintf(0x04, descriptionFormat, args);
    putchar('\n');
    va_end(args);
}

void PrintWarning(const char* location, const char* descriptionFormat, ...){
    va_list args;
    va_start(args, descriptionFormat);
    crun::ProjectCommon::ColorPrintf(0x06, "%s: ", location);
    crun::ProjectCommon::ColorPrintf(0x06, descriptionFormat, args);
    putchar('\n');
    va_end(args);
}

void DisplayErrorTips(const char* optionStr) {
    if (optionStr == NULL) {
        printf("Please enter the parameters.\n");
    }
    else {
        printf("Unknow options `%s`.\n", optionStr);
    }
    printf("You can use the parameter \"--help\" or \"-h\" to get help.\n\n");
}