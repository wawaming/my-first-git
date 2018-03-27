#ifndef _ADC_H
#define _ADC_H

#include "includes.h"

void ADC_Config(GPIO_TypeDef *GPIOx , uint16_t PINX, ADC_TypeDef *ADCx, FunctionalState ADC_Continuous_Mode);
u16 Get_Adc(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);  






/************************************************用户自定义函数定义********************************************************/
/************************************************用户自定义函数定义********************************************************/
/************************************************用户自定义函数定义********************************************************/




#endif




