#include "../include/level_common.h"
#include <stdlib.h>
#include <string.h>

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