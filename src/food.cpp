#include <../include/food.h>
#include <../include/snake_class.h>

int Food::place_food_safe(Food& f, const Snake& s) {
    int tries = 0;
    while(tries < 1000) {
        int x = rand() % (WIDTH - 2);   // 可玩区域 x: [0, WIDTH-3]
        int y = rand() % (HEIGHT - 2);  // 可玩区域 y: [0, HEIGHT-3]

        int conflict = 0;
        for(int i = 0; i < s.get_length(); i++) {
            if(s.get_x(i) == x && s.get_y(i) == y){
                conflict = 1; break;
            }
        }

        if(!conflict) {
            f.x = x;
            f.y = y;
            return 1;
        }

        tries++;
    }
    return 0; // 放置失败，返回0
}