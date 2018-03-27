#ifndef	_UNDER_PAN_H
#define _UNDER_PAN_H

#include "motor_control.h"
#define TIM_Piece 20	//����ʱ��Ƭ---20ms
typedef struct
{
	int MOTOR_SPEED_X;
	int MOTOR_SPEED_Y;
}MOTOR_SPEED_STRUCT;

#define speed25_max  330
#define speed25_min  -330 

#define WHEEL_SPEED_MAX		(500)
#define WHEEL_SPEED_MIN		(-WHEEL_SPEED_MAX)

#define WHEEL_SET_ACCEL_MAX	500

#define WHEEL_SET_SPEED_MAX	500
#define WHEEL_SET_SPEED_MIN	(-WHEEL_SET_SPEED_MAX)

#define MOTOR_DELAY_US			delay_us		//��ʱ΢��
#define MOTOR_DELAY_MS			delay_ms		//��ʱ����

/*		�Թ̶����ٶ�	�̶��ٶ�����	*/
/*���̵��1*/
#define UNDER1_MOVE(move_speed,move_acc)		Motor_AccelSpeed(&UNDER1_MLCB,move_acc);\
												MOTOR_DELAY_US(200);\
												Motor_SetSpeed(&UNDER1_MLCB,move_speed);\
												MOTOR_DELAY_US(200)
																						
																						
																						
#define UNDER2_MOVE(move_speed,move_acc)		Motor_AccelSpeed(&UNDER2_MLCB,move_acc);\
												MOTOR_DELAY_US(200);\
												Motor_SetSpeed(&UNDER2_MLCB,move_speed);\
												MOTOR_DELAY_US(200)
																						
																						

#define UNDER3_MOVE(move_speed,move_acc)		Motor_AccelSpeed(&UNDER3_MLCB,move_acc);\
												MOTOR_DELAY_US(200);\
												Motor_SetSpeed(&UNDER3_MLCB,move_speed);\
												MOTOR_DELAY_US(200)
																																											
#define UNDER4_MOVE(move_speed,move_acc)		Motor_AccelSpeed(&UNDER4_MLCB,move_acc);\
												MOTOR_DELAY_US(200);\
												Motor_SetSpeed(&UNDER4_MLCB,move_speed);\
												MOTOR_DELAY_US(200)
												
#define UNDER5_MOVE(move_speed,move_acc)		Motor_AccelSpeed(&UNDER5_MLCB,move_acc);\
												MOTOR_DELAY_US(2000);\
												Motor_SetSpeed(&UNDER5_MLCB,move_speed);\
												MOTOR_DELAY_US(2000)												
#define UNDER6_MOVE(move_speed,move_acc)		Motor_AccelSpeed(&UNDER6_MLCB,move_acc);\
																						MOTOR_DELAY_MS(2);\
																						Motor_SetSpeed(&UNDER6_MLCB,move_speed);\
																						MOTOR_DELAY_MS(2)														
												
//#define UNDER1_MOVE(move_speed,move_acc)		Motor_SetSpeed(&UNDER1_MLCB,move_speed);\
//												MOTOR_DELAY_US(1000)
//																						
//																						
//																						
//#define UNDER2_MOVE(move_speed,move_acc)		Motor_SetSpeed(&UNDER2_MLCB,move_speed);\
//												MOTOR_DELAY_US(1000)
//																						
//																						

//#define UNDER3_MOVE(move_speed,move_acc)		Motor_SetSpeed(&UNDER3_MLCB,move_speed);\
//												MOTOR_DELAY_US(1000)
//																						
//																						

//#define UNDER4_MOVE(move_speed,move_acc)		Motor_SetSpeed(&UNDER4_MLCB,move_speed);\
//												MOTOR_DELAY_US(1000)																						
																						

/******************************************************************************************************/
/*									*	���㷨���� *																									*/
/******************************************************************************************************/


/***************************************************************************************************************/
/*										*	�������� *																											 */
/***************************************************************************************************************/
void fasheceshi(int32_t fashe_speed,int32_t fashe_acc, uint8_t direction);
void Underpan_Motor_Init(void);																										//���̳�ʼ��
void Move_XY_Speed(int32_t move_speed, int32_t move_acc, uint8_t direction);	//�Թ̶��ٶ�,�̶����ٶ���ʻ
void Move_XY_Speed_Angle(int32_t move_speed, int32_t move_acc, int32_t angle);//������ת����Ƕ�ƽ��
void Move_Z_Rotate_Speed(int32_t move_speed, int32_t move_acc, uint8_t direction);//������ת
void Move_XY_Freedom(int32_t move_speed_x, int32_t move_speed_y, int32_t move_acc);//���ⷽ��ƽ��
void Move_XY_Rotate_Freedom(int32_t move_speed_x, int32_t move_speed_y, int32_t rotate_angle, int32_t move_acc);
//void UNDER1_MOVE(int move_speed,int move_acc);
//void UNDER2_MOVE(int move_speed,int move_acc);
//void UNDER3_MOVE(int move_speed,int move_acc);
//void UNDER4_MOVE(int move_speed,int move_acc);


/*	���㷨	*/
uint8_t SIN_Control_Speed_ADD(int32_t now_speed_x, int32_t now_speed_y, int32_t target_speed_x, int32_t target_speed_y, int32_t accel_time);
uint8_t SIN_Control_Speed_DEC(int32_t now_speed_x, int32_t now_speed_y, int32_t target_speed_x, int32_t target_speed_y, int32_t accel_time, int32_t position);
MOTOR_SPEED_STRUCT Translation_Speed_Reverse(int32_t wheel1_speed, int32_t wheel2_speed,int32_t wheel3_speed,int32_t wheel4_speed );
uint8_t Pitch_Adjustment(float C_targetangle,int Accel_time,float P,float D,uint8_t direction);

#endif












