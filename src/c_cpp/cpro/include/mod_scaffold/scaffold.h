#ifndef SCAFFOLD_H
#define SCAFFOLD_H

#include <string>

namespace cpro {

std::string snake_to_pascal(const std::string& snake); // 将小蛇形命名转换为大驼峰命名
std::string snake_to_screaming_snake(const std::string& snake); // 将小蛇形命名转换为大蛇形命名（全大写）
int handle_cmd_scaffold(int argc, char* argv[]);

} // namespace cpro;


#endif
