#ifndef AISNAKE_CLASS_H
#define AISNAKE_CLASS_H


#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "shield.h"
#include "snake_class.h"
#include "obstacle.h"

#ifndef WIDTH
#define WIDTH 40
#endif

#ifndef HEIGHT
#define HEIGHT 20
#endif

#ifndef MAX_SNAKE
#define MAX_SNAKE ((WIDTH -2) * (HEIGHT -2))
#endif

const int aibaseSpeedMs = 150;

class aiSnake: public Snake {
public:
    aiSnake(int startX, int startY);
    void updateAIDirection(const ObstacleManager& obstacleManager);
    int safeAIDirection(int cur_dir, const ObstacleManager& obstacleManager);
protected:

private:

};



#endif