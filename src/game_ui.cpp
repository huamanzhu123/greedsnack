/* 新增模块：game_ui - 负责渲染与输入封装，供各关卡复用 */
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "../include/game_ui.h"


/*
* 负责渲染游戏界面，以及键盘输入的处理，需完成：
* 1. 绘制游戏界面，包括边框、分数、蛇、食物等视觉元素
* 2. 处理输入：修改 s->dir；若检测到退出（Esc）则设置 *game_over = 1
*/