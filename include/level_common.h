#ifndef LEVEL_COMMON_H
#define LEVEL_COMMON_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "../include/level_common.h"
#include <stdlib.h>
#include <string.h>

#ifndef WIDTH
#define WIDTH 40
#endif

#ifndef HEIGHT
#define HEIGHT 20
#endif

	/* 如果外层没有定义 MAX_SNAKE，可在此提供默认值 */
#ifndef MAX_SNAKE
#define MAX_SNAKE ((WIDTH -2) * (HEIGHT -2))
#endif // !MAX_SNAKE

	/*
	此文件定义了 Snake 和 Food 结构体及其相关操作函数，供各关卡使用。
	需完成：
	1. 定义 Snake 结构体，包含位置数组、长度、方向、分数和存活状态等信息。
	2. 定义 Food 结构体，包含位置、类型和刷新时间等信息。
	3. 实现 Snake 的初始化、移动、增长和缩小等基本操作函数。
	4. 实现碰撞检查函数，包括自撞和墙壁碰撞等。
	5. 实现安全放置食物的函数，确保食物不会生成在蛇身上。
*/

typedef struct {
	int x[MAX_SNAKE];  // 蛇身体各部分的x坐标
	int y[MAX_SNAKE];  // 蛇身体各部分的y坐标
	int length;        // 蛇的长度
	int dir;           // 蛇的当前方向（0-3分别表示上、右、下、左）
	int score;         // 当前分数
	int alive;         // 是否存活（1表示存活，0表示死亡）
} Snake;

typedef struct {
	int x;             // 食物的x坐标
	int y;             // 食物的y坐标
	int type;          // 食物类型（0-普通食物，1-特殊食物）
	unsigned long refresh_time;  // 食物刷新时间（毫秒级）
} Food;

void snake_init(Snake* s, int startX, int sstartY);
void snake_move(Snake* s);
void snake_grow(Snake* s);

int check_self_collision(Snake* s);
int check_wall_collision(Snake* s);

int place_food_safe(Food* f, const Snake* snakes[]);


#ifdef __cplusplus
}
#endif

#endif /* LEVEL_COMMON_H */