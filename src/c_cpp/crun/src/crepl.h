#ifndef CREPL_INC
#define CREPL_INC

/* 函数原型 */
int CREPL();
bool IsItSpecialCommand(const char* str);
int RunOrNot(const char* str);
int determineType(const char* str);

#endif