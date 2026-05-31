#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdio>
// 排行榜配置的常量
#define LEADERBOARD_FILE "leaderboard.txt"
#define LEADERBOARD_SIZE 10
#define NAME_LEN 32

// 排行榜条目结构
struct LBEntry {
    char name[NAME_LEN]; //玩家姓名
    int score; //玩家的分数
};

class Leaderboard {
public:
    // 构造函数，指定文件名和最大条目数
    Leaderboard(const char* filename = LEADERBOARD_FILE, int maxSize = LEADERBOARD_SIZE);
    ~Leaderboard() = default;

    // 从文件加载排行榜到内部数组，返回实际条目数
    int load();

    // 将内部数组保存到文件
    void save() const;

    // 更新排行榜函数，如果新分数更高或出现新玩家，更新并保持降序排序
    void update(const char* name, int score);

    // 提示用户输入姓名，更新排行榜，显示当前排行榜
    void promptAndUpdate(int finalScore);

    // 在控制台显示排行榜
    void display() const;

    // 获取内部数组和条目数，供外部显示使用
    const LBEntry* getEntries() const { return entries; }
    int getCount() const { return count; }

private:
    // 降序排序的比较函数
    static int cmpDesc(const void* a, const void* b);

    char filename[256];            // 排行榜文件路径
    int maxSize;                   // 最大条目数
    LBEntry entries[LEADERBOARD_SIZE]; // 固定大小数组存储条目
    int count;                     // 当前实际条目数
};

#endif // LEADERBOARD_H
//