#include "../include/aiSnake_class.h"

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

void aiSnake::updateAIDirection(const ObstacleManager& obstacleManager) {
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