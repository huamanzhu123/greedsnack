#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include "../include/level1.h"
#include "../include/game_shared.h"

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    srand((unsigned int)time(NULL));
    
    Console console;
    console.hideCursor();

    int running = 1;
    while (running) {
        system("cls");
        console.show_menu();

        int choice = console.get_menu_choice();

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
