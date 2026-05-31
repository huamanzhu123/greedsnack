#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include "../include/level1.h"
#include "../include/game_shared.h"

void show_menu() {
    printf("===== 欢迎来到贪吃蛇游戏 =====\n");
    printf("==============主菜单==============\n");
    printf("1- 第一关（简单模式）\n");
    printf("0- 退出游戏\n");
    printf("=================================\n");
    printf("请选择游戏模式：");
}

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
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    srand((unsigned int)time(NULL));
    
    Console console;
    console.hideCursor();

    int running = 1;
    while (running) {
        system("cls");
        show_menu();

        int choice = get_menu_choice();

        switch (choice) {
            case '1': {
                Level1 level1;   // 每次游戏创建新对象，状态全新
                level1.run();
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
    printf("感谢游玩，再见！\n");
    system("pause");
    return 0;
}
