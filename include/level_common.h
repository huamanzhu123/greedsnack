#ifndef LEVEL_COMMON_H
#define LEVEL_COMMON_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

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


#ifdef __cplusplus
}
#endif

#endif /* LEVEL_COMMON_H */