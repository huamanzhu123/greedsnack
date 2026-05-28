#ifndef GAME_UI_H
#define GAME_UI_H

#ifdef __cplusplus
extern "C" {
#endif

/* 新增模块：game_ui - 负责渲染与输入封装，供各关卡复用 */
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "../include/game_ui.h"
#include "../include/game_shared.h"

#include "level_common.h"

	/* 绘制整个棋盘（整板重绘，后续可替换为增量更新） */
	void draw_board(const Snake* s, const Food* f, int score, int width, int height, int speed_ms);

	/* 处理输入：修改 s->dir；若检测到退出（Esc）则设置 *game_over = 1 */
	void process_input(Snake* s, int* game_over);

#ifdef __cplusplus
}
#endif

#endif /* GAME_UI_H */