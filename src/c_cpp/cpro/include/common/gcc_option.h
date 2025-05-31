#ifndef GCC_OPTION_H
#define GCC_OPTION_H

#include <string>
#include <unordered_set>
#include <unordered_map>

namespace cpro {

class GCCOption {
private:
	std::string compiler_;
	bool syntax_only_; // -fsyntax-only 只检查语法，不进行编译
	std::string output_;	// -o <file>
	std::string language_; // -x <language> {c, c++, assembler, none}

	std::unordered_set<std::string> src_files_;	// 若为空则使用内存源代码即 "-"（stdin），并忽略 inc_dirs_
	std::unordered_set<std::string> inc_dirs_; // -I<dir> 添加头文件搜索路径
	std::unordered_set<std::string> lib_dirs_; // -L<dir> 添加库搜索路径
	std::unordered_set<std::string> lib_names_; // -l<name> 链接库名称
	std::unordered_map<std::string, std::string> macros_; // -D<name>=<value> 定义宏

    bool g_; // -g 生成调试信息
    bool Wall_; // -Wall 开启所有警告
    char optimization_level_; // -O<level> 优化等级 {0, 1, 2, 3, s}

public:
	GCCOption(void);
	~GCCOption(void);

	std::string generate_command(void) const;
	void print_command(void) const;

	void set_compiler(const std::string& compiler);
	void set_syntax_only(bool syntax_only);
    void set_output(const std::string& output);
	void set_language(const std::string& language = "none");

    void append_src_files(const std::string& src_file);
    void erase_src_files(const std::string& src_file);
    void clear_src_files(void);

    void append_inc_dirs(const std::string& inc_dir);
	void erase_inc_dirs(const std::string& inc_dir);
	void clear_inc_dirs(void);

    void append_lib_dirs(const std::string& lib_dir);
	void erase_lib_dirs(const std::string& lib_dir);
	void clear_lib_dirs(void);

    void append_lib_names(const std::string& lib_name);
	void erase_lib_names(const std::string& lib_name);
	void clear_lib_names(void);

    void append_macros(const std::string& name, const std::string& value);
	void erase_macros(const std::string& name);
	void clear_macros(void);

    void set_g(bool g);
	void set_Wall(bool w);
	void set_optimization_level(char level);

};

} // namespace cpro

#endif //GCC_OPTION_H
