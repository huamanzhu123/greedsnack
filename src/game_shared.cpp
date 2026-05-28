/*
	此文件封装控制台位置设置、隐藏光标、画边框、通用时间函数、随机初始化等平台相关工具。需实现以下功能：
	1. 设置光标位置
	2. 隐藏光标
	3. 获取毫秒级时间函数
	4. 清屏函数
*/
#include "../include/game_shared.h"

// 设置光标位置
void set_cursor_pos(int x, int y) {
	// 这句代码是使用Windows API函数GetStdHandle获取标准输出设备的句柄，并将其存储在变量hConsole中。STD_OUTPUT_HANDLE是一个常量，表示标准输出设备的句柄。
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// pos是一个COORD结构体，包含两个SHORT类型的成员：X和Y，分别表示光标的水平和垂直位置。
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}


// 隐藏光标
void hide_cursor(void) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;  // 定义一个CONSOLE_CURSOR_INFO结构体变量，用于存储光标信息
    GetConsoleCursorInfo(hConsole, &cursorInfo);  // 调用GetConsoleCursorInfo函数获取当前光标信息，并将其存储在cursorInfo变量中
    cursorInfo.bVisible = FALSE;    // 设置光标不可见
    SetConsoleCursorInfo(hConsole, &cursorInfo);  // 调用SetConsoleCursorInfo函数将修改后的光标信息应用到控制台
}

// 毫秒级时间获取
unsigned long get_tick_ms(void) {
    return GetTickCount();
}

void clear_screen(void) {
    system("cls");
}