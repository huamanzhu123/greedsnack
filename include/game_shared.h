#ifndef GAME_SHARED_H
#define GAME_SHARED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include <stdint.h>

	/* 如果外层没有定义这些常量，可以在各模块中定义或使用这里的默认值 */
#ifndef WIDTH
#define WIDTH 40
#endif

#ifndef HEIGHT
#define HEIGHT 20
#endif

/* 控制台相关工具 */
	void set_cursor_pos(int x, int y);
	void hide_cursor(void);
	void clear_screen(void);

	/* 毫秒级时间函数（封装 GetTickCount） */
	unsigned long get_tick_ms(void);

#ifdef __cplusplus
}
#endif

#endif /* GAME_SHARED_H */