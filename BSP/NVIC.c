/*************************************************************************************
*	@file			NVIC.c
* @author	 	yun
*	@version 	V1.0
*	@date			2017/3/3
* @brief		配置完相应的中断优先级后，需要打开相应的中断组
*************************************************************************************/

#include "NVIC.h"

/**
	*	@brief	外部中断优先级配置
	*	@param	IRQChannel							全局查找IRQ，即可找到相应外设对应的通道标志 如：TIM2_IRQn
						PreemptionPriority			抢占优先级，根据中断组配置
						SubPriority							响应有限级，根据中断组配置
						相应的中断函数编写为:   例: TIM2_IRQHandler
	*	@retval	NONE
	*/
void NVIC_Config(uint8_t IRQChannel, uint8_t PreemptionPriority, uint8_t SubPriority)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		
		NVIC_InitStructure.NVIC_IRQChannel										 = 	IRQChannel;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	 = 	PreemptionPriority;		//	抢占优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority					 =  SubPriority;					//	响应优先级
		NVIC_InitStructure.NVIC_IRQChannelCmd									 = 	ENABLE;
	
		NVIC_Init(&NVIC_InitStructure);
}

/**
	*	@brief	相应的外部中断配置函数
	*	@equation		
						NVIC_PriorityGroupConfig(NVIC_PriorityGroup_x);		中断组的选择
	*/

/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/


