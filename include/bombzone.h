#ifndef BOMBZONE_H
#define BOMBZONE_H

#include<windows.h>

class BombZone {
public:
    BombZone();
    //生成爆炸区域（3*3），倒计时5s之后会爆炸，如果身体或者头部还在该区域就会扣血量
    void spawn(int x,int y,unsigned long now);
    void deactivate();
    
    bool isActive() const {
        return active; 
    }
    int getX() const {
        return x; 
    }
    int getY() const {
        return y; 
    }
    static int getSize() {
        return 3; 
    }
    int getRemainingTime(unsigned long now) const;
    bool isInside(int snakeX, int snakeY) const;
private:
    int x, y;
    bool active;
    unsigned long spawnTime;
    static const unsigned long countdown = 5000; // 5秒倒计时
};

#endif