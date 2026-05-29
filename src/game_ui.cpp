/*
* 负责渲染游戏界面，以及键盘输入的处理，需完成：
* 1. 绘制游戏界面，包括边框、分数、蛇、食物等视觉元素
* 2. 处理输入：修改 s->dir；若检测到退出（Esc）则设置 *game_over = 1
*/
#include "../include/game_ui.h"

GameUI::GameUI(int w, int h) : width(w), height(h) {
    console = new Console();
}

GameUI::~GameUI() {
    delete console;
}

// 绘制游戏界面
void GameUI::drawBoard(const Snake* s, const Food* f, int score, int speedMs) {
    if (!s || !f) return;
    
    console->setCursorPos(0, 0);  // 重置光标位置，准备重绘

    // 游戏基础信息
    printf("Score: %d     speed_ms: %d     Controls: Arrow keys or WASD     (Esc to quit)\n", 
           score, speedMs);

    // 上边框
    for (int i = 0; i < width; i++) {
        putchar('#');
    }
    putchar('\n');

    // 游戏区域（含左右边框）
    for (int y = 0; y < height - 2; y++) {
        putchar('#');  // 左边框
        
        for (int x = 0; x < width - 2; x++) {
            char ch = ' ';

            // 绘制食物
            if (x == f->get_x() && y == f->get_y()) {
                ch = '*';
            }

            // 绘制蛇身（蛇头为 'O'，蛇身为 'o'）
            // 使用 get_length(), get_x(), get_y() 方法访问蛇的数据
            for (int k = 0; k < s->get_length(); k++) {
                if (x == s->get_x(k) && y == s->get_y(k)) {
                    ch = (k == 0) ? 'O' : 'o';
                    break;
                }
            }
            putchar(ch);
        }
        
        putchar('#');  // 右边框
        putchar('\n');
    }

    // 下边框
    for (int i = 0; i < width; i++) {
        putchar('#');
    }
    putchar('\n');
}

// 处理键盘输入
void GameUI::processInput(Snake* s, int* gameOver) {
    if (!s || !gameOver) return;

    if (!_kbhit()) return;  // 如果没有按键输入，直接返回

    int ch = _getch();
    int currentDir = s->get_dir();  // 获取当前方向

    // 方向键：第一个字节为 224 或 0，第二个字节为扫描码
    // 扫描码：72=上, 80=下, 75=左, 77=右
    if (ch == 0 || ch == 224) {
        ch = _getch();

        if (ch == 72 && currentDir != 2) {       // 上（禁止反向）
            s->setDir(0);
        }
        else if (ch == 80 && currentDir != 0) {  // 下（禁止反向）
            s->setDir(2);
        }
        else if (ch == 75 && currentDir != 1) {  // 左（禁止反向）
            s->setDir(3);
        }
        else if (ch == 77 && currentDir != 3) {  // 右（禁止反向）
            s->setDir(1);
        }
    } else {
        // WASD 按键支持
        if (ch == 'W' || ch == 'w') {
            if (currentDir != 2) s->setDir(0);   // 上
        } else if (ch == 'S' || ch == 's') {
            if (currentDir != 0) s->setDir(2);   // 下
        } else if (ch == 'A' || ch == 'a') {
            if (currentDir != 1) s->setDir(3);   // 左
        } else if (ch == 'D' || ch == 'd') {
            if (currentDir != 3) s->setDir(1);   // 右
        } else if (ch == 27) {  // Esc键
            *gameOver = 1;
        }
    }
}

// 显示游戏结束信息
void GameUI::showGameOver(int finalScore) {
    console->setCursorPos(0, height + 2);
    printf("\n=== Game Over ===\n");
    printf("Final Score: %d\n", finalScore);
}

// 清空键盘缓冲区
void GameUI::clearInputBuffer() {
    while (_kbhit()) _getch();
}