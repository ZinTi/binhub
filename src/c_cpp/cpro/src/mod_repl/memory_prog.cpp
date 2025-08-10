#include "memory_prog.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif
#include "help.h"
#include "match_regex.h"
#include "gen_template.h"
#include "str.h"

namespace cpro{

MemoryProg::MemoryProg(void) {}
MemoryProg::~MemoryProg(void) = default;

int MemoryProg::repl(int argc, char* argv[]){
    BINHUB_UNUSED(argc);
    BINHUB_UNUSED(argv);

    std::string input;

    do{
        binhub_cprintf((binhub_tcolor_t){CLR_BRT_CYAN, CLR_NONE}, "%s", this->prompt_.c_str()); // 打印提示符
        std::getline(std::cin, input); // 读取整行到字符串
        trim(input);    // 去除前导空格和尾随空格

        if(is_match(input.c_str(), R"(^\.(exit|quit)$)")){ // .exit, .quit
            binhub_cprintf((binhub_tcolor_t){CLR_BRT_CYAN, CLR_NONE}, "Bye!"); BINHUB_ENDLINE(1);
            return 0;
        } else if (is_match(input.c_str(), R"(^\.(help|manual)$)")){ // .help, .manual
            cpro::print_manual("repl");
        } else if (is_match(input.c_str(), R"(^\.renew$)")){
            this->handle_renew();
        } else if (is_match(input.c_str(), R"(^\.(system|shell|cmd)\s{1,}.{1,}$)")) { // 执行系统命令
            this->handle_system(input.c_str());
        } else if (is_match(input.c_str(), R"(^\.undo$)")) {
            this->handle_undo();
        } else if(is_match(input.c_str(), R"(^\.(cls|clear)$)")) {
            BINHUB_CLEAR_SCREEN;
        } else if(is_match(input.c_str(), R"(^\.inc.*)")) {    // 匹配以 ".inc" 为首的字符串
            this->handle_include_header(input);
        } else if(is_match(input.c_str(), R"(^\.macro\s+-\w+$)")) { // ".macro -option"
            this->handle_macro(input);
        } else if(is_match(input.c_str(), R"(^\.struct.*)")) {
            this->handle_struct(input);
        } else if(is_match(input.c_str(), R"(^\.union.*)")) {
            this->handle_union(input);
        } else if(is_match(input.c_str(), R"(^\.func.*)")) {

        } else if (is_match(input.c_str(), R"(^\..{1,}$)")) {
            binhub_cprintf((binhub_tcolor_t){CLR_RED, CLR_NONE}, "Invalid command!"); BINHUB_ENDLINE(1);
        }

    } while(true);

    // remove(D_TMP_SRC_FILENAME);
    remove(D_TMP_EXE_FILENAME);
    return 0;
}

int MemoryProg::handle_renew(void){
    remove(this->tmp_output_filename_.c_str());
    this->mem_headers_.clear();
    this->mem_macros_.clear();
    this->mem_functions_.clear();
    this->mem_structures_.clear();
    this->mem_unions_.clear();
    BINHUB_CLEAR_SCREEN;
    puts("-- renew --");
    return 0;
}

int MemoryProg::handle_system(const std::string& full_cmd){
    std::string system_cmd;
    size_t pos = full_cmd.find(' '); // 查找第一个空格的位置
    if (pos != std::string::npos) {
        system_cmd = full_cmd.substr(pos + 1); // 找到空格：截取 pos+1 至末尾的子串
    } else {
        system_cmd.clear(); // 未找到空格：system_cmd 留空
    }
    return system(system_cmd.c_str());
}

int MemoryProg::handle_undo(void){
    printf("pop the stack...\n");
    return 0;
}

int MemoryProg::handle_include_header(const std::string& full){
    bool remove = false;
    std::vector<std::string> tokens = split_string(full);
    if(tokens.size() == 1){    // 打印现有 headers
        mem_headers_.print("=== Headers ===");
        return 0;
    }

    std::vector<std::string> headers;

    for (const std::string& token : tokens) {
        const char first_char = token[0]; // 获取第一个字符
        const char last_char = token[token.size() - 1]; // 获取最后一个字符

        if(token == ".inc"){
            ;
        } else if (token == "-d"){
            remove = true;
        } else if ((first_char == '<' && last_char == '>') || (first_char == '\"' && last_char == '\"')){
            headers.push_back(token);
        } else {
            binhub_cprintf(binhub_tcolor_t{CLR_RED, CLR_NONE}, "Invalid arguments: %s", token.c_str()); BINHUB_ENDLINE(1);
        }
    }

    for (const std::string& h : headers) {
        if(remove){
            this->mem_headers_.erase(h);
        } else {
            this->mem_headers_.insert(h);
        }
    }
    return 0;
}


int MemoryProg::handle_macro(const std::string& full){
    std::vector<std::string> tokens = split_string(full);

    for (const std::string& token : tokens) {
        if(token == ".macro"){
            ;
        } else if (token == "-c"){
            this->mem_macros_.clear(); // 清除所有
        } else if (token == "-e") {
            this->mem_macros_.edit();
        } else if (token == "-p") {
            mem_macros_.print("=== MACROS ===");
        } else if (token == "-w") {

        } else {
            //binhub_cprintf(binhub_tcolor_t{CLR_RED, CLR_NONE}, "Invalid arguments: %s", token.c_str()); BINHUB_ENDLINE(1);
        }
    }

    return 0;
}

int MemoryProg::handle_struct(const std::string& full){
    return 0;
}

int MemoryProg::handle_union(const std::string& full){
    return 0;
}

int MemoryProg::handle_function(const std::string& full){
    return 0;
}


void MemoryProg::write_to_file(const std::string& output){
    BINHUB_UNUSED(output);
/*
    int len_inc_list = incList.get_len();
    int len_def_list = defList.get_len();
    int num_s_u = struct_union.get_len();
    int num_func = func_list_.get_len();
    int num_statements = mainbody.get_len();
    FILE* fp = fopen(filename.c_str(), mode);
    for (int i = 0; i < len_inc_list; i++){
        fprintf(fp, "%s\n", incList.get_elem(i));
    }
    for (int i = 0; i < len_def_list; i++){
        fprintf(fp, "%s\n", defList.get_elem(i));
    }
    for (int i = 0; i < num_s_u; i++){
        fprintf(fp, "%s\n", struct_union.get_elem(i));
    }
    for (int i = 0; i < num_func; i++){
        fprintf(fp, "%s\n", func_list_.get_elem(i));
    }
    fprintf(fp, "int main(int argc, char* argv[]){\n");
    for (int i = 0; i < num_statements; i++){
        fprintf(fp, "\t%s\n", mainbody.get_elem(i));
    }
    fprintf(fp, "\treturn 0;\n}\n");
    fclose(fp);*/
}



}