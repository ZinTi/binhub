#ifndef MEMORY_GCC_COMPILER_H
#define MEMORY_GCC_COMPILER_H

#include <string>
#include "gcc_option.h"

namespace cpro {

class MemoryGCCCompiler {
private:
    cpro::GCCOption gcc_option_;    // gcc compiler option
    std::string mem_src_code_;

public:
    MemoryGCCCompiler(void);
    MemoryGCCCompiler(const std::string& mem_src_code);
    ~MemoryGCCCompiler(void);

    void set_syntax_only(bool syntax_only);

    std::string get_mem_src_code(void) const;
    void set_mem_src_code(const std::string& mem_src_code);

    int compile(void);

};

} // namespace cpro

#endif //MEMORY_GCC_COMPILER_H
