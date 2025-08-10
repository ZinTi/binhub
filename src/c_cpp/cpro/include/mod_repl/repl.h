#ifndef REPL_H
#define REPL_H

int handle_cmd_repl(const int argc, char* argv[]);
int run_or_not(const char* str);
int determine_type(const char* str);

#endif