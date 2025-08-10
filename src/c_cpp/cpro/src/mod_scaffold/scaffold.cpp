#include "scaffold.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

#include "fixed_template.h"

namespace cpro {

int handle_cmd_scaffold(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " -t{c|cpp} <filename>\n";
        return 1;
    }

    std::string type = argv[1];
    std::string fileName = argv[2];

    // 检查文件名是否合法（只允许小写字母、数字和下划线）
    if (fileName.empty() ||
        !std::all_of(fileName.begin(), fileName.end(), [](char c) {
            return std::islower(c) || std::isdigit(c) || c == '_';
        })) {
        // std::cerr << "ERROR: 文件名必须使用小蛇形命名法（小写字母、数字、下划线）\n";
        std::cerr << "ERROR: Filenames must use snake case naming convention (lowercase letters, digits, underscores)\n";
        return 1;
    }

    // 生成必要的命名变体
    std::string className = snake_to_pascal(fileName);
    std::string guardName = snake_to_screaming_snake(fileName) + "_H";

    if (type == "-tcpp") {
        // 生成C++文件
        std::ofstream headerFile(fileName + ".h");
        if (!headerFile) {
            // std::cerr << "错误: 无法创建头文件\n";
            std::cerr << "Error: Unable to create header file\n";
            return 1;
        }
        headerFile << generate_cpp_header(className, guardName);

        std::ofstream sourceFile(fileName + ".cpp");
        if (!sourceFile) {
            // std::cerr << "错误: 无法创建源文件\n";
            std::cerr << "Error: Unable to create source file\n";
            return 1;
        }
        sourceFile << generate_cpp_source(className, fileName);

        // std::cout << "已生成: " << fileName << ".h 和 " << fileName << ".cpp\n";
        std::cout << "Generated: " << fileName << ".h and " << fileName << ".cpp\n";
    } else if (type == "-tc") {
        // 生成C文件
        std::ofstream headerFile(fileName + ".h");
        if (!headerFile) {
            // std::cerr << "错误: 无法创建头文件\n";
            std::cerr << "Error: Unable to create header file\n";
            return 1;
        }
        headerFile << generate_c_header(guardName);

        std::ofstream sourceFile(fileName + ".c");
        if (!sourceFile) {
            // std::cerr << "错误: 无法创建源文件\n";
            std::cerr << "Error: Unable to create source file\n";
            return 1;
        }
        sourceFile << generate_c_source(fileName);

        // std::cout << "已生成: " << fileName << ".h 和 " << fileName << ".c\n";
        std::cout << "Generated: " << fileName << ".h and " << fileName << ".c\n";
    } else {
        // std::cerr << "错误: 无效类型参数。使用 -tc 或 -tcpp\n";
        std::cerr << "Error: Invalid type argument. Use -tc or -tcpp\n";
        return 1;
    }

    return 0;
}

// 将小蛇形命名转换为大驼峰命名
std::string snake_to_pascal(const std::string& snake) {
    std::string result;
    bool capitalize = true;

    for (char c : snake) {
        if (c == '_') {
            capitalize = true;
        } else if (capitalize) {
            result += std::toupper(c);
            capitalize = false;
        } else {
            result += c;
        }
    }
    return result;
}

// 将小蛇形命名转换为大蛇形命名（全大写）
std::string snake_to_screaming_snake(const std::string& snake) {
    std::string result;
    for (char c : snake) {
        if (c == '_') {
            result += '_';
        } else {
            result += std::toupper(c);
        }
    }
    return result;
}

} // namespace cpro;
