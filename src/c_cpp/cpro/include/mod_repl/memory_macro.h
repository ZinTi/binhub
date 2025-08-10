#ifndef MEMORY_MACRO_H
#define MEMORY_MACRO_H

#include <string>
#include "text_editor.h"

namespace cpro {

class MemoryMacro {
private:
    std::string macros_;
    cpro::TextEditor editor_;

public:
    MemoryMacro(void);
    ~MemoryMacro(void);

    void append(const std::string& macro);
    void edit(void);
    void clear(void);
    void print(const std::string& title);
    void write_to_file(const std::string& file);

};

} // namespace cpro

#endif
