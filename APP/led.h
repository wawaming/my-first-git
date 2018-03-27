#ifndef	_LED_H
#define _LED_H


#include "includes.h"

//#define LED_GPIOX				GPIOG
//#define LED_GPIO_PINX		GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8
/************************** led enable or disable *****************/
#define	LED0(x)	if(x==1) GPIO_ResetBits(GPIOB, GPIO_Pin_3);\
								else		 GPIO_SetBits(GPIOB, GPIO_Pin_3)
#define LED1(x) if(x==1) GPIO_ResetBits(GPIOE, GPIO_Pin_1);\
								else 		 GPIO_SetBits(GPIOE, GPIO_Pin_1)
#define	LED2(x)	if(x==1) GPIO_ResetBits(GPIOE, GPIO_Pin_0);\
								else		 GPIO_SetBits(GPIOE, GPIO_Pin_0)
#define LED3(x) if(x==1) GPIO_ResetBits(GPIOC, GPIO_Pin_14);\
								else 		 GPIO_SetBits(GPIOC, GPIO_Pin_14)

#define	QFLED0	if(GPIO_ReadOutputDataBit(GPIOG, GPIO_Pin_15)==0) GPIO_SetBits(GPIOG, GPIO_Pin_15);\
								else		 GPIO_ResetBits(GPIOG, GPIO_Pin_15)
#define QFLED1 if(GPIO_ReadOutputDataBit(GPIOG, GPIO_Pin_14)==0) GPIO_SetBits(GPIOG, GPIO_Pin_14);\
								else 		 GPIO_ResetBits(GPIOG, GPIO_Pin_14)
#define	QFLED2	if(GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_0)==0) GPIO_SetBits(GPIOE, GPIO_Pin_0);\
								else		 GPIO_ResetBits(GPIOE, GPIO_Pin_0)
#define QFLED3 if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_14)==0) GPIO_SetBits(GPIOC, GPIO_Pin_14);\
								else 		 GPIO_ResetBits(GPIOC, GPIO_Pin_14)							
									
								
								
								
								
void LED_Init(void);
#endif




