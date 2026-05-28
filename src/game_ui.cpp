/*
* 负责渲染游戏界面，以及键盘输入的处理，需完成：
* 1. 绘制游戏界面，包括边框、分数、蛇、食物等视觉元素
* 2. 处理输入：修改 s->dir；若检测到退出（Esc）则设置 *game_over = 1
*/
#include "../include/game_ui.h"

void draw_board(const Snake* s, const Food* f, int score, int width, int height, int speed_ms) {
    if(!s || !f) return;
    set_cursor_pos(0, 0); // 重置光标位置，准备重绘

    // 游戏基础信息
    printf("Score: %d     speed_ms: %d     Controls Arrow keys or WASD     (Esc to quit)\n", score, speed_ms);

    for(int i = 0; i < WIDTH; i++) {
        putchar('#');
    }
    putchar('\n');
    for(int y = 0; y < HEIGHT - 1; y++) {
        putchar('#');
        for(int x = 0; x < WIDTH - 1; x++) {
            char ch = ' ';

            if(x == f->x && y == f->y) {
                ch = '*'; // 食物
            }

            for(int k = 0; k < s->length; k++) {
                if(x == s->x[k] && y == s->y[k]) {
                    ch = (k == 0)? 'O' : 'o'; // 蛇头和蛇身
                }
            }
            putchar(ch);
        }
    }
    for(int i = 0; i < WIDTH; i++) {
        putchar('#');
    }
    putchar('\n');
}

void process_input(Snake* s, int* game_over) {
    if(!s || !game_over) return;

    if(_kbhit()) return; // 如果没有按键输入，直接返回

    int ch = _getch();

    if(ch == '0' || ch == '224') {
        ch = _getch();

        if(ch == '72' && s->dir != 3) s->dir = 2;  // 上
        else if(ch == '80' && s->dir != 2) s->dir = 3;  // 下
        else if(ch == '75' && s->dir != 1) s->dir = 0;  // 左
        else if(ch == '77' && s->dir != 0) s->dir = 1;  // 右
    } else{
        if(ch == 'W' || ch == 'w') {
            if(s->dir != 3) s->dir = 2;
        } else if(ch == 'S' || ch == 's') {
            if(s->dir != 2) s->dir = 3;
        } else if(ch == 'A' || ch == 'a') {
            if(s->dir != 1) s->dir = 0;
        } else if(ch == 'D' || ch == 'd') {
            if(s->dir != 0) s->dir = 1;
        } else if(ch == 27) { // Esc键
            *game_over = 1;
        }
    }
}