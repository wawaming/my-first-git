#ifndef _I2C_H
#define _I2C_H

#include "includes.h"

#define IIC_GPIOX			GPIOB
#define IIC_SCL_PIN		GPIO_Pin_6
#define IIC_SDA_PIN		GPIO_Pin_7

#define IIC_Delay	 				delay_us(5)

#define IIC_SDL_IN_SET()		{GPIO_Config(IIC_GPIOX, IIC_SDA_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL);}
#define IIC_SDL_OUT_SET()		{GPIO_Config(IIC_GPIOX, IIC_SDA_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);}


#define IIC_SCL_IN()				GPIO_ReadInputDataBit(IIC_GPIOX,IIC_SCL_PIN)
#define IIC_SDA_IN()				GPIO_ReadInputDataBit(IIC_GPIOX,IIC_SDA_PIN)
#define IIC_SCL_OUT(X)		if(X==1)		GPIO_SetBits(IIC_GPIOX,IIC_SCL_PIN);\
													else				GPIO_ResetBits(IIC_GPIOX,IIC_SCL_PIN)		
#define IIC_SDA_OUT(X)		if(X==1)		GPIO_SetBits(IIC_GPIOX,IIC_SDA_PIN);\
													else        GPIO_ResetBits(IIC_GPIOX,IIC_SDA_PIN)

void IIC_Init(void);
void IIC_leisure(void);
void IIC_Star(void);
void IIC_Stop(void);
void IIC_SendAck(uint8_t ack);
u8 IIC_RiceveAck(void);
void IIC_SendByte(uint8_t data);
u8 IIC_RiceveByte(uint8_t ack);
void IIC_SendData(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data);
u8 IIC_RiceveData(uint8_t SlaveAddress, uint8_t REG_Address);
													
													
													
													
													
/************************************************用户自定义函数定义********************************************************/
/************************************************用户自定义函数定义********************************************************/
/************************************************用户自定义函数定义********************************************************/

#endif


