#include "../include/level1.h"

// 辅助绘制函数，包含高分食物紫色闪烁效果
static void drawGameFrame(const Snake& snake, const Food& food,
                          const ObstacleManager& obstacles,
                          const ShieldItem& shield,
                          const BombZone& bombZone,
                          int score, int speedMs) {
    Console console;
    console.setCursorPos(0, 0);
    printf("分数: %d     速度: %d ms   血量：%d  能量：%d  方向: Arrow/WASD,    Shift加速, Esc退出\n", score, speedMs, snake.get_blood(), snake.get_energy());

    // 上边框
    for (int i = 0; i < WIDTH; ++i) putchar('#');
    putchar('\n');

    for (int y = 0; y < HEIGHT - 2; ++y) {
        putchar('#');
        for (int x = 0; x < WIDTH - 2; ++x) {
            char ch = ' ';

            // 食物处理（区分普通/高分）
            if (x == food.get_x() && y == food.get_y()) {
                if (food.get_type() == Food::TYPE_HIGHSCORE) {
                    ch = '*';
                    // 紫色闪烁实现频闪 每200ms切换深浅
                    unsigned long now = console.getTickMs();
                    bool bright = ((now / 200) % 2) == 0;
                    WORD color;
                    if (bright)
                        color = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; // 亮紫
                    else
                        color = FOREGROUND_RED | FOREGROUND_BLUE;                       // 暗紫
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
            // 在绘制食物/护盾/障碍物之后，蛇之前插入以下代码
            else if (bombZone.isActive() && x >= bombZone.getX() - 1 && x <= bombZone.getX() + 1 &&
                    y >= bombZone.getY() - 1 && y <= bombZone.getY() + 1) {
                if(x == bombZone.getX() && y == bombZone.getY()){
                    ch = 'Q';
                } else {
                    ch = ' ';
                }
                // 设置红色背景或高亮
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED);
                putchar(ch);
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                continue;
            }
            // 蛇
            else {
                bool flashing = snake.isDamageFlashing(console.getTickMs());
                bool bright = flashing && ((console.getTickMs() / 100) % 2 == 0);
                WORD normalColor;
                if (snake.getShield().isActive()) {
                    normalColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;  // 无敌表现为亮蓝
                } else {
                    normalColor = FOREGROUND_GREEN;  // 常规是绿色
                }
                WORD flashColor = bright ? (FOREGROUND_RED | FOREGROUND_INTENSITY) : normalColor;
                for (int k = 0; k < snake.get_length(); ++k) {
                    if (x == snake.get_x(k) && y == snake.get_y(k)) {
                        ch = (k == 0) ? 'O' : 'o';
                        break;
                    }
                }
                SetConsoleTextAttribute(console.getHandle(), flashColor);
                putchar(ch);
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                continue;
            }
            putchar(ch);
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

    // 显示炸弹区域倒计时
    if (bombZone.isActive()) {
        int remain = bombZone.getRemainingTime(console.getTickMs());
        printf("炸弹区域倒计时: %d 秒\n", remain);
    }
}

Level1::Level1()
    : score(0), gameOver(0),
      startX((WIDTH - 2) / 2), startY((HEIGHT - 2) / 2),
      snake(startX, startY),
      lastMoveTime(0), lastShieldSpawnTime(0), shieldEaten(false) ,lastBombZoneSpawnTime(0) {}

// 处理加速逻辑，按住Shift键时启动5秒加速（速度减半，但最低不能低于60ms）
void Level1::handleSpeedBoost(Snake& s) {
    static unsigned long boostEndTime = 0;
    unsigned long now = GetTickCount();
    bool shiftDown = (GetAsyncKeyState(VK_LSHIFT) & 0x8000) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000);

    if (shiftDown && s.get_energy() == 3 && now >= boostEndTime) {
        // 启动加速持续5秒
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

void Level1::trySpawnShield(unsigned long now) {
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

void Level1::checkCollisionsAndEat(unsigned long now) {
    // 吃到护盾道具
    if (shieldItem.isActive() &&
        snake.get_x(0) == shieldItem.getX() && snake.get_y(0) == shieldItem.getY()) {
        snake.getShield().activate(now);
        shieldItem.setActive(false);
        shieldEaten = true;
        lastShieldSpawnTime = now;
    }

    // 吃到食物（根据类型加分）
    if (snake.get_x(0) == food.get_x() && snake.get_y(0) == food.get_y()) {
        snake.grow();
        if (food.get_type() == Food::TYPE_HIGHSCORE)
            score += 3;
            if(snake.get_energy() < 3){
                snake.set_energy(snake.get_energy() + 1);
            }
        else
            if(snake.get_energy() < 3){
                    snake.set_energy(snake.get_energy() + 1);
                }
            score += 1;
        food.place_food_safe(food, snake);
        food.setSpawnTime(now);
    }

    // 碰撞检测（护盾只免疫障碍物）
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
        return;
    }
}
// 轰炸区相关函数的定义
void Level1::trySpawnBombZone(unsigned long now) {
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
            bombZone.spawn(cx, cy, now);
            lastBombZoneSpawnTime = now;
            break;
        }
    }
}

void Level1::checkBombZoneDamage(unsigned long now){
    if(!bombZone.isActive()) return;
    int remain = bombZone.getRemainingTime(now);
    if(remain ==0){
        bool damaged = false;
        for (int i = 0; i < snake.get_length(); ++i){
            if(bombZone.isInside(snake.get_x(i), snake.get_y(i))){
                snake.set_blood(snake.get_blood() - 1);
                snake.setDamageFlash(now);
                damaged = true;
                break;
            }
        }
        if(damaged && snake.get_blood() <= 0){
            gameOver = 1;
        }
        bombZone.deactivate();
    }
}

void Level1::run() {
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
            if (gameOver) break;
        }

        if (now - lastObstacleMove >= 200) {
            lastObstacleMove = now;
            obstacleManager.update(now);
        }

        trySpawnShield(now);

        drawGameFrame(snake, food, obstacleManager, shieldItem, bombZone, score, snake.get_speed());

        snake.getShield().update(now);

        Sleep(10);
    }

    console.setCursorPos(0, HEIGHT + 2);
    while (_kbhit()) _getch();

    printf("\n游戏结束!你的最终得分为: %d\n", score);
    printf("输入你的名字(最多31个字符): ");
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

    printf("\n按任意键返回菜单...");
    while (!_kbhit()) {}
    while (_kbhit()) _getch();
}