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

    for (int i = 0; i < length; i++) {
        if (playerId == 1) {
            x[i] = startX - i;  // 玩家1从左到右
        } else {
            x[i] = startX + i;  // 玩家2从右到左
        }
        y[i] = startY;
    }
}

void Snake::move(Snake& s) {
    if(!s.alive) return;
    
    for (int i = s.length - 1; i > 0; i--) {
        s.x[i] = s.x[i - 1];
        s.y[i] = s.y[i - 1];
    }

    switch (s.dir) {
        case 0: s.y[0]--; break;
        case 1: s.x[0]++; break;
        case 2: s.y[0]++; break;
        case 3: s.x[0]--; break;
    }
}

void Snake::grow(Snake& s) {
    if(s.length >= MAX_SNAKE) return;

    s.x[s.length] = s.x[s.length - 1];
    s.y[s.length] = s.y[s.length - 1];
    s.length++;
}

int Snake::check_self_collision(const Snake& s) {
    for (int i = 1; i < s.length; i++) {
        if (s.x[0] == s.x[i] && s.y[0] == s.y[i]) {
            return 1;
        }
    }
    return 0;
}

int Snake::check_wall_collision(const Snake& s) {
    if (s.x[0] < 0 || s.x[0] >= WIDTH - 2 || s.y[0] < 0 || s.y[0] >= HEIGHT - 2) {
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