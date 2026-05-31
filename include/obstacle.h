#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "snake_class.h"
#include "food.h"

#ifndef WIDTH
#define WIDTH 40
#endif

#ifndef HEIGHT
#define HEIGHT 20
#endif

#define MAX_OBSTACLES 20

enum ObstacleType {
    OBS_FIXED = 0,    // 固定障碍物
    OBS_MOVING = 1    // 移动障碍物
};

class Obstacle {
public:
    Obstacle();
    Obstacle(int x, int y, ObstacleType type);
    
    bool placeObstacle(const Snake& s1, const Snake& s2, const Food& f);
    void move();
    bool checkCollision(int px, int py) const;
    int getX() const { return x; }
    int getY() const { return y; }
    ObstacleType getType() const { return type; }
    bool isActive() const { return active; }
    
private:
    int x, y;
    ObstacleType type;
    int dx, dy;  // 移动方向
    bool active;
    unsigned long lastMoveTime;
};

class ObstacleManager {
public:
    ObstacleManager();
    
    void initialize(const Snake& s1, const Snake& s2, const Food& f, int fixedCount, int movingCount);
    void update(unsigned long currentTime);
    bool checkCollision(int x, int y) const;
    int getCount() const { return obstacleCount; }
    const Obstacle& getObstacle(int index) const { return obstacles[index]; }
    
private:
    Obstacle obstacles[MAX_OBSTACLES];
    int obstacleCount;
};

#endif