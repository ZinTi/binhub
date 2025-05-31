#ifndef MEMORY_PROG_H
#define MEMORY_PROG_H

#include <string>
#include <vector>
#include "memory_header.h"
#include "memory_macro.h"
#include "memory_function.h"
#include "memory_struct.h"
#include "memory_union.h"

#define D_TMP_SRC_FILENAME		"~temp.c"

#ifdef _WIN32
	#define D_TMP_EXE_FILENAME		"~temp.exe"
#else
	#define D_TMP_EXE_FILENAME		"~temp.out"
#endif

#define DEFAULT_PROMPT			"C> "

namespace cpro {

class MemoryProg {
private:
	std::string prompt_ = DEFAULT_PROMPT;

	// memory
	MemoryHeader mem_headers_;
	MemoryMacro mem_macros_;
	MemoryFunction mem_functions_;
	MemoryStruct mem_structures_; // struct
	MemoryUnion mem_unions_;	// union

    std::vector<std::string> main_stmts_; // main function body statements
    std::vector<std::string> typedef_list_; // typedef alias

	// output
	std::string tmp_output_filename_;

public:
	MemoryProg(void);
	~MemoryProg(void);

	int repl(int argc, char* argv[]);

private:
	int handle_renew(void);
	int handle_system(const std::string& full_cmd);
	int handle_undo(void);
	int handle_include_header(const std::string& full);
	int handle_macro(const std::string& full);
	int handle_struct(const std::string& full);
	int handle_union(const std::string& full);
	int handle_function(const std::string& full);

	void write_to_file(const std::string& output = D_TMP_SRC_FILENAME);

}; // class: cpro::MemoryProg


}; // namespace: cpro

#endif
