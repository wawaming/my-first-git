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
/*		任务数		*/
#define TASK_COUNT	8		//用户每次添加新任务,需要在此处修改任务数
/**
	*@brief			star_task				-----------------------------1	第几个任务	
	*@describe	
	*/
#define STAR_TASK_PRIO	20					//任务优先级
#define STAR_STK_SIZE		128					//任务堆栈大小
void star_task(void *p_arg);				//任务函数声明


/**
	*@brief			led_task				------------------------------2
	*@describe	
	*/

#define LED_TASK_PRIO		8					//任务优先级
#define LED_STK_SIZE 		128					//任务堆栈大小
void led_task(void *p_arg);					//任务函数声明

/*抛掷任务*/
#define throw_TASK_PRIO		5					//任务优先级
#define throw_STK_SIZE 		128					//任务堆栈大小
void throw_task(void *p_arg);					//任务函数声明


/**
	*@brief			LCD_task				------------------------------3
	*@describe	
	*/
#define LCD_TASK_PRIO		17					//任务优先级
#define LCD_STK_SIZE		256					//任务堆栈大小
void LCD_task(void *p_arg);					//任务函数声明


/**
	*@brief			KEY_task				------------------------------4
	*@describe	
	*/
#define KEY_TASK_PRIO		19					//任务优先级
#define KEY_STK_SIZE		128					//任务堆栈大小
void KEY_task(void *p_arg);					//任务函数声明


/**
	*@brief			MPU6050_task		------------------------------5
	*@describe	
	*/
#define MPU6050_TASK_PRIO		11					//任务优先级
#define MPU6050_STK_SIZE		128					//任务堆栈大小
void MPU6050_task(void *p_arg);					//任务函数声明


/**
	*@brief			LineWalk_task		------------------------------6
	*@describe	
	*/
#define LineWalk_TASK_PRIO		9					//任务优先级
#define LineWalk_STK_SIZE		128					//任务堆栈大小
void LineWalk_task(void *p_arg);					//任务函数声明


/**
	*@brief			HandleControl_task		------------------------------7
	*@describe	
	*/
#define HandleControl_TASK_PRIO		7					//任务优先级
#define HandleControl_STK_SIZE		256				//任务堆栈大小
void HandleControl_task(void *p_arg);					//任务函数声明



/**
	*@brief			OffsetControl_task		------------------------------8
	*@describe	
	*/
#define OffsetControl_TASK_PRIO		10					//任务优先级
#define OffsetControl_STK_SIZE		1024					//任务堆栈大小
void OffsetControl_task(void *p_arg);					//任务函数声明

#endif






