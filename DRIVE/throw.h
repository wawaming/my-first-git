#ifndef _THROW_H
#define _THROW_H


#include "includes.h"
#include "Under_Pan.h"

/*以下为参数初始化，全部抛掷入口参数*/
/*两圈包括抛掷和减速时候，参数分别为50000、71356、p=0.0000139 d=0.000215*/
#define throw_point  3//10000            //抛掷点,如果要200r/min则要第一圈就能抛出
#define set_distance 182034          //71356减速距离设定，单位脉冲，消除误差后为60678脉冲一圈132034
#define throw_speed  200              //释放速度，单位r/min
#define throw_P      0.000021                 
#define throw_D      0.0013923
#define throw_speed_max 200  
#define throw_speed_min -200   //抛掷速度 r/min
#define Pi 3.1416            //π
#define TIME_cyc 20         //控制周期 ms
#define control_num_max (set_distance*2/throw_speed_max/TIME_cyc) //最大控制次数
/*以上为参数初始化，全部抛掷入口参数*/

void throw_ball(void);











#endif
