#ifndef GAME_SHARED_H
#define GAME_SHARED_H

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

	/* 如果外层没有定义这些常量，可以在各模块中定义或使用这里的默认值 */
#ifndef WIDTH
#define WIDTH 40
#endif

#ifndef HEIGHT
#define HEIGHT 20
#endif

class Console {
		private:
			HANDLE hConsole;  // 控制台句柄

	public:
		Console();
		// 设置光标位置
		void setCursorPos(int x, int y);
		// 隐藏光标
		void hideCursor();
		// 显示光标
		void showCursor();
		// 清屏
		void clear();
		// 获取毫秒级时间戳
		unsigned long getTickMs();
		//控制台编码为UTF-8
		void setUTF8();
};

#ifdef __cplusplus
}
#endif

#endif /* GAME_SHARED_H */
