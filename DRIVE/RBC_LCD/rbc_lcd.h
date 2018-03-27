#ifndef __RBC_LCD_H
#define __RBC_LCD_H
#include "includes.h"

#define RBC_LCD_GPIOX	GPIOF

#define RBC_LCD_PINX	GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10

#define RBC_LCD_CS		GPIO_Pin_10
#define RBC_LCD_SDA		GPIO_Pin_0
#define RBC_LCD_RS		GPIO_Pin_8
#define RBC_LCD_SCK		GPIO_Pin_1
#define RBC_LCD_RST		GPIO_Pin_9


#define CS(a)	if (a)	\
					GPIO_SetBits(		RBC_LCD_GPIOX,RBC_LCD_CS);\
					else		\
					GPIO_ResetBits(	RBC_LCD_GPIOX,RBC_LCD_CS)
#define RES(a)	if (a)	\
					GPIO_SetBits(		RBC_LCD_GPIOX,RBC_LCD_RST);\
					else		\
					GPIO_ResetBits(	RBC_LCD_GPIOX,RBC_LCD_RST)
#define RS(a)	if (a)	\
					GPIO_SetBits(		RBC_LCD_GPIOX,RBC_LCD_RS);\
					else		\
					GPIO_ResetBits(	RBC_LCD_GPIOX,RBC_LCD_RS)
#define SCLK(a)	if (a)	\
					GPIO_SetBits(		RBC_LCD_GPIOX,RBC_LCD_SCK);\
					else		\
					GPIO_ResetBits(	RBC_LCD_GPIOX,RBC_LCD_SCK)
#define SID(a)	if (a)	\
					GPIO_SetBits(		RBC_LCD_GPIOX,RBC_LCD_SDA);\
					else		\
					GPIO_ResetBits(	RBC_LCD_GPIOX,RBC_LCD_SDA)


void LCD_GPIO_Config(void);
void ROC_LCD_Init(void);
void Send_Command(unsigned char command);
void Send_data(unsigned char data);
void Clear_LCD(void);
void LCD_set_XY(unsigned char X, unsigned char Y);
void LCD_write_char(unsigned char c);
void LCD_Write_string(unsigned char *table,unsigned char X, unsigned char Y);
void LCD_Display_float(float Data,unsigned char X, unsigned char Y);
void LCD_Display_int(signed int Data,unsigned char X, unsigned char Y);
void float2str(float data,unsigned char*s);
void int2str(signed int data,unsigned char*s);					

					
#endif					
					
					
	


