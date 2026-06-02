#include "../include/food.h"
#include "../include/snake_class.h"
#include <cstdlib>

Food::Food() : x(0), y(0), type(TYPE_NORMAL), refresh_time(0) {}

int Food::place_food_safe(Food& f, const Snake& s) {
    int tries = 0;
    while (tries < 1000) {
        int nx = rand() % (WIDTH - 2);
        int ny = rand() % (HEIGHT - 2);

        int conflict = 0;
        for (int i = 0; i < s.get_length(); i++) {
            if (s.get_x(i) == nx && s.get_y(i) == ny) {
                conflict = 1;
                break;
            }
        }

        if (!conflict) {
            f.x = nx;
            f.y = ny;
            // 随机决定食物类型,有15% 概率为高分食物
            if (rand() % 100 < 15)
                f.type = Food::TYPE_HIGHSCORE;
            else
                f.type = Food::TYPE_NORMAL;
            return 1;
        }
        tries++;
    }
    return 0;
}