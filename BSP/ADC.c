/*************************************************************************************
*	@file			ADC.c
* @author	 	yun
*	@version 	V1.0
*	@date			2016/3/27
* @brief		NONE
*************************************************************************************/

#include "ADC.h"

/**
	*	@brief	ADCģ������(������ɨ��ģʽ)
	*	@param	GPIOx		GPIOA~GPIOE			��Ҫ���õ�GPIO�˿�
				PINX		GPIO_Pin_0~GPIO_Pin_15~GPIO_Pin_ALL		��Ҫ���õ����ź�
				ADCx		ADC1~ADC3				ѡ���Ӧ��ADC�˿�
				ADC_ContinuousConvMode		ENABLE/DISABLE			�������ߵ���ת��
	*	@retval	NONE
	*/
void ADC_Config(GPIO_TypeDef *GPIOx , uint16_t PINX, ADC_TypeDef *ADCx, FunctionalState ADC_Continuous_Mode)
{
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_Config(GPIOx, PINX, GPIO_Mode_AN, GPIO_OType_PP, GPIO_PuPd_NOPULL);		//����ADCͨ����Ӧ��GPIO�˿�,ģ������
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1<<(((uint32_t)ADCx-ADC1_BASE)/0x0400),ENABLE);//����Ӧ��ADCʱ��
	

	ADC_CommonInitStructure.ADC_Mode					   = ADC_Mode_Independent;							//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;			//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;				//DMAʧ��
	ADC_CommonInitStructure.ADC_Prescaler        = ADC_Prescaler_Div4;								//84MHZ/4 = 21MHZ	Ԥ��Ƶ4��Ƶ��ADCʱ����ò�Ҫ����36MHZ
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_ScanConvMode			  = DISABLE;							//��ɨ��ģʽ					
	ADC_InitStructure.ADC_ContinuousConvMode	= ADC_Continuous_Mode;	//������ת��
	ADC_InitStructure.ADC_DataAlign				= ADC_DataAlign_Right;			//�Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion			= 1;
	ADC_InitStructure.ADC_ExternalTrigConvEdge		= ADC_ExternalTrigConvEdge_None;	//��ֹ������⣬ʹ���������
	ADC_Init(ADCx,&ADC_InitStructure);
	
	ADC_Cmd(ADCx, ENABLE);							//ʹ��ָ����ADCx
}


/**
	*	@brief	ADCת������Ļ�ȡ
	*	@param	ADCx			ADC1~ADC3				ѡ���Ӧ��ADC�˿�
				ADC_Channel		ADC_Channel_0~ADC_Channel_17	ADCת����ͨ��
				Rank					����Ϊ�ڼ���ת�������У�
				ADC_SampleTime			ADCת��������T=(ADC_SampleTime+12.5)
	*	@retval	NONE
	*/
u16 Get_Adc(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADCx, ADC_Channel, Rank, ADC_SampleTime );	//ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConv(ADCx);		//�������ADCx��ת��
	 
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADCx);	//�������һ��ADC�������ת�����
}




/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/




