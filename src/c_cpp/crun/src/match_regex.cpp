#include <regex>

bool isMatch(const char* text, const char* pattern) {
    const std::regex reg(pattern);
    return regex_match(text, reg);
}