#ifndef MEMORY_HEADER_H
#define MEMORY_HEADER_H

#include <string>
#include <set>
#include <algorithm>
#include <iomanip>

namespace cpro {

class MemoryHeader {
private:
    std::set<std::string> headers_;    //

public:
    MemoryHeader(void);
    ~MemoryHeader(void);

    bool test_header_include(const std::string& header);
    void insert(const std::string& header);
    void erase(const std::string& header);
    int find(const std::string& header);
    int get_size(void);
    bool is_empty(void);
    void clear(void); // clear all headers

    void print(const std::string& title);
    void write_to_file(const std::string& file);

};

} // namespace cpro

#endif //MEMORY_HEADER_H
