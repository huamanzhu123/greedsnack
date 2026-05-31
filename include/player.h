#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
public:
    Player(const std::string& name = "", int highestScore = 0, int playCount = 0);

    // getters (const)
    std::string getName() const;
    int getHighestScore() const;
    int getPlayCount() const;

    // modifiers
    void addPlay();
    void updateScore(int newScore);

    // 排序，最高分降序，同分则游玩次数降序
    bool operator>(const Player& other) const;

    // 文件读写
    std::string toString() const;
    static Player fromString(const std::string& line);

private:
    std::string name;
    int highestScore;
    int playCount;
};

#endif