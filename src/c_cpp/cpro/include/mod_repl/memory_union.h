#ifndef MEMORY_UNION_H
#define MEMORY_UNION_H

#include <string>
#include <vector>
#include <unordered_map>
#include "text_editor.h"

namespace cpro {

class MemoryUnion {
private:
    std::unordered_map<std::string, std::string> unions_;
    cpro::TextEditor editor_;

    std::string generate_union(const std::string& tag);

public:
    MemoryUnion(void);
    ~MemoryUnion(void);

    void append(const std::string& tag, const std::string& body);

    void edit_body(const std::string& tag);

    /**
     * @brief 删除元素
     * @param tag 共用体 tag
     */
    void remove(const std::string& tag);

    void list_tag(void);

    void print(const std::string& tag);

    void clear(void);

};

} // namespace cpro

#endif //MEMORY_UNION_H
