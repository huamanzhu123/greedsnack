#include "../include/level3.h"

// 辅助绘制函数，包含高分食物紫色闪烁效果
static void drawGameFrame(const Snake& snake, const Food& food,
                          const ObstacleManager& obstacles,
                          const ShieldItem& shield,
                          const Snake& aiSnake,
                          const BombZone& bombZone,
                          int score, int speedMs) {
    Console console;
    console.setCursorPos(0, 0);
    printf("分数: %d  血量: %d  能量：%d  |  AI血量: %d     速度: %d ms    Shift加速, Esc退出\n",
           score, snake.get_blood(), snake.get_energy(), aiSnake.get_blood(), speedMs);

    for (int i = 0; i < WIDTH; ++i) putchar('#');
    putchar('\n');

    for (int y = 0; y < HEIGHT - 2; ++y) {
        putchar('#');
        for (int x = 0; x < WIDTH - 2; ++x) {
            char ch = ' ';

            if (x == food.get_x() && y == food.get_y()) {
                if (food.get_type() == Food::TYPE_HIGHSCORE) {
                    ch = '*';
                    unsigned long now = console.getTickMs();
                    bool bright = ((now / 200) % 2) == 0;
                    WORD color;
                    if (bright)
                        color = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                    else
                        color = FOREGROUND_RED | FOREGROUND_BLUE;
                    SetConsoleTextAttribute(console.getHandle(), color);
                    putchar(ch);
                    SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    continue;
                } else {
                    ch = '*';
                    SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    putchar(ch);
                    SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    continue;
                }
            }
            else if (shield.isActive() && x == shield.getX() && y == shield.getY()) {
                ch = 'S';
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                putchar(ch);
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                continue;
            }
            else if (obstacles.checkCollision(x, y)) {
                ch = '@';
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED);
                putchar(ch);
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                continue;
            }
            else if (bombZone.isActive() && x >= bombZone.getX() - 1 && x <= bombZone.getX() + 1 &&
                    y >= bombZone.getY() - 1 && y <= bombZone.getY() + 1) {
                if(x == bombZone.getX() && y == bombZone.getY()){
                    ch = 'Q';
                } else {
                    ch = ' ';
                }
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED);
                putchar(ch);
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                continue;
            }
            else {
                for (int k = 0; k < snake.get_length(); ++k) {
                    if (x == snake.get_x(k) && y == snake.get_y(k)) {
                        ch = (k == 0) ? 'O' : 'o';
                        break;
                    }
                }
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
                    normalColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                } else {
                    normalColor = FOREGROUND_GREEN;
                }
                WORD flashColor = bright ? (FOREGROUND_RED | FOREGROUND_INTENSITY) : normalColor;
                SetConsoleTextAttribute(console.getHandle(), flashColor);
                putchar(ch);
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            else if (ch == 'X' || ch == 'x') {
                bool flashing = aiSnake.isDamageFlashing(console.getTickMs());
                bool bright = flashing && ((console.getTickMs() / 100) % 2 == 0);
                WORD normalColor;
                if (aiSnake.getShield().isActive()) {
                    normalColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                } else {
                    normalColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
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

    for (int i = 0; i < WIDTH; ++i) putchar('#');
    putchar('\n');

    if (snake.getShield().isActive()) {
        unsigned long now = console.getTickMs();
        float remain = snake.getShield().getRemainingTime(now) / 1000.0f;
        if (remain > 0)
            printf("护盾激活中: %.1f 秒 (免疫障碍物)\n", remain);
    }
    if (bombZone.isActive()) {
        int remain = bombZone.getRemainingTime(console.getTickMs());
        printf("炸弹区域倒计时: %d 秒\n", remain);
    }
}

Level3::Level3()
    : score(0), gameOver(0),
      snake((WIDTH - 2) / 4, (HEIGHT - 2) / 2),
      aiSnake(3 * (WIDTH - 2) / 4, (HEIGHT - 2) / 2),
      lastMoveTime(0), lastShieldSpawnTime(0), lastBombZoneSpawnTime(0), shieldEaten(false) {}

void Level3::handleSpeedBoost(Snake& s) {
    static unsigned long boostEndTime = 0;
    unsigned long now = GetTickCount();
    bool shiftDown = (GetAsyncKeyState(VK_LSHIFT) & 0x8000) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000);

    if (shiftDown && s.get_energy() == 3 && now >= boostEndTime) {
        s.set_energy(0);
        boostEndTime = now + 5000;
    }

    if (now < boostEndTime) {
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
    if (shieldItem.isActive() &&
        snake.get_x(0) == shieldItem.getX() && snake.get_y(0) == shieldItem.getY()) {
        snake.getShield().activate(now);
        shieldItem.setActive(false);
        shieldEaten = true;
        lastShieldSpawnTime = now;
    }

    if (snake.get_x(0) == food.get_x() && snake.get_y(0) == food.get_y()) {
        snake.grow();
        if (food.get_type() == Food::TYPE_HIGHSCORE)
            score += 3;
        else
            score += 1;
        if (snake.get_energy() < 3)
            snake.set_energy(snake.get_energy() + 1);
        food.place_food_safe(food, snake);
        food.setSpawnTime(now);
    }

    if (snake.check_wall_collision()) {
        gameOver = 1;
        return;
    }
    if (snake.check_self_collision()) {
        snake.set_blood(snake.get_blood() - 1);
        snake.setDamageFlash(now);
        snake.move_with_collision();
        if (snake.get_blood() <= 0) {
            gameOver = 1;
        }
        return;
    }
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

void Level3::checkAICollisionsAndEat(unsigned long now) {
    if (!aiSnake.is_alive()) return;

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

    if (aiSnake.check_self_collision()) {
        aiSnake.setDamageFlash(now);
        aiSnake.move_with_collision();
        if (aiSnake.get_blood() <= 0) {
            aiSnake.setAlive(0);
        }
        return;
    }

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

    if (aiSnake.get_x(0) == food.get_x() && aiSnake.get_y(0) == food.get_y()) {
        aiSnake.grow();
        food.place_food_safe(food, snake);
        food.setSpawnTime(now);
    }

    if (shieldItem.isActive() &&
        aiSnake.get_x(0) == shieldItem.getX() && aiSnake.get_y(0) == shieldItem.getY()) {
        aiSnake.getShield().activate(now);
        shieldItem.setActive(false);
        shieldEaten = true;
        lastShieldSpawnTime = now;
    }
}

void Level3::trySpawnBombZone(unsigned long now) {
    if (now - lastBombZoneSpawnTime < 15000) return;
    int half = BombZone::getSize() / 2;
    int minX = 1 + half;
    int maxX = (WIDTH - 2) - half;
    int minY = 1 + half;
    int maxY = (HEIGHT - 2) - half;
    if (minX > maxX || minY > maxY) return;
    for (int tries = 0; tries < 10; ++tries) {
        int cx = rand() % (maxX - minX + 1) + minX;
        int cy = rand() % (maxY - minY + 1) + minY;
        bool overlap = false;
        for (int i = 0; i < snake.get_length(); ++i) {
            if (snake.get_x(i) >= cx - half && snake.get_x(i) <= cx + half &&
                snake.get_y(i) >= cy - half && snake.get_y(i) <= cy + half) {
                overlap = true;
                break;
            }
        }
        if (!overlap) {
            for (int i = 0; i < aiSnake.get_length(); ++i) {
                if (aiSnake.get_x(i) >= cx - half && aiSnake.get_x(i) <= cx + half &&
                    aiSnake.get_y(i) >= cy - half && aiSnake.get_y(i) <= cy + half) {
                    overlap = true;
                    break;
                }
            }
        }
        if (!overlap) {
            bombZone.spawn(cx, cy, now);
            lastBombZoneSpawnTime = now;
            break;
        }
    }
}

void Level3::checkBombZoneDamage(unsigned long now) {
    if (!bombZone.isActive()) return;
    int remain = bombZone.getRemainingTime(now);
    if (remain == 0) {
        bool damaged = false;
        for (int i = 0; i < snake.get_length(); ++i) {
            if (bombZone.isInside(snake.get_x(i), snake.get_y(i))) {
                snake.set_blood(snake.get_blood() - 1);
                snake.setDamageFlash(now);
                damaged = true;
                break;
            }
        }
        if (damaged && snake.get_blood() <= 0) {
            gameOver = 1;
        }
        for (int i = 0; i < aiSnake.get_length(); ++i) {
            if (bombZone.isInside(aiSnake.get_x(i), aiSnake.get_y(i))) {
                aiSnake.set_blood(aiSnake.get_blood() - 1);
                aiSnake.setDamageFlash(now);
                if (aiSnake.get_blood() <= 0) {
                    aiSnake.setAlive(0);
                }
                break;
            }
        }
        bombZone.deactivate();
    }
}

void Level3::run() {
    Console console;
    console.hideCursor();
    console.clear();
    console.setUTF8();

    food.place_food_safe(food, snake);
    food.setSpawnTime(console.getTickMs());
    obstacleManager.initialize(snake, snake, food, 5, 3);
    shieldItem.place(WIDTH, HEIGHT, snake, obstacleManager, food);
    lastShieldSpawnTime = console.getTickMs();
    shieldEaten = false;

    lastMoveTime = console.getTickMs();
    unsigned long lastObstacleMove = lastMoveTime;
    lastBombZoneSpawnTime = 0;

    while (!gameOver) {
        if (_kbhit()) {
            int ch = _getch();
            int currentDir = snake.get_dir();
            if (ch == 0 || ch == 224) {
                ch = _getch();
                if (ch == 72 && currentDir != 2) snake.setDir(0);
                else if (ch == 80 && currentDir != 0) snake.setDir(2);
                else if (ch == 75 && currentDir != 1) snake.setDir(3);
                else if (ch == 77 && currentDir != 3) snake.setDir(1);
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
        trySpawnBombZone(now);
        checkBombZoneDamage(now);

        if (food.isExpired(now)) {
            food.place_food_safe(food, snake);
            food.setSpawnTime(now);
        }

        if (now - lastMoveTime >= static_cast<unsigned long>(snake.get_speed())) {
            lastMoveTime = now;
            snake.move();
            checkCollisionsAndEat(now);

            aiSnake.updateAIDirection(obstacleManager, food, shieldItem, bombZone, snake);
            if (aiSnake.is_alive()) aiSnake.move();
            checkAICollisionsAndEat(now);

            if (gameOver) break;
        }

        if (now - lastObstacleMove >= 200) {
            lastObstacleMove = now;
            obstacleManager.update(now);
        }

        trySpawnShield(now);

        drawGameFrame(snake, food, obstacleManager, shieldItem, aiSnake, bombZone, score, snake.get_speed());

        snake.getShield().update(now);
        aiSnake.getShield().update(now);

        Sleep(10);
    }

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