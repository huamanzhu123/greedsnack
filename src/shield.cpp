#include "../include/shield.h"

Shield::Shield() : active(false), activateTime(0) {}

void Shield::activate(unsigned long currentTime) {
    active = true;
    activateTime = currentTime;
}

void Shield::update(unsigned long currentTime) {
    if (active && (currentTime - activateTime >= SHIELD_DURATION)) {
        active = false;
    }
}

bool Shield::tryDefend() {
    if (active) {
        active = false;  // 使用后护盾消失
        return true;
    }
    return false;
}

unsigned long Shield::getRemainingTime(unsigned long currentTime) const {
    if (!active) return 0;
    unsigned long elapsed = currentTime - activateTime;
    if (elapsed >= SHIELD_DURATION) return 0;
    return SHIELD_DURATION - elapsed;
}
