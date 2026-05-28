#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "../include/leaderboard.h"

#define MAX_LINE 256  // 排行榜文件每行的最大长度

// 降序排序比较函数
static int cmp_desc(const void* a, const void* b) {
    const LBEntry* ea = (const LBEntry*)a;
    const LBEntry* eb = (const LBEntry*)b;
    return eb->score - ea->score;  // 降序
}

int load_leaderboard(LBEntry arr[], int max) {
    if (!arr || max <= 0) return 0;
    FILE* fp = fopen(LEADERBOARD_FILE, "r");
    if (!fp) return 0;

    int count = 0;
    char line[MAX_LINE];
    while (count < max && fgets(line, sizeof(line), fp) != NULL) {
        LBEntry tmp;
        if (sscanf_s(line, "%31s %d", tmp.name, (unsigned)sizeof(tmp.name), &tmp.score) == 2) {
            arr[count++] = tmp;
        }
    }
    fclose(fp);

    if (count > 1) {
        qsort(arr, count, sizeof(LBEntry), cmp_desc);
    }
    return count;
}

void save_leaderboard(const LBEntry arr[], int count) {
    if (!arr || count <= 0) return;
    FILE* fp = fopen(LEADERBOARD_FILE, "w");
    if (!fp) return;
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %d\n", arr[i].name, arr[i].score);
    }
    fclose(fp);  // 循环外关闭文件
}

void update_leaderboard(const char* name, int score) {
    if (!name || name[0] == '\0') return;
    LBEntry arr[LEADERBOARD_SIZE];

    int count = load_leaderboard(arr, LEADERBOARD_SIZE);

    // 查找是否有同名玩家
    int found = -1;
    for (int i = 0; i < count; i++) {
        if (strncmp(arr[i].name, name, NAME_LEN) == 0) {  // strncmp 返回 0 表示相等
            found = i;
            break;
        }
    }

    if (found != -1) {
        // 同名玩家：仅在新分数更高时更新
        if (score > arr[found].score) {
            arr[found].score = score;
            qsort(arr, count, sizeof(LBEntry), cmp_desc);
        }
    } else if (count < LEADERBOARD_SIZE) {
        // 排行榜未满，直接追加
        memset(&arr[count], 0, sizeof(LBEntry));
        strncpy_s(arr[count].name, NAME_LEN, name, NAME_LEN - 1);
        arr[count].score = score;
        count++;
        qsort(arr, count, sizeof(LBEntry), cmp_desc);
    } else {
        // 排行榜已满，若新分数高于最低分则替换
        if (score > arr[count - 1].score) {
            memset(&arr[count - 1], 0, sizeof(LBEntry));
            strncpy_s(arr[count - 1].name, NAME_LEN, name, NAME_LEN - 1);
            arr[count - 1].score = score;
            qsort(arr, count, sizeof(LBEntry), cmp_desc);
        }
    }

    save_leaderboard(arr, count);
}

void prompt_and_update_leaderboard(int final_score) {
    char namebuf[NAME_LEN] = { 0 };
    printf("Game over! Your final score: %d\n", final_score);
    printf("Enter your name (max %d chars): ", NAME_LEN - 1);

    if (fgets(namebuf, sizeof(namebuf), stdin) == NULL) return;

    // 去除末尾换行符
    size_t len = strlen(namebuf);
    if (len > 0 && (namebuf[len - 1] == '\n' || namebuf[len - 1] == '\r')) {
        namebuf[len - 1] = '\0';
        if (len > 1 && namebuf[len - 2] == '\r') {
            namebuf[len - 2] = '\0';
        }
    }

    if (namebuf[0] == '\0') {
        printf("未输入姓名，成绩未保存。\n");
        return;
    }

    update_leaderboard(namebuf, final_score);

    LBEntry arr[LEADERBOARD_SIZE];
    int count = load_leaderboard(arr, LEADERBOARD_SIZE);
    printf("\n==== 排行榜（前%d名） ====\n", LEADERBOARD_SIZE);
    for (int i = 0; i < count; i++) {
        printf("%2d. %-*s %5d\n", i + 1, NAME_LEN - 1, arr[i].name, arr[i].score);
    }
    printf("========================\n");
}
