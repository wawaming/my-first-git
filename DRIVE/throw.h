#ifndef _THROW_H
#define _THROW_H


#include "includes.h"
#include "Under_Pan.h"

/*����Ϊ������ʼ����ȫ��������ڲ���*/
/*��Ȧ���������ͼ���ʱ�򣬲����ֱ�Ϊ50000��71356��p=0.0000139 d=0.000215*/
#define throw_point  3//10000            //������,���Ҫ200r/min��Ҫ��һȦ�����׳�
#define set_distance 182034          //71356���پ����趨����λ���壬��������Ϊ60678����һȦ132034
#define throw_speed  200              //�ͷ��ٶȣ���λr/min
#define throw_P      0.000021                 
#define throw_D      0.0013923
#define throw_speed_max 200  
#define throw_speed_min -200   //�����ٶ� r/min
#define Pi 3.1416            //��
#define TIME_cyc 20         //�������� ms
#define control_num_max (set_distance*2/throw_speed_max/TIME_cyc) //�����ƴ���
/*����Ϊ������ʼ����ȫ��������ڲ���*/

void throw_ball(void);











#endif
