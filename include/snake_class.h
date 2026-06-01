#ifndef SNAKE_CLASS_H
#define SNAKE_CLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "shield.h"

#ifndef WIDTH
#define WIDTH 40
#endif

#ifndef HEIGHT
#define HEIGHT 20
#endif

#ifndef MAX_SNAKE
#define MAX_SNAKE ((WIDTH -2) * (HEIGHT -2))
#endif

const int baseSpeedMs = 200;

class Snake {
public:
    Snake(int startX = 0, int startY = 0, int playerId = 1);
    void move(Snake& s);
    void grow(Snake& s);

    int check_self_collision(const Snake& s);
    int check_wall_collision(const Snake& s);
    
    // 检查是否与另一条蛇碰撞（头碰身体或头碰头）
    int check_snake_collision(const Snake& other, bool& headToHead);

    int get_length() const { return length; }
    int get_dir() const { return dir; }
    int getPlayerId() const { return playerId; }

    void setDir(int d) { dir = d; }

    int is_alive() const { return alive; }
    void setAlive(int a) { alive = a; }
    
    int get_blood() const { return blood; }
    int get_energy() const { return energy; }
    int get_x(int index) const { return (index >= 0 && index < length) ? x[index] : -1; }
    int get_y(int index) const { return (index >= 0 && index < length) ? y[index] : -1; }
    int get_speed() const { return speedMS; }
    
    void set_speed(int ms) { speedMS = ms; }
    
    // 护盾相关
    Shield& getShield() { return shield; }
    const Shield& getShield() const { return shield; }
    
private:
    int x[MAX_SNAKE];
    int y[MAX_SNAKE];
    int length;
    int dir;
    int alive;
    int blood;
    int energy;
    int playerId;
    int speedMS;
    Shield shield;
};

#ifdef __cplusplus
}
#endif

#endif