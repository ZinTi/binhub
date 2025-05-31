#include "win_res.h"
#include <cstdlib>

#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif

namespace cpro {

WinRes::WinRes(void) {}
WinRes::WinRes(const std::string& ico_fn) : ico_fn_(ico_fn){}
WinRes::WinRes(const std::string& ico_fn, const std::string& res_fn) : ico_fn_(ico_fn), res_fn_(res_fn){}
WinRes::~WinRes() = default;

void WinRes::set_ico_fn(const std::string& ico_fn){
    this->ico_fn_ = ico_fn;
}

void WinRes::set_res_fn(const std::string& res_fn){
    this->res_fn_ = res_fn;
}

std::string WinRes::get_ico_fn(void) const {
    return this->ico_fn_;
}

std::string WinRes::get_res_fn(void) const {
    return this->res_fn_;
}

int WinRes::generate_res_file(void){
    // 1. Check if the ico filename is empty
    if (this->ico_fn_.empty()){
        binhub_cerr("The ico filename is empty");
        return -1;
    }

    // 2. 检查是否以 ".ico" 结尾 或 末尾有 "."
    std::string file_no_ext;    // 不带扩展名的文件名
    size_t len = this->ico_fn_.length();

    if (len >= 4 && this->ico_fn_.substr(len - 4) == ".ico") {
        file_no_ext = this->ico_fn_.substr(0, len - 4);
    } else if (!this->ico_fn_.empty() && this->ico_fn_.back() == '.') {
        file_no_ext = this->ico_fn_.substr(0, len - 1);
    } else {
        file_no_ext = this->ico_fn_;
    }

    // 3. Set the ico, rc and res filenames
    this->ico_fn_ = file_no_ext + ".ico";
    std::string rc_fn = file_no_ext + ".rc";
    if(this->res_fn_.empty()){    // 未指定输出文件名
        this->res_fn_ = file_no_ext + ".res";
    }

    // 4. Write .rc file
    FILE* fp = fopen(rc_fn.c_str(), "w");
    if (!fp){
        binhub_cperror("Failed to open .rc file");
        exit(-1);
    }
    fprintf(fp, "A ICON \"%s\"", this->ico_fn_.c_str());
    fclose(fp);

    // 5. Convert to .res
    std::string cmd_str = "windres -i " + rc_fn + " --input-format=rc -o "  + this->res_fn_ + " -O coff";
    system(cmd_str.c_str());

    // 6. Clean up
    remove(rc_fn.c_str()); // 删除 .rc 文件

    return 0;
}

} // namespace cpro