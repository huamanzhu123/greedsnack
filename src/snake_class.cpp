#include <../include/snake_class.h>

Snake::Snake(int startX, int startY) {
    x[0] = startX;
    y[0] = startY;
    length = 3;  // 初始长度为3
    dir = 1;     // 初始方向向右
    alive = 1;   // 蛇存活
    blood = 3; // 初始血量为100
    energy = 0; // 初始能量为0

    // 初始化蛇身体位置，初始位置为水平向右
    for (int i = 0; i < length; i++) {
        x[i] = startX - i;  // 从起始点向左放置身体
        y[i] = startY;
    }
}

void Snake::move(Snake& s) {
    if(!s.alive) return;  // 如果蛇不存在或已死亡，直接返回
    
    // 移动身体：从尾部开始，每个部分移动到前一个部分的位置
    for (int i = s.length - 1; i > 0; i--) {
        s.x[i] = s.x[i - 1];
        s.y[i] = s.y[i - 1];
    }

    // 根据当前方向更新蛇头位置
    switch (s.dir) {
        case 0: s.y[0]--; break; // 向上
        case 1: s.x[0]++; break; // 向右
        case 2: s.y[0]++; break; // 向下
        case 3: s. x[0]--; break; // 向左
    }
}

void Snake::grow(Snake& s) {
    if(s.length >= MAX_SNAKE) return;  // 如果蛇不存在或已达最大长度，直接返回

    s.x[s.length] = s.x[s.length - 1];  // 新增部分初始位置与当前尾部相同
    s.y[s.length] = s.y[s.length - 1];
    s.length++;  // 增加长度
}

int Snake::check_self_collision(const Snake& s) {
    // 检查蛇头是否与身体其他部分重叠
    for (int i = 1; i < s.length; i++) {
        if (s.x[0] == s.x[i] && s.y[0] == s.y[i]) {
            return 1; // 碰撞发生，返回1
        }
    }
    return 0; // 没有碰撞，返回0
}

int Snake::check_wall_collision(const Snake& s) {
    // 检查蛇头是否碰到墙壁（游戏区域为 [0, WIDTH-3] × [0, HEIGHT-3]）
    if (s.x[0] < 0 || s.x[0] >= WIDTH - 2 || s.y[0] < 0 || s.y[0] >= HEIGHT - 2) {
        return 1; // 碰撞发生，返回1
    }
    return 0; // 没有碰撞，返回0
}