#include "../include/leaderboard.h"
#include <cstdlib>
#include <cstdio>

// 降序比较函数
int Leaderboard::cmpDesc(const void* a, const void* b) {
    const LBEntry* ea = static_cast<const LBEntry*>(a);
    const LBEntry* eb = static_cast<const LBEntry*>(b);
    return eb->score - ea->score;  // 分数高排在前面
}

// 构造函数，含默认的形参值
Leaderboard::Leaderboard(const char* filename, int maxSize)
    : maxSize(maxSize), count(0) {
    strncpy_s(this->filename, sizeof(this->filename), filename, _TRUNCATE);
    // 初始化数组
    for (int i = 0; i < LEADERBOARD_SIZE; ++i) {
        entries[i].name[0] = '\0';
        entries[i].score = 0;
    }
}

// 加载排行榜
int Leaderboard::load() {
    // 以只读的模式打开排行榜文件
    FILE* fp = fopen(filename, "r");
    if (!fp) return 0; // 如果文件不存在或者无法打开的话，就返回0
    count = 0;
    char line[256];
    // 逐行读取文件内容，直到达到最大条目数或文件结束
    while (count < maxSize && fgets(line, sizeof(line), fp) != nullptr) {
        LBEntry tmp;
        // 解析每行的用户名和分数，格式为"name score"
        if (sscanf_s(line, "%31s %d", tmp.name, (unsigned)sizeof(tmp.name), &tmp.score) == 2) {
            entries[count++] = tmp;
        }
    }
    fclose(fp);

    // 如果读取到多于一个条目，则按分数降序排序，一个的话就没必要做什么排序的操作了
    if (count > 1) {
        qsort(entries, count, sizeof(LBEntry), cmpDesc);
    }
    return count;
}

// 保存排行榜功能到指定文件
void Leaderboard::save() const {
    FILE* fp = fopen(filename, "w");
    if (!fp) return;
    for (int i = 0; i < count; ++i) {
        fprintf(fp, "%s %d\n", entries[i].name, entries[i].score);
    }
    fclose(fp);
}

// 更新排行榜
void Leaderboard::update(const char* name, int score) {
    // 如果传入的名字为空指针或空字符串，不做任何更新
    if (!name || name[0] == '\0') return;

    // 先加载当前排行榜文件内容，确保内存中的 entries 数组与文件同步
    // 这样可以避免在多次调用 update 时丢失之前保存的记录
    load();

    // 在现有排行榜条目中查找是否已有同名玩家记录
    int found = -1;
    for (int i = 0; i < count; ++i) {
        if (strncmp(entries[i].name, name, NAME_LEN) == 0) {
            found = i;
            break;
        }
    }

    if (found != -1) {
        // 找到同名玩家记录：只有当新分数更高时才更新记录，避免覆盖更高分数
        if (score > entries[found].score) {
            entries[found].score = score;
            // 更新分数后重新进行排序，保持排行榜降序排列
            qsort(entries, count, sizeof(LBEntry), cmpDesc);
        }
    }
    else if (count < maxSize) {
        // 当前排行榜条目未达到最大容量，直接追加新的玩家记录
        strncpy_s(entries[count].name, NAME_LEN, name, _TRUNCATE);
        entries[count].score = score;
        count++;
        // 追加后重新排序，确保新条目按分数位置正确
        qsort(entries, count, sizeof(LBEntry), cmpDesc);
    }
    else {
        // 排行榜已满的情况下，只有当新分数超过当前最低分时才替换最低记录
        if (score > entries[count - 1].score) {
            strncpy_s(entries[count - 1].name, NAME_LEN, name, _TRUNCATE);
            entries[count - 1].score = score;
            // 替换最低记录后重新排序，保证排行榜是降序排序的
            qsort(entries, count, sizeof(LBEntry), cmpDesc);
        }
    }
    // 将更新后的排行榜保存回文件，保持下次运行时数据一致
    save();
}



// 提示用户输入姓名并更新排行榜，同时显示最新排行榜
void Leaderboard::promptAndUpdate(int finalScore) {
    // 定义一个字符数组用于保存用户名输入，长度为 NAME_LEN。
    // 用 { 0 } 进行初始化，确保所有字符都被置为 '\0'。
    char namebuf[NAME_LEN] = { 0 };

    // 向用户显示游戏结束信息和最终得分。
    printf("Game over! Your final score: %d\n", finalScore);
    printf("Enter your name (max %d chars): ", NAME_LEN - 1);

    // 从标准输入读取一行用户输入，并存入 namebuf。
    // fgets 会读取最多 sizeof(namebuf) - 1 个字符，
    // 并在末尾自动添加结束符 '\0'。
    if (fgets(namebuf, sizeof(namebuf), stdin) == nullptr) {
        // 如果读取失败，就直接返回，不做后续处理。
        return;
    }

    // 由于fgets 会保留用户输入行末的换行符，因此需要手动去掉。
    size_t len = strlen(namebuf);
    if (len > 0 && (namebuf[len - 1] == '\n' || namebuf[len - 1] == '\r')) {
        // 将最后一个换行符替换为字符串结束符 '\0'。
        namebuf[len - 1] = '\0';
        // 有时由于输入的问题，还需要去掉前面的 '\r'。
        if (len > 1 && namebuf[len - 2] == '\r') {
            namebuf[len - 2] = '\0';
        }
    }
    // 检查用户输入的名字是否有效
    // 如果用户直接按回车，namebuf 仍然是空字符串，namebuf[0] == '\0'。
    if (namebuf[0] == '\0') {
        printf("未输入姓名，成绩未保存。\n");
        return;
    }
    // 使用用户输入的姓名和最终得分来更新排行榜。
    // 会处理排行榜文件的读取、更新、排序和保存。
    update(namebuf, finalScore);

    // 更新完成后，显示当前排行榜内容。
    display();
}

// 显示排行榜的功能
void Leaderboard::display() const {
    printf("\n==== 排行榜（前%d名） ====\n", maxSize);
    for (int i = 0; i < count; ++i) {
        printf("%2d. %-*s %5d\n", i + 1, NAME_LEN - 1, entries[i].name, entries[i].score);
    }
    printf("========================\n");
}
