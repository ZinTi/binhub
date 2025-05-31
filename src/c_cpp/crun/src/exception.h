#ifndef PRINT_ERROR_INC
#define PRINT_ERROR_INC

#pragma once
void PrintError(const char* location, const char* descriptionFormat, ...);
void PrintWarning(const char* location, const char* descriptionFormat, ...);
void DisplayErrorTips(const char* optionStr);

#endif