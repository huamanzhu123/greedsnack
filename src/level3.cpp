#include "../include/level3.h"

// 辅助绘制函数，GameUI中无障碍物和护盾道具相关部分
static void drawGameFrame(const Snake& snake, const Food& food,
                          const ObstacleManager& obstacles,
                          const ShieldItem& shield,
                          const Snake& aiSnake,
                          int score, int speedMs) {
    Console console;
    console.setCursorPos(0, 0);
    printf("分数: %d  血量: %d  |  AI血量: %d     速度: %d ms    Shift加速, Esc退出\n",
+           score, snake.get_blood(), aiSnake.get_blood(), speedMs);

    // 上边框
    for (int i = 0; i < WIDTH; ++i) putchar('#');
    putchar('\n');

    for (int y = 0; y < HEIGHT - 2; ++y) {
        putchar('#');
        for (int x = 0; x < WIDTH - 2; ++x) {
            char ch = ' ';

            // 食物
            if (x == food.get_x() && y == food.get_y()) {
                ch = '*';
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                putchar(ch);
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                continue;
            }
            // 护盾道具
            else if (shield.isActive() && x == shield.getX() && y == shield.getY()) {
                ch = 'S';
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                putchar(ch);
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                continue;
            }
            // 障碍物
            else if (obstacles.checkCollision(x, y)) {
                ch = '@';
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED);
                putchar(ch);
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                continue;
            }
            // 蛇
            else {
                // 检查玩家蛇 (O/o)
                for (int k = 0; k < snake.get_length(); ++k) {
                    if (x == snake.get_x(k) && y == snake.get_y(k)) {
                        ch = (k == 0) ? 'O' : 'o';
                        break;
                    }
                }
                // 检查AI蛇 (X/x)
                if (ch == ' ') {
                    for (int k = 0; k < aiSnake.get_length(); ++k) {
                        if (x == aiSnake.get_x(k) && y == aiSnake.get_y(k)) {
                            ch = (k == 0) ? 'X' : 'x';
                            break;
                        }
                    }
                }
            }
            if (ch == 'O' || ch == 'o') {
                bool flashing = snake.isDamageFlashing(console.getTickMs());
                bool bright = flashing && ((console.getTickMs() / 100) % 2 == 0);
                WORD normalColor;
                if (snake.getShield().isActive()) {
                    normalColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;  // 无敌：亮蓝
                } else {
                    normalColor = FOREGROUND_GREEN;  // 正常：绿色
                }
                WORD flashColor = bright ? (FOREGROUND_RED | FOREGROUND_INTENSITY) : normalColor;
                SetConsoleTextAttribute(console.getHandle(), flashColor);
                putchar(ch);
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            // AI蛇颜色：正常品红，护盾亮蓝，受伤闪烁红色
            else if (ch == 'X' || ch == 'x') {
                bool flashing = aiSnake.isDamageFlashing(console.getTickMs());
                bool bright = flashing && ((console.getTickMs() / 100) % 2 == 0);
                WORD normalColor;
                if (aiSnake.getShield().isActive()) {
                    normalColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;  // 无敌：亮蓝
                } else {
                    normalColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;  // 正常：品红
                }
                WORD flashColor = bright ? (FOREGROUND_RED | FOREGROUND_INTENSITY) : normalColor;
                SetConsoleTextAttribute(console.getHandle(), flashColor);
                putchar(ch);
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            else {
                putchar(ch);
            }
        }
        putchar('#');
        putchar('\n');
    }

    // 下边框
    for (int i = 0; i < WIDTH; ++i) putchar('#');
    putchar('\n');

    // 显示护盾剩余时间
    if (snake.getShield().isActive()) {
        unsigned long now = console.getTickMs();
        float remain = snake.getShield().getRemainingTime(now) / 1000.0f;
        if (remain > 0)
            printf("护盾激活中: %.1f 秒 (免疫障碍物)\n", remain);
    }
}

Level3::Level3()
    : score(0), gameOver(0),
      snake((WIDTH - 2) / 4, (HEIGHT - 2) / 2),
      aiSnake(3 * (WIDTH - 2) / 4, (HEIGHT - 2) / 2),
      lastMoveTime(0), lastShieldSpawnTime(0), shieldEaten(false) {}

void Level3::handleSpeedBoost(Snake& s) {
    if (GetAsyncKeyState(VK_LSHIFT) & 0x8000 || GetAsyncKeyState(VK_RSHIFT) & 0x8000) {
        s.set_speed(baseSpeedMs / 2);
        if (s.get_speed() < 60) s.set_speed(60);
    } else {
        s.set_speed(baseSpeedMs);
    }
}

void Level3::trySpawnShield(unsigned long now) {
    if (!shieldItem.isActive() && !shieldEaten) {
        if (now - lastShieldSpawnTime > 5000) {
            shieldItem.place(WIDTH, HEIGHT, snake, obstacleManager, food);
            if (shieldItem.isActive()) {
                shieldEaten = false;
                lastShieldSpawnTime = now;
            } else {
                lastShieldSpawnTime = now;
            }
        }
    } else if (shieldEaten && !shieldItem.isActive()) {
        if (now - lastShieldSpawnTime > 5000) {
            shieldEaten = false;
            lastShieldSpawnTime = now;
        }
    }
}

void Level3::checkCollisionsAndEat(unsigned long now) {
    // 吃到护盾道具
    if (shieldItem.isActive() &&
        snake.get_x(0) == shieldItem.getX() && snake.get_y(0) == shieldItem.getY()) {
        snake.getShield().activate(now);
        shieldItem.setActive(false);
        shieldEaten = true;
        lastShieldSpawnTime = now;
    }

    // 吃到食物
    if (snake.get_x(0) == food.get_x() && snake.get_y(0) == food.get_y()) {
        snake.grow();
        score++;
        food.place_food_safe(food, snake);
    }

    // 碰撞检测（护盾只免疫障碍物）
    // 撞墙：直接游戏结束
    if (snake.check_wall_collision()) {
            gameOver = 1;
        return;
    }
    // 撞自己：直接游戏结束
    if (snake.check_self_collision()) {
        snake.set_blood(snake.get_blood() - 1);
        snake.setDamageFlash(now);
        snake.move_with_collision();
        if (snake.get_blood() <= 0) {
            gameOver = 1;
        }
        return;
    }
    // 撞障碍物：如果有护盾则抵消，否则扣血
    if (obstacleManager.checkCollision(snake.get_x(0), snake.get_y(0))) {
        if (!snake.getShield().tryDefend()) {
            snake.set_blood(snake.get_blood() - 1);
            snake.setDamageFlash(now);
            snake.move_with_collision();
            if (snake.get_blood() <= 0) {
                gameOver = 1;
            }
            return;
        }
        snake.move_with_collision();
    }
    // 撞AI蛇：玩家扣血
    if (aiSnake.is_alive()) {
        for (int i = 0; i < aiSnake.get_length(); ++i) {
            if (snake.get_x(0) == aiSnake.get_x(i) && snake.get_y(0) == aiSnake.get_y(i)) {
                if(!snake.getShield().tryDefend()) {
                    snake.set_blood(snake.get_blood() - 1);
                    snake.setDamageFlash(now);
                    snake.move_with_collision();
                    if (snake.get_blood() <= 0) {
                        gameOver = 1;
                    }
                }
                snake.move_with_collision();
                break;
            }
        }
        return;
    }
}

// AI蛇碰撞检测：撞墙/撞自己/撞障碍物/撞玩家身体 → 扣血或死亡
void Level3::checkAICollisionsAndEat(unsigned long now) {
    if (!aiSnake.is_alive()) return;

    // 撞墙：AI同撞障碍物
    if (aiSnake.check_wall_collision()) {
        if (!aiSnake.getShield().tryDefend()) {
            aiSnake.set_blood(aiSnake.get_blood() - 1);
            aiSnake.setDamageFlash(now);
            aiSnake.move_with_collision();
            if (aiSnake.get_blood() <= 0) {
                aiSnake.setAlive(0);
            }
            return;
        }
        aiSnake.move_with_collision();
        return;
    }

    // 撞自己：扣血，随机转向
    if (aiSnake.check_self_collision()) {
        aiSnake.setDamageFlash(now);
        aiSnake.move_with_collision();
        if (aiSnake.get_blood() <= 0) {
            aiSnake.setAlive(0);
        }
        return;
    }

    // 撞障碍物：有护盾抵消，否则扣血
    if (obstacleManager.checkCollision(aiSnake.get_x(0), aiSnake.get_y(0))) {
        if (!aiSnake.getShield().tryDefend()) {
            aiSnake.set_blood(aiSnake.get_blood() - 1);
            aiSnake.setDamageFlash(now);
            aiSnake.move_with_collision();
            if (aiSnake.get_blood() <= 0) {
                aiSnake.setAlive(0);
            }
            return;
        }
        aiSnake.move_with_collision();
        return;
    }

    // 撞玩家身体：AI扣血
    if (snake.is_alive()) {
        for (int i = 0; i < snake.get_length(); ++i) {
            if (aiSnake.get_x(0) == snake.get_x(i) && aiSnake.get_y(0) == snake.get_y(i)) {
                if(!aiSnake.getShield().tryDefend()) {
                    aiSnake.set_blood(aiSnake.get_blood() - 1);
                    aiSnake.setDamageFlash(now);
                    aiSnake.move_with_collision();
                    if (aiSnake.get_blood() <= 0) {
                        aiSnake.setAlive(0);
                        return;
                    }
                }
                aiSnake.move_with_collision();
                break;
            }
        }

    }

    // AI吃食物：不加分，但AI成长并刷新食物
    if (aiSnake.get_x(0) == food.get_x() && aiSnake.get_y(0) == food.get_y()) {
        aiSnake.grow();
        food.place_food_safe(food, snake);
    }

    // AI吃护盾道具
    if (shieldItem.isActive() &&
        aiSnake.get_x(0) == shieldItem.getX() && aiSnake.get_y(0) == shieldItem.getY()) {
        aiSnake.getShield().activate(now);
        shieldItem.setActive(false);
        shieldEaten = true;
        lastShieldSpawnTime = now;
    }
}

void Level3::run() {
    Console console;
    console.hideCursor();
    console.clear();
    console.setUTF8();

    // 放置第一个食物
    food.place_food_safe(food, snake);

    // 初始化障碍物，5个固定，3个移动，
    obstacleManager.initialize(snake, snake, food, 5, 3);

    // 初始化护盾
    shieldItem.place(WIDTH, HEIGHT, snake, obstacleManager, food);
    lastShieldSpawnTime = console.getTickMs();
    shieldEaten = false;

    lastMoveTime = console.getTickMs();
    unsigned long lastObstacleMove = lastMoveTime;  // 新增变量，跟踪上次障碍物移动时间

    while (!gameOver) {
        // 输入处理
        if (_kbhit()) {
            int ch = _getch();
            int currentDir = snake.get_dir();
            if (ch == 0 || ch == 224) {
                ch = _getch();
                if (ch == 72 && currentDir != 2) snake.setDir(0);      // 上
                else if (ch == 80 && currentDir != 0) snake.setDir(2); // 下
                else if (ch == 75 && currentDir != 1) snake.setDir(3); // 左
                else if (ch == 77 && currentDir != 3) snake.setDir(1); // 右
            } else {
                if ((ch == 'W' || ch == 'w') && currentDir != 2) snake.setDir(0);
                else if ((ch == 'S' || ch == 's') && currentDir != 0) snake.setDir(2);
                else if ((ch == 'A' || ch == 'a') && currentDir != 1) snake.setDir(3);
                else if ((ch == 'D' || ch == 'd') && currentDir != 3) snake.setDir(1);
                else if (ch == 27) gameOver = 1;
            }
        }

        handleSpeedBoost(snake);  

        unsigned long now = console.getTickMs();

        // 蛇的移动
        if (now - lastMoveTime >= static_cast<unsigned long>(snake.get_speed())) {
            lastMoveTime = now;
            snake.move();
            checkCollisionsAndEat(now);

            aiSnake.updateAIDirection(obstacleManager, food, shieldItem);
            if (aiSnake.is_alive()) aiSnake.move();
            checkAICollisionsAndEat(now);
            
            if (gameOver) break;
        }

        // 移动障碍物更新（每200ms）
        if (now - lastObstacleMove >= 200) {
            lastObstacleMove = now;
            obstacleManager.update(now);
        }

        // 生成新的护盾道具
        trySpawnShield(now);

        // 绘制画面
        drawGameFrame(snake, food, obstacleManager, shieldItem, aiSnake, score, snake.get_speed());

        // 更新护盾持续时间
        snake.getShield().update(now);
        aiSnake.getShield().update(now);

        Sleep(10);
    }

    // 游戏结束处理
    console.setCursorPos(0, HEIGHT + 2);
    while (_kbhit()) _getch();

    printf("\nGame Over! Your final score: %d\n", score);
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