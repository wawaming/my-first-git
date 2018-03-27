///////////////////////////////////////////////////////////////////
//										*		LED	CONFIG	*
//	
//
//
//	*@author		yun
//	*@vertion		V1.0
//	*@date			2016/6/9
////////////////////////////////////////////////////////////////////
#include "LED.h"

/**
	*@brief: 	LED_Init	LED≥ı ºªØ
	*@param:	NONE
	*@retval:	NONE
	*/
void LED_Init(void)
{
	
	GPIO_Config(GPIOB, GPIO_Pin_3, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);
	GPIO_Config(GPIOE, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);
	GPIO_Config(GPIOE, GPIO_Pin_0,  GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);
	GPIO_Config(GPIOC, GPIO_Pin_14, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);

	LED0(0);
	LED1(0);
	LED2(0);
	LED3(0);
}



