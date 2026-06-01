#include "../include/snake_class.h"

Snake::Snake(int startX, int startY, int playerId) : playerId(playerId) {
    x[0] = startX;
    y[0] = startY;
    length = 3;
    dir = (playerId == 1) ? 1 : 3;  // 玩家1向右，玩家2向左
    alive = 1;
    blood = 3;
    energy = 0;
    speedMS = baseSpeedMs;
    damageFlashTime = 0;

    for (int i = 0; i < length; i++) {
        if (playerId == 1) {
            x[i] = startX - i;  // 玩家1从左到右
        } else {
            x[i] = startX + i;  // 玩家2从右到左
        }
        y[i] = startY;
    }
}

void Snake::move() {
    if(!alive) return;
    
    for (int i = length - 1; i > 0; i--) {
        x[i] = x[i - 1];
        y[i] = y[i - 1];
    }

    switch (dir) {
        case 0: y[0]--; break;  // 上
        case 1: x[0]++; break;  // 右
        case 2: y[0]++; break;  // 下
        case 3: x[0]--; break;  // 左
    }
}

// 根据输入方向，随机返回一个垂直的方向
int Snake::randomPerpendicularDirection(int dir) {
    if (dir == 0 || dir == 2) {          // 上(0) 或 下(2) → 垂直方向
        int choices[2] = {3, 1};         // 候选：左(3), 右(1)
        return choices[rand() % 2];      // 随机选一个
    } 
    else if (dir == 3 || dir == 1) {     // 左(3) 或 右(1) → 水平方向
        int choices[2] = {0, 2};         // 候选：上(0), 下(2)
        return choices[rand() % 2];
    } 
    else {
        return -1;                       // 无效方向
    }
}

void Snake::move_with_collision() {
    if(!alive) return;

    dir = randomPerpendicularDirection(dir);
    switch (dir) {
        case 0:
            x[0] = x[1], y[0] = y[1] - 1;
            break;
        case 1:
            x[0] = x[1] + 1, y[0] = y[1];
            break;
        case 2:
            x[0] = x[1], y[0] = y[1] + 1;
            break;
        case 3:
            x[0] = x[1] - 1, y[0] = y[1];
            break;
    }
}

void Snake::grow() {
    if(length >= MAX_SNAKE) return;

    x[length] = x[length - 1];
    y[length] = y[length - 1];
    length++;
}

int Snake::check_self_collision() {
    for (int i = 1; i < length; i++) {
        if (x[0] == x[i] && y[0] == y[i]) {
            return 1;
        }
    }
    return 0;
}

int Snake::check_wall_collision() {
    if (x[0] < 0 || x[0] >= WIDTH - 2 || y[0] < 0 || y[0] >= HEIGHT - 2) {
        return 1;
    }
    return 0;
}

int Snake::check_snake_collision(const Snake& other, bool& headToHead) {
    headToHead = false;
    
    // 检查头碰头
    if (x[0] == other.x[0] && y[0] == other.y[0]) {
        headToHead = true;
        return 1;
    }
    
    // 检查头碰对方身体
    for (int i = 1; i < other.length; i++) {
        if (x[0] == other.x[i] && y[0] == other.y[i]) {
            return 1;
        }
    }
    
    return 0;
}