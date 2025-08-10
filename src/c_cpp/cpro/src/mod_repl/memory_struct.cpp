#include "memory_struct.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif

namespace cpro {

MemoryStruct::MemoryStruct(void){}
MemoryStruct::~MemoryStruct(void) = default;

std::string MemoryStruct::generate_struct(const std::string& tag){
    std::string st;
    auto it = this->structures_.find(tag); // 查找键为2的元素
    if (it != this->structures_.end()) {
        st = "typedef struct " + tag + "{\n" + it->second + "\n}" + tag + ";\n";
    } else {
        st = "";
    }
    return st;
}

void MemoryStruct::append(const std::string& tag, const std::string& body){
    this->structures_.insert({tag, body});
}

void MemoryStruct::edit_body(const std::string& tag){
    std::string st_body;
    auto it = this->structures_.find(tag);
    if (it != this->structures_.end()) {
        st_body = it->second;
    } else {
        std::cout << "struct " << tag << " does not exist\n";
        return;
    }
    this->editor_.set_text(st_body);
    this->editor_.edit();
    this->structures_[tag] = this->editor_.get_text();  // 直接修改值
}

void MemoryStruct::remove(const std::string& tag){
    this->structures_.erase(tag);
}

void MemoryStruct::list_tag(void){
    int index = 1;
    for (const auto& s : this->structures_) {
        binhub_cprintf((binhub_tcolor_t){CLR_BRT_YELLOW, CLR_NONE}, "%d\t", index);
        std::cout << s.first << std::endl;
        ++index;
    }
}

void MemoryStruct::print(const std::string& tag){
    const std::string st = this->generate_struct(tag);
    if(st.empty()){
        std::cout << "Not found: struct " << tag << std::endl;
    } else {
        std::cout << st << std::endl;
    }
}

void MemoryStruct::clear(void){
    this->structures_.clear();
}

} // namespace cpro