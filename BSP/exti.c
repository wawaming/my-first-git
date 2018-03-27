/*************************************************************************************
*	@file			exti.c
* @author	 	yun
*	@version 	V1.0
*	@date			2016/3/27
* @brief		NONE
*************************************************************************************/

#include "exti.h"

/**
	*@brief:	外部中断设置
	*@param:	GPIO_TypeDef GPIOx	   GPIOA~GPIOG	外部中断需要连接的IO口
						uint32_t EXTI_Linex		即外部中断线	EXTI_Line0~EXTI_Linex15；
						EXTITrigger_TypeDef extiTrigger     触发方式：EXTI_Trigger_Rising		上升沿触发
																													EXTI_Trigger_Falling	下降沿触发
																													EXTI_Trigger_Rising_Falling	跳变沿触发
	*@supplement:		启用时还需初始化相关的GPIO模式,调用GPIO_Config函数
									其中中断线5~9共用中断函数EXTI9_5_IRQHandler
									中断线10~15公用中断函数EXTI15_10_IRQHandler
	*@retval:				NONE
	*/
int dif_2 = -1;
void Exti_config(GPIO_TypeDef	*GPIOx, uint32_t EXTI_Linex, EXTITrigger_TypeDef extiTrigger)
{
	uint32_t dif;		//GPIOx地址偏移量
	int i = EXTI_Linex;
	
	EXTI_InitTypeDef EXTI_InitStructure;
	
	dif=(((uint32_t)(GPIOx)-GPIOA_BASE)/0x0400);
	while(i >>=1,dif_2++,i);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	SYSCFG_EXTILineConfig((uint8_t)EXTI_PortSourceGPIOA + dif,(uint8_t)(GPIO_PinSource0+dif_2));
	
	EXTI_InitStructure.EXTI_Line 		= EXTI_Linex;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;								//外部中断线使能
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;	//中断模式
	EXTI_InitStructure.EXTI_Trigger = extiTrigger;
	EXTI_Init(&EXTI_InitStructure);
	
}

/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/





