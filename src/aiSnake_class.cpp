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

int aiSnake::safeAIDirection(int cur_dir, const ObstacleManager& obstacleManager) {
    int hx = x[0];  // AI蛇头x坐标
    int hy = y[0];  // AI蛇头y坐标

    // 模拟当前方向走一步
    int nx = hx, ny = hy;
    switch (cur_dir) {
        case 0: ny--; break;  // 上
        case 1: nx++; break;  // 右
        case 2: ny++; break;  // 下
        case 3: nx--; break;  // 左
    }

    // 当前方向安全，直接返回
    bool hitWall = (nx < 0 || nx >= WIDTH - 2 || ny < 0 || ny >= HEIGHT - 2);
    bool hitSelf = false;
    if (!hitWall) {
        for (int i = 1; i < length - 1; ++i) {
            if (nx == x[i] && ny == y[i]) { hitSelf = true; break; }
        }
    }
    bool hitObs = (!hitWall && obstacleManager.checkCollision(nx, ny));
    if (!hitWall && !hitSelf && !hitObs)
        return cur_dir;

    // 当前方向不安全，尝试两个垂直方向
    int candidates[2];
    if (cur_dir == 0 || cur_dir == 2) {       // 上/下 → 候选左/右
        candidates[0] = 3; candidates[1] = 1;
    } else {                                  // 左/右 → 候选上/下
        candidates[0] = 0; candidates[1] = 2;
    }

    // 随机顺序尝试两个候选方向
    int start = rand() % 2;
    for (int i = 0; i < 2; ++i) {
        int tryDir = candidates[(start + i) % 2];
        int tx = hx, ty = hy;
        switch (tryDir) {
            case 0: ty--; break;  // 上
            case 1: tx++; break;  // 右
            case 2: ty++; break;  // 下
            case 3: tx--; break;  // 左
        }
        bool hitWall = (tx < 0 || tx >= WIDTH - 2 || ty < 0 || ty >= HEIGHT - 2);
        bool hitSelf = false;
        if (!hitWall) {
            for (int i = 1; i < length - 1; ++i) {
                if (tx == x[i] && ty == y[i]) { hitSelf = true; break; }
            }
        }
        bool hitObs = (!hitWall && obstacleManager.checkCollision(tx, ty));
        if (!hitWall && !hitSelf && !hitObs)
            return tryDir;
        }

    return cur_dir;
}

void aiSnake::updateAIDirection(const ObstacleManager& obstacleManager, const Food& food, const ShieldItem& shieldItem) {
    // 获取食物的位置
    int foodX = food.get_x();
    int foodY = food.get_y();

    // 只有距离较近时才追踪食物
    int dist = abs(x[0] - foodX) + abs(y[0] - foodY);
    // 如果食物在foodDistance距离内，尝试用A*追踪
    if (dist <= foodDistance) {
        int nextDir = astarPathFind(x[0], y[0], foodX, foodY, obstacleManager);
        if (nextDir != -1) {
            int safeDir = safeAIDirection(nextDir, obstacleManager);
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
        int nextDir = astarPathFind(x[0], y[0], sx, sy, obstacleManager);
        if (nextDir != -1) {
            int safeDir = safeAIDirection(nextDir, obstacleManager);
            if (safeDir == nextDir) {
                dir = safeDir;
                return;
            }
        }
    }
}

    if (rand() % 10 < 4) {  // 40% 概率转向
        int curDir = dir;
        int newDir;

        // 随机选一个垂直方向
        if (curDir == 0 || curDir == 2) {       // 上/下 → 随机选左/右
            int choices[2] = {3, 1};
            newDir = choices[rand() % 2];
        } else {                                  // 左/右 → 随机选上/下
            int choices[2] = {0, 2};
            newDir = choices[rand() % 2];
        }

        newDir = safeAIDirection(newDir, obstacleManager);
        dir = newDir;
    } else {
         // 不转向时也要检查当前方向是否安全（贴墙时自动修正）
        int curDir = dir;
        int safeDir = safeAIDirection(curDir, obstacleManager);
        if (safeDir != curDir) dir = safeDir;
    }
}

// 曼哈顿距离启发函数
int manhattan(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}


bool aiSnake::isBlocked(int x, int y, const ObstacleManager& obstacleManager) {
    // 撞墙
    if (x < 0 || x >= WIDTH - 2 || y < 0 || y >= HEIGHT - 2)
        return true;
    // 撞自己身体
    for (int i = 1; i < length; i++) {
        if (x == this->x[i] && y == this->y[i])
            return true;
    }
    // 撞障碍物
    if (obstacleManager.checkCollision(x, y))
        return true;
    return false;
}

int aiSnake::astarPathFind(int startX, int startY, int goalX, int goalY,
                           const ObstacleManager& obstacleManager) {
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
            if (isBlocked(nx, ny, obstacleManager)) continue;

            int g = cur->g + 1;
            int h = manhattan(nx, ny, goalX, goalY);
            AStarNode* neighbor = new AStarNode(nx, ny, g, h, cur);
            // 入队
            open.push(neighbor);
        }
    }

    return -1; // 找不到路
}