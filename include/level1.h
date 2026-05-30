#ifndef LEVEL1_H
#define LEVEL1_H

#include "../include/level_common.h" 

class Level1 {
public:
    Level1();                      // 构造函数
    void run();                    // 游戏主循环

private:
    // 游戏状态变量
    int score;          // 当前得分
    int gameOver;       // 游戏结束标志
    int speedMs;        // 蛇移动速度（毫秒）
    int startX;         // 蛇初始 X 坐标
    int startY;         // 蛇初始 Y 坐标
    Snake snake;        // 蛇对象
    Food food;          // 食物对象
    unsigned long lastMoveTime;  // 上一次移动的时刻（毫秒）
    
    void init();        // 初始化游戏数据
    void processGameLoop(); // 游戏主循环逻辑
};

#endif // LEVEL1_H