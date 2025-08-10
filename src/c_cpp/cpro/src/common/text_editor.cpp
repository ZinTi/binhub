#include "text_editor.h"

namespace cpro {

    // 设置终端为原始模式
    void TextEditor::setup_terminal() {
#ifdef _WIN32
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        GetConsoleMode(hStdin, &mode);
        SetConsoleMode(hStdin, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
#else
        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ICANON | ECHO);
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 1;
        // 禁用流量控制（Ctrl+S/Ctrl+Q）
        raw.c_iflag &= ~(IXON | IXOFF);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
#endif
    }

    // 恢复终端设置
    void TextEditor::restore_terminal() {
#ifdef _WIN32
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        GetConsoleMode(hStdin, &mode);
        SetConsoleMode(hStdin, mode | ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
#else
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
#endif
    }

    // 获取终端大小
    void TextEditor::get_terminal_size() {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        screen_cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        screen_rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        screen_cols = w.ws_col;
        screen_rows = w.ws_row;
#endif
    }

    // 清屏
    void TextEditor::clear_screen() {
#ifdef _WIN32
        COORD coord = {0, 0};
        DWORD count;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(hStdout, &csbi);
        FillConsoleOutputCharacter(hStdout, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
        SetConsoleCursorPosition(hStdout, coord);
#else
        std::cout << "\033[2J\033[1;1H";
#endif
    }

    // 移动光标到指定位置
    void TextEditor::move_cursor(int row, int col) {
#ifdef _WIN32
        COORD coord = {static_cast<SHORT>(col), static_cast<SHORT>(row)};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
        std::cout << "\033[" << row + 1 << ";" << col + 1 << "H";
        std::cout.flush();
#endif
    }

    // 确保光标在可见区域内
    void TextEditor::ensure_cursor_visible() {
        // 更新行号宽度
        line_num_width = std::to_string(lines.size()).size() + 3; // 行号 + " | "

        // 水平边界检查
        if (cursor_col < 0) cursor_col = 0;
        if (cursor_col > lines[cursor_row].size()) cursor_col = lines[cursor_row].size();

        // 垂直边界检查
        if (cursor_row < 0) cursor_row = 0;
        if (cursor_row >= lines.size()) cursor_row = lines.size() - 1;

        // 调整可见区域
        if (cursor_row < first_visible_row) {
            first_visible_row = cursor_row;
        } else if (cursor_row >= first_visible_row + screen_rows - 1) {
            first_visible_row = cursor_row - screen_rows + 2;
            if (first_visible_row < 0) first_visible_row = 0;
        }
    }

    // 显示编辑器界面
    void TextEditor::display_editor() {
        clear_screen();
        ensure_cursor_visible();

        // 显示文本内容
        int visible_rows = std::min(screen_rows - 1, static_cast<int>(lines.size()) - first_visible_row);

        for (int i = 0; i < visible_rows; i++) {
            int line_idx = first_visible_row + i;
            std::string line = lines[line_idx];

            // 显示行号（右对齐）
            std::cout << "\033[34m"; // 蓝色
            std::string num_str = std::to_string(line_idx + 1);
            std::cout << std::string(line_num_width - num_str.size() - 3, ' ')
                      << num_str << " | ";
            std::cout << "\033[0m"; // 重置颜色

            // 处理长行换行显示
            int col = 0;
            int content_width = screen_cols - line_num_width;

            while (col < line.size()) {
                int segment_length = std::min(content_width, static_cast<int>(line.size()) - col);
                std::cout << line.substr(col, segment_length);
                col += segment_length;

                if (col < line.size()) {
                    // 换行显示续行
                    std::cout << "\n";
                    // 续行显示特殊标记
                    std::cout << std::string(line_num_width - 4, ' ') << "~ | ";
                }
            }

            if (i < visible_rows - 1) {
                std::cout << "\n";
            }
        }

        // 显示帮助信息
        move_cursor(screen_rows - 1, 0);
        std::cout << "\033[44m\033[37m"; // 蓝底白字
        std::cout << "Ctrl+S: Save and Exit | Ctrl+Q: Quit without saving | Tab: Insert 4 spaces";
        std::cout << "\033[0m"; // 重置样式
        std::cout.flush();

        // 定位光标到正确位置
        int display_row = cursor_row - first_visible_row;
        int display_col = cursor_col + line_num_width;

        // 考虑换行显示的情况
        int content_width = screen_cols - line_num_width;
        if (content_width > 0) {
            display_row += cursor_col / content_width;
            display_col = line_num_width + (cursor_col % content_width);
        }

        // 确保光标不会出现在状态栏上
        if (display_row >= screen_rows - 1) {
            display_row = screen_rows - 2;
        }

        move_cursor(display_row, display_col);
        std::cout.flush();
    }

    // 处理键盘输入
    void TextEditor::process_input() {
        while (editing) {
            display_editor();

#ifdef _WIN32
            int ch = _getch();
            if (ch == 0 || ch == 224) { // 特殊键
                ch = _getch(); // 获取扩展码
                switch (ch) {
                    case 72: // 上箭头
                        if (cursor_row > 0) {
                            cursor_row--;
                            cursor_col = std::min(cursor_col, static_cast<int>(lines[cursor_row].size()));
                        }
                        break;
                    case 80: // 下箭头
                        if (cursor_row < lines.size() - 1) {
                            cursor_row++;
                            cursor_col = std::min(cursor_col, static_cast<int>(lines[cursor_row].size()));
                        }
                        break;
                    case 75: // 左箭头
                        if (cursor_col > 0) cursor_col--;
                        break;
                    case 77: // 右箭头
                        if (cursor_col < lines[cursor_row].size()) cursor_col++;
                        break;
                    case 73: // Page Up
                        cursor_row = std::max(0, cursor_row - (screen_rows - 1));
                        cursor_col = std::min(cursor_col, static_cast<int>(lines[cursor_row].size()));
                        break;
                    case 81: // Page Down
                        cursor_row = std::min(static_cast<int>(lines.size()) - 1, cursor_row + (screen_rows - 1));
                        cursor_col = std::min(cursor_col, static_cast<int>(lines[cursor_row].size()));
                        break;
                }
            } else if (ch == 13) { // 回车
                if (cursor_row < lines.size() - 1) {
                    std::string remainder = lines[cursor_row].substr(cursor_col);
                    lines[cursor_row] = lines[cursor_row].substr(0, cursor_col);
                    lines.insert(lines.begin() + cursor_row + 1, remainder);
                } else {
                    lines.push_back("");
                }
                cursor_col = 0;
                cursor_row++;
            } else if (ch == 8) { // 退格
                if (cursor_col > 0) {
                    lines[cursor_row].erase(cursor_col - 1, 1);
                    cursor_col--;
                } else if (cursor_row > 0) {
                    cursor_col = lines[cursor_row - 1].size();
                    lines[cursor_row - 1] += lines[cursor_row];
                    lines.erase(lines.begin() + cursor_row);
                    cursor_row--;
                }
            } else if (ch == 9) { // Tab键
                lines[cursor_row].insert(cursor_col, 4, ' ');
                cursor_col += 4;
            } else if (ch == 3) { // Ctrl+C
                editing = false;
            } else if (ch == 19) { // Ctrl+S
                editing = false;
            } else if (ch == 17) { // Ctrl+Q
                lines.clear();
                editing = false;
            } else if (ch >= 32 && ch <= 126) { // 可打印字符
                lines[cursor_row].insert(cursor_col, 1, static_cast<char>(ch));
                cursor_col++;
            }
#else
            char ch;
            if (read(STDIN_FILENO, &ch, 1) <= 0) continue;

            if (ch == '\033') { // 转义序列
                char seq[3];
                if (read(STDIN_FILENO, &seq[0], 1) <= 0) continue;
                if (seq[0] == '[') {
                    if (read(STDIN_FILENO, &seq[1], 1) <= 0) continue;

                    if (seq[1] >= '0' && seq[1] <= '9') {
                        // 处理带数字的转义序列
                        if (read(STDIN_FILENO, &seq[2], 1) <= 0) continue;
                        if (seq[2] == '~') {
                            switch (seq[1]) {
                                case '5': // Page Up
                                    cursor_row = std::max(0, cursor_row - (screen_rows - 1));
                                    cursor_col = std::min(cursor_col, static_cast<int>(lines[cursor_row].size()));
                                    break;
                                case '6': // Page Down
                                    cursor_row = std::min(static_cast<int>(lines.size()) - 1, cursor_row + (screen_rows - 1));
                                    cursor_col = std::min(cursor_col, static_cast<int>(lines[cursor_row].size()));
                                    break;
                            }
                        }
                    } else {
                        switch (seq[1]) {
                            case 'A': // 上箭头
                                if (cursor_row > 0) {
                                    cursor_row--;
                                    cursor_col = std::min(cursor_col, static_cast<int>(lines[cursor_row].size()));
                                }
                                break;
                            case 'B': // 下箭头
                                if (cursor_row < lines.size() - 1) {
                                    cursor_row++;
                                    cursor_col = std::min(cursor_col, static_cast<int>(lines[cursor_row].size()));
                                }
                                break;
                            case 'C': // 右箭头
                                if (cursor_col < lines[cursor_row].size()) cursor_col++;
                                break;
                            case 'D': // 左箭头
                                if (cursor_col > 0) cursor_col--;
                                break;
                        }
                    }
                }
            } else if (ch == '\n') { // 回车
                if (cursor_row < lines.size() - 1) {
                    std::string remainder = lines[cursor_row].substr(cursor_col);
                    lines[cursor_row] = lines[cursor_row].substr(0, cursor_col);
                    lines.insert(lines.begin() + cursor_row + 1, remainder);
                } else {
                    lines.push_back("");
                }
                cursor_col = 0;
                cursor_row++;
            } else if (ch == 127) { // 退格
                if (cursor_col > 0) {
                    lines[cursor_row].erase(cursor_col - 1, 1);
                    cursor_col--;
                } else if (cursor_row > 0) {
                    cursor_col = lines[cursor_row - 1].size();
                    lines[cursor_row - 1] += lines[cursor_row];
                    lines.erase(lines.begin() + cursor_row);
                    cursor_row--;
                }
            } else if (ch == '\t') { // Tab键
                lines[cursor_row].insert(cursor_col, 4, ' ');
                cursor_col += 4;
            } else if (ch == 3) { // Ctrl+C
                editing = false;
            } else if (ch == 19) { // Ctrl+S
                editing = false;
            } else if (ch == 17) { // Ctrl+Q
                lines.clear();
                editing = false;
            } else if (ch >= 32 && ch <= 126) { // 可打印字符
                lines[cursor_row].insert(cursor_col, 1, ch);
                cursor_col++;
            }
#endif
        }
    }


    TextEditor::TextEditor() : cursor_row(0), cursor_col(0), editing(false),
                   first_visible_row(0), line_num_width(0) {
        lines.push_back(""); // 初始空行
    }

    void TextEditor::edit() {
        editing = true;
        get_terminal_size();
        setup_terminal();
        process_input();
        restore_terminal();
    }

    std::vector<std::string> TextEditor::get_lines(void) {
        return lines;
    }

    std::string TextEditor::get_text(void) const {
        std::string result;
        for (const auto& line : lines) {
            result += line + "\n";
        }
        if (!result.empty()) {
            result.pop_back(); // 移除最后一个换行符
        }
        return result;
    }

    void TextEditor::set_lines(const std::vector<std::string>& value) {
        lines = value;
    }

    void TextEditor::set_text(const std::string& text) {
        this->lines.clear();  // 先清空原有内容

        if (text.empty()) {
            this->lines.push_back("");  // 处理空输入
            return;
        }

        size_t start = 0;
        size_t end = text.find('\n');

        while (end != std::string::npos) {
            // 截取当前行 (不包括换行符)
            this->lines.push_back(text.substr(start, end - start));

            // 移动到下一个字符 (跳过当前换行符)
            start = end + 1;
            end = text.find('\n', start);
        }

        // 添加最后一行 (可能为空)
        this->lines.push_back(text.substr(start));
    }

} // namespace cpro
