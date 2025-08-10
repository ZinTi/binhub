#include <regex>

bool is_match(const char* text, const char* pattern) {
    const std::regex reg(pattern);
    return regex_match(text, reg);
}