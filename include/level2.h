#ifndef LEVEL2_H
#define LEVEL2_H

#include "../include/snake_class.h"
#include "../include/food.h"
#include "../include/obstacle.h"
#include "../include/shield_item.h"
#include "../include/game_shared.h"

class Level2 {
public:
    Level2();
    void run();

private:
    int score1, score2;
    int gameOver;
    int baseSpeedMs;
    int currentSpeedMs;
    
    Snake snake1;
    Snake snake2;
    Food food;
    ObstacleManager obstacleManager;
    ShieldItem shieldItem;
    
    unsigned long lastMoveTime;
    unsigned long lastObstacleMoveTime;
    unsigned long lastShieldSpawnTime;
    bool shieldEaten;
    
    void handleInput();
    void handleSpeedBoost();
    void updateGame(unsigned long now);
    void trySpawnShield(unsigned long now);
    void checkShieldPickup(unsigned long now);
    void handleNonLethal(unsigned long now);
    void drawGame();
    void showResults();
};

#endif