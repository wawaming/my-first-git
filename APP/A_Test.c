/*************************************************************************************
*	@file			A_Test.c
*	@supplement A车各模块初始化函数(调试所用)
* @author	 	yun
*	@version 	V1.2
*	@date			2016/8/8
* @brief		NONE
*************************************************************************************/

#include "A_Test.h"

///////////////////////////////////////////////////////////
/************************全局变量************************/
///////////////////////////////////////////////////////////

/**
	*	@brief	调试按键扫描程序		
						调试按键是不需要初始化的，在外部已经进行了硬件上拉或者下拉的情况下
						因为io口不论什么状态，都可以出去io的电平情况
	*	@param 	NONE
	* @retval NONE
	*/
	
void KEY_Init(void)
{
  GPIO_Config(GPIOF, GPIO_Pin_6, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOF, GPIO_Pin_5, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOF, GPIO_Pin_4, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOF, GPIO_Pin_3, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOF, GPIO_Pin_2, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);
}

uint8_t Key_Scan(void)
{
	if(!KEY1_CHECK)      return KEY1;
	else if(!KEY2_CHECK) return KEY2;
	else if(!KEY3_CHECK) return KEY3;
	else if(!KEY4_CHECK) return KEY4;
	else if(!KEY5_CHECK) return KEY5;
	else return 0;
}


/**
	*	@brief	蜂鸣器调试	
	*	@param 	NONE
	* @retval NONE
	*/
void BEED_Init(void)
{
	GPIO_Config(GPIOD, GPIO_Pin_3, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP);
	BEED_OFF;
}


