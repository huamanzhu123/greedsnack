#ifndef GAME_UI_H
#define GAME_UI_H


/* 新增模块：game_ui - 负责渲染与输入封装，供各关卡复用 */
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "../include/snake_class.h"
#include "../include/food.h"
#include "../include/game_shared.h"

class GameUI {
	private:
		Console* console;  // 控制台对象指针
		int width;         // 游戏区域宽度
		int height;        // 游戏区域高度
	public:
		// 构造函数：初始化UI，指定游戏区域大小
		GameUI(int w, int h);
		// 析构函数
		~GameUI();
		// 绘制游戏界面（包括边框、蛇、食物、分数等）
		void drawBoard(const Snake* s, const Food* f, int score, int speedMs);
		// 处理键盘输入（方向控制和退出）
		void processInput(Snake* s, int* gameOver);
		// 显示游戏结束信息
		void showGameOver(int finalScore);
		// 清空键盘缓冲区
		void clearInputBuffer();
		// 获取控制台对象
		Console* getConsole() { return console; }
};


#endif /* GAME_UI_H */