#ifndef _simulate_H
#define _simulate_H

#include "includes.h"

/*	Ħ��  �������޸�*/
#define SERVO_KAI	 100									
#define SERVO_GUAN  200

#define Model_Aircraft_1_3		1000			
#define Model_Aircraft_1_5		1600
#define Model_Aircraft_1_9		2000

/*	����	*/
void  cunpan(void);
void  yu_zhuangpan(uint8_t fashe_x);
uint8_t  zhuangpan(uint8_t fashe_x);
uint8_t  fashe(uint8_t fashe_x);


/*	Ħ��	*/
void qidong(uint8_t jipan_x);
void jipan(void);

void Friction_Falling(int distance,int distance_speed);
void Clear_Struct(char *pta,int size);


/*����*/
void Falling_3m1();

#endif



