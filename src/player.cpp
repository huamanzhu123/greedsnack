#include "../include/player.h"
#include <sstream>
#include <algorithm>

// Player类实现
// Playe对象保存玩家名称、最高分以及游玩次数

// 构造函数
// name，玩家显示名称。默认玩家可为空字符串
// highestScore:，玩家达到的最高分
// playCount: 玩家游玩的次数
Player::Player(const std::string& name, int highestScore, int playCount)
    : name(name), highestScore(highestScore), playCount(playCount) {}

// 返回存储的玩家名称。标记为const，这样就修改了上次的错误，不会修改对象状态。
std::string Player::getName() const { return name; }

// 返回该玩家记录的最高分。
int Player::getHighestScore() const { return highestScore; }
// 返回该玩家的游玩次数。
int Player::getPlayCount() const { return playCount; }
// 将游玩次数加一。每当玩家结束一次游戏就调用
void Player::addPlay() {
    // 将游玩计数增加一
    playCount++;
}

// 仅当 newScore 大于当前最高分时才更新 stored highestScore。
// newScore，用来与当前最高分比较，更新最高分
void Player::updateScore(int newScore) {
    if (newScore > highestScore)
        highestScore = newScore;
}

// 为Player对象提供排序。主要按最高分排序
// 但是如果最高分相同，就按游玩次数作为排序的标准
// 标记为 const，不修改任一操作数。
bool Player::operator>(const Player& other) const {
    if (highestScore != other.highestScore)
        return highestScore > other.highestScore;
    return playCount > other.playCount;
}

std::string Player::toString() const {
    return name + " " + std::to_string(highestScore) + " " + std::to_string(playCount);
}

Player Player::fromString(const std::string& line) {
    // 将输入行解析为 Player 对象
    //有两种可用的格式
    //  name score count
    //  name score 默认游玩次数是1
    std::istringstream iss(line);
    std::string name;
    int score, count;

    // 尝试解析完整格式 name score count
    if (iss >> name >> score >> count) {
        return Player(name, score, count);
    }

    // 如果没有解析出第三个字段，就用旧的格式 name score
    iss.clear();
    iss.str(line);
    if (iss >> name >> score) {
        // 旧格式没有游玩次数，使用默认值 1
        return Player(name, score, 1);
    }

    // 如果解析失败，那么返回默认构造的 Player（空名、0 分、0 次）
    return Player(); 
}