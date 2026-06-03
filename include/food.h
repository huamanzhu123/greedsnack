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
    void setSpawnTime(unsigned long t) { spawn_time = t; }
    bool isExpired(unsigned long now) const;

    static const int TYPE_NORMAL = 0;
    static const int TYPE_HIGHSCORE = 1;
    static const unsigned long NORMAL_LIFETIME = 10000;  // 普通食物10秒
    static const unsigned long HIGHSCORE_LIFETIME = 5000; // 高分食物5秒

private:
    int x;
    int y;
    int type;
    unsigned long spawn_time;
};

#endif