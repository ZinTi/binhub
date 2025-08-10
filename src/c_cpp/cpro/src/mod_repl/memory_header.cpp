#include "memory_header.h"
#include <iostream>
#include <iomanip>
#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif
#include "memory_gcc_compiler.h"

namespace cpro {

MemoryHeader::MemoryHeader(void){
    this->headers_ = {"<stdio.h>", "<stdlib.h>", "<string.h>", "<stdint.h>", "<stdbool.h>", "<stddef.h>", "<math.h>", "<time.h>"};
}

MemoryHeader::~MemoryHeader(void){

}


bool MemoryHeader::test_header_include(const std::string& header){
#if defined(_WIN32) || defined(_WIN64)
    MemoryGCCCompiler mgcc_compiler;
    mgcc_compiler.set_syntax_only(true);
    std::string src_code = "#include " + header;
    mgcc_compiler.set_mem_src_code(src_code);
    mgcc_compiler.set_mem_src_code(header);
    const int result = mgcc_compiler.compile();
#else
    // echo "#include <stdio.h>" | gcc -fsyntax-only -x c - >/dev/null 2>&1 && echo "√" || echo "×"
    std::string command = "echo \"#include " + header + "\" | gcc -fsyntax-only -x c - >/dev/null 2>&1";
    const int result = system(command.c_str()); // 执行命令
#endif
    return (result == 0); // 检查结果
}

void MemoryHeader::insert(const std::string& header){
    if((header.front() == '<' && header.back() == '>') || (header.front() == '\"' && header.back() == '\"')){
        std::string test_inc = "#include " + header;
        bool success = test_header_include(test_inc);
        if(success){
            binhub_cprintf((binhub_tcolor_t){CLR_GREEN, CLR_NONE}, "ok"); BINHUB_ENDLINE(1);
        } else {
            return;
        }
    } else {
        binhub_cprintf((binhub_tcolor_t){CLR_RED, CLR_NONE}, "Error: Invalid library name format."); BINHUB_ENDLINE(1);
        return;
    }
    this->headers_.insert(header);
}

void MemoryHeader::erase(const std::string& header){
    auto header_it = this->headers_.find(header);
    if (header_it != this->headers_.end()) {
        this->headers_.erase(header_it);
    } else {
        std::cout << header << " not found\n";
    }
}

int MemoryHeader::find(const std::string& header){
    auto header_it = this->headers_.find(header);
    if (header_it != this->headers_.end()) {
        std::cout << "Found: " << *header_it << "\n";
        return 0;
    } else {
        std::cout << header << " not found\n";
        return -1;
    }
}

int MemoryHeader::get_size(void){
    return this->headers_.size();
}

bool MemoryHeader::is_empty(void){
    return this->headers_.empty();
}

void MemoryHeader::clear(void){
    this->headers_.clear();
}

// 打印 headers_;
void MemoryHeader::print(const std::string& title) {
    std::cout << title << ":\n";
    std::cout << std::string(40, '-') << "\n";
    int count = 1;
    for (const auto& item : this->headers_) {
        std::cout << std::setw(2) << count++ << ": " << item << "\n";
    }
    std::cout << std::string(40, '-') << "\n\n";
}

void MemoryHeader::write_to_file(const std::string& file){
    BINHUB_UNUSED(file);
}

} // namespace cpro