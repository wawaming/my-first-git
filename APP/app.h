#ifndef _APP_H
#define _APP_H
#include "throw.h"
#include "BSP.h"
#include "simulate.h"
#include "shangweiji.h"
/************************* Task Information *******************************/
/*		if the Program running not properly	
			Please check the STK_SIZE
*/
/*		������		*/
#define TASK_COUNT	8		//�û�ÿ�����������,��Ҫ�ڴ˴��޸�������
/**
	*@brief			star_task				-----------------------------1	�ڼ�������	
	*@describe	
	*/
#define STAR_TASK_PRIO	20					//�������ȼ�
#define STAR_STK_SIZE		128					//�����ջ��С
void star_task(void *p_arg);				//����������


/**
	*@brief			led_task				------------------------------2
	*@describe	
	*/

#define LED_TASK_PRIO		8					//�������ȼ�
#define LED_STK_SIZE 		128					//�����ջ��С
void led_task(void *p_arg);					//����������

/*��������*/
#define throw_TASK_PRIO		5					//�������ȼ�
#define throw_STK_SIZE 		128					//�����ջ��С
void throw_task(void *p_arg);					//����������


/**
	*@brief			LCD_task				------------------------------3
	*@describe	
	*/
#define LCD_TASK_PRIO		17					//�������ȼ�
#define LCD_STK_SIZE		256					//�����ջ��С
void LCD_task(void *p_arg);					//����������


/**
	*@brief			KEY_task				------------------------------4
	*@describe	
	*/
#define KEY_TASK_PRIO		19					//�������ȼ�
#define KEY_STK_SIZE		128					//�����ջ��С
void KEY_task(void *p_arg);					//����������


/**
	*@brief			MPU6050_task		------------------------------5
	*@describe	
	*/
#define MPU6050_TASK_PRIO		11					//�������ȼ�
#define MPU6050_STK_SIZE		128					//�����ջ��С
void MPU6050_task(void *p_arg);					//����������


/**
	*@brief			LineWalk_task		------------------------------6
	*@describe	
	*/
#define LineWalk_TASK_PRIO		9					//�������ȼ�
#define LineWalk_STK_SIZE		128					//�����ջ��С
void LineWalk_task(void *p_arg);					//����������


/**
	*@brief			HandleControl_task		------------------------------7
	*@describe	
	*/
#define HandleControl_TASK_PRIO		7					//�������ȼ�
#define HandleControl_STK_SIZE		256				//�����ջ��С
void HandleControl_task(void *p_arg);					//����������



/**
	*@brief			OffsetControl_task		------------------------------8
	*@describe	
	*/
#define OffsetControl_TASK_PRIO		10					//�������ȼ�
#define OffsetControl_STK_SIZE		1024					//�����ջ��С
void OffsetControl_task(void *p_arg);					//����������

#endif






