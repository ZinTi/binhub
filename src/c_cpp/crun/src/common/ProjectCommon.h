#ifndef PROJECTCOMMON_H
#define PROJECTCOMMON_H

#include <string>

namespace crun {

class ProjectCommon {
public:
    /**
     * @brief 检查环境变量项目HOME目录并返回路径
     * @return 路径，若为空字符串即代表未配置环境变量
     */
    static std::string checkEnv();

    /**
     * @brief 读取用户使用手册
     * @param path IN 用户使用手册文件路径
     * @return void
     */
    static void ReadManual(const char* path);

    /**
     * @brief
     * @param
     * @param
     * @return
     */
    static int ConvertHex(char* hexadecimalStr, long* pDecimal);

    /**
     * @brief
     * @param
     * @param
     * @return
     */
    static int PutStr(const char* Str);

    /**
     * @brief
     * @param
     * @param
     * @return
     */
    static int ColorPrintf(unsigned short color, const char* format, ...);

    /**
     * @brief
     * @param
     * @param
     * @return
     */
    static int ColorPuts(unsigned short color, const char* Str, unsigned short line);

    /**
     * @brief
     * @param
     * @param
     * @return
     */
    static int ChColor(unsigned short color, unsigned short* pOldColor);

    /**
     * @brief
     * @param
     * @param
     * @return
     */
    static int ChCP(unsigned int newInputCP, unsigned int newOutputCP);

    /**
     * @brief
     * @param
     * @param
     * @return
     */
    static int IsUTF8(const char* data, int len); // 0不是UTF-8, 1是UTF-8

    /**
     * @brief
     * @param
     * @param
     * @return
     */
    static int AutomaticallySelectCodePage(const char* filename);

    /**
     * @brief
     * @param
     * @param
     * @return
     */
    static void StrToUppercase(char *str);

    /**
     * @brief
     * @param
     * @param
     * @return
     */
    static void StrToLowercase(char *str);

    /**
     * @brief
     * @param
     * @param
     * @return
     */
    static int StrCmpIgnoreCase(const char *str1, const char *str2);

    /**
     * @brief
     * @param
     * @param
     * @return
     */
    static int StrNCmpIgnoreCase(const char *str1, const char *str2, unsigned long long n);

    /**
     * @brief
     * @param
     * @param
     * @return
     */
    static void LoadLoop(const char* text, int style);
};

} // crun

#endif //PROJECTCOMMON_H
