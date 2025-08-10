#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#endif

namespace cpro {

    class TextEditor {
    private:
        std::vector<std::string> lines;
        int cursor_row;
        int cursor_col;
        bool editing;
        int screen_rows;
        int screen_cols;
        int first_visible_row;  // 当前可见的第一行
        int line_num_width;     // 行号区域宽度

        // 保存原始终端设置（用于Linux）
#ifndef _WIN32
        struct termios orig_termios;
#endif

        // 设置终端为原始模式
        void setup_terminal();

        // 恢复终端设置
        void restore_terminal();

        // 获取终端大小
        void get_terminal_size();

        // 清屏
        void clear_screen();

        // 移动光标到指定位置
        void move_cursor(int row, int col);

        // 确保光标在可见区域内
        void ensure_cursor_visible();

        // 显示编辑器界面
        void display_editor();

        // 处理键盘输入
        void process_input();

    public:
        TextEditor();

        void edit();

        std::vector<std::string> get_lines(void);

        std::string get_text(void) const;

        void set_lines(const std::vector<std::string>& value);

        void set_text(const std::string& text);

    }; // class TextEdit

} // namespace cpro

#endif //TEXT_EDITOR_H
