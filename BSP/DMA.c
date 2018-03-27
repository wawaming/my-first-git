/*************************************************************************************
*	@file			DMA.c
* @author	 	yun
*	@version 	V1.0
*	@date			2016/3/27
* @brief		NONE
*************************************************************************************/
#include "DMA.h"

/**
	*	@brief	DMA����
	*	@param	DR_Add			����Դ��ַ
				MMB_Add			����Ŀ���ַ
				BufferSize	�������ݵĴ�С
				Channel			����ͨ����ѡ��
	*	@attention			���Ҫʹ��DMAͨ��������DMA�����⣬����ʹ��DMA��Ӧ��ͨ�����磺DMA_Cmd(DMA1_Channel5,ENABLE);
							���⻹��ʹ����Ӧ�����DMAͨ��,TIM_DMACmd(TIM2, TIM_DMA_CC1, ENABLE)
							�������жϺ󣬻����д�жϺ�����DMA1_Channel1_IRQHandler
							FlagStatus DMA_GetFlagStatus(uint32_t DMAy_FLAG);	//��ȡ��־λ��ֵ
							void DMA_ClearFlag(uint32_t DMAy_FLAG);						//�����־λ��ֵ
	*	@retval	NONE
	*/
void DMA_Config(DMA_Stream_TypeDef *DMA_Streamx, uint32_t DR_Add, uint32_t MMB_Add, uint32_t BufferSize, uint32_t	Channel)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	if((uint32_t)DMA_Streamx>(uint32_t)DMA2)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); 
	else
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); 

	DMA_DeInit(DMA_Streamx); 
	DMA_InitStructure.DMA_Channel = Channel;
	DMA_InitStructure.DMA_PeripheralBaseAddr = DR_Add;			//Դ���ݵ�ַ  
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)MMB_Add; 	//Ŀ���ڴ��ַ	
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;        	//���ݴ��ͷ���
	DMA_InitStructure.DMA_BufferSize = BufferSize;    	      	//���ݴ�С����λΪDMA_PeripheralDataSize��
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;          //�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //��������Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  	    //�ڴ�����Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;            	//ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;        	//���ȼ���
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA_Streamx,&DMA_InitStructure); 	                   	//����ͨ��
	
	DMA_ITConfig(DMA_Streamx, DMA_IT_TC, ENABLE);					//��ͨ����������ж�
	
}



/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/








