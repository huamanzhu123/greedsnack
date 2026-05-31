#ifndef SHIELD_ITEM_H
#define SHIELD_ITEM_H

class Snake;
class ObstacleManager;
class Food;

class ShieldItem {
public:
    ShieldItem();
    
    //放置护盾道具
    void place(int w, int h, const Snake& s, const ObstacleManager& obs, const Food& f);
    
    int getX() const { return x; }
    int getY() const { return y; }
    bool isActive() const { return active; }
    void setActive(bool a) { active = a; }

private:
    int x, y;
    bool active;
};

#endif