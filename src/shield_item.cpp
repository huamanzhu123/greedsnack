#include "../include/shield_item.h"
#include "../include/snake_class.h"
#include "../include/obstacle.h"
#include "../include/food.h"
#include <cstdlib>

ShieldItem::ShieldItem() : x(0), y(0), active(false) {}

void ShieldItem::place(int w, int h, const Snake& s, const ObstacleManager& obs, const Food& f) {
    int tries = 0;
    while (tries < 500) {
        int nx = rand() % (w - 2);
        int ny = rand() % (h - 2);
        bool conflict = false;

        // 避开蛇
        for (int i = 0; i < s.get_length(); ++i) {
            if (s.get_x(i) == nx && s.get_y(i) == ny) {
                conflict = true;
                break;
            }
        }
        // 避开障碍物
        if (!conflict && obs.checkCollision(nx, ny))
            conflict = true;
        // 避开食物
        if (!conflict && f.get_x() == nx && f.get_y() == ny)
            conflict = true;

        if (!conflict) {
            x = nx;
            y = ny;
            active = true;
            return;
        }
        ++tries;
    }
    active = false;  // 无法放置
}