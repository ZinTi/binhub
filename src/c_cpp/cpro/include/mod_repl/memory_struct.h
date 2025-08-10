#ifndef MEMORY_STRUCT_H
#define MEMORY_STRUCT_H

#include <string>
#include <vector>
#include <unordered_map>
#include "text_editor.h"

namespace cpro {

class MemoryStruct {
private:
    // tag, body
    std::unordered_map<std::string, std::string> structures_;
    cpro::TextEditor editor_;

    std::string generate_struct(const std::string& tag);

public:
    MemoryStruct(void);
    ~MemoryStruct(void);

    void append(const std::string& tag, const std::string& body);

    void edit_body(const std::string& tag);

    /**
     * @brief 删除元素
     * @param tag 结构体 tag
     */
    void remove(const std::string& tag);

    void list_tag(void);

    void print(const std::string& tag);

    void clear(void);

};

} // namespace cpro

#endif //MEMORY_STRUCT_H
