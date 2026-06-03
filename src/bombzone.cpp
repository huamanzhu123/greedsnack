#include "../include/bombzone.h"
BombZone::BombZone() : active(false), x(0), y(0), spawnTime(0) {}
void BombZone::spawn(int x, int y, unsigned long now) {
    this->x = x;
    this->y = y;
    this->active = true;
    this->spawnTime = now;
}
void BombZone::deactivate() {
    this->active = false;
}
int BombZone::getRemainingTime(unsigned long now) const {
    if (!active) return 0;
    unsigned long elapsed = now - spawnTime;
    if (elapsed >= countdown) {
        return 0;
    }
    return static_cast<int>((countdown - elapsed) / 1000); // 返回剩余秒数
}
bool BombZone::isInside(int snakeX, int snakeY) const {
    if (!active) return false;
    // 爆炸区域为3x3的网格
    return (snakeX >= x - 1 && snakeX <= x + 1 && snakeY >= y - 1 && snakeY <= y + 1);
}