#ifndef FIXED_TEMPLATE_H
#define FIXED_TEMPLATE_H

#include <iostream>
#include <string>

// 生成C++头文件内容
std::string generate_cpp_header(const std::string& className, const std::string& guardName);

// 生成C++源文件内容
std::string generate_cpp_source(const std::string& className, const std::string& fileName);

// 生成C头文件内容
std::string generate_c_header(const std::string& guardName);

// 生成C源文件内容
std::string generate_c_source(const std::string& fileName);

#endif
