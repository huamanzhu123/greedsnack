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
#include "bombzone.h"

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
    BombZone bombZone;
    unsigned long lastMoveTime;
    unsigned long lastShieldSpawnTime;
    unsigned long lastBombZoneSpawnTime;
    bool shieldEaten;

    void handleSpeedBoost(Snake& s);
    void checkCollisionsAndEat(unsigned long now);
    void checkAICollisionsAndEat(unsigned long now);
    void trySpawnShield(unsigned long now);
    void trySpawnBombZone(unsigned long now);
    void checkBombZoneDamage(unsigned long now);
};
#endif