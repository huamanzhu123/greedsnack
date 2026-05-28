#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>


#include "../include/level1.h"



/* 单人普通模式（关卡1）实现，需要完成：
*   1.声明变量score, gameOver, speedMs, startX, startY
*   2.隐藏光标
*   3.初始化蛇与首个食物
*   4.清屏
*   5.游戏主循环
*    5.1 渲染画面（调用 draw_board）
*    5.2 输入与定时（调用 process_input）
*       5.3 移动蛇（调用 snake_move）
*       5.4 碰撞检测（调用 check_self_collision 与 check_wall_collision）
*       5.5 吃食物（调用 snake_grow 与 place_food_safe）
*   6.游戏结束后将光标移到画面下面再读取名字
*   7.调用 leaderboard 模块的提示与更新函数
*/

void level1_run(void) {
    int score = 0;
    int gameOver = 0;//游戏的结束标志，当这个变量为1时表示游戏结束
    int speedMs = 200;//蛇移动的初始速度
    int startX = WIDTH;
    int startY = HEIGHT;
    //初始化蛇的位置在界面的中央（高度的一半，宽度的一半）
    int currentLevel = 1;//方便后续识别关卡

    hide_cursor();//隐藏光标
    //蛇的初始化，调用类内的方法
    Snake s;
    Snake_init(&s, startX,startY);//初始化位置，长度等
    //食物类的初始化部分
    Food f;
    Snake snakes[1]={s};
    place_food_safe(&f,&s,1);

    clean_screen();//执行清屏的操作，准备进入循环
    
    while(!gameOver){
        draw_board(&s,&f,score,speedMs);//画面的渲染

        //输入与定时
        int ticks;
        //通过延时速度确定循环次数，保证蛇的移动速度以及相应移动按键
        while(ticks<speedMs/20+1){
            //检测是否有按键按下
            process_input(&s,&gameOver);
            if(gameOver){
                //按“ESC”会改变gameOver的值，退出循环
                break;
            }
            Sleep(20);//每次循环睡眠20ms，防止cpu占用过高
            ticks++;
        }
        snake_move();//调用蛇移动的方法

        //碰撞检测：碰到墙，碰到自己
        if(check_self_collision(&s)||check_wall_collision(&s)){
            gameOver = 1;
            break;
        }
        //吃到食物检测
        //主要通过判断蛇头的组表与食物的坐标是否重合
        if(s.getX()==f.getX() && s.getY()==f.getY()){
            s.snake_grow();//长度增加,新节点位置设置到蛇的尾部的位置
            score++;
            place_food_safe(&f,&s,1);//放置新的食物
        }

    }
    //游戏结束之后，移动光标到画面下边，读取名字
    set_cusor_pos(0,HEIGHT+2);
    while(_kbhit()) _getch;//清空缓冲区

    prompt_and_update_leaderboard(score);



}