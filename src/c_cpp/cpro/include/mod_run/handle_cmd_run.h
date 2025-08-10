#ifndef HANDLE_COMMAND_RUN_H
#define HANDLE_COMMAND_RUN_H

#include <utility> // std::pair
#include <string>
#include <set>
#include <vector>



namespace cpro {

int handle_cmd_run(const int argc, char* argv[]);

class RunOptions {
private:
    std::pair<bool, std::string> sub_args_; // enable, values
    std::pair<bool, std::string> output_;    // enable, output_name
    std::set<std::string> sources_;    // sources (若为空则检测当前目录下的所有 .c 文件)

    void input_sources_from_dir(void);

public:
    RunOptions(void);
    ~RunOptions(void);

    std::string get_sub_args(void);
    std::string get_output(void);
    std::set<std::string> get_sources(void);

    /**
     * @brief 分析参数
     * @return 0
     */
    int analyze_args(int argc, char* argv[]);

}; // class RunOptions

} // namespace cpro

#endif //HANDLE_COMMAND_RUN_H
