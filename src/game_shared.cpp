/*
	此文件封装控制台位置设置、隐藏光标、画边框、通用时间函数、随机初始化等平台相关工具。需实现以下功能：
	1. 设置光标位置
	2. 隐藏光标
	3. 获取毫秒级时间函数
	4. 清屏函数
*/
#include "game_shared.h"

Console::Console() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

// 设置光标位置
void Console::setCursorPos(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}

// 隐藏光标
void Console::hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// 显示光标
void Console::showCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// 清屏
void Console::clear() {
    system("cls");
}

// 获取毫秒级时间戳
unsigned long Console::getTickMs() {
    return GetTickCount();
}

//设置为UTF-8
void Console::setUTF8() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}