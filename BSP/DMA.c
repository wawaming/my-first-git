/*************************************************************************************
*	@file			DMA.c
* @author	 	yun
*	@version 	V1.0
*	@date			2016/3/27
* @brief		NONE
*************************************************************************************/
#include "DMA.h"

/**
	*	@brief	DMA配置
	*	@param	DR_Add			数据源地址
				MMB_Add			数据目标地址
				BufferSize	传输数据的大小
				Channel			传输通道的选择
	*	@attention			如果要使能DMA通道，除了DMA配置外，还需使能DMA相应的通道，如：DMA_Cmd(DMA1_Channel5,ENABLE);
							此外还需使能相应外设的DMA通道,TIM_DMACmd(TIM2, TIM_DMA_CC1, ENABLE)
							当开启中断后，还需编写中断函数：DMA1_Channel1_IRQHandler
							FlagStatus DMA_GetFlagStatus(uint32_t DMAy_FLAG);	//获取标志位的值
							void DMA_ClearFlag(uint32_t DMAy_FLAG);						//清除标志位的值
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
	DMA_InitStructure.DMA_PeripheralBaseAddr = DR_Add;			//源数据地址  
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)MMB_Add; 	//目标内存地址	
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;        	//数据传送方向
	DMA_InitStructure.DMA_BufferSize = BufferSize;    	      	//数据大小（单位为DMA_PeripheralDataSize）
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;          //内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  	    //内存数据为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;            	//循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;        	//优先级高
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA_Streamx,&DMA_InitStructure); 	                   	//配置通道
	
	DMA_ITConfig(DMA_Streamx, DMA_IT_TC, ENABLE);					//打开通道传输完成中断
	
}



/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/








