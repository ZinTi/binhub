#ifndef MEMORY_FUNCTION_H
#define MEMORY_FUNCTION_H

#include <iostream>
#include <vector>
#include <string>
#include "text_editor.h"

namespace cpro {

class MemoryFunction {
private:
    std::vector<std::string> functions_;
    cpro::TextEditor editor_;

public:
    MemoryFunction(void);
    ~MemoryFunction(void);

    void append(void);

    /**
     * @brief 删除元素
     * @param index 索引，从 0 开始
     */
    void remove(int index);

    // index 从 0 开始
    void edit(int index);

    void print_size(void);

    void print(int index);

    void print_all(void);

    void clear(void);

};

} // namespace cpro

#endif //MEMORY_FUNCTION_H
