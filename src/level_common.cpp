/* 说明
*  本文件实现蛇的初始化、移动、增长、缩短、碰撞检测等通用功能，以及食物的随机放置功能。
*  1.snake_init: 初始化蛇的位置、长度、方向等属性；
*  2.snake_move: 根据当前方向移动蛇的位置；
*  3.snake_grow: 增加蛇的长度；
*  4.check_self_collision: 检测蛇是否碰到自己；
*  5.check_wall_collision: 检测蛇是否碰到墙壁；
*  6.place_food_safe: 在不与任意蛇身体重叠的位置放置食物。
*
*/
#include "../include/level_common.h"

void snake_init(Snake* s, int startX, int startY) {
    s->length = 3;  // 初始长度为3
    s->dir = 1;     // 初始方向向右
    s->score = 0;   // 初始分数为0
    s->alive = 1;   // 蛇存活

    // 初始化蛇身体位置，初始位置为水平向右
    for (int i = 0; i < s->length; i++) {
        s->x[i] = startX - i;  // 从起始点向左放置身体
        s->y[i] = startY;
    }
}

void snake_move(Snake* s) {
    if(!s || !s->alive) return;  // 如果蛇不存在或已死亡，直接返回
    
    // 移动身体：从尾部开始，每个部分移动到前一个部分的位置
    for (int i = s->length - 1; i > 0; i--) {
        s->x[i] = s->x[i - 1];
        s->y[i] = s->y[i - 1];
    }

    // 根据当前方向更新蛇头位置
    switch (s->dir) {
        case 0: s->y[0]--; break; // 向上
        case 1: s->x[0]++; break; // 向右
        case 2: s->y[0]++; break; // 向下
        case 3: s->x[0]--; break; // 向左
    }
}

void snake_grow(Snake* s) {
    if(!s || s->length >= MAX_SNAKE) return;  // 如果蛇不存在或已达最大长度，直接返回

    s->x[s->length] = s->x[s->length - 1];  // 新增部分初始位置与当前尾部相同
    s->y[s->length] = s->y[s->length - 1];
    s->length++;  // 增加长度
}

int check_self_collision(Snake* s) {
    if(!s) return;  // 如果蛇不存在或已死亡，直接返回

    // 检查蛇头是否与身体其他部分重叠
    for (int i = 1; i < s->length; i++) {
        if (s->x[0] == s->x[i] && s->y[0] == s->y[i]) {
            return 1; // 碰撞发生，返回1
        }
    }
    return 0; // 没有碰撞，返回0
}

int check_wall_collision(Snake* s) {
    if(!s) return;  // 如果蛇不存在或已死亡，直接返回

    // 检查蛇头是否碰到墙壁
    if (s->x[0] < 0 || s->x[0] >= WIDTH || s->y[0] < 0 || s->y[0] >= HEIGHT) {
        return 1; // 碰撞发生，返回1
    }
    return 0; // 没有碰撞，返回0
}

int place_food_safe(Food* f, const Snake* s) {
    if(!f || !s) return 0;

    int tries = 0;
    while(tries < 1000) {
        int x = rand() % (WIDTH - 2) - 1;
        int y = rand() % (HEIGHT - 2) - 1;

        int conflict = 0;
        for(int i = 0; i < s->length; i++) {
            if(s->x[i] == x && s->y[i] == y){
                conflict = 1; break;
            }
        }

        if(!conflict) {
            f->x = x;
            f->y = y;
            return 1;
        }

        tries++;
    }
    return 0; // 放置失败，返回0
}