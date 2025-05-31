#include <stdio.h>
#include <stdlib.h>

#define HOME_PATH_VAR_NAME      "BINHUB_HOME"

int main(void) {
    const char* home_path = getenv(HOME_PATH_VAR_NAME);
    if (home_path == (void*)0) {
        puts("BinHub is not installed!");
        return 1;
    }else{
        printf(HOME_PATH_VAR_NAME ": %s", home_path);
    }
    return 0;
}
