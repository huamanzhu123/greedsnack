#include "../include/level1.h"
#include "../include/game_shared.h"
#include "../include/game_ui.h"
#include "../include/snake_class.h"
#include "../include/food.h"
#include "../include/leaderboard.h" 
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <windows.h>

// 构造函数
Level1::Level1()
    : score(0)
    , gameOver(0)
    , speedMs(200)
    ,startX(WIDTH / 2) // 蛇的初始位置会在 run() 中重新设置
    ,startY(HEIGHT / 2)
    ,snake(HEIGHT / 2, WIDTH / 2) // 蛇的初始位置会在 run() 中重新设置 
    , lastMoveTime(0) {
    // 初始化蛇和食物的工作放在 run() 中
}

// 运行关卡1
void Level1::run() {
    GameUI ui(WIDTH, HEIGHT);
    // 初始化游戏状态
    Console console;
    console.hideCursor();
    // 初始化蛇的位置
    Snake snake(startX, startY);
    // 初始化食物
    Food food;
    food.place_food_safe(food,snake);
    // 清屏
    console.clear();

    // 记录当前开始的时间，作为蛇移动的基准时间点。
    // 方便后面进行时间比较来控制移动频率，以及添加其他拓展或创新功能
    lastMoveTime = console.getTickMs();


    //循环的逻辑实现
    // 当gameOver等于0时才会继续执行。
    // 每一轮的循环内部都在执行下面的东西
    // 读取键盘输入，根据它来更新蛇的方向，或者设置游戏结束标志
    // 判断蛇是否已经到了下一次移动的时间
    // 如果到了，就移动蛇并检测碰撞、吃食物
    // 重绘当前画面，展示在控制台上，方便用户根据这个画面来决定下一步操作
    // 等待一小段时间，避免 CPU 占用过高。
    while (!gameOver) {
        // 处理玩家输入，读取方向键或退出命令，并把结果写到snake和gameOver里面
        // 比如当玩家按下ESC或关闭键的时候，调用process_input函数会把gameOver设为 1。
        ui.processInput(&snake,&gameOver) ;
        //把蛇的地址传进来，通过函数直接修改蛇的方向、状态
        //函数可以检测ESC键或者关闭窗口的事件，把gameOver设置为1，来结束游戏循环。

        if (gameOver) break;

        // 通过时间戳控制蛇的移动速度。
        // lastMoveTime存储上次实际移动的时间点，now是现在的时间。
        // 只有当距离上次移动经过了至少speedMs毫秒，蛇才会再移动一步。
        unsigned long now = console.getTickMs();
        if (now - lastMoveTime >= static_cast<unsigned long>(speedMs)) {
            // 记录本次移动的时间，以便下次比较。
            lastMoveTime = now;

            // 根据当前方向让蛇移动一步。
            snake.move(snake);

            // 如果蛇头碰到了自己的身体，或者碰到了边界墙壁，游戏结束。
            // check_self_collisio和check_wall_collision会返回非零表示发生碰撞。
            if (snake.check_self_collision(snake) || snake.check_wall_collision(snake)) {
                gameOver = 1;
                break;
            }
            // 如果蛇头的位置与食物重合，就表示吃到了食物。
            //执行相应的加分（后面拓展功能可能也要判断要不要减分）
            
            if (snake.get_x(0) == food.get_x() && snake.get_y(0) == food.get_y()) {
                // 增长蛇身长度，增加分数，并生成下一个食物。
                snake.grow(snake);
                score++;
                food.place_food_safe(food, snake);
            }
        }

        // 这里无论蛇是否移动，都要刷新画面。
        // 这个函数要根据当前蛇身、食物位置、分数和速度绘制游戏地图。
        GameUI ui(WIDTH, HEIGHT);
        ui.drawBoard(&snake, &food, score, speedMs);

        // Sleep只是为了让循环不要无限制地占用CPU
        // 蛇的移动节奏由上面的时间判断决定，Sleep只是控制屏幕刷新频率，与速度无关
        Sleep(10);
    }

    // 游戏结束后，将光标移到画面下方
    console.setCursorPos(0, HEIGHT + 2);
    while (_kbhit()) _getch();   //清空键盘缓冲区

    // 使用Leaderboard类处理得分记录
    Leaderboard lb;
    lb.promptAndUpdate(score);

    printf("\nPress any key to return to menu...");
    while (!_kbhit()) {}
    while (_kbhit()) _getch();
}