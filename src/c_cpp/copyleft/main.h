#ifndef MAIN_H
#define MAIN_H

#define PROJ_NAME            "AddCopyleft"
#define PROJ_VER             "2025.02.10-01"
#define PROJ_YEARS           "2023-2025"
#define AUTHOR               "Mr. ZENG Lai"
#define AUTHOR_CONTACT       "vip201@126.com"

/**
 * @brief 打印指南
 * @param mPath 可执行程序的文件所在路径（用于组成完整路径）
 */
void printManual(const char* mPath);

/**
 * @brief 在文件头部新增版权注释信息
 * @param srcFilename 目标文件
 * @return 返回 0 成功
 */
int addCopyleft(const char* srcFilename);


#endif //MAIN_H
