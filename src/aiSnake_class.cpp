#include "../include/aiSnake_class.h"
#include "../include/level3.h"
#include <cstdlib>
#include "../include/obstacle.h"
#include "../include/snake_class.h"

aiSnake::aiSnake(int startX, int startY) : Snake(startX, startY, 2) {
    x[0] = startX;
    y[0] = startY;
    length = 3;
    dir = 3;
    alive = 1;
    blood = 3;
    speedMS = baseSpeedMs;
    damageFlashTime = 0;
    speedMS = aibaseSpeedMs;  // AI蛇默认速度更快

    for (int i = 0; i < length; i++) {
        x[i] = startX + i;  //ai蛇从右到左
        y[i] = startY;
    }
}

int aiSnake::safeAIDirection(int cur_dir, const ObstacleManager& obstacleManager, const BombZone& bombZone, const Snake& snake) {
    int hx = x[0];
    int hy = y[0];

    // 先检查当前方向是否安全
    int nx = hx, ny = hy;
    switch (cur_dir) {
        case 0: ny--; break;
        case 1: nx++; break;
        case 2: ny++; break;
        case 3: nx--; break;
    }

    bool hitWall = (nx < 0 || nx >= WIDTH - 2 || ny < 0 || ny >= HEIGHT - 2);
    bool hitSelf = false;
    if (!hitWall) {
        for (int i = 1; i < length; ++i) {
            if (nx == x[i] && ny == y[i]) { hitSelf = true; break; }
        }
    }
    bool hitObs = (!hitWall && obstacleManager.checkCollision(nx, ny));
    bool inBomb = (!hitWall && bombZone.isActive() && bombZone.isInside(nx, ny));
    bool hitsnake = false;
    int fate = rand() % 2;
    if (fate == 0) {
        for (int i = 1; i < snake.get_length(); ++i) {
                if (nx == snake.get_x(i) && ny == snake.get_y(i)) { hitsnake = true; break; }
        }
    }

    if (!hitWall && !hitSelf && !hitObs && !inBomb && !hitsnake)
        return cur_dir;

    // 当前方向不安全，尝试所有四个方向
    int allDirs[4] = {0, 1, 2, 3};
    // 随机打乱顺序
    for (int i = 3; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = allDirs[i]; allDirs[i] = allDirs[j]; allDirs[j] = tmp;
    }

    for (int i = 0; i < 4; ++i) {
        int tryDir = allDirs[i];
        int tx = hx, ty = hy;
        switch (tryDir) {
            case 0: ty--; break;
            case 1: tx++; break;
            case 2: ty++; break;
            case 3: tx--; break;
        }
        bool hitWall = (tx < 0 || tx >= WIDTH - 2 || ty < 0 || ty >= HEIGHT - 2);
        bool hitSelf = false;
        if (!hitWall) {
            for (int i = 1; i < length; ++i) {
                if (tx == x[i] && ty == y[i]) { hitSelf = true; break; }
            }
        }
        bool hitObs = (!hitWall && obstacleManager.checkCollision(tx, ty));
        bool inBomb = (!hitWall && bombZone.isActive() && bombZone.isInside(tx, ty));
        bool hitsnake = false;
        int fate = rand() % 2;
        if (fate == 0) {
            for (int i = 1; i < snake.get_length(); ++i) {
                    if (tx == snake.get_x(i) && ty == snake.get_y(i)) { hitsnake = true; break; }
            }
        }
        if (!hitWall && !hitSelf && !hitObs && !inBomb && !hitsnake)
            return tryDir;
    }

    return cur_dir;
}

