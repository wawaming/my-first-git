#ifndef _DELAY_H
#define _DELAY_H

#include "includes.h"

#define SYSTEM_SUPPORT_OS		1						//1,支持系统.  0,不支持系统


/*************** Enabled Sched or Disabled Sched of system task **************/
//void delay_OSSchedlock(void);				//任务调度上锁
//void delay_OSSchedunlock(void);			//任务调度解锁

//void delay_OSTimedly(u32 ticks);			//系统延时函数
//void SysTick_Handler(void);				//滴答定时器中断
/*************** user program	*******************************/
void delay_init(void);			//延时函数初始化
void delay_us(u32 nus);			//微秒级延时
void delay_ms(u16 nms);			//毫秒级延时
void delay_xms(u16 nms);		//延时任意毫秒
////以下为汇编函数
//void WFI_SET(void);		//执行WFI指令
//void INTX_DISABLE(void);	//关闭所有中断
//void INTX_ENABLE(void);	//开启所有中断
//void MSR_MSP(u32 addr);	//设置堆栈地址


/************************************************用户自定义函数定义********************************************************/
/************************************************用户自定义函数定义********************************************************/
/************************************************用户自定义函数定义********************************************************/

void delay(int n);

#endif




