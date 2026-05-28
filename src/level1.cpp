#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "../include/level1.h"
#include "../include/game_shared.h"
#include "../include/game_ui.h"
#include "../include/level_common.h"
#include "../include/leaderboard.h"


/* 单人普通模式（关卡1）实现：
*   1. 声明变量 score, gameOver, speedMs, startX, startY
*   2. 隐藏光标
*   3. 初始化蛇与首个食物
*   4. 清屏
*   5. 游戏主循环
*     5.1 渲染画面（调用 draw_board）
*     5.2 输入与定时（调用 process_input）
*     5.3 移动蛇（调用 snake_move）
*     5.4 碰撞检测（调用 check_self_collision 与 check_wall_collision）
*     5.5 吃食物（调用 snake_grow 与 place_food_safe）
*   6. 游戏结束后将光标移到画面下面再读取名字
*   7. 调用 leaderboard 模块的提示与更新函数
*/

void level1_run(void) {
    int score = 0;
    int gameOver = 0;          // 游戏结束标志
    int speedMs = 200;         // 蛇移动间隔（毫秒），值越小速度越快
    int startX = (WIDTH - 2) / 2;   // 蛇起始x坐标（地图中央）
    int startY = (HEIGHT - 2) / 2;  // 蛇起始y坐标（地图中央）

    hide_cursor();

    // 初始化蛇
    Snake s;
    snake_init(&s, startX, startY);

    // 初始化食物
    Food f;
    place_food_safe(&f, &s);

    clear_screen();

    unsigned long lastMoveTime = get_tick_ms();  // 上一次移动的时间戳

    while (!gameOver) {
        // 处理输入
        process_input(&s, &gameOver);
        if (gameOver) break;

        // 判断是否到达移动间隔
        unsigned long now = get_tick_ms();
        if (now - lastMoveTime >= (unsigned long)speedMs) {
            lastMoveTime = now;

            // 移动蛇
            snake_move(&s);

            // 碰撞检测：撞墙或撞自身
            if (check_self_collision(&s) || check_wall_collision(&s)) {
                gameOver = 1;
                break;
            }

            // 吃到食物检测：蛇头坐标与食物坐标重合
            if (s.x[0] == f.x && s.y[0] == f.y) {
                snake_grow(&s);          // 蛇长度增加
                score++;
                place_food_safe(&f, &s); // 放置新食物
            }
        }

        // 绘制画面
        draw_board(&s, &f, score, WIDTH, HEIGHT, speedMs);

        // 短暂延时，控制帧率
        Sleep(10);
    }

    // 游戏结束后，将光标移到画面下方
    set_cursor_pos(0, HEIGHT + 2);
    while (_kbhit()) _getch();  // 清空键盘缓冲区

    prompt_and_update_leaderboard(score);

    printf("\nPress any key to return to menu...");
    while (!_kbhit()) {}        // 等待按键
    while (_kbhit()) _getch();  // 清除按键缓冲
}
