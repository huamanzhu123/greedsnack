#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#define LEADERBOARD_FILE "leaderboard.txt"
#define LEADERBOARD_SIZE 10
#define NAME_LEN 32

#include "player.h"

class Leaderboard {
public:
    Leaderboard(const std::string& filename = LEADERBOARD_FILE);
    ~Leaderboard() = default;

    void load();
    void save() const;
    void update(const std::string& name, int score);
    void displayTop5() const;
    void displayPlayerInfo(const std::string& name) const;   // 查看单个玩家信息

private:
    std::vector<Player> players;
    std::string filename;

    void sortPlayers();
    int getRank(const std::string& name) const;  // 返回排名（1-based）
};

#endif