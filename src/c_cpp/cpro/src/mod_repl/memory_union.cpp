#include "memory_union.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif

namespace cpro {

MemoryUnion::MemoryUnion(void){}
MemoryUnion::~MemoryUnion(void) = default;

std::string MemoryUnion::generate_union(const std::string& tag){
    std::string un;
    auto it = this->unions_.find(tag); // 查找键为2的元素
    if (it != this->unions_.end()) {
        un = "typedef union " + tag + "{\n" + it->second + "\n}" + tag + ";\n";
    } else {
        un = "";
    }
    return un;
}

void MemoryUnion::append(const std::string& tag, const std::string& body){
    this->unions_.insert({tag, body});
}

void MemoryUnion::edit_body(const std::string& tag){
    std::string un_body;
    auto it = this->unions_.find(tag);
    if (it != this->unions_.end()) {
        un_body = it->second;
    } else {
        std::cout << "union " << tag << " does not exist\n";
        return;
    }
    this->editor_.set_text(un_body);
    this->editor_.edit();
    this->unions_[tag] = this->editor_.get_text();  // 直接修改值
}

void MemoryUnion::remove(const std::string& tag){
    this->unions_.erase(tag); // 删除第三个元素
}

void MemoryUnion::list_tag(void){
    int index = 1;
    for (const auto& u : this->unions_) {
        binhub_cprintf((binhub_tcolor_t){CLR_BRT_YELLOW, CLR_NONE}, "%d\t", index);
        std::cout << u.first << std::endl;
        ++index;
    }
}

void MemoryUnion::print(const std::string& tag){
    const std::string un = this->generate_union(tag);
    if(un.empty()){
        std::cout << "Not found: union " << tag << std::endl;
    } else {
        std::cout << un << std::endl;
    }
}

void MemoryUnion::clear(void){
    this->unions_.clear();
}

} // namespace cpro