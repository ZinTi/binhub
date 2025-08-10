#ifndef WIN_RES_H
#define WIN_RES_H

#include <string>

namespace cpro {

class WinRes {
private:
    std::string ico_fn_;
    std::string res_fn_;

public:
    // Constructor && Destructor
    WinRes(void);
    WinRes(const std::string& ico_fn);
    WinRes(const std::string& ico_fn, const std::string& res_fn);
    ~WinRes(void);

    /**
     * @brief 设置 icon_filename.ico 文件名
     * @param ico_fn 以 .ico 结尾的 Windows icon 文件名
     */
    void set_ico_fn(const std::string& ico_fn);

    /**
     * @brief 设置 res_filename.res 文件名
     * @param res_fn 以 .res 结尾的 Windows resource 文件名
     */
    void set_res_fn(const std::string& res_fn);

    /**
     * @brief 获取 ico_filename 文件名
     * @return ico_fn_
     */
    std::string get_ico_fn(void) const;

    /**
     * @brief 获取 res_filename 文件名
     * @return res_fn_
     */
    std::string get_res_fn(void) const;

    /**
     * @brief 生成 Windows 资源文件
     * @return 返回 0 表示成功
     */
    int generate_res_file(void);
};

};

#endif