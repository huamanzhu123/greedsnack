#ifndef SHIELD_H
#define SHIELD_H

class Shield {
public:
    Shield();
    
    // 激活护盾
    void activate(unsigned long currentTime);

    void update(unsigned long currentTime);
    bool tryDefend();
    bool isActive() const { return active; }
    
    //剩余时间
    unsigned long getRemainingTime(unsigned long currentTime) const;
    
private:
    bool active;
    unsigned long activateTime;
    static const unsigned long SHIELD_DURATION = 10000;
};

#endif