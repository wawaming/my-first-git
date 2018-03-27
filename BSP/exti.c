/*************************************************************************************
*	@file			exti.c
* @author	 	yun
*	@version 	V1.0
*	@date			2016/3/27
* @brief		NONE
*************************************************************************************/

#include "exti.h"

/**
	*@brief:	�ⲿ�ж�����
	*@param:	GPIO_TypeDef GPIOx	   GPIOA~GPIOG	�ⲿ�ж���Ҫ���ӵ�IO��
						uint32_t EXTI_Linex		���ⲿ�ж���	EXTI_Line0~EXTI_Linex15��
						EXTITrigger_TypeDef extiTrigger     ������ʽ��EXTI_Trigger_Rising		�����ش���
																													EXTI_Trigger_Falling	�½��ش���
																													EXTI_Trigger_Rising_Falling	�����ش���
	*@supplement:		����ʱ�����ʼ����ص�GPIOģʽ,����GPIO_Config����
									�����ж���5~9�����жϺ���EXTI9_5_IRQHandler
									�ж���10~15�����жϺ���EXTI15_10_IRQHandler
	*@retval:				NONE
	*/
int dif_2 = -1;
void Exti_config(GPIO_TypeDef	*GPIOx, uint32_t EXTI_Linex, EXTITrigger_TypeDef extiTrigger)
{
	uint32_t dif;		//GPIOx��ַƫ����
	int i = EXTI_Linex;
	
	EXTI_InitTypeDef EXTI_InitStructure;
	
	dif=(((uint32_t)(GPIOx)-GPIOA_BASE)/0x0400);
	while(i >>=1,dif_2++,i);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	SYSCFG_EXTILineConfig((uint8_t)EXTI_PortSourceGPIOA + dif,(uint8_t)(GPIO_PinSource0+dif_2));
	
	EXTI_InitStructure.EXTI_Line 		= EXTI_Linex;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;								//�ⲿ�ж���ʹ��
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;	//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = extiTrigger;
	EXTI_Init(&EXTI_InitStructure);
	
}

/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/





