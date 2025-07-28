#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

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

// 生成C++头文件内容
std::string generate_cpp_header(const std::string& className, const std::string& guardName) {
    std::string content = 
        "#ifndef " + guardName + "\n"
        "#define " + guardName + "\n\n"
        "/**\n"
        " * @class " + className + "\n"
        " * @brief 示例类模板\n"
        " */\n"
        "class " + className + " {\n"
        "public:\n"
        "    /**\n"
        "     * @brief 默认构造函数\n"
        "     */\n"
        "    " + className + "(void);\n\n"
        "    /**\n"
        "     * @brief 拷贝构造函数\n"
        "     * @param other 拷贝源对象\n"
        "     */\n"
        "    " + className + "(const " + className + "& other);\n\n"
        "    /**\n"
        "     * @brief 析构函数\n"
        "     */\n"
        "    ~" + className + "(void);\n\n"
        "    /**\n"
        "     * @brief 赋值运算符重载\n"
        "     * @param other 赋值源对象\n"
        "     * @return " + className + "& 当前对象的引用\n"
        "     */\n"
        "    " + className + "& operator=(const " + className + "& other);\n\n"
        "    /**\n"
        "     * @brief 初始化函数\n"
        "     * @return int 错误码，0表示成功\n"
        "     */\n"
        "    int init(void);\n\n"
        "    /**\n"
        "     * @brief 示例成员函数\n"
        "     * @param param1 第一个参数\n"
        "     * @param param2 第二个参数\n"
        "     * @param param3 第三个参数\n"
        "     * @return int 计算结果\n"
        "     */\n"
        "    int example_method(int param1, \n"
        "                       int param2, \n"
        "                       int param3);\n\n"
        "protected:\n"
        "    // 保护成员区域\n\n"
        "private:\n"
        "    int member_var_;   // 示例成员变量\n"
        "};\n\n"
        "#endif // " + guardName + "\n";
    return content;
}

// 生成C++源文件内容
std::string generate_cpp_source(const std::string& className, const std::string& fileName) {
    std::string content = 
        "#include \"" + fileName + ".h\"\n\n"
        "/**\n"
        " * @brief 默认构造函数\n"
        " */\n"
        + className + "::" + className + "(void) : member_var_(0) {}\n\n"
        "/**\n"
        " * @brief 拷贝构造函数\n"
        " * @param other 拷贝源对象\n"
        " */\n"
        + className + "::" + className + "(const " + className + "& other) : member_var_(other.member_var_) {}\n\n"
        "/**\n"
        " * @brief 析构函数\n"
        " */\n"
        + className + "::~" + className + "(void) {}\n\n"
        "/**\n"
        " * @brief 赋值运算符重载\n"
        " * @param other 赋值源对象\n"
        " * @return " + className + "& 当前对象的引用\n"
        " */\n"
        + className + "& " + className + "::operator=(const " + className + "& other) {\n"
        "    if (this != &other) {\n"
        "        member_var_ = other.member_var_;\n"
        "    }\n"
        "    return *this;\n"
        "}\n\n"
        "/**\n"
        " * @brief 初始化函数\n"
        " * @return int 错误码，0表示成功\n"
        " */\n"
        "int " + className + "::init(void) {\n"
        "    // 初始化代码\n"
        "    return 0;\n"
        "}\n\n"
        "/**\n"
        " * @brief 示例成员函数\n"
        " * @param param1 第一个参数\n"
        " * @param param2 第二个参数\n"
        " * @param param3 第三个参数\n"
        " * @return int 计算结果\n"
        " */\n"
        "int " + className + "::example_method(int param1, \n"
        "                                  int param2, \n"
        "                                  int param3) {\n"
        "    return param1 + param2 + param3;\n"
        "}\n";
    return content;
}

// 生成C头文件内容
std::string generate_c_header(const std::string& guardName) {
    std::string content = 
        "#ifndef " + guardName + "\n"
        "#define " + guardName + "\n\n"
        "/**\n"
        " * @brief 示例函数\n"
        " * @return int 状态码\n"
        " */\n"
        "int example_func(void);\n\n"
        "/**\n"
        " * @brief 复杂参数示例函数\n"
        " * @param param1 第一个参数\n"
        " * @param param2 第二个参数\n"
        " * @param param3 第三个参数\n"
        " * @return float 计算结果\n"
        " */\n"
        "float complex_func(int param1, \n"
        "                   long param2, \n"
        "                   double param3);\n\n"
        "#endif // " + guardName + "\n";
    return content;
}

// 生成C源文件内容
std::string generate_c_source(const std::string& fileName) {
    std::string content = 
        "#include \"" + fileName + ".h\"\n"
        "#include <stdio.h>\n\n"
        "/**\n"
        " * @brief 示例函数\n"
        " * @return int 状态码\n"
        " */\n"
        "int example_func(void) {\n"
        "    printf(\"Hello from C!\\n\");\n"
        "    return 0;\n"
        "}\n\n"
        "/**\n"
        " * @brief 复杂参数示例函数\n"
        " * @param param1 第一个参数\n"
        " * @param param2 第二个参数\n"
        " * @param param3 第三个参数\n"
        " * @return float 计算结果\n"
        " */\n"
        "float complex_func(int param1, \n"
        "                   long param2, \n"
        "                   double param3) {\n"
        "    return param1 + param2 + param3;\n"
        "}\n";
    return content;
}

int main(int argc, char* argv[]) {
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