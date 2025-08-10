#include "gcc_option.h"
#include <iostream>

namespace cpro {

GCCOption::GCCOption(void){
#if defined(_WIN32) || defined(_WIN64)
    this->compiler_ = "gcc.exe";
    this->output_ = "a.exe";
#else
    this->compiler_ = "gcc";
    this->output_ = "a.out";
#endif

    this->syntax_only_ = false;

    this->language_ = "c";
    this->g_ = false;
    this->Wall_ = false;
    this->optimization_level_ = '0';
}

GCCOption::~GCCOption(void) = default;


std::string GCCOption::generate_command(void) const{
    std::string command = this->compiler_;
    if(this->syntax_only_){
        command += " -fsyntax-only";
    }
    if(!this->output_.empty()){
        command += " -o " + this->output_;
    }
    if(!this->language_.empty()){ // 语言选项放在所有源文件之前
        command += " -x " + this->language_;
    }
    if(this->src_files_.empty()){
        command += " -";
    } else {
        for(auto& src_file : this->src_files_){
            command += " " + src_file;
        }
    }
    if(!this->inc_dirs_.empty()){
        for(auto& inc_dir : this->inc_dirs_){
            command += " -I" + inc_dir;
        }
    }
    if(false == this->lib_dirs_.empty()){
        for(auto& lib_dir : this->lib_dirs_){
            command += " -L" + lib_dir;
        }
    }
    if(false == this->lib_names_.empty()){
        for(auto& lib_name : this->lib_names_){
            command += " -l" + lib_name;
        }
    }
    if(false == this->macros_.empty()){
        for(auto& macro : this->macros_){
            command += " -D" + macro.first + "=" + macro.second;
        }
    }
    if(this->g_){
        command += " -g";
    }
    if(this->Wall_){
        command += " -Wall";
    }
    if(this->optimization_level_!= '0'){
        command += " -O" + this->optimization_level_;
    }
    return command;
}

void GCCOption::print_command(void) const{
    std::cout << this->generate_command() << std::endl;
}

void GCCOption::set_compiler(const std::string& compiler){
    this->compiler_ = compiler;
}

void GCCOption::set_syntax_only(bool syntax_only){
    this->syntax_only_ = syntax_only;
}

void GCCOption::set_output(const std::string& output){
    this->output_ = output;
}

void GCCOption::set_language(const std::string& language){
    if(language == "c" || language == "c++" || language == "assembler" || language == "none"){
        this->language_ = language;
    } else {
        std::cout << "Invalid language: " << language << "\n";
    }
}


void GCCOption::append_src_files(const std::string& src_file){
    this->src_files_.insert(src_file);
}

void GCCOption::erase_src_files(const std::string& src_file){
    auto src_files_it = this->src_files_.find(src_file);
    if (src_files_it != this->src_files_.end()) {
        this->src_files_.erase(src_files_it);
    } else {
        std::cout << src_file << " not found\n";
    }
}

void GCCOption::clear_src_files(void){
    this->src_files_.clear();
}

void GCCOption::append_inc_dirs(const std::string& inc_dir){
    this->inc_dirs_.insert(inc_dir);
}

void GCCOption::erase_inc_dirs(const std::string& inc_dir){
    auto inc_dirs_it = this->inc_dirs_.find(inc_dir);
    if (inc_dirs_it != this->inc_dirs_.end()) {
        this->inc_dirs_.erase(inc_dirs_it);
    } else {
        std::cout << inc_dir << " not found\n";
    }
}

void GCCOption::clear_inc_dirs(void){
    this->inc_dirs_.clear();
}

void GCCOption::append_lib_dirs(const std::string& lib_dir){
    this->lib_dirs_.insert(lib_dir);
}

void GCCOption::erase_lib_dirs(const std::string& lib_dir){
    auto lib_dirs_it = this->lib_dirs_.find(lib_dir);
    if (lib_dirs_it != this->lib_dirs_.end()) {
        this->lib_dirs_.erase(lib_dirs_it);
    } else {
        std::cout << lib_dir << " not found\n";
    }
}

void GCCOption::clear_lib_dirs(void){
    this->lib_dirs_.clear();
}

void GCCOption::append_lib_names(const std::string& lib_name){
    this->lib_names_.insert(lib_name);
}

void GCCOption::erase_lib_names(const std::string& lib_name){
    auto lib_names_it = this->lib_names_.find(lib_name);
    if (lib_names_it != this->lib_names_.end()) {
        this->lib_names_.erase(lib_names_it);
    } else {
        std::cout << lib_name << " not found\n";
    }
}

void GCCOption::clear_lib_names(void){
    this->lib_names_.clear();
}

void GCCOption::append_macros(const std::string& name, const std::string& value){
    this->macros_.insert({name, value});
}

void GCCOption::erase_macros(const std::string& name){
    auto macros_it = this->macros_.find(name); // 查找键为2的元素
    if (macros_it != this->macros_.end()) {
        this->macros_.erase(macros_it);
    } else {
        std::cout << name << " not found\n";
    }
}

void GCCOption::clear_macros(void){
    this->macros_.clear();
}

void GCCOption::set_g(bool g){
    this->g_ = g;
}

void GCCOption::set_Wall(bool w){
    this->Wall_ = w;
}

void GCCOption::set_optimization_level(char level){
    if ((level >= '0' && level <= '3') || level == 's' ) {
        this->optimization_level_ = level;
    } else {
        std::cout << "Invalid optimization level: " << level << "\n";
    }
}

} // namespace cpro