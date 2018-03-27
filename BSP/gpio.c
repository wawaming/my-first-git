/*************************************************************************************
*	@file			gpio.c
* @author	 	yun
*	@version 	V1.0
*	@date			2017/3/3
* @brief		NONE
*************************************************************************************/

#include "gpio.h"

/**
	*	@brief		gpioͨ��ģʽ����
	* @param		
							*GPIOx			GPIOA~GPIOK		��Ҫ���õ����Ŷ˿�
													GPIOx: where x can be (A..K) to select the GPIO peripheral for STM32F405xx/407xx and STM32F415xx/417xx devices
  *                      				x can be (A..I) to select the GPIO peripheral for STM32F42xxx/43xxx devices.
  *                      				x can be (A, B, C, D and H) to select the GPIO peripheral for STM32F401xx devices.   
							
							GPIO_Pin_x			GPIO_Pin_0~GPIO_Pin_15~GPIO_Pin_ALL		��Ҫ���õ����ź�
							
							GPIOMode_TypeDef							GPIO_Mode_IN   = 0x00, < GPIO Input Mode >
																						GPIO_Mode_OUT  = 0x01, < GPIO Output Mode >
																						GPIO_Mode_AF   = 0x02, < GPIO Alternate function Mode >
																						GPIO_Mode_AN   = 0x03  < GPIO Analog Mode >
																						
							GPIO_MODE		GPIO_Mode_AIN = 0x0,				//ģ������
											GPIO_Mode_IN_FLOATING = 0x04,		//��������
											GPIO_Mode_IPD = 0x28,				//��������
											GPIO_Mode_IPU = 0x48,				//��������
											GPIO_Mode_Out_OD = 0x14,			//��©���
											GPIO_Mode_Out_PP = 0x10,			//�������
											GPIO_Mode_AF_OD = 0x1C,				//���ÿ�©���
											GPIO_Mode_AF_PP = 0x18				//�����������
	*	@attention	PC13,PC14,PC15ֻ������2MHz�����ģʽ�����ֻ�ܴ�30pf�ĸ��أ�����ͬʱֻ������һ������
	*	@retval		NONE
	*/
void GPIO_Config(GPIO_TypeDef	*GPIOx, uint16_t GPIO_Pin_x, GPIOMode_TypeDef GPIO_MODE, GPIOOType_TypeDef GPIO_TYPE, GPIOPuPd_TypeDef GPIO_PUPD)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA<<(((uint32_t)(GPIOx)-GPIOA_BASE)/0x0400),ENABLE);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Mode  = GPIO_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_TYPE;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PUPD;
	
	GPIO_Init(GPIOx, &GPIO_InitStructure);
	
}


/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/






