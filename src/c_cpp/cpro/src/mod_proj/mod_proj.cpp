#include "mod_proj.h"
#include <iostream>
#include <cstdlib>
#include "gen_template.h"

namespace cpro {

ModProj::ModProj(const std::string& proj_name, const std::string& proj_type) : proj_name_(proj_name), proj_type_(proj_type) {
}

ModProj::~ModProj(void) = default;

void ModProj::set_proj_name(const std::string& proj_name){
    this->proj_name_ = proj_name;
}

std::string ModProj::get_proj_name(void){
    return this->proj_name_;
}

void ModProj::set_proj_type(const std::string& proj_type){
    this->proj_type_ = proj_type;
}

std::string ModProj::get_proj_type(void){
    return this->proj_type_;
}

void ModProj::input_proj_info(void){
    do{
        std::cout << "Input proj name> ";
        std::cin >> this->proj_name_;
        if(this->proj_name_.empty()){
            std::cerr << "The input is invalid. Please re-enter." << std::endl;
        } else {
            break;
        }
    } while (true);

    do{
        std::cout << "Input proj type(c, c++)> ";
        std::cin >> this->proj_type_;
        if(this->proj_type_ == "c" || this->proj_type_ == "C"){
            this->proj_type_ = "c";
            break;
        } else if  (this->proj_type_ == "c++" || this->proj_type_ == "C++" || this->proj_type_ == "cpp" || this->proj_type_ == "CPP") {
            this->proj_type_ = "c++";
            break;
        } else {
            std::cerr << "The input is invalid. Please re-enter." << std::endl;
        }
    } while (true);

    std::cout << "=== NEW PROJECT INFO ===" << std::endl;
    std::cout << "01. proj name: " << this->proj_name_ << std::endl;
    std::cout << "02. proj type: " << this->proj_type_ << std::endl;
    std::cout << "========================" << std::endl;
}

void ModProj::create_cmake_project(void){
#ifdef _WIN32
    system(std::string("md " + this->proj_name_).c_str());
    system(std::string("md " + this->proj_name_ + "\\src").c_str());
    system(std::string("md " + this->proj_name_ + "\\include").c_str());
    system(std::string("md " + this->proj_name_ + "\\build").c_str());
    system(std::string("md " + this->proj_name_ + "\\bin").c_str());
    system(std::string("md " + this->proj_name_ + "\\doc").c_str());
    system(std::string("echo cmake_minimum_required(VERSION " + this->cmake_mini_req_ + ") > " + this->proj_name_ + "\\CMakeLists.txt").c_str());
    if(this->proj_type_ == "c" || this->proj_type_ == "C"){
        write_template_src(std::string(this->proj_name_ + "\\src\\main.cpp").c_str());
    } else {
        write_template_src(std::string(this->proj_name_ + "\\src\\main.cpp").c_str());
    }
#else
    system(std::string("mkdir " + this->proj_name_).c_str());
    system(std::string("mkdir " + this->proj_name_ + "/src").c_str());
    system(std::string("mkdir " + this->proj_name_ + "/include").c_str());
    system(std::string("mkdir " + this->proj_name_ + "/build").c_str());
    system(std::string("mkdir " + this->proj_name_ + "/bin").c_str());
    system(std::string("mkdir " + this->proj_name_ + "/doc").c_str());
    system(std::string("echo cmake_minimum_required(VERSION " + this->cmake_mini_req_ + ") > " + this->proj_name_ + "/CMakeLists.txt").c_str());
    if(this->proj_type_ == "c" || this->proj_type_ == "C"){
        write_template_src(std::string(this->proj_name_ + "/src/main.cpp").c_str());
    } else {
        write_template_src(std::string(this->proj_name_ + "/src/main.cpp").c_str());
    }
#endif
}


} // namespace cpro