#ifndef _TIME_H
#define _TIME_H
#include "includes.h"

typedef enum
{
	TIM_MIN = 1,
	TIM_S ,
	TIM_MS,
	TIM_US,
}TIM_Size_TypeDef;

void TIM_Timing_Config(TIM_TypeDef *TIMx, uint16_t  arr, TIM_Size_TypeDef TIM_Size, uint32_t on_or_off);
void TIM_Capture_Config(TIM_TypeDef *TIMx, uint16_t TIM_Channel_x, uint16_t TIM_ICPolarity_Mode);
void TIM_PWM_Config(TIM_TypeDef *TIMx,uint16_t Pulse1, uint16_t Pulse2, uint16_t Pulse3, uint16_t Pulse4);
void TIM_ENCODER_Config(TIM_TypeDef *TIMx,uint8_t channel_choose);	//编码器模式

                                                                                                                                   


/************************************************用户自定义函数定义********************************************************/
/************************************************用户自定义函数定义********************************************************/
/************************************************用户自定义函数定义********************************************************/




#endif

