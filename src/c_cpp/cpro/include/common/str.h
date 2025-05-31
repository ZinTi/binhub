#ifndef STR_H
#define STR_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cctype>

/**
 * @brief 清除左右空格
 * @param str 要处理的字符串目标
 */
void trim(std::string &str);

/**
 * @brief 字符串分割函数，将字符串按空白字符分割并存储到std::vector<std::string>容器中
 * @param str 分割目标
 * @return 字符串容器
 */
std::vector<std::string> split_string(const std::string& str);

/**
 * @brief 修改文件扩展名得到一个新的文件名
 * @param old_fn 原文件名
 * @param new_extension 新文件扩展名（若原文件名无扩展名，拼接到原文件名尾部得到新文件名）
 * @return 返回新文件名
 */
std::string change_filename_extension(const std::string& old_fn, const char* new_extension);

#endif
