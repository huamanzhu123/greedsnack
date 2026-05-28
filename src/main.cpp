// main.c : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <cstdio>
#include <cstdlib>
#include <ctime>
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
    printf("2- 第二关（困难模式）\n");
    printf("0- 退出游戏\n");
    printf("=================================\n");
    printf("请选择游戏模式：");
}

int main() {
    // 生成随机种子
    srand((unsigned int)time(NULL));
    // 设置光标位置
    set_cursor_pos(0, 0);
    // 隐藏光标
    hide_cursor();
    // 主菜单功能实现
    while (true) {
        // 清屏
        system("cls");
        // 显示主菜单
        show_menu();

        int choice = get_menu_choice(); // 获取用户选择
        while (getchar() != '\n') {
            if (choice == '1') {
                level1_run();
            } else if (choice == '2') {
                level2_run();
            } else if (choice == '0') {
                break;
            } else{
                printf("无效的选择，请重新输入。\n");
                getchar();
                break;
            }
        }
        printf("游戏结束，按任意按键放回主菜单...\n");
        while (!_kbhit())
        while (_kbhit()) _getch();  // 清除输入缓冲区
    }
    return 0;
}

