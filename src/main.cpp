#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "../include/level1.h"
#include "../include/game_shared.h"
#include "../include/leaderboard.h"
#include "../include/level2.h"

// 自定义菜单显示（包含三个选项）
void show_menu() {
    printf("===== 欢迎来到贪吃蛇游戏 =====\n");
    printf("==============主菜单==============\n");
    printf("1- 单人模式（第一关）\n");
    printf("2- 双人对战模式\n");
    printf("3- 查看排行榜（前五名）\n");
    printf("4- 查看玩家信息\n");
    printf("0- 退出游戏\n");
    printf("=================================\n");
    printf("请选择：");
}

// 自定义菜单输入（接受 0,1,2,3）
int get_menu_choice() {
    int ch;
    while (1) {
        ch = _getch();
        if (ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '0')
            return ch;
    }
}

// 清空输入缓冲区
void clear_input_buffer() {
    while (_kbhit()) _getch();
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    srand((unsigned int)time(NULL));

    Console console;
    console.hideCursor();

    int running = 1;
    while (running) {
        system("cls");
        show_menu();                     // 使用自定义菜单
        int choice = get_menu_choice();  // 使用自定义输入

        switch (choice) {
            case '1': {
                Level1 game;
                game.run();
                break;
            }
            case '2': {
                Level2 game;
                game.run();
                break;
            }
            case '3': {
                Leaderboard lb;
                lb.displayTop5();
                printf("\n按任意键返回菜单...");
                clear_input_buffer();
                while (!_kbhit()) {}
                while (_kbhit()) _getch();
                break;
            }
            case '4': {
                printf("请输入要查询的玩家姓名：");
                clear_input_buffer();
                char name[32] = {0};
                if (fgets(name, sizeof(name), stdin)) {
                    size_t len = strlen(name);
                    if (len > 0 && (name[len-1] == '\n' || name[len-1] == '\r'))
                        name[len-1] = '\0';
                    if (name[0] != '\0') {
                        Leaderboard lb;
                        lb.displayPlayerInfo(name);
                    } else {
                        printf("姓名不能为空。\n");
                    }
                } else {
                    printf("读取输入失败。\n");
                }
                printf("\n按任意键返回菜单...");
                while (!_kbhit()) {}
                while (_kbhit()) _getch();
                break;
            }
            case '0':
                running = 0;
                break;
            default:
                break;
        }
    }

    system("cls");
    console.setCursorPos(0, 0);
    console.showCursor();
    printf("感谢游玩，再见！\n");
    system("pause");
    return 0;
}