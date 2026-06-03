#ifndef LEVEL1_H
#define LEVEL1_H

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
#include "bombzone.h"

class Level1 {
public:
    Level1();
    void run();

private:
    int score;
    int gameOver;
    int startX, startY;
    Snake snake;
    Food food;
    ObstacleManager obstacleManager;
    ShieldItem shieldItem;
    BombZone bombZone;
    void trySpawnBombZone(unsigned long now);
    void checkBombZoneDamage(unsigned long now);
    unsigned long lastMoveTime;   // 上次移动的时间
    bool shieldEaten;
    unsigned long lastShieldSpawnTime;   // 上次生成护盾的时间（若吃掉了，延迟5秒再生成）
    unsigned long lastBombZoneSpawnTime;   // 上次生成炸弹区域的时间                   

    void handleSpeedBoost(Snake& s);
    void checkCollisionsAndEat(unsigned long now);
    void trySpawnShield(unsigned long now);
};

#endif