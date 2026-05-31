#ifndef LEVEL1_H
#define LEVEL1_H

#include "../include/snake_class.h"
#include "../include/food.h"

class Level1 {
public:
    Level1();
    void run();

private:
    int score;
    int gameOver;
    int baseSpeedMs;       // 基础移动间隔（毫秒）
    int currentSpeedMs;    // 当前移动间隔（受shift影响）
    int startX, startY;
    Snake snake;
    Food food;
    unsigned long lastMoveTime;

    void handleSpeedBoost();  // 检测shift并更新currentSpeedMs
};

#endif