#ifndef LEVEL1_H
#define LEVEL1_H

#include "snake_class.h"
#include "food.h"
#include "obstacle.h"
#include "shield_item.h"
#include "game_shared.h"
#include "game_ui.h"
#include "leaderboard.h"

class Level1 {
public:
    Level1();
    void run();

private:
    int score;
    int gameOver;
    int baseSpeedMs;
    int currentSpeedMs;
    int startX, startY;
    Snake snake;
    Food food;
    ObstacleManager obstacleManager;
    ShieldItem shieldItem;
    unsigned long lastMoveTime;
    unsigned long lastShieldSpawnTime;   // 上次生成护盾的时间（若吃掉了，延迟5秒再生成）
    bool shieldEaten;                     // 护盾是否被吃掉，等待重生

    void handleSpeedBoost();
    void checkCollisionsAndEat(unsigned long now);
    void trySpawnShield(unsigned long now);
};

#endif