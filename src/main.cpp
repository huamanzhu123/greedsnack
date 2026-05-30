// main.c : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include "../include/level1.h"
#include "../include/game_shared.h"

/*
    说明：
    本文件需实现游戏主菜单功能，包括：
    1. 生成随机种子
    2. 设置光标位置
    3. 隐藏光标
    4. 主菜单功能实现
    5. 封装菜单选项函数
*/

void show_menu() {
    printf("===== 欢迎来到贪吃蛇游戏 =====\n");
    printf("==============主菜单==============\n");
    printf("1- 第一关（简单模式）\n");
    printf("0- 退出游戏\n");
    printf("=================================\n");
    printf("请选择游戏模式：");
}

// 获取用户菜单选择，返回有效的选项字符 
int get_menu_choice() {
    int ch;
    while (1) {
        ch = _getch();
        if (ch == '1' || ch == '0') {
            return ch;
        }
    }
}

int main() {
    SetConsoleOutputCP(65001);   // 输出 UTF-8 
    SetConsoleCP(65001);         // 输入 UTF-8
    // 生成随机种子
    srand((unsigned int)time(NULL));
    // 隐藏光标
    hide_cursor();

    int running = 1;
    while (running) {
        // 清屏
        system("cls");
        // 显示主菜单
        show_menu();

        int choice = get_menu_choice();

        switch (choice) {
            case '1':
                level1_run();
                break;
            case '0':
                running = 0;
                break;
            default:
                break;
        }
    }

    // 退出前恢复光标和清屏
    system("cls");
    set_cursor_pos(0, 0);
    printf("感谢游玩，再见！\n");
    system("pause");
    return 0;
}
