#include "memory_macro.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif

namespace cpro{

MemoryMacro::MemoryMacro(void){

}

MemoryMacro::~MemoryMacro(void) = default;


void MemoryMacro::append(const std::string& macro){
    this->macros_ = this->macros_ + "\n" + macro;
}

void MemoryMacro::edit(void){
    // std::cout << "Starting text editor...\n";
    // std::cout << "Press Ctrl+S to save, Ctrl+Q to quit without saving, Tab to insert 4 spaces.\n";
    this->editor_.set_text(this->macros_);
    this->editor_.edit();
    this->macros_ = this->editor_.get_text();
}

void MemoryMacro::clear(void){
    this->macros_ = "";
}

void MemoryMacro::print(const std::string& title){
    std::cout << title << ":\n";
    std::cout << std::string(40, '-') << "\n";
    std::cout << this->macros_;
    std::cout << std::string(40, '-') << "\n\n";
}

void MemoryMacro::write_to_file(const std::string& file){
    BINHUB_UNUSED(file);
}

} // namespace cpro