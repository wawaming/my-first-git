#ifndef	_A_TEST_H
#define _A_TEST_H

#include "includes.h"

/*		蜂鸣器		*/
#define BEED_ON		GPIO_SetBits(GPIOD, GPIO_Pin_3)
#define BEED_OFF	GPIO_ResetBits(GPIOD, GPIO_Pin_3)
#define QBEED		 if(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_3))  GPIO_ResetBits(GPIOD, GPIO_Pin_3);  else GPIO_SetBits(GPIOD, GPIO_Pin_3);
/*		按键			*/

#define KEY1_CHECK			GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_6)				//返回按键io上的电平状态
#define KEY2_CHECK			GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_5)
#define KEY3_CHECK			GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_4)
#define KEY4_CHECK			GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_3)
#define KEY5_CHECK			GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_2)

#define KEY1	1
#define KEY2	2
#define KEY3	3
#define KEY4	4
#define KEY5	5
#define KEY_NONE	0				

/*  电磁阀   */

#define RL1_OFF GPIO_ResetBits(GPIOE, GPIO_Pin_12)
#define RL2_OFF GPIO_ResetBits(GPIOG, GPIO_Pin_1)
#define RL3_OFF GPIO_ResetBits(GPIOE, GPIO_Pin_14)
#define RL4_OFF GPIO_ResetBits(GPIOE, GPIO_Pin_8)
#define RL5_OFF GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define RL6_OFF GPIO_ResetBits(GPIOB, GPIO_Pin_1)
#define RL7_OFF GPIO_ResetBits(GPIOB, GPIO_Pin_2)
#define RL8_OFF GPIO_ResetBits(GPIOF, GPIO_Pin_11)
#define RL9_OFF GPIO_ResetBits(GPIOF, GPIO_Pin_12)
#define RL10_OFF GPIO_ResetBits(GPIOF, GPIO_Pin_13)
#define RL11_OFF GPIO_ResetBits(GPIOF, GPIO_Pin_14)
#define RL12_OFF GPIO_ResetBits(GPIOF, GPIO_Pin_15)

#define RL1_ON GPIO_SetBits(GPIOE, GPIO_Pin_12)
#define RL2_ON GPIO_SetBits(GPIOG, GPIO_Pin_1)
#define RL3_ON GPIO_SetBits(GPIOE, GPIO_Pin_14)
#define RL4_ON GPIO_SetBits(GPIOE, GPIO_Pin_8)
#define RL5_ON GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define RL6_ON GPIO_SetBits(GPIOB, GPIO_Pin_1)
#define RL7_ON GPIO_SetBits(GPIOB, GPIO_Pin_2)
#define RL8_ON GPIO_SetBits(GPIOF, GPIO_Pin_11)
#define RL9_ON GPIO_SetBits(GPIOF, GPIO_Pin_12)
#define RL10_ON GPIO_SetBits(GPIOF, GPIO_Pin_13)
#define RL11_ON GPIO_SetBits(GPIOF, GPIO_Pin_14)
#define RL12_ON GPIO_SetBits(GPIOF, GPIO_Pin_15)

									

								

#define Model_Aircraft_1					TIM1->CCR1
#define Model_Aircraft_2					TIM1->CCR2
#define Model_Aircraft_3					TIM1->CCR3
#define Model_Aircraft_4					TIM1->CCR4


#define SERVO_PWM1								TIM3->CCR1
#define SERVO_PWM2								TIM3->CCR2
#define SERVO_PWM3								TIM3->CCR3
#define SERVO_PWM4								TIM3->CCR4

#define SERVO_TOP					2000
#define SERVO_BOTTOM			960

void BEED_Init(void);												//蜂鸣器
uint8_t Key_Scan(void);											//按键
void Encoder_Init(void);										//编码器
void TIM5_Init(void);												//定时器5
void TIM3_Init(void);												//定时器3
void CAN_Delay(uint8_t times);							//CAN延时
void RELAVY_Init(void);											//气缸
void Model_Aircraft_Init(void);							//无刷电机
void SERVO_Init(void);
void Photoelectric_Init(void);
void KEY_Init(void);
void DI_Init(void);
void TIM7_Init();
void TIM1_Init(void);												//定时器1


#endif



