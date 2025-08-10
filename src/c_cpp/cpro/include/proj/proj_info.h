#ifndef PROJ_INFO_H
#define PROJ_INFO_H

#include <string>

namespace cpro {

/**
 * @brief 检查环境变量项目HOME目录并返回路径
 * @return 路径，若为空字符串即代表未配置环境变量
 */
std::string check_env(void);

/**
 * @brief 读取用户使用手册
 * @param path IN 用户使用手册文件路径
 * @return void
 */
void read_manual_from_disk(const char* path);


};

#endif