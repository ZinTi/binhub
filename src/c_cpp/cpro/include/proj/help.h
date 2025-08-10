#ifndef HELP_H
#define HELP_H

#define PROJ_ENV_VAR                "BINHUB_HOME"
#define PROJ_NAME		            "BINHUB CPro"
#define PROJ_VER			        "0.0.1"
#define PROJ_YEARS                  "2023-2025"
#define AUTHOR                      "Mr. ZENG Lai(Zin)"
#define AUTHOR_CONTACT			    "vip201@126.com"

namespace cpro {

void print_manual(const char* command);    // print help infomation

void print_env(void);    // print CPro environment information

void print_version(void); // print CPro version

void print_repl_command(void); // print interactive mode command

void print_run_command(void);

void print_scaffold_command(void);

void print_winres_command(void);

};

#endif