void aiSnake::updateAIDirection(const ObstacleManager& obstacleManager, const Food& food, const ShieldItem& shieldItem, const BombZone& bombZone, const Snake& snake) {
    int foodX = food.get_x();
    int foodY = food.get_y();

    int dist = abs(x[0] - foodX) + abs(y[0] - foodY);
    if (dist <= foodDistance) {
        int nextDir = astarPathFind(x[0], y[0], foodX, foodY, obstacleManager, bombZone);
        if (nextDir != -1) {
            int safeDir = safeAIDirection(nextDir, obstacleManager, bombZone, snake);
            if (safeDir == nextDir) {
                dir = nextDir;
                return;
            }
        }
    }

    if (shieldItem.isActive()) {
    int sx = shieldItem.getX(), sy = shieldItem.getY();
    int distShield = abs(x[0] - sx) + abs(y[0] - sy);
    if (distShield <= shieldDistance) {
        int nextDir = astarPathFind(x[0], y[0], sx, sy, obstacleManager, bombZone);
        if (nextDir != -1) {
            int safeDir = safeAIDirection(nextDir, obstacleManager, bombZone, snake);
            if (safeDir == nextDir) {
                dir = safeDir;
                return;
            }
        }
    }
}

    if (rand() % 10 < 4) {
        int curDir = dir;
        int newDir;

        if (curDir == 0 || curDir == 2) {
            int choices[2] = {3, 1};
            newDir = choices[rand() % 2];
        } else {
            int choices[2] = {0, 2};
            newDir = choices[rand() % 2];
        }

        newDir = safeAIDirection(newDir, obstacleManager, bombZone, snake);
        dir = newDir;
    } else {
        int curDir = dir;
        int safeDir = safeAIDirection(curDir, obstacleManager, bombZone, snake);
        if (safeDir != curDir) dir = safeDir;
    }
}

// 曼哈顿距离启发函数
int manhattan(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}


bool aiSnake::isBlocked(int x, int y, const ObstacleManager& obstacleManager, const BombZone& bombZone) {
    if (x < 0 || x >= WIDTH - 2 || y < 0 || y >= HEIGHT - 2)
        return true;
    for (int i = 1; i < length; i++) {
        if (x == this->x[i] && y == this->y[i])
            return true;
    }
    if (obstacleManager.checkCollision(x, y))
        return true;
    if (bombZone.isActive() && bombZone.isInside(x, y))
        return true;
    return false;
}

int aiSnake::astarPathFind(int startX, int startY, int goalX, int goalY,
                           const ObstacleManager& obstacleManager, const BombZone& bombZone) {
    // 目标就在脚下
    if (startX == goalX && startY == goalY)
        return -1;

    // 用于判断是否走过了
    bool closed[WIDTH][HEIGHT] = {};

    // 四个方向：上、右、下、左
    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {-1, 0, 1, 0};
    // 方向编号：  0   1   2    3

    // open 是待探索的格子，按 f 值从小到大排列
    std::priority_queue<AStarNode*, std::vector<AStarNode*>, CompareNode> open;
    // 创建优先队列
    // <元素类型 底层容器 比较器类型>

    // 起点入队
    AStarNode* startNode = new AStarNode(startX, startY, 0,
                                          manhattan(startX, startY, goalX, goalY), nullptr);
    // push 入队
    open.push(startNode);

    while (!open.empty()) {
        // 取出 f 值最小的节点
        AStarNode* cur = open.top();
        // 出队
        open.pop();

        // 已经探索过就跳过
        if (closed[cur->x][cur->y])
            continue;
        closed[cur->x][cur->y] = true;

        // 到达终点！回溯路径，返回起点方向
        if (cur->x == goalX && cur->y == goalY) {
            // 往回找，直到找到起点的下一个点
            AStarNode* p = cur;
            while (p->parent != nullptr && p->parent->parent != nullptr) {
                p = p->parent;
            }
            // 从起点到 p 的方向就是答案
            if (p->x == startX && p->y < startY) return 0; // 上
            if (p->x > startX && p->y == startY) return 1; // 右
            if (p->x == startX && p->y > startY) return 2; // 下
            if (p->x < startX && p->y == startY) return 3; // 左
            return -1;
        }

        // 探索四个邻居
        for (int i = 0; i < 4; i++) {
            int nx = cur->x + dx[i];
            int ny = cur->y + dy[i];

            // 越界、走过、不可通行，都跳过
            if (nx < 0 || nx >= WIDTH - 2 || ny < 0 || ny >= HEIGHT - 2) continue;
            if (closed[nx][ny]) continue;
            if (isBlocked(nx, ny, obstacleManager, bombZone)) continue;

            int g = cur->g + 1;
            int h = manhattan(nx, ny, goalX, goalY);
            AStarNode* neighbor = new AStarNode(nx, ny, g, h, cur);
            // 入队
            open.push(neighbor);
        }
    }

    return -1; // 找不到路
}