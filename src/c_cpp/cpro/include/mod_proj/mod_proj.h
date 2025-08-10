#ifndef MOD_PROJ_H
#define MOD_PROJ_H

#include <string>

namespace cpro {

class ModProj {
private:
    std::string proj_name_;
    std::string proj_type_;
    std::string cmake_mini_req_ = "3.20";

public:
    ModProj(const std::string& proj_name, const std::string& proj_type);
    ~ModProj(void);
    void set_proj_name(const std::string& proj_name);
    std::string get_proj_name(void);
    void set_proj_type(const std::string& proj_type);
    std::string get_proj_type(void);

    void input_proj_info(void);
    void create_cmake_project(void);
};

} // namespace cpro

#endif //MOD_PROJ_H
