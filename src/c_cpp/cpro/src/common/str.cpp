#include "str.h"

// 清除左右空格
void trim(std::string &str) {
    // 删除左侧空格
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));

    // 删除右侧空格
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), str.end());
}

std::vector<std::string> split_string(const std::string& str) {
    std::vector<std::string> tokens;
    auto start = str.begin();

    while (start != str.end()) {
        // 跳过开头的空白字符
        while (start != str.end() && std::isspace(static_cast<unsigned char>(*start))) {
            ++start;
        }
        // 如果没有非空白字符则退出
        if (start == str.end()) break;

        // 标记单词的起始位置
        auto end = start;
        // 找到单词结束位置（遇到空白或字符串结尾）
        while (end != str.end() && !std::isspace(static_cast<unsigned char>(*end))) {
            ++end;
        }

        // 将单词存入容器
        tokens.emplace_back(start, end);
        // 继续处理剩余字符串
        start = end;
    }
    return tokens;
}

std::string change_filename_extension(const std::string& old_fn, const char* new_extension) {
    // 处理空指针情况
    if (new_extension == nullptr) {
        new_extension = "";
    }

    // 查找最后一个路径分隔符（支持Unix和Windows路径）
    size_t last_sep = old_fn.find_last_of("/\\");
    // 查找最后一个点（扩展名分隔符）
    size_t last_dot = old_fn.find_last_of('.');

    // 确定文件名部分起始位置
    size_t fn_start = (last_sep == std::string::npos) ? 0 : last_sep + 1;

    // 检查是否应保留原扩展名点号
    bool keep_dot = false;
    if (last_dot != std::string::npos) {
        // 点必须在文件名部分且不在开头位置
        if (last_dot > fn_start && last_dot > last_sep) {
            keep_dot = true;
        }
    }

    // 构建新扩展名（确保以点开头）
    std::string new_ext = "";
    if (new_extension[0] != '\0') {  // 非空扩展名
        if (new_extension[0] == '.') {
            new_ext = new_extension;
        } else {
            new_ext = '.';
            new_ext += new_extension;
        }
    }

    // 根据是否保留原扩展名点号处理主文件名
    if (keep_dot) {
        return old_fn.substr(0, last_dot) + new_ext;
    } else {
        return old_fn + new_ext;
    }
}

