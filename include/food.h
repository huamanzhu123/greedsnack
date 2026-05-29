#ifndef FOOD_H
#define FOOD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
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

class Food {
public:
    Food();
    int place_food_safe(Food& f, const Snake& s);
private:
	int x;             // 食物的x坐标
	int y;             // 食物的y坐标
	int type;          // 食物类型（0-普通食物，1-特殊食物）
    unsigned long refresh_time;  // 食物刷新时间（毫秒级）
};

#ifdef __cplusplus
}
#endif

#endif