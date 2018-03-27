#ifndef	_USART_H
#define _USART_H

#include "includes.h"




void Usart_Config(USART_TypeDef* USARTx, uint32_t bound);		//�������ú���
void Correspond_Receipt();
void Communication_request();
int fputc(int ch,FILE *f);	//printf�����ض���


/****************************************************�û��Զ���ģ��************************************************/
/****************************************************�û��Զ���ģ��************************************************/
/****************************************************�û��Զ���ģ��************************************************/
/* �ֱ�����	*/
typedef	struct
{
	uint8_t	KEY;
	uint8_t LEFT_RIGHT;
	uint8_t UP_DOWN;
	uint8_t	BEFORE_RIGHT;
	uint8_t finish_flag;
}HandlesStruct;

/* ����ģ�����ݽ���	*/
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


/*	�����˸�ģ��ͨѶ	*/
typedef struct
{
	uint8_t FH1;		//frame header
	uint8_t FH2;
	uint8_t FH3;
	uint8_t FH4;
	uint8_t KEY;
	U8_INT_UNION Locat_Coord_X;
	U8_INT_UNION Locat_Coord_Y;
	U8_FLOAT_UNION Locat_Coord_W;		//��λ�ĽǶ�����
	U8_INT_UNION Rub_Speed;					//Ħ���������ٶ�
	U8_INT_UNION Simulate_Position;	//�����������ͷ�λ��
	U8_INT_UNION Elevation_Position;//�������������ǵ���
	uint8_t Receipt;//��ִ
	uint8_t FT;			//frame tail
}Robocon_Handle_Struct;



void A_Usart_Config(void);



#endif




