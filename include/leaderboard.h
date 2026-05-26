#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

    /* 若外部未定义 NAME_LEN/LEADERBOARD_FILE/LEADERBOARD_SIZE，则使用默认值 */
#ifndef NAME_LEN
#define NAME_LEN 32
#endif

#ifndef LEADERBOARD_FILE
#define LEADERBOARD_FILE "leaderboard.txt"
#endif

#ifndef LEADERBOARD_SIZE
#define LEADERBOARD_SIZE 5
#endif

    typedef struct {
        char name[NAME_LEN];
        int score;
    } LBEntry;

    /* 加载最多 max 条记录，返回实际读取数量 */
    int load_leaderboard(LBEntry arr[], int max);
    /* 保存 count 条记录到文件（覆盖） */
    void save_leaderboard(const LBEntry arr[], int count);

    /* 将一条记录合并到排行榜（内部会排序并截断到 LEADERBOARD_SIZE） */
    void update_leaderboard(const char* name, int score);

    /* 在控制台提示玩家输入名字并将分数写入排行榜*/
    void prompt_and_update_leaderboard(int final_score);


#ifdef __cplusplus
}
#endif

#endif /* LEADERBOARD_H */