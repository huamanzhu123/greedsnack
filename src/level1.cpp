#include "../include/level1.h"
#include "../include/game_shared.h"
#include "../include/game_ui.h"
#include "../include/leaderboard.h"
#include <cstring>
#include <conio.h>
#include <windows.h>

Level1::Level1()
    : score(0)
    , gameOver(0)
    , baseSpeedMs(200)
    , currentSpeedMs(200)
    , startX((WIDTH - 2) / 2)
    , startY((HEIGHT - 2) / 2)
    , snake(startX, startY)
    , lastMoveTime(0) {}

void Level1::handleSpeedBoost() {
    if (GetAsyncKeyState(VK_LSHIFT) & 0x8000 || GetAsyncKeyState(VK_RSHIFT) & 0x8000) {
        // 加速到原来的 2 倍  间隔变为 1/2
        currentSpeedMs = baseSpeedMs / 2;
        if (currentSpeedMs < 30) currentSpeedMs = 30;  // 速度下限
    } else {
        currentSpeedMs = baseSpeedMs;
    }
}


void Level1::run() {
    GameUI ui(WIDTH, HEIGHT);
    Console console;
    console.hideCursor();

    // 重置游戏状态
    snake = Snake(startX, startY);
    score = 0;
    gameOver = 0;
    baseSpeedMs = 200;
    currentSpeedMs = 200;
    food.place_food_safe(food, snake);
    console.clear();
    lastMoveTime = console.getTickMs();
    //循环的逻辑实现
    // 当gameOver等于0时才会继续执行。
    // 每一轮的循环内部都在执行下面的东西
    // 读取键盘输入，根据它来更新蛇的方向，或者设置游戏结束标志
    // 判断蛇是否已经到了下一次移动的时间
    // 如果到了，就移动蛇并检测碰撞、吃食物
    // 重绘当前画面，展示在控制台上，方便用户根据这个画面来决定下一步操作
    // 等待一小段时间，避免 CPU 占用过高。
    while (!gameOver) {
        ui.processInput(&snake, &gameOver);
        if (gameOver) break;

        handleSpeedBoost();

        unsigned long now = console.getTickMs();
        
        // 通过时间戳控制蛇的移动速度。
        // lastMoveTime存储上次实际移动的时间点，now是现在的时间。
        // 只有当距离上次移动经过了至少speedMs毫秒，蛇才会再移动一步。
        if (now - lastMoveTime >= static_cast<unsigned long>(currentSpeedMs)) {
            lastMoveTime = now;

            snake.move(snake);
             // 如果蛇头碰到了自己的身体，或者碰到了边界墙壁，游戏结束。
            // check_self_collisio和check_wall_collision会返回非零表示发生碰撞。
            if (snake.check_self_collision(snake) || snake.check_wall_collision(snake)) {
                gameOver = 1;
                break;
            }
            if (snake.get_x(0) == food.get_x() && snake.get_y(0) == food.get_y()) {
                snake.grow(snake);
                score++;
                food.place_food_safe(food, snake);
            }
        }

        ui.drawBoard(&snake, &food, score, currentSpeedMs);
        Sleep(10);
    }

    // 游戏结束：记录成绩
    console.setCursorPos(0, HEIGHT + 2);
    while (_kbhit()) _getch();

    printf("Game over! Your final score: %d\n", score);
    printf("Enter your name (max 31 chars, or press Enter to skip): ");
    char namebuf[32] = {0};
    if (fgets(namebuf, sizeof(namebuf), stdin) != nullptr) {
        size_t len = strlen(namebuf);
        if (len > 0 && (namebuf[len-1] == '\n' || namebuf[len-1] == '\r'))
            namebuf[len-1] = '\0';
        if (namebuf[0] != '\0') {
            Leaderboard lb;
            lb.update(namebuf, score);
            lb.displayTop5();
        } else {
            printf("成绩未保存。\n");
        }
    } else {
        printf("输入错误，成绩未保存。\n");
    }

    printf("\nPress any key to return to menu...");
    while (!_kbhit()) {}
    while (_kbhit()) _getch();
}