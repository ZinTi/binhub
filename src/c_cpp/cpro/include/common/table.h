#ifndef TABLE_H
#define TABLE_H

#include <cstdint>
#include <string>

namespace cpro {

int handle_cmd_table(int argc, char* argv[]);

void print_operator_priority_tbl(void);


typedef struct operator_priority_t {
    uint8_t priority;
    std::string symbol;
    std::string meaning;
} operator_priority_t;


} // namespace cpro

#endif //TABLE_H
