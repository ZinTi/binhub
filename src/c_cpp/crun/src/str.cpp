#include <cstring>
#include "safe_exit.h"
#include "exception.h"

char* ChFilenameExtension(const char* oldFn, char* newFn, const char* newExtension){
	if (oldFn == nullptr){
		PrintError("ChFilenameExtention(oldFn, newExtension)", "Error! oldFn is NULL.\n");
		SafeExit(-1);
	}
	const char* dot_position = strrchr(oldFn, '.');
    const auto n = static_cast<unsigned short>(dot_position - oldFn);
	if (dot_position != nullptr){
		strncpy(newFn, oldFn, n);
		newFn[n] = '\0';
		strcat(newFn, newExtension);
	}
	else{
		strcpy(newFn, oldFn);
		strcat(newFn, newExtension);
	}
	return newFn;
}