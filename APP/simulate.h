#ifndef _simulate_H
#define _simulate_H

#include "includes.h"

/*	摩擦  参数待修改*/
#define SERVO_KAI	 100									
#define SERVO_GUAN  200

#define Model_Aircraft_1_3		1000			
#define Model_Aircraft_1_5		1600
#define Model_Aircraft_1_9		2000

/*	仿生	*/
void  cunpan(void);
void  yu_zhuangpan(uint8_t fashe_x);
uint8_t  zhuangpan(uint8_t fashe_x);
uint8_t  fashe(uint8_t fashe_x);


/*	摩擦	*/
void qidong(uint8_t jipan_x);
void jipan(void);

void Friction_Falling(int distance,int distance_speed);
void Clear_Struct(char *pta,int size);


/*仿生*/
void Falling_3m1();

#endif



