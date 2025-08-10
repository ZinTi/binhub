#include "table.h"
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>

#include <cmath>
#include <ctime>
#include <cstdarg>
#include <list>

#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif

namespace cpro {

#define SAME_AS_ABOVE "(same as above)"


extern operator_priority_t a1[46];
extern const char* op_type[15];

int handle_cmd_table(int argc, char* argv[]){
    if(argc < 2){
        printf("Usage: cpro %s <table_name>\n", argv[0]);
        return 1;
    }

    std::string table_name = argv[1];
    if(table_name == "ascii"){

    } else if(table_name == "priority"){
        print_operator_priority_tbl();
    } else {
        printf("Unknown table: %s\n", table_name.c_str());
        return 1;
    }

    return 0;
}

void print_operator_priority_tbl(void) {
    printf("%-8s %-10s %-35s %-20s %s\n", "Priority", "Operator", "Meaning", "Type", "Associativity");

    for (uint8_t i = 0; i < 46; i++) {
        binhub_cprintf(
            (binhub_tcolor_t){(binhub_ansi_color_t)a1[i].priority, CLR_NONE},
            "%-8d %-10s %-35s %-20s %s\n",
            a1[i].priority,
            a1[i].symbol.c_str(),
            a1[i].meaning.c_str(),
            op_type[a1[i].priority - 1],
            ((a1[i].priority != 2 && a1[i].priority != 13 && a1[i].priority != 14) ? "Left to Right" : "Right to Left")
        );
    }
}

operator_priority_t a1[46] = {
    {1, "()", "Parentheses, function arguments"},
    {1, "[]", "Array subscript"},
    {1, "->", "Member access via pointer"},
    {1, ".", "Member access"},
    {1, "a++", "Post-increment"},
    {1, "a--", "Post-decrement"},
    {2, "!", "Logical NOT"},
    {2, "~", "Bitwise complement"},
    {2, "++a", "Pre-increment"},
    {2, "--a", "Pre-decrement"},
    {2, "-a", "Unary minus"},
    {2, "*p", "Indirection (dereference)"},
    {2, "&a", "Address-of"},
    {2, "(type)", "Type cast"},
    {2, "sizeof", "Size in bytes"},
    {3, "*", "Multiplication"},
    {3, "/", "Division"},
    {3, "\x25", "Modulus (remainder)"},
    {4, "+", "Addition"},
    {4, "-", "Subtraction"},
    {5, "<<", "Bitwise left shift"},
    {5, ">>", "Bitwise right shift"},
    {6, "<", "Less than"},
    {6, "<=", "Less than or equal"},
    {6, ">", "Greater than"},
    {6, ">=", "Greater than or equal"},
    {7, "==", "Equal to"},
    {7, "!=", "Not equal to"},
    {8, "&", "Bitwise AND"},
    {9, "^", "Bitwise XOR"},
    {10, "|", "Bitwise OR"},
    {11, "&&", "Logical AND"},
    {12, "||", "Logical OR"},
    {13, "?:", "Conditional operator"},
    {14, "=", "Assignment"},
    {14, "+=", "Compound assignment"},
    {14, "-=", SAME_AS_ABOVE},
    {14, "*=", SAME_AS_ABOVE},
    {14, "/=", SAME_AS_ABOVE},
    {14, "\x25=", SAME_AS_ABOVE},
    {14, "&=", SAME_AS_ABOVE},
    {14, "^=", SAME_AS_ABOVE},
    {14, "|=", SAME_AS_ABOVE},
    {14, "<<=", SAME_AS_ABOVE},
    {14, ">>=", SAME_AS_ABOVE},
    {15, ",", "Comma operator"}
};

const char* op_type[15] = {
    "Grouping/Access", "Unary", "Arithmetic", "Arithmetic",
    "Bitwise", "Relational", "Relational", "Bitwise", "Bitwise",
    "Bitwise", "Logical", "Logical", "Ternary", "Assignment",
    "Comma"
};



} // namespace cpro