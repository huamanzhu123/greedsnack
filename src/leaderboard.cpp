#include "../include/leaderboard.h"
#include <cstdio>
#include <fstream>
#include <algorithm>
#include <conio.h>
#include "player.h" 

Leaderboard::Leaderboard(const std::string& filename) : filename(filename) {
    load();
}

void Leaderboard::sortPlayers() {
    std::sort(players.begin(), players.end(), std::greater<Player>());
}

void Leaderboard::load() {
    players.clear();
    std::ifstream infile(filename);
    if (!infile.is_open()) return;

    std::string line;
    while (std::getline(infile, line)) {
        Player p = Player::fromString(line);
        if (p.getName().empty()) continue;
        players.push_back(p);
    }
    infile.close();
    sortPlayers();
}

void Leaderboard::save() const {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) return;
    for (const auto& p : players) {
        outfile << p.toString() << "\n";
    }
    outfile.close();
}

void Leaderboard::update(const std::string& name, int score) {
    if (name.empty()) return;

    auto it = std::find_if(players.begin(), players.end(),
        [&name](const Player& p) { return p.getName() == name; });

    if (it != players.end()) {
        // 同名玩家：增加游玩次数，更新最高分
        it->addPlay();
        it->updateScore(score);
    } else {
        players.emplace_back(name, score, 1);
    }

    sortPlayers();
    save();
}

void Leaderboard::displayTop5() const {
    printf("\n===== 排行榜（前五名） =====\n");
    if (players.empty()) {
        printf("暂无记录。\n");
    } else {
        int count = std::min(5, (int)players.size());
        for (int i = 0; i < count; ++i) {
            const Player& p = players[i];
            printf("%d. %-10s | 最高分: %3d | 游玩次数: %d\n",
                   i + 1, p.getName().c_str(), p.getHighestScore(), p.getPlayCount());
        }
    }
    printf("============================\n");
}

int Leaderboard::getRank(const std::string& name) const {
    for (size_t i = 0; i < players.size(); ++i) {
        if (players[i].getName() == name)
            return static_cast<int>(i) + 1;
    }
    return -1;
}

void Leaderboard::displayPlayerInfo(const std::string& name) const {
    if (name.empty()) {
        printf("玩家姓名不能为空。\n");
        return;
    }

    // 查找玩家索引
    int index = -1;
    for (size_t i = 0; i < players.size(); ++i) {
        if (players[i].getName() == name) {
            index = static_cast<int>(i);
            break;
        }
    }

    if (index == -1) {
        printf("未找到玩家 \"%s\"。\n", name.c_str());
        return;
    }

    const Player& p = players[index];
    int rank = index + 1;   // 因为 players 已排序

    printf("\n===== 玩家信息 =====\n");
    printf("姓名：%s\n", p.getName().c_str());
    printf("排名：%d\n", rank);
    printf("最高得分：%d\n", p.getHighestScore());
    printf("游玩次数：%d\n", p.getPlayCount());
    printf("====================\n");
}