/*************************************************************************************
*	@file			NVIC.c
* @author	 	yun
*	@version 	V1.0
*	@date			2017/3/3
* @brief		��������Ӧ���ж����ȼ�����Ҫ����Ӧ���ж���
*************************************************************************************/

#include "NVIC.h"

/**
	*	@brief	�ⲿ�ж����ȼ�����
	*	@param	IRQChannel							ȫ�ֲ���IRQ�������ҵ���Ӧ�����Ӧ��ͨ����־ �磺TIM2_IRQn
						PreemptionPriority			��ռ���ȼ��������ж�������
						SubPriority							��Ӧ���޼��������ж�������
						��Ӧ���жϺ�����дΪ:   ��: TIM2_IRQHandler
	*	@retval	NONE
	*/
void NVIC_Config(uint8_t IRQChannel, uint8_t PreemptionPriority, uint8_t SubPriority)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		
		NVIC_InitStructure.NVIC_IRQChannel										 = 	IRQChannel;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	 = 	PreemptionPriority;		//	��ռ���ȼ�
		NVIC_InitStructure.NVIC_IRQChannelSubPriority					 =  SubPriority;					//	��Ӧ���ȼ�
		NVIC_InitStructure.NVIC_IRQChannelCmd									 = 	ENABLE;
	
		NVIC_Init(&NVIC_InitStructure);
}

/**
	*	@brief	��Ӧ���ⲿ�ж����ú���
	*	@equation		
						NVIC_PriorityGroupConfig(NVIC_PriorityGroup_x);		�ж����ѡ��
	*/

/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/


