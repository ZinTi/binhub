#ifndef SETCHARACTERENCODE_H
#define SETCHARACTERENCODE_H

/**
 * @brief 修改 Windows 命令行的活动代码页 CodePage
 * @param newInputCP 输入代码页
 * @param newOutputCP 输出代码页
 * @return
 */
int changeCodePage(unsigned int newInputCP, unsigned int newOutputCP);

/**
 * @brief 猜测数据的字符集编码是否为 UTF-8 格式
 * @param data 判断目标
 * @param len 数据长度
 * @return 如果返回值为 1 代表可能是 UTF-8 ，如果为 0 代表不太可能为 UTF-8
 */
int isDataUTF8(const char* data, int len);

/**
 * @brief 根据目标源文件自动选择活动代码页 CodePage
 * @param filename 目标文件名
 * @return
 */
int automaticallySelectCodePage(const char* filename);

#endif //SETCHARACTERENCODE_H
