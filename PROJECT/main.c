///////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************
*											STM32F407ZG工程模板——带UCOSIII操作系统
*											此工程模板类的驱动只适应STM32F4XX系统板
*	ucosiii为UCOSII的升级版，中断和任务管理更具有优势，用于嵌入式控制效果更加
*	增加了时间片轮转调度来执行任务的方式
* 任务模块放入APP分组中
*	外设模块放入MYBSP分组中
*	传感器模块放入DRIVE分组中
* 中断入口函数放入stm32f10x_it.c中
---------------------调试模块方法-----------------------
		1、使用非操作系统下进行调试，验证模块功能（主要是验证简单逻辑和驱动程序之类的功能）
		2、使用操作系统下进行调试，进行模块的最终测试
*@author：	yun
*@version： v1.0
*@data：		2017/3/3
******************************************************************************************/
/////////////////////////////////////////////////////////////////////////////
#include "includes.h" 
#include "drive.h"
#include "app.h"

//-------Start_task
OS_TCB StarTaskTCB;									//任务控制块
CPU_STK STAR_TASK_STK[STAR_STK_SIZE];	
	
/********************************选择调试模式**********************************/
#define DEBUG_MODE_UCOSIII		ON

/*
************************************代码区****************************************
*/
#if	DEBUG_MODE_UCOSIII

/*
///////////////////////////////////////////////////////////////////////////////////////////////
**********************************************************************************************
														*				UCOSIII调试模式				*
**********************************************************************************************
///////////////////////////////////////////////////////////////////////////////////////////////
*/
int main(void)
{
	
	/****************** Operation System Initialization ***************/
	OS_ERR err;
	CPU_SR_ALLOC();			//保存CPU状态,将值放入寄存器中,此函数在栈顶定义了一个变量cpu_sr,堆栈需要,否则会报警
	OSInit(&err);				//初始化UCOSIII
 
	/******************	Operation Sy9888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888stem Task **************/
	OS_CRITICAL_ENTER();		//进入临界区
	
	/****************** Persional Init	Code **************/
	BSP_Init();					//初始化各外设

	OSTaskCreate ((OS_TCB    *)   &StarTaskTCB,			//任务控制块
								(CPU_CHAR  *)   "star_task",			//任务名称,该名字能被调试器显示出来
								(OS_TASK_PTR)    star_task,				//任务函数
								(void      *)    0,								//传入函数的参数
								(OS_PRIO    )    STAR_TASK_PRIO,	//任务的优先级
								(CPU_STK   *)   &STAR_TASK_STK[0],//任务堆栈基地址
								(CPU_STK_SIZE)   STAR_STK_SIZE/10,//任务堆栈深度限制,用于堆栈溢出检测
								(CPU_STK_SIZE)   STAR_STK_SIZE,		//任务堆栈大小
								(OS_MSG_QTY  )   0,								//任务内部消息队列接收的最大消息数目,0则是禁止消息队列
								(OS_TICK     )   0,								//使能时间片轮转调度时设定当前任务的时间片长度,0为默认长度
								(void       *)   0,								//用户补充的存储区,一般为一个数据结构,用来扩展任务的TCB,如:放浮点运算内容
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //包含任务的特定选项：检测该任务的堆栈|需要堆栈清零
								(OS_ERR     *)  &err);							//指定一个变量存储错误码
	OS_CRITICAL_EXIT();			//退出临界区
	
	/****************** Open UCOSIII System ****************/
	OSStart(&err);  
	/****************** test *******************************/
	return 0;
}

#else
/*
///////////////////////////////////////////////////////////////////////////////////////////////
**********************************************************************************************
														*				非UCOSIII调试模式				*
					此模式不是完全脱离了操作系统，其中delay函数中加入了系统任务调度函数
**********************************************************************************************
///////////////////////////////////////////////////////////////////////////////////////////////
*/
int main(void)
{
	
	/****************** Persional Init	Code **************/
		BSP_Init();					//初始化各外设
		while(1)
		{
			LED0(1);LED1(0);
			delay_ms(100);
			BEED_ON;
			LED0(0);LED1(1);
			delay_ms(100);
			BEED_OFF;
		}
		return	0;
}
#endif










