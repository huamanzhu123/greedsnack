#ifndef FOOD_H
#define FOOD_H


#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "snake_class.h"

#ifndef WIDTH
#define WIDTH 40
#endif

#ifndef HEIGHT
#define HEIGHT 20
#endif

#ifndef MAX_SNAKE
#define MAX_SNAKE ((WIDTH -2) * (HEIGHT -2))
#endif

class Food {
public:
    Food();
    int place_food_safe(Food& f, const Snake& s);
    int get_x() const { return x; }
    int get_y() const { return y; }
    int get_type() const { return type; }
    void set_type(int t) { type = t; }

    // 食物类型常量
    static const int TYPE_NORMAL = 0;    // 普通食物 +1 分
    static const int TYPE_HIGHSCORE = 1; // 高分食物 +3 分

private:
    int x;             // 食物的x坐标
    int y;             // 食物的y坐标
    int type;          // 食物类型
    unsigned long refresh_time;  // 食物刷新时间（毫秒级）
};

#endif