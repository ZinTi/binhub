#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>

#include "color.h"

#define SAME_AS_ABOVE "(same as above)"

typedef struct OperatorPriority {
    short int priority;
    char symbol[10];
    char* meaning;
} OPPRI;

OPPRI a1[46] = {
    {1, "()", "Parentheses, function arguments"}, {1, "[]", "Array subscript"}, 
    {1, "->", "Member access via pointer"}, {1, ".", "Member access"}, 
    {1, "a++", "Post-increment"}, {1, "a--", "Post-decrement"},
    {2, "!", "Logical NOT"}, {2, "~", "Bitwise complement"}, 
    {2, "++a", "Pre-increment"}, {2, "--a", "Pre-decrement"}, 
    {2, "-a", "Unary minus"}, {2, "*p", "Indirection (dereference)"}, 
    {2, "&a", "Address-of"}, {2, "(type)", "Type cast"}, 
    {2, "sizeof", "Size in bytes"},
    {3, "*", "Multiplication"}, {3, "/", "Division"}, 
    {3, "\x25", "Modulus (remainder)"},
    {4, "+", "Addition"}, {4, "-", "Subtraction"},
    {5, "<<", "Bitwise left shift"}, {5, ">>", "Bitwise right shift"},
    {6, "<", "Less than"}, {6, "<=", "Less than or equal"}, 
    {6, ">", "Greater than"}, {6, ">=", "Greater than or equal"},
    {7, "==", "Equal to"}, {7, "!=", "Not equal to"},
    {8, "&", "Bitwise AND"},
    {9, "^", "Bitwise XOR"},
    {10, "|", "Bitwise OR"},
    {11, "&&", "Logical AND"},
    {12, "||", "Logical OR"},
    {13, "?:", "Conditional operator"},
    {14, "=", "Assignment"}, {14, "+=", "Compound assignment"}, 
    {14, "-=", SAME_AS_ABOVE}, {14, "*=", SAME_AS_ABOVE}, 
    {14, "/=", SAME_AS_ABOVE}, {14, "\x25=", SAME_AS_ABOVE}, 
    {14, "&=", SAME_AS_ABOVE}, {14, "^=", SAME_AS_ABOVE}, 
    {14, "|=", SAME_AS_ABOVE}, {14, "<<=", SAME_AS_ABOVE}, 
    {14, ">>=", SAME_AS_ABOVE},
    {15, ",", "Comma operator"}
};

const char* opType[15] = {
    "Grouping/Access", "Unary", "Arithmetic", "Arithmetic", 
    "Bitwise", "Relational", "Relational", "Bitwise", "Bitwise", 
    "Bitwise", "Logical", "Logical", "Ternary", "Assignment", 
    "Comma"
};

void ShowTable(int x);

int main(int argc, char** argv) {
    ShowTable(1);
    return 0;
}

void ShowTable(int x) {
    printf("%-8s %-10s %-35s %-20s %s\n", "Priority", "Operator", "Meaning", "Type", "Associativity");
    
    for (int i = 0; i < 46; i++) {
        /*
        ChangeColor(a1[i].priority, NULL);
        printf("%-8d %-10s %-35s %-20s ", a1[i].priority, a1[i].symbol, a1[i].meaning, opType[a1[i].priority - 1]);
        if (a1[i].priority != 2 && a1[i].priority != 13 && a1[i].priority != 14) {
            printf("Left to Right");
        } else {
            printf("Right to Left");
        }
        ChangeColor(0x7, NULL);
        printf("\n");*/

        ColorPrintf((TextColor){a1[i].priority, COLOR_NONE}, "%-8d %-10s %-35s %-20s %s\n", a1[i].priority, a1[i].symbol, a1[i].meaning, opType[a1[i].priority - 1], ((a1[i].priority != 2 && a1[i].priority != 13 && a1[i].priority != 14) ? "Left to Right" : "Right to Left"));
    }
}
