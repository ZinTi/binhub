#include <stdio.h>
#include <time.h>
#include <windows.h>               //Sleep(ms)
#include <conio.h>                 //kbhit()/_kbhit(),getch()
#include "binhub.h"

#define true        1
#define false       0

binhub_tcolor_t color_ce = {CLR_BRT_YELLOW, CLR_BRT_RED};
binhub_tcolor_t color_ed = {CLR_BRT_MAGENTA, CLR_BRT_YELLOW};

void set_cursor_visible(short visible);         // 设置控制台光标可见性的函数
void system_beep();

int main(void){
#ifdef _WIN32 // Linux 默认
    binhub_setenc(ENCODING_UTF8);
#endif
    //动态计时参数（打印值）
    unsigned int hour = 0, min = 0, sec = 0;
    //默认值（设定的计时参数）
    unsigned int hour_def = 0, min_def = 0, sec_def = 0;
    time_t time_sec = 0;
    time_t old_sec = 0;
    char key;
    int pause_flag = 1;            //暂停标志
    int menu_flag = 1;             //菜单打印标志
    while(1){
        /**************** 菜单打印 ****************/
        if(menu_flag == 1){
            menu_flag = 0;
            BINHUB_CLEAR_SCREEN;
            printf("═════════════════════════════════\n"); //菜单
            if(hour_def || min_def || sec_def){
                printf("│\t倒计时时间：%02d:%02d:%02d\t│\n", hour_def, min_def, sec_def);
            }
            else
                printf("|\t未设置计时时间\t\t|\n");
            printf("│ S:设置 空格:开始/暂停 Q:退出\t│\n");
            printf("═════════════════════════════════\n");
            if(pause_flag == 0) {
                binhub_cprintf(color_ed, "计时结束"); putchar('\n');
                system_beep();
                pause_flag = 1;          //停止计时
            }
        }

        /**************** 键盘按键扫描+操作 ****************/
        set_cursor_visible(false);
        key = 0;
        if(_kbhit())                      //检测到按键按下
            key = getch();                //读取按键
        switch(key) {
            case 's':                     //按s/S设置计时时间
            case 'S':
                set_cursor_visible(true);
                printf("请设置计时时间——时 分 秒\n");
                scanf("%d %d %d", &hour_def, &min_def, &sec_def);
                if(hour_def > 24 || min_def > 59 || sec_def > 59) {
                    //printf("时间设置失败\n");
                    hour_def = min_def = sec_def = 0; //重置时间
                }
                hour = min = sec = 0;       //计时值清零
                menu_flag = 1;              //打印菜单
                break;
            case ' ':                       //按空格键开始/暂停计时
                set_cursor_visible(false);
                if(hour_def || min_def || sec_def) { //如果时间有效
                    if(pause_flag){
                        pause_flag = 0;     //开始计时或继续计时
                        if(!hour && !min && !sec) { //计数值为0
                            hour = hour_def;   //读取上次设置的时间
                            min = min_def;
                            sec = sec_def;
                            binhub_cprintf(color_ce, "%02d:%02d:%02d\r", hour, min, sec); //打印初始时间
                        }
                    } else
                        pause_flag = 1;     //暂停计时
                    time(&time_sec);        //获取当前秒数（1970-1-1 00:00:00到现在）
                    old_sec = time_sec;     //更新旧的秒数
                }
                break;
            case 'q':
            case 'Q':
                printf("程序退出\n");return 0;
        }

        /**************** 计时操作 ****************/
        time(&time_sec);              //获取秒数保存到time_t变量
        if(pause_flag == 0 && old_sec != time_sec){
            old_sec = time_sec;       //更新旧的秒数
            if(sec > 0)
                sec--;                //计时秒数减1
            else{
                sec = 59;             //如果原秒数为0，则变为59
                if(min > 0)
                    min--;            //计时分钟减1
                else {
                    min = 59;         //如果分钟数为0，则变为59
                    hour--;           //计时小时数减1
                }
            }
            binhub_cprintf(color_ce, "%02d:%02d:%02d\r", hour, min, sec);

            if(!hour && !min && !sec) //计时结束
                menu_flag = 1;        //打印菜单
        }
    }
    return 0;
}

void set_cursor_visible(short visible) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);  // 获取标准输出句柄
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);        // 修改可见性设置
    cursorInfo.bVisible = visible ? TRUE : FALSE;   // 修改可见性设置
    SetConsoleCursorInfo(hOut, &cursorInfo);        // 应用新设置
}

void system_beep() {
    // 方法1：尝试控制台响铃（适用于大多数终端）
    printf("\a");   // \a 是ASCII的响铃字符
    fflush(stdout); // 确保立即输出
    
    // 方法2：尝试播放旋律音效（需要声卡支持）
    // 定义音符数组：{频率, 持续时间(ms)}
    const int melody[][2] = {
        {659, 250}, // E5
        {587, 250}, // D5
        {523, 250}, // C5
        {587, 250}, // D5
        {659, 250}, // E5
        {659, 250}, // E5
        {659, 500}, // E5 (长音)
        {587, 250}, // D5
        {587, 250}, // D5
        {587, 500}, // D5 (长音)
        {659, 250}, // E5
        {784, 250}, // G5
        {784, 500}, // G5 (长音)
        {0, 0}      // 结束标记
    };

    /*
    const int melody[][2] = {
        {262, 200}, // C4
        {294, 200}, // D4
        {330, 200}, // E4
        {349, 200}, // F4
        {392, 200}, // G4
        {440, 200}, // A4
        {494, 200}, // B4
        {523, 400}, // C5
        {0, 0}
    };*/
    
    Sleep(100); // 避免与控制台响铃重叠
    
    // 遍历并播放所有音符
    for (int i = 0; melody[i][0] != 0; i++) {
        if (!Beep(melody[i][0], melody[i][1])) {
            // 如果Beep失败，停止播放旋律
            break;
        }
    }
}