#ifndef LEVEL3_H
#define LEVEL3_H

#include "snake_class.h"
#include "food.h"
#include "obstacle.h"
#include "shield_item.h"
#include "game_shared.h"
#include "game_ui.h"
#include "leaderboard.h"
#include <cstring>
#include <conio.h>
#include <windows.h>
#include "aiSnake_class.h"

class Level3 {
public:
    Level3();
    void run();

private:
    int score;
    int gameOver;
    Snake snake;
    aiSnake aiSnake;
    Food food;
    ObstacleManager obstacleManager;
    ShieldItem shieldItem;
    unsigned long lastMoveTime;   // 上次移动的时间
    unsigned long lastShieldSpawnTime;   // 上次生成护盾的时间（若吃掉了，延迟5秒再生成）
    bool shieldEaten;                     // 护盾是否被吃掉，等待重生

    void handleSpeedBoost(Snake& s);
    void checkCollisionsAndEat(unsigned long now);
    void checkAICollisionsAndEat(unsigned long now);
    void trySpawnShield(unsigned long now);
};
#endif