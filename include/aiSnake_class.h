#ifndef AISNAKE_CLASS_H
#define AISNAKE_CLASS_H


#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "shield.h"
#include "snake_class.h"
#include "obstacle.h"
#include "shield_item.h"
#include "bombzone.h"
#include <queue>

#ifndef WIDTH
#define WIDTH 40
#endif

#ifndef HEIGHT
#define HEIGHT 20
#endif

#ifndef MAX_SNAKE
#define MAX_SNAKE ((WIDTH -2) * (HEIGHT -2))
#endif

const int aibaseSpeedMs = 150;
const int foodDistance = 10;
const int shieldDistance = 8;

struct AStarNode {
    int x, y;       // 坐标
    int g;           // 从起点走到这里的步数
    int h;           // 从这里到终点的估算距离（曼哈顿）
    int f;           // f = g + h，越小越优先
    AStarNode* parent; // 从哪个节点走过来的（用于回溯路径）

    AStarNode(int x, int y, int g, int h, AStarNode* parent)
        : x(x), y(y), g(g), h(h), f(g + h), parent(parent) {}
};

// 优先队列比较器：f 值小的排前面
struct CompareNode {
    bool operator()(AStarNode* a, AStarNode* b) {
        return a->f > b->f;
    }
};


class aiSnake: public Snake {
public:
    aiSnake(int startX, int startY);
    void updateAIDirection(const ObstacleManager& obstacleManager, const Food& food, const ShieldItem& shieldItem, const BombZone& bombZone, const Snake& snake);
    int safeAIDirection(int cur_dir, const ObstacleManager& obstacleManager, const BombZone& bombZone, const Snake& snake);
protected:

private:
    bool isBlocked(int x, int y, const ObstacleManager& obstacleManager, const BombZone& bombZone);
    int astarPathFind(int startX, int startY, int goalX, int goalY,
                           const ObstacleManager& obstacleManager, const BombZone& bombZone);
};



#endif