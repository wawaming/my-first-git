#ifndef	_USART_H
#define _USART_H

#include "includes.h"




void Usart_Config(USART_TypeDef* USARTx, uint32_t bound);		//串口配置函数
void Correspond_Receipt();
void Communication_request();
int fputc(int ch,FILE *f);	//printf函数重定向


/****************************************************用户自定义模块************************************************/
/****************************************************用户自定义模块************************************************/
/****************************************************用户自定义模块************************************************/
/* 手柄控制	*/
typedef	struct
{
	uint8_t	KEY;
	uint8_t LEFT_RIGHT;
	uint8_t UP_DOWN;
	uint8_t	BEFORE_RIGHT;
	uint8_t finish_flag;
}HandlesStruct;

/* 发射模块数据接收	*/
typedef union
{
	u8  _u8_data[4];
	int _int_date;
}U8_INT_UNION;
typedef union
{
	u8  _u8_data[4];
	float _int_date;
}U8_FLOAT_UNION;


/*	机器人各模块通讯	*/
typedef struct
{
	uint8_t FH1;		//frame header
	uint8_t FH2;
	uint8_t FH3;
	uint8_t FH4;
	uint8_t KEY;
	U8_INT_UNION Locat_Coord_X;
	U8_INT_UNION Locat_Coord_Y;
	U8_FLOAT_UNION Locat_Coord_W;		//定位的角度数据
	U8_INT_UNION Rub_Speed;					//摩擦机构的速度
	U8_INT_UNION Simulate_Position;	//仿生机构的释放位置
	U8_INT_UNION Elevation_Position;//仿生机构的仰角调节
	uint8_t Receipt;//回执
	uint8_t FT;			//frame tail
}Robocon_Handle_Struct;



void A_Usart_Config(void);



#endif




