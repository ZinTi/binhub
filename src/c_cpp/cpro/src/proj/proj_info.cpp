#include "proj_info.h"
#include <cstdio>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <ctime>
#include <string>
#include <windows.h>
#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif

#define PROJ_ENV_VAR                "BINHUB_HOME"
#define PROJ_NAME		            "BINHUB CPro"
#define PROJ_VER			        "0.0.1"
#define PROJ_YEARS                  "2023-2025"
#define AUTHOR                      "Mr. ZENG Lai(Zin)"
#define AUTHOR_CONTACT			    "vip201@126.com"

namespace cpro {



std::string check_env(void) {
    if(!getenv(PROJ_ENV_VAR)){
        puts("Project is not installed!");
        return "";
    }
    return getenv(PROJ_ENV_VAR);
}

void read_manual_from_disk(const char* path){
    BINHUB_UNUSED(path);
}



} // namespace cpro