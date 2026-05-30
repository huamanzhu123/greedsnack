#ifndef SNAKE_CLASS_H
#define SNAKE_CLASS_H

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

class Snake {
public:
    Snake(int startX, int startY);
    void move(Snake& s);
    void grow(Snake& s);

    int check_self_collision(const Snake& s);
    int check_wall_collision(const Snake& s);

    int get_length() const { return length; }
    int get_dir() const { return dir; }

    void setDir(int d) { dir = d; }

    int is_alive() const { return alive; }
    int get_blood() const { return blood; }
    int get_energy() const { return energy; }
    int get_x(int index) const { return (index >= 0 && index < length) ? x[index] : -1; }
    int get_y(int index) const { return (index >= 0 && index < length) ? y[index] : -1; }
private:
    int x[MAX_SNAKE];  // 蛇身体各部分的x坐标
	int y[MAX_SNAKE];  // 蛇身体各部分的y坐标
    int length;        // 蛇的长度
	int dir;           // 蛇的当前方向（0-3分别表示上、右、下、左）
    int alive;
    int blood;         // 蛇的血量
    int energy;        // 蛇的能量
};

#ifdef __cplusplus
}
#endif

#endif