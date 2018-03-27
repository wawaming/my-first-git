#ifndef _MOTOR_CONTROL_H
#define _MOTOR_CONTROL_H

#include "includes.h"
#include "ml_order.h"

/**********************�������************************/
#define  RE40_MaxSpeed   300   
#define  RE35_MaxSpeed   250
#define  RE25_MaxSpeed   500
/******���̵��********/
//---���1	
#define DEVICE1_ID	10
#define RATIO1			16
#define CODER1			1000
//---���2	
#define DEVICE2_ID	17
#define RATIO2			16
#define CODER2			1000
//---���3	
#define DEVICE3_ID	11
#define RATIO3			16
#define CODER3			1000
//---���4	
#define DEVICE4_ID	16
#define RATIO4			16
#define CODER4			1000

/*******���˵��***********/
//---���5	(��)
#define DEVICE5_ID	1
#define RATIO5			14
#define CODER5			1000
//---���6	
#define DEVICE6_ID	0//�Ѹ�Ϊ35���
#define RATIO6			15
#define CODER6			1000

/*******�������***********/
//---���7	
#define DEVICE7_ID	56
#define RATIO7			16
#define CODER7			1000

/*******�������***********/
//---���8	
#define DEVICE8_ID	35
#define RATIO8			19
#define CODER8			1000



typedef enum
{
	Write_Success=0,
}SERVO_ERROR;

/********�����ŷ�������ƿ�********/
typedef struct
{
	char		   *Servo_Mlcb_Name;			//�ŷ����ƿ�����
	uint8_t     Star_Id;							//��ʶ��ID
	uint8_t     Device_Id;						//�豸ID
	uint8_t     Ratio;								//���������
	uint16_t    Coder;								//����������
	SERVO_ERROR Servo_Error;					//�������
}SERVO_MLCB;

typedef struct
{
	unsigned Servo_ID:7;
	unsigned Data_Len:4;
	unsigned Data_Format:2;
	unsigned Data_Type:2;
	uint8_t  Front_Data[4];
	int32_t  Last_Data;
}Servo_Struct;

uint8_t Servo_Set_Function(Servo_Struct *Servo_Structure);
uint8_t Servo_Param_Set( uint8_t servo_id,uint8_t order,uint8_t len,int32_t data);


void Create_MLServo_CB(SERVO_MLCB		*servo_mlcb,					//�ŷ����ƿ�
											 char       	*servo_mlcb_name,			//�ŷ����ƿ�����
											 uint8_t     	 star_id,							//��ʶ��ID
											 uint8_t       ratio,								//���������
											 uint16_t      coder);							//����������

uint8_t MLServo_Write(SERVO_MLCB *Servo_MLCB, uint8_t order, uint8_t len, int32_t data);

/*****************************************��ؼ�Ҫ���*********************************************/
/*	����ٶ����	*/
uint8_t Motor_MastSpeed(SERVO_MLCB *Servo_MLCB,int32_t data);	//	�������ٶ�����
uint8_t Motor_MinSpeed(SERVO_MLCB *Servo_MLCB,int32_t data);	//	�����С�ٶ�����
uint8_t Motor_SetSpeed(SERVO_MLCB *Servo_MLCB,int32_t data);	//	��������ٶ�����

/*	���ٶ�����	*/
uint8_t Motor_AccelSpeed(SERVO_MLCB *Servo_MLCB,int32_t data);	//���õ�����ٶ�

/*	���λ������	*/
uint8_t Motor_AbsolutePlace(SERVO_MLCB *Servo_MLCB,int32_t data);	//���õ����ǰλ��Ϊ����λ��
uint8_t Motor_ToAbsolutePlace(SERVO_MLCB *Servo_MLCB,int32_t data);	//���õ�������λ�ÿ�ʼ����
uint8_t Motor_ToRelativePlace(SERVO_MLCB *Servo_MLCB,int32_t data);	//���õ��������ϴ�����λ�ÿ�ʼ����

/*	�����ʼ������	*/
uint8_t Motor_P_SET(SERVO_MLCB *Servo_MLCB,int32_t data);	//���λ�ñ���P����
uint8_t Motor_Code_Set(SERVO_MLCB *Servo_MLCB,int32_t data);	//����������ֱ�������
uint8_t Motor_Speed_Mode(SERVO_MLCB *Servo_MLCB);						//���õ��Ϊ�ٶ�ģʽ
uint8_t Motor_Position_Mode(SERVO_MLCB *Servo_MLCB);					//���õ��Ϊλ��(�Ƕ�)ģʽ
uint8_t Motor_Save_Param(SERVO_MLCB *Servo_MLCB);					//�������ϲ���

#endif




