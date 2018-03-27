/*************************************************************************************
*	@file			ADC.c
* @author	 	yun
*	@version 	V1.0
*	@date			2016/3/27
* @brief		NONE
*************************************************************************************/

#include "ADC.h"

/**
	*	@brief	ADC模块配置(独立非扫描模式)
	*	@param	GPIOx		GPIOA~GPIOE			需要配置的GPIO端口
				PINX		GPIO_Pin_0~GPIO_Pin_15~GPIO_Pin_ALL		需要配置的引脚号
				ADCx		ADC1~ADC3				选择对应的ADC端口
				ADC_ContinuousConvMode		ENABLE/DISABLE			连续或者单次转换
	*	@retval	NONE
	*/
void ADC_Config(GPIO_TypeDef *GPIOx , uint16_t PINX, ADC_TypeDef *ADCx, FunctionalState ADC_Continuous_Mode)
{
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_Config(GPIOx, PINX, GPIO_Mode_AN, GPIO_OType_PP, GPIO_PuPd_NOPULL);		//配置ADC通道对应的GPIO端口,模拟输入
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1<<(((uint32_t)ADCx-ADC1_BASE)/0x0400),ENABLE);//打开相应的ADC时钟
	

	ADC_CommonInitStructure.ADC_Mode					   = ADC_Mode_Independent;							//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;			//两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;				//DMA失能
	ADC_CommonInitStructure.ADC_Prescaler        = ADC_Prescaler_Div4;								//84MHZ/4 = 21MHZ	预分频4分频，ADC时钟最好不要超过36MHZ
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_ScanConvMode			  = DISABLE;							//非扫描模式					
	ADC_InitStructure.ADC_ContinuousConvMode	= ADC_Continuous_Mode;	//打开连续转换
	ADC_InitStructure.ADC_DataAlign				= ADC_DataAlign_Right;			//右对齐
	ADC_InitStructure.ADC_NbrOfConversion			= 1;
	ADC_InitStructure.ADC_ExternalTrigConvEdge		= ADC_ExternalTrigConvEdge_None;	//禁止触发检测，使用软件触发
	ADC_Init(ADCx,&ADC_InitStructure);
	
	ADC_Cmd(ADCx, ENABLE);							//使能指定的ADCx
}


/**
	*	@brief	ADC转换结果的获取
	*	@param	ADCx			ADC1~ADC3				选择对应的ADC端口
				ADC_Channel		ADC_Channel_0~ADC_Channel_17	ADC转换的通道
				Rank					设置为第几个转换（序列）
				ADC_SampleTime			ADC转换的周期T=(ADC_SampleTime+12.5)
	*	@retval	NONE
	*/
u16 Get_Adc(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADCx, ADC_Channel, Rank, ADC_SampleTime );	//ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConv(ADCx);		//软件触发ADCx的转换
	 
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADCx);	//返回最近一次ADC规则组的转换结果
}




/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/




