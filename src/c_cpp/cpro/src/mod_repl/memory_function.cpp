#include "memory_function.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif

namespace cpro{

MemoryFunction::MemoryFunction(void){}
MemoryFunction::~MemoryFunction(void) = default;

void MemoryFunction::append(void){
    this->editor_.edit();
    const std::string func = this->editor_.get_text();
    this->functions_.push_back(func);
}

void MemoryFunction::remove(int index){
    this->functions_.erase(this->functions_.begin() + index); // 删除第三个元素
}

void MemoryFunction::edit(int index){
    this->editor_.set_text(this->functions_[index]);
    this->editor_.edit();
    std::string new_func = this->editor_.get_text();

    try {
        this->functions_.at(index) = new_func; // 使用at()方法更新（会检查边界）
        // 若尝试访问越界索引会抛出 std::out_of_range 异常
    } catch (const std::out_of_range& e) {
        std::cout << "索引越界错误: " << e.what() << std::endl;
    }
}

void MemoryFunction::print_size(void){
    std::cout << this->functions_.size() << "\n";
}

void MemoryFunction::print(int index){
    try {
        std::cout << this->functions_.at(index) << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "索引越界错误: " << e.what() << std::endl;
    }
}

void MemoryFunction::print_all(void){
    int index = 0;
    for (auto it = this->functions_.begin(); it != this->functions_.end(); ++it) {
        binhub_cprintf((binhub_tcolor_t){CLR_BRT_YELLOW, CLR_NONE}, "=== index: %d ===", index);
        std::cout << "\n" << *it << std::endl;
        ++index;
    }
}

void MemoryFunction::clear(void){
    this->functions_.clear();
}

};


