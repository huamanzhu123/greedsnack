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

    // 上边框
    for(int i = 0; i < WIDTH; i++) {
        putchar('#');
    }
    putchar('\n');

    // 游戏区域（含左右边框）
    for(int y = 0; y < HEIGHT - 2; y++) {
        putchar('#');  // 左边框
        for(int x = 0; x < WIDTH - 2; x++) {
            char ch = ' ';

            // 绘制食物
            if(x == f->x && y == f->y) {
                ch = '*';
            }

            // 绘制蛇身（蛇头为 'O'，蛇身为 'o'）
            for(int k = 0; k < s->length; k++) {
                if(x == s->x[k] && y == s->y[k]) {
                    ch = (k == 0) ? 'O' : 'o';
                }
            }
            putchar(ch);
        }
        putchar('#');  // 右边框
        putchar('\n');
    }

    // 下边框
    for(int i = 0; i < WIDTH; i++) {
        putchar('#');
    }
    putchar('\n');
}

void process_input(Snake* s, int* game_over) {
    if(!s || !game_over) return;

    if(!_kbhit()) return; // 如果没有按键输入，直接返回

    int ch = _getch();

    // 方向键：第一个字节为 224 或 0，第二个字节为扫描码
    // 扫描码：72=上, 80=下, 75=左, 77=右
    if(ch == 0 || ch == 224) {
        ch = _getch();

        if(ch == 72 && s->dir != 2)      s->dir = 0;  // 上（禁止反向）
        else if(ch == 80 && s->dir != 0) s->dir = 2;  // 下（禁止反向）
        else if(ch == 75 && s->dir != 1) s->dir = 3;  // 左（禁止反向）
        else if(ch == 77 && s->dir != 3) s->dir = 1;  // 右（禁止反向）
    } else {
        // WASD 按键支持
        if(ch == 'W' || ch == 'w') {
            if(s->dir != 2) s->dir = 0;   // 上
        } else if(ch == 'S' || ch == 's') {
            if(s->dir != 0) s->dir = 2;   // 下
        } else if(ch == 'A' || ch == 'a') {
            if(s->dir != 1) s->dir = 3;   // 左
        } else if(ch == 'D' || ch == 'd') {
            if(s->dir != 3) s->dir = 1;   // 右
        } else if(ch == 27) { // Esc键
            *game_over = 1;
        }
    }
}