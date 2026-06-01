#include "../include/level2.h"
#include "../include/game_ui.h"
#include "../include/leaderboard.h"
#include <conio.h>
#include <windows.h>
#include <cstdio>

static void drawGameFrame2(const Snake& s1, const Snake& s2,
                           const Food& food,
                           const ObstacleManager& obstacles,
                           const ShieldItem& shield,
                           int score1, int score2, int speedMs) {
    Console console;
    console.setCursorPos(0, 0);
    printf("=== 双人对战模式 ===\n");
    printf("玩家1 (O): %d 分  ", score1);
    if (s1.getShield().isActive()) {
        unsigned long now = console.getTickMs();
        float remain = s1.getShield().getRemainingTime(now) / 1000.0f;
        if (remain > 0) printf("[护盾: %.1fs] ", remain);
    }
    printf(" | 玩家2 (X): %d 分  ", score2);
    if (s2.getShield().isActive()) {
        unsigned long now = console.getTickMs();
        float remain = s2.getShield().getRemainingTime(now) / 1000.0f;
        if (remain > 0) printf("[护盾: %.1fs]", remain);
    }
    printf("\n");
    printf("玩家1: 方向键移动 | 玩家2: WASD移动 | 加速: 玩家1按Shift, 玩家2按E | Esc退出\n");
    printf("护盾道具(S) : 免疫障碍物10秒\n");
    printf("当前速度: %d ms\n", speedMs);

    for (int i = 0; i < WIDTH; ++i) putchar('#');
    putchar('\n');

    for (int y = 0; y < HEIGHT - 2; ++y) {
        putchar('#');
        for (int x = 0; x < WIDTH - 2; ++x) {
            char ch = ' ';

            if (x == food.get_x() && y == food.get_y()) {
                ch = '*';
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                putchar(ch);
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                continue;
            }
            else if (shield.isActive() && x == shield.getX() && y == shield.getY()) {
                ch = 'S';
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                putchar(ch);
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                continue;
            }
            else if (obstacles.checkCollision(x, y)){
                ch = '@';
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED);
                putchar(ch);
                SetConsoleTextAttribute(console.getHandle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                continue;
            }
            else {
                for (int k = 0; k < s1.get_length(); ++k) {
                    if (x == s1.get_x(k) && y == s1.get_y(k)) {
                        ch = (k == 0) ? 'O' : 'o';
                        break;
                    }
                }
                if (ch == ' ') {
                    for (int k = 0; k < s2.get_length(); ++k) {
                        if (x == s2.get_x(k) && y == s2.get_y(k)) {
                            ch = (k == 0) ? 'X' : 'x';
                            break;
                        }
                    }
                }
            }
            if (ch == 'O' || ch == 'o') {
                bool flashing = s1.isDamageFlashing(console.getTickMs());
                bool bright = flashing && ((console.getTickMs() / 100) % 2 == 0);
                WORD normalColor;
                if (s1.getShield().isActive()) {
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
                bool flashing = s2.isDamageFlashing(console.getTickMs());
                bool bright = flashing && ((console.getTickMs() / 100) % 2 == 0);
                WORD normalColor;
                if (s2.getShield().isActive()) {
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
}

Level2::Level2()
    : score1(0), score2(0), gameOver(0),
      snake1((WIDTH - 2) / 4, (HEIGHT - 2) / 2, 1),
      snake2(3 * (WIDTH - 2) / 4, (HEIGHT - 2) / 2, 2),
      lastMoveTime(0), lastObstacleMoveTime(0),
      lastShieldSpawnTime(0), shieldEaten(false) {}

void Level2::handleInput() {
    while (_kbhit()) {
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch();
            int dir1 = snake1.get_dir();
            if (ch == 72 && dir1 != 2) snake1.setDir(0);
            else if (ch == 80 && dir1 != 0) snake1.setDir(2);
            else if (ch == 75 && dir1 != 1) snake1.setDir(3);
            else if (ch == 77 && dir1 != 3) snake1.setDir(1);
        } else {
            int dir2 = snake2.get_dir();
            if ((ch == 'W' || ch == 'w') && dir2 != 2) snake2.setDir(0);
            else if ((ch == 'S' || ch == 's') && dir2 != 0) snake2.setDir(2);
            else if ((ch == 'A' || ch == 'a') && dir2 != 1) snake2.setDir(3);
            else if ((ch == 'D' || ch == 'd') && dir2 != 3) snake2.setDir(1);
            else if (ch == 27) gameOver = 1;
        }
    }
}

void Level2::handleSpeedBoost() {
    bool boost = false;
    if (GetAsyncKeyState(VK_LSHIFT) & 0x8000 || GetAsyncKeyState(VK_RSHIFT) & 0x8000)
        boost = true;
    if (GetAsyncKeyState('E') & 0x8000)
        boost = true;
    if (boost) {
        snake1.set_speed(snake1.get_speed() / 2);
        snake2.set_speed(snake2.get_speed() / 2);
        if (snake1.get_speed() < 60) snake1.set_speed(60);
        if (snake2.get_speed() < 60) snake2.set_speed(60);
    } else {
        snake1.set_speed(200);
        snake2.set_speed(200);
    }
}

void Level2::trySpawnShield(unsigned long now) {
    if (!shieldItem.isActive() && !shieldEaten) {
        if (now - lastShieldSpawnTime > 5000) {
            shieldItem.place(WIDTH, HEIGHT, snake1, obstacleManager, food);
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

void Level2::checkShieldPickup(unsigned long now) {  // 吃到护盾道具
    if (shieldItem.isActive() && snake1.is_alive() &&
        snake1.get_x(0) == shieldItem.getX() && snake1.get_y(0) == shieldItem.getY()) {
        snake1.getShield().activate(now);
        shieldItem.setActive(false);
        shieldEaten = true;
        lastShieldSpawnTime = now;
    }
    else if (shieldItem.isActive() && snake2.is_alive() &&
        snake2.get_x(0) == shieldItem.getX() && snake2.get_y(0) == shieldItem.getY()) {
        snake2.getShield().activate(now);
        shieldItem.setActive(false);
        shieldEaten = true;
        lastShieldSpawnTime = now;
    }
}

void Level2::handleNonLethal() {  // 吃到食物
    // 吃食物
    if (snake1.is_alive() && snake1.get_x(0) == food.get_x() && snake1.get_y(0) == food.get_y()) {
        snake1.grow(snake1);
        score1++;
        food.place_food_safe(food, snake1);
    }
    if (snake2.is_alive() && snake2.get_x(0) == food.get_x() && snake2.get_y(0) == food.get_y()) {
        snake2.grow(snake2);
        score2++;
        food.place_food_safe(food, snake2);
    }
}

void Level2::updateGame(unsigned long now) {
    if (now - lastMoveTime >= static_cast<unsigned long>(snake1.get_speed())) {
        lastMoveTime = now;
        if (gameOver) return;

        // 预计算新头位置
        int newX1 = snake1.get_x(0), newY1 = snake1.get_y(0);
        int newX2 = snake2.get_x(0), newY2 = snake2.get_y(0);
        switch(snake1.get_dir()) {
            case 0: newY1--; break;
            case 1: newX1++; break;
            case 2: newY1++; break;
            case 3: newX1--; break;
        }
        switch(snake2.get_dir()) {
            case 0: newY2--; break;
            case 1: newX2++; break;
            case 2: newY2++; break;
            case 3: newX2--; break;
        }

        // 预判玩家1是否会死亡
        bool die1 = false;
        if (snake1.is_alive()) {
            if (newX1 < 0 || newX1 >= WIDTH-2 || newY1 < 0 || newY1 >= HEIGHT-2)
                die1 = true;
            if (!die1) {
                for (int i = 1; i < snake1.get_length(); ++i) {
                    if (newX1 == snake1.get_x(i) && newY1 == snake1.get_y(i)) {
                        die1 = true;
                        break;
                    }
                }
            }
            if (!die1 && obstacleManager.checkCollision(newX1, newY1)) {
                if (!snake1.getShield().isActive()){
                    snake1.set_blood(snake1.get_blood() - 1);
                    snake1.setDamageFlash(now);
                    snake1.move_with_collision(snake1);
                    if (snake1.get_blood() <= 0) {
                        die1 = true;
                    }
                }
                snake1.move_with_collision(snake1);
            }
            if (!die1 && snake2.is_alive()) {
                for (int i = 0; i < snake2.get_length(); ++i) {
                    if (newX1 == snake2.get_x(i) && newY1 == snake2.get_y(i)) {
                        snake1.set_blood(snake1.get_blood() - 1);
                        snake1.setDamageFlash(now);
                        snake1.move_with_collision(snake1);
                        if (snake1.get_blood() <= 0) {
                            die1 = true;
                        }
                        break;
                    }
                }
            }
        }

        // 预判玩家2
        bool die2 = false;
        if (snake2.is_alive()) {
            if (newX2 < 0 || newX2 >= WIDTH-2 || newY2 < 0 || newY2 >= HEIGHT-2)
                die2 = true;
            if (!die2) {
                for (int i = 1; i < snake2.get_length(); ++i) {
                    if (newX2 == snake2.get_x(i) && newY2 == snake2.get_y(i)) {
                        die2 = true;
                        break;
                    }
                }
            }
            if (!die2 && obstacleManager.checkCollision(newX2, newY2)) {
                if (!snake2.getShield().isActive()){
                    snake2.set_blood(snake2.get_blood() - 1);
                    snake2.setDamageFlash(now);
                    snake2.move_with_collision(snake2);
                    if (snake2.get_blood() <= 0) {
                        die2 = true;
                    }
                }
                snake2.move_with_collision(snake2);
            }
            if (!die2 && snake1.is_alive()) {
                for (int i = 0; i < snake1.get_length(); ++i) {
                    if (newX2 == snake1.get_x(i) && newY2 == snake1.get_y(i)) {
                        snake2.set_blood(snake2.get_blood() - 1);
                        snake2.setDamageFlash(now);
                        snake2.move_with_collision(snake2);
                        if (snake2.get_blood() <= 0) {
                            die2 = true;
                        }
                        break;
                    }
                }
            }
        }

        // 如果任何一方会死，游戏结束
        if (die1 || die2) {
            gameOver = 1;
            return;
        }

        // 安全移动
        if (snake1.is_alive()) snake1.move(snake1);
        if (snake2.is_alive()) snake2.move(snake2);

        // 消耗护盾
        if (snake1.is_alive() && obstacleManager.checkCollision(snake1.get_x(0), snake1.get_y(0))) {
            snake1.getShield().tryDefend();
        }
        if (snake2.is_alive() && obstacleManager.checkCollision(snake2.get_x(0), snake2.get_y(0))) {
            snake2.getShield().tryDefend();
        }

        // 处理食物和护盾拾取
        handleNonLethal();
    }

    // 移动障碍物
    if (now - lastObstacleMoveTime >= 200) {
        lastObstacleMoveTime = now;
        obstacleManager.update(now);
    }

    // 护盾生成和拾取
    trySpawnShield(now);
    checkShieldPickup(now);

    // 更新护盾计时
    snake1.getShield().update(now);
    snake2.getShield().update(now);
}

void Level2::drawGame() {
    drawGameFrame2(snake1, snake2, food, obstacleManager, shieldItem,
                   score1, score2, snake1.get_speed());
}

void Level2::showResults() {
    Console console;
    console.setCursorPos(0, HEIGHT + 5);
    printf("\n=== 游戏结束 ===\n");
    printf("玩家1最终得分: %d\n", score1);
    printf("玩家2最终得分: %d\n", score2);
    if (score1 > score2) printf("玩家1获胜！\n");
    else if (score2 > score1) printf("玩家2获胜！\n");
    else printf("平局！\n");

    printf("\n玩家1姓名 (回车跳过): ");
    char name1[32] = {0};
    if (fgets(name1, sizeof(name1), stdin)) {
        size_t len = strlen(name1);
        if (len > 0 && name1[len-1] == '\n') name1[len-1] = '\0';
        if (name1[0] != '\0') {
            Leaderboard lb;
            lb.update(name1, score1);
        }
    }
    printf("玩家2姓名 (回车跳过): ");
    char name2[32] = {0};
    if (fgets(name2, sizeof(name2), stdin)) {
        size_t len = strlen(name2);
        if (len > 0 && name2[len-1] == '\n') name2[len-1] = '\0';
        if (name2[0] != '\0') {
            Leaderboard lb;
            lb.update(name2, score2);
        }
    }

    printf("\n按任意键返回菜单...");
    while (!_kbhit()) {}
    while (_kbhit()) _getch();
}

void Level2::run() {
    Console console;
    console.hideCursor();
    console.clear();
    console.setUTF8();

    snake1 = Snake((WIDTH - 2) / 4, (HEIGHT - 2) / 2, 1);
    snake2 = Snake(3 * (WIDTH - 2) / 4, (HEIGHT - 2) / 2, 2);
    score1 = score2 = 0;
    gameOver = 0;

    food.place_food_safe(food, snake1);
    obstacleManager.initialize(snake1, snake2, food, 5, 3);

    shieldItem.place(WIDTH, HEIGHT, snake1, obstacleManager, food);
    lastShieldSpawnTime = console.getTickMs();
    shieldEaten = false;

    lastMoveTime = console.getTickMs();
    lastObstacleMoveTime = lastMoveTime;

    while (!gameOver) {
        handleInput();
        handleSpeedBoost();

        unsigned long now = console.getTickMs();
        updateGame(now);
        drawGame();

        Sleep(10);
    }

    showResults();
}