/**************************************************************************************
**
	*	@brief	Encoder_Init(void)
	*	@param 	NONE
	*	@supplement GPIOA0	GPIOA1	一组		中断 0 2
								GPIOD12	GPIOD13	一组		中断 0 1 
	* @retval NONE
	*
***************************************************************************************/
void Encoder_Init(void)
{	
	/*	TIM4的编码器模式	通道1、2*/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_Config(GPIOD,GPIO_Pin_12|GPIO_Pin_13,GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	TIM_ENCODER_Config(TIM4,0);
	NVIC_Config(TIM4_IRQn, 0, 1);
	
	/*	TIM2的编码器模式	通道1、2*/
	GPIO_Config(GPIOA, GPIO_Pin_0|GPIO_Pin_1, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
	
	TIM_ENCODER_Config(TIM2,0);
	NVIC_Config(TIM2_IRQn, 0, 2);
	
	/*	TIM8的编码器模式	通道1、2*/
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
	GPIO_Config(GPIOC, GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	TIM_ENCODER_Config(TIM8,0);
	NVIC_Config(TIM8_UP_TIM13_IRQn , 0, 3);
	
	/*	TIM5的编码器模式	通道3、4*/
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);
//	GPIO_Config(GPIOA, GPIO_Pin_2|GPIO_Pin_3, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
//	TIM_ENCODER_Config(TIM5,1);
//	NVIC_Config(TIM5_IRQn ,0, 3);
}

/***************************************************************************************
**
	*	@brief	TIM5_Init(void)
	*	@param
	*	@supplement	
	*	@retval	NONE
****************************************************************************************/
 void TIM5_Init(void)
{
	TIM_Timing_Config(TIM5, 500, TIM_US, ON);	//84MHZ,定时5ms
	NVIC_Config(TIM5_IRQn, 0, 0);
}

void TIM7_Init(void)
{
	TIM_Timing_Config(TIM7, 20, TIM_MS, ON);	//
	NVIC_Config(TIM7_IRQn, 0, 3);
	TIM_Cmd(TIM7, DISABLE);
}

// void TIM1_Init(void)
//{
//	TIM_Timing_Config(TIM1, 20, TIM_MS, ON);	//84MHZ,定时5ms
//	NVIC_Config(TIM1_CC_IRQn , 0, 3);
//}

/***************************************************************************************
**
	*	@brief	TIM5_Init(void)
	*	@param
	*	@supplement	
	*	@retval	NONE
****************************************************************************************/
void TIM3_Init(void)
{
	TIM_Timing_Config(TIM3, 200, TIM_US, ON);	//
	NVIC_Config(TIM3_IRQn, 0, 0);
	TIM_Cmd(TIM7, DISABLE);
}

/****************************************************************************************
**
	*	@brief	CAN_Delay(uint8_t times)
	*	@param	times	多少次
	*	@supplement
	*	@retval	NONE
	*
******************************************************************************************/
void CAN_Delay(uint8_t times)
{
	uint8_t i=0;
	while(times--,times){
		for(i=0;i<40;i++)
		;
	}
	
}

/**
	*	@brief  气缸调试
	*	@param 	NONE
	* @retval NONE
	*/
void RELAVY_Init(void)
{
	GPIO_Config(GPIOE, GPIO_Pin_12, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //软件上拉，普通推挽输出  回程
	GPIO_Config(GPIOG, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //软件上拉，普通推挽输出	 释放
	GPIO_Config(GPIOE, GPIO_Pin_14, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //软件上拉，普通推挽输出  装盘
	GPIO_Config(GPIOE, GPIO_Pin_8, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //软件上拉，普通推挽输出
	GPIO_Config(GPIOB, GPIO_Pin_0, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //软件上拉，普通推挽输出
	GPIO_Config(GPIOB, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //软件上拉，普通推挽输出
	GPIO_Config(GPIOB, GPIO_Pin_2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //软件上拉，普通推挽输出
	GPIO_Config(GPIOF, GPIO_Pin_11, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //软件上拉，普通推挽输出	
	GPIO_Config(GPIOF, GPIO_Pin_12, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //软件上拉，普通推挽输出
	GPIO_Config(GPIOF, GPIO_Pin_13, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //软件上拉，普通推挽输出	
	GPIO_Config(GPIOF, GPIO_Pin_14, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //软件上拉，普通推挽输出
	GPIO_Config(GPIOF, GPIO_Pin_15, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //软件上拉，普通推挽输出	
}


/**
	*	@brief	定时器1 无刷电机 pwm输出 
	*	@param 	NONE
	* @retval NONE
	*/
void Model_Aircraft_Init(void)
{
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);
	GPIO_Config(GPIOA, GPIO_Pin_8, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOA, GPIO_Pin_9, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOA, GPIO_Pin_10, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOA, GPIO_Pin_11, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	TIM_Timing_Config(TIM1, 20000, TIM_US, ON);	
	TIM_PWM_Config(TIM1,1400, 1000,  1000, 1000);
}

/***************************************************************************************
**
	*	@brief	TIM3_Init(void)
	*	@param
	*	@supplement	用于PWM波输出,控制舵机       50HZ、一通道
	*	@retval	NONE
****************************************************************************************/
void SERVO_Init(void)
{
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_Config(GPIOB, GPIO_Pin_4, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);//通道1
	GPIO_Config(GPIOB, GPIO_Pin_5, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);//2
	GPIO_Config(GPIOC, GPIO_Pin_8, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);//3
	GPIO_Config(GPIOC, GPIO_Pin_9, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);//4
	TIM_Timing_Config(TIM3, 20000, TIM_US,ON);	//20MS周期
	TIM_PWM_Config(TIM3, 1000, 1000, 1000, 1000);	
}

/**
  * @brief  光电对管外部中断配置
  * @param  无
  * @retval 无
  */
//void Photoelectric_Init(void)
//{
//	  GPIO_Config(GPIOD,GPIO_Pin_2,GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);//DI1,2
//	  GPIO_Config(GPIOD,GPIO_Pin_3,GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);//DI3,4
//	  GPIO_Config(GPIOG,GPIO_Pin_13,GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);//DI5
//	  GPIO_Config(GPIOB,GPIO_Pin_3,GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);//DI6
//		Exti_config(GPIOD,EXTI_Line2, EXTI_Trigger_Rising_Falling);
//	  Exti_config(GPIOD,EXTI_Line3, EXTI_Trigger_Rising_Falling);
//	  Exti_config(GPIOG,EXTI_Line13, EXTI_Trigger_Rising_Falling);
//	  Exti_config(GPIOB,EXTI_Line3, EXTI_Trigger_Rising_Falling);
//	  NVIC_Config(EXTI15_10_IRQn, 1, 4);
//}


/**
	*	@brief  光电 
	*	@param 	NONE
	* @retval NONE
	*/
void DI_Init(void)
{
  GPIO_Config(GPIOA, GPIO_Pin_8, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//PAOZHI
	GPIO_Config(GPIOG, GPIO_Pin_13, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI6
	GPIO_Config(GPIOG, GPIO_Pin_15, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI7
	GPIO_Config(GPIOB, GPIO_Pin_8, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI8
	GPIO_Config(GPIOA, GPIO_Pin_2, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI1    //软件上拉，上拉输入
	GPIO_Config(GPIOD, GPIO_Pin_4, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI2
	GPIO_Config(GPIOD, GPIO_Pin_6, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI3
	GPIO_Config(GPIOG, GPIO_Pin_9, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI4
	GPIO_Config(GPIOG, GPIO_Pin_11, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI5
  Exti_config(GPIOG, EXTI_Line13|EXTI_Line15|EXTI_Line9|EXTI_Line11, EXTI_Trigger_Rising_Falling);
	Exti_config(GPIOB, EXTI_Line8, EXTI_Trigger_Rising_Falling);
	Exti_config(GPIOD, EXTI_Line4|EXTI_Line6, EXTI_Trigger_Rising_Falling);
	Exti_config(GPIOA, EXTI_Line2, EXTI_Trigger_Rising_Falling);
	Exti_config(GPIOA, EXTI_Line8, EXTI_Trigger_Rising_Falling);//PAOZHI
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource8);//PAOZHI
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource4);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource6);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource9);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource11);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource13);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource15);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource8);
	NVIC_Config(EXTI15_10_IRQn, 2, 0);
	NVIC_Config(EXTI9_5_IRQn, 2,3);
	NVIC_Config(EXTI2_IRQn, 1, 2);
	NVIC_Config(EXTI4_IRQn, 2, 2);

}




