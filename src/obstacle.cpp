#include "../include/obstacle.h"
#include "../include/game_shared.h"
#include <cstdlib>

Obstacle::Obstacle() : x(0), y(0), type(OBS_FIXED), dx(0), dy(0), active(false), lastMoveTime(0) {}

Obstacle::Obstacle(int x, int y, ObstacleType type)
    : x(x), y(y), type(type), active(true), lastMoveTime(0) {
    if (type == OBS_MOVING) {
        int dir = rand() % 4;
        switch (dir) {
            case 0: dx = 0; dy = -1; break;
            case 1: dx = 1; dy = 0; break;
            case 2: dx = 0; dy = 1; break;
            case 3: dx = -1; dy = 0; break;
        }
    } else {
        dx = dy = 0;
    }
}

bool Obstacle::placeObstacle(const Snake& s1, const Snake& s2, const Food& f) {
    int tries = 0;
    while (tries < 500) {
        int nx = rand() % (WIDTH - 2);
        int ny = rand() % (HEIGHT - 2);

        bool conflict = false;
        for (int i = 0; i < s1.get_length(); i++) {
            if (s1.get_x(i) == nx && s1.get_y(i) == ny) {
                conflict = true;
                break;
            }
        }
        if (!conflict) {
            for (int i = 0; i < s2.get_length(); i++) {
                if (s2.get_x(i) == nx && s2.get_y(i) == ny) {
                    conflict = true;
                    break;
                }
            }
        }
        if (!conflict && f.get_x() == nx && f.get_y() == ny) {
            conflict = true;
        }

        if (!conflict) {
            x = nx;
            y = ny;
            active = true;
            return true;
        }
        tries++;
    }
    return false;
}

void Obstacle::move() {
    if (!active || type != OBS_MOVING) return;

    int newX = x + dx;
    int newY = y + dy;

    if (newX < 0 || newX >= WIDTH - 2) {
        dx = -dx;
        newX = x + dx;
    }
    if (newY < 0 || newY >= HEIGHT - 2) {
        dy = -dy;
        newY = y + dy;
    }

    x = newX;
    y = newY;
}

bool Obstacle::checkCollision(int px, int py) const {
    return active && (x == px && y == py);
}

ObstacleManager::ObstacleManager() : obstacleCount(0) {}

void ObstacleManager::initialize(const Snake& s1, const Snake& s2, const Food& f, int fixedCount, int movingCount) {
    obstacleCount = 0;

    for (int i = 0; i < fixedCount && obstacleCount < MAX_OBSTACLES; i++) {
        Obstacle obs(0, 0, OBS_FIXED);
        if (obs.placeObstacle(s1, s2, f)) {
            obstacles[obstacleCount++] = obs;
        }
    }

    for (int i = 0; i < movingCount && obstacleCount < MAX_OBSTACLES; i++) {
        Obstacle obs(0, 0, OBS_MOVING);
        if (obs.placeObstacle(s1, s2, f)) {
            obstacles[obstacleCount++] = obs;
        }
    }
}

void ObstacleManager::update(unsigned long /*currentTime*/) {
    for (int i = 0; i < obstacleCount; i++) {
        if (obstacles[i].getType() == OBS_MOVING) {
            obstacles[i].move();
        }
    }
}

bool ObstacleManager::checkCollision(int x, int y) const {
    for (int i = 0; i < obstacleCount; i++) {
        if (obstacles[i].checkCollision(x, y)) {
            return true;
        }
    }
    return false;
}