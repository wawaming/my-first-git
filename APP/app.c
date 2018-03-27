/////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************
*										Persional Task
*		以下优先级已分给系统任务,不能使用
*		优先级0：中断服务服务管理任务 		OS_IntQTask()
		优先级1：时钟节拍任务 				OS_TickTask()
		优先级2：定时任务 					OS_TmrTask()
		优先级OS_CFG_PRIO_MAX-2：	统计任务 OS_StatTask()
		优先级OS_CFG_PRIO_MAX-1：	空闲任务 OS_IdleTask()
		
*		临界区代码编写
		CPU_SR_ALLOC();
		OS_CRITICAL_ENTER();
		//	用户需要保护的代码	//
		OS_CRITICAL_EXIT();

////////////////////////////////////////////////////////////////////////////////////////////
*	创建信号量方法：
	----------------定义信号变量----------------
	OS_SEM	Handle_Sem;			
	----------------创建信号量------------------
	OSSemCreate(	(OS_SEM		*)&Handle_Sem,				//需要创建的信号量(需要先声明)
								(CPU_CHAR	*)"Handle_Sem",				//信号量的名字
								(OS_SEM_CTR)0,									//信号量的初始值
								(OS_ERR   *)&HandleControlTaskErr);		//错误返回代码
	----------------申请信号量----------------
	OSSemPend(	(OS_SEM	*)&Handle_Sem,						//申请的信号量
							(OS_TICK )0,											//等待时间,0为无限等待,其他为等待的确切时钟节拍数
							(OS_OPT  )OS_OPT_PEND_BLOCKING,		//若没申请到信号量,则任务被挂起
							(CPU_TS *)0,											//获得时间戳
							(OS_ERR *)&HandleControlTaskErr);	//错误代码
	----------------释放信号量----------------
	OSSemPost(	(OS_SEM	*)&Handle_Sem,					//需要释放的信号量
							(OS_OPT	 )OS_OPT_POST_1,		//释放给等待此信号量的任务中具有最高优先级者
							(OS_ERR	*)&err);						//错误代码返回
	信号量经过以上四个步骤即可进行简单使用，如果需要高级用法，则需用户自行查找资料即可
////////////////////////////////////////////////////////////////////////////////////////////
*	任务挂起和解挂操作：
	----------------挂起任务-----------------
	OS_TaskSuspend((OS_TCB*)&MPU6050TaskTCB, &MPUTaskErr);		
	----------------解挂任务-----------------
	OS_TaskResume((OS_TCB*)&MPU6050TaskTCB, &MPUTaskErr);
	被挂起的任务只能通过这个函数进行解挂操作
////////////////////////////////////////////////////////////////////////////////////////////

*	@attention		请调试人员注意ReadMe.text文件	保存在APP文件夹中

*	@supplement：	优先级/栈空间大小/函数申明在app.h完成
*	@author:	yun
*	@version 	V2.0
*	@data:		2016/12/7
*************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "app.h"
#include "throw.h"
///////////////////////////////////////////////////////////////////////////////////////
/************************************ 任务创建所需 ***********************************/
/*				任务控制块声明							任务堆栈空间申请			 */
///////////////////////////////////////////////////////////////////////////////////////
//-----star_task
extern OS_TCB StarTaskTCB;			

//-----led_task
OS_TCB LedTaskTCB;					
CPU_STK LED_TASK_STK[LED_STK_SIZE];	

//-----throw_task
OS_TCB throwTaskTCB;					
CPU_STK throw_TASK_STK[throw_STK_SIZE];

//-----LCD_display
OS_TCB	LCDTaskTCB;
CPU_STK LCD_TASK_STK[LCD_STK_SIZE];

//-----KEY_display
OS_TCB	KEYTaskTCB;
CPU_STK KEY_TASK_STK[KEY_STK_SIZE];

//-----LINE_WALKING
OS_TCB	LineWalkTaskTCB;
CPU_STK LineWalk_TASK_STK[LineWalk_STK_SIZE];

//-----Handle_Control
OS_TCB	HandleControlTaskTCB;
CPU_STK HandleControl_TASK_STK[HandleControl_STK_SIZE];

//-----Offset_Control
OS_TCB	OffsetControlTaskTCB;
CPU_STK OffsetControl_TASK_STK[OffsetControl_STK_SIZE];
///////////////////////////////////////////////////////////////////////////////////////
/*									ERROR											 */
/********************************错误代码返回数组*************************************/
/******************************请用户根据任务自行添加*********************************/
/*****************************用于检查各任务的错误原因********************************/
///////////////////////////////////////////////////////////////////////////////////////
OS_ERR	StarTaskErr = OS_ERR_NONE;			/*	开始任务	OS_ERR_NONE为初始无错误	*/
OS_ERR	LedTaskErr  = OS_ERR_NONE;			/*	闪灯任务	*/
OS_ERR	throwTaskErr  = OS_ERR_NONE;    /*  抛掷任务  */
OS_ERR	LCDTaskErr	= OS_ERR_NONE;			/*	LCD任务		*/
OS_ERR	KeyTaskErr	= OS_ERR_NONE;			/*	KEY任务		*/
OS_ERR	LineWalkTaskErr	 = OS_ERR_NONE;		/*	LineWalk任务*/
OS_ERR	HandleControlTaskErr	 = OS_ERR_NONE;	/*	手柄遥控任务	*/
OS_ERR	OffsetControlTaskErr	 = OS_ERR_NONE;	/*	偏差采集任务	灯板	摄像头*/
/*	错误返回地址	*/

/*	任务程序运行判断数组	需要此调试辅助时	请用LCD或串口等获取此数组信息	*/
/*	此数组显示任务正在运行	以检查任务是否死掉	*/
/*	任务数目改变时	需要改变数组大小	*/
uint8_t	TASK_RUNNING[TASK_COUNT] = {0};

///////////////////////////////////////////////////////////////////////////////////////
/**********************************全局变量申明***************************************/
///////////////////////////////////////////////////////////////////////////////////////
/*ENCODER示数*/
volatile int32_t Encoder1_Count=0;
volatile int32_t Encoder2_Count=0;
volatile int32_t Encoder_TIM2=0;	//65536*示数+TIMx->CNT，即溢出标志
volatile int32_t Encoder_TIM4=0;
volatile int32_t Encoder_TIM8=0;
/*电机控制块*/
extern SERVO_MLCB UNDER1_MLCB;
extern SERVO_MLCB UNDER2_MLCB;
extern SERVO_MLCB UNDER3_MLCB;
extern SERVO_MLCB UNDER4_MLCB;
extern SERVO_MLCB UNDER5_MLCB;
extern SERVO_MLCB UNDER6_MLCB;

/*	底盘电机速度反馈	*/
int Under_Motor1[2] = {0};
int Under_Motor2[2] = {0};
int Under_Motor3[2] = {0};
int Under_Motor4[2] = {0};

/*	测试开始标志	*/
uint8_t ROBOCON_Start=0;

/*	手柄数据	*/
HandlesStruct UART_CH;
uint8_t send_flag = 0;
uint8_t Line_Walk_Flag = 0;
int debug_symbol = 1;
int fire_throw=0;
Robocon_Handle_Struct Rb_Handle_Control = {0XFF, 0XFE, 0XFD, 0XF2};
Robocon_Handle_Struct tab[31];
uint8_t Robocon_Handle_Flag = 0;

/*	摄像头模块	*/
int16_t Camera_Offset = 1;	//摄像头偏差

/*	巡线模块	*/
uint8_t Line_Walk_Start = 0;
uint8_t Line_Speed_val = 0;
float	Line_Angle = 0;
uint8_t Accel_Line_Walk_Flag = 0;
uint8_t	Angle_Line_Walk_Flag = 0;
float Adjust_P = 8;
float Adjust_Time = 2;
									
/*	测试部分计数	*/
uint8_t Error_Count = 0;
int Init_Time = 0;
int End_Time = 0;
int Direction = 1;
extern int a;
/*	全场定位程序测试算法速度变量	*/
int Time_5S;

/*	东大码盘接收数据	*/
uint8_t Locate_Alldate[24] = {0};


/*   发射模块		*/
uint8_t flag_off =0;
uint8_t flag_line =0;

int place;
extern uint16_t  safeplace_345;					//
extern uint16_t Launchplace;					//仿生 编码器触发释放位置

//摩擦
extern int32_t  bru_speed;			//无刷电机自转速度	-编码器
extern float  rotate_speed;	  //飞盘自转速度      -光电对管
uint16_t moc_speed_3m = 1200;
uint16_t moc_speed_6m = 1300;
uint16_t speed_3m = 2000;
uint16_t speed_6m = 3000;
/*仿生*/
int Launchplace_3m1 = 800;
int Launchplace_6m2 = 850;
int Launchplace_6m3 = 900;
int Launchplace_6m4 = 950;
int Launchplace_6m5 = 900;
int Launchplace_6m6 = 850;
int Launchplace_9m7 = 1000;

/*俯仰角*/
extern int speed;
extern float C_angleoffset;
extern float C_nowangle;//俯仰机构当前角度
extern int16_t C_differedge;
extern int16_t C_nowedge;//俯仰机构当前边长
extern int16_t C_targetedge;
extern int speed_adjust;
///////////////////////////////////////////////////////////////////////////////////////
/**********************************信号量申明*****************************************/
///////////////////////////////////////////////////////////////////////////////////////
OS_SEM	Handle_Sem;
OS_SEM	LineWalk_task_Sem1;
OS_SEM	LineWalk_task_Sem2;

///////////////////////////////////////////////////////////////////////////////////////
/******************************** 任务函数创建 ***************************************/
///////////////////////////////////////////////////////////////////////////////////////

/**
	*@brief	star_task
	*/ 
void star_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	CPU_Init();
	p_arg = p_arg;//防止编译器报警
	if(TASK_RUNNING[0]++,TASK_RUNNING[0]>9)		TASK_RUNNING[0] = 0;		//程序运行记录	
	
	OS_CRITICAL_ENTER();	//进入临界区
	OSTaskCreate ((OS_TCB*     )  &LedTaskTCB,						//任务控制块
								(CPU_CHAR   *)  "led_task",			//任务名称,该名字能被调试器显示出来
								(OS_TASK_PTR )   led_task,			//任务函数
								(void       *)   0,					//传入函数的参数
								(OS_PRIO     )   LED_TASK_PRIO,		//任务的优先级
								(CPU_STK    *)   &LED_TASK_STK[0],	//任务堆栈基地址
								(CPU_STK_SIZE)   LED_STK_SIZE/10,	//任务堆栈深度限制,用于堆栈溢出检测
								(CPU_STK_SIZE)   LED_STK_SIZE,		//任务堆栈大小
								(OS_MSG_QTY  )   0,					//任务内部消息队列接收的最大消息数目,0则是禁止消息队列
								(OS_TICK     )   0,					//使能时间片轮转调度时设定当前任务的时间片长度,0为默认长度
								(void       *)   0,					//用户补充的存储区,一般为一个数据结构,用来扩展任务的TCB,如:放浮点运算内容
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //包含任务的特定选项：检测该任务的堆栈|需要堆栈清零
								(OS_ERR     *)  &err);				//指定一个变量存储错误码
								
					
	OSTaskCreate ((OS_TCB*     )  &throwTaskTCB,						//任务控制块
								(CPU_CHAR   *)  "throw_task",			//任务名称,该名字能被调试器显示出来
								(OS_TASK_PTR )   throw_task,			//任务函数
								(void       *)   0,					//传入函数的参数
								(OS_PRIO     )   throw_TASK_PRIO,		//任务的优先级
								(CPU_STK    *)   &throw_TASK_STK[0],	//任务堆栈基地址
								(CPU_STK_SIZE)   throw_STK_SIZE/10,	//任务堆栈深度限制,用于堆栈溢出检测
								(CPU_STK_SIZE)   throw_STK_SIZE,		//任务堆栈大小
								(OS_MSG_QTY  )   0,					//任务内部消息队列接收的最大消息数目,0则是禁止消息队列
								(OS_TICK     )   0,					//使能时间片轮转调度时设定当前任务的时间片长度,0为默认长度
								(void       *)   0,					//用户补充的存储区,一般为一个数据结构,用来扩展任务的TCB,如:放浮点运算内容
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //包含任务的特定选项：检测该任务的堆栈|需要堆栈清零
								(OS_ERR     *)  &err);				//指定一个变量存储错误码
								
	OSTaskCreate ((OS_TCB*     )  &LCDTaskTCB,						//任务控制块
								(CPU_CHAR   *)  "LCD_task",			//任务名称,该名字能被调试器显示出来
								(OS_TASK_PTR )   LCD_task,			//任务函数
								(void       *)   0,					//传入函数的参数
								(OS_PRIO     )   LCD_TASK_PRIO,		//任务的优先级
								(CPU_STK    *)   &LCD_TASK_STK[0],	//任务堆栈基地址
								(CPU_STK_SIZE)   LCD_STK_SIZE/10,	//任务堆栈深度限制,用于堆栈溢出检测
								(CPU_STK_SIZE)   LCD_STK_SIZE,		//任务堆栈大小
								(OS_MSG_QTY  )   0,					//任务内部消息队列接收的最大消息数目,0则是禁止消息队列
								(OS_TICK     )   0,					//使能时间片轮转调度时设定当前任务的时间片长度,0为默认长度
								(void       *)   0,					//用户补充的存储区,一般为一个数据结构,用来扩展任务的TCB,如:放浮点运算内容
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //包含任务的特定选项：检测该任务的堆栈|需要堆栈清零
								(OS_ERR     *)  &err);				//指定一个变量存储错误码				

	OSTaskCreate ((OS_TCB*     )  &KEYTaskTCB,						//任务控制块
								(CPU_CHAR   *)  "KEY_task",			//任务名称,该名字能被调试器显示出来
								(OS_TASK_PTR )   KEY_task,			//任务函数
								(void       *)   0,					//传入函数的参数
								(OS_PRIO     )   KEY_TASK_PRIO,		//任务的优先级
								(CPU_STK    *)   &KEY_TASK_STK[0],	//任务堆栈基地址
								(CPU_STK_SIZE)   KEY_STK_SIZE/10,	//任务堆栈深度限制,用于堆栈溢出检测
								(CPU_STK_SIZE)   KEY_STK_SIZE,		//任务堆栈大小
								(OS_MSG_QTY  )   0,					//任务内部消息队列接收的最大消息数目,0则是禁止消息队列
								(OS_TICK     )   0,					//使能时间片轮转调度时设定当前任务的时间片长度,0为默认长度
								(void       *)   0,					//用户补充的存储区,一般为一个数据结构,用来扩展任务的TCB,如:放浮点运算内容
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //包含任务的特定选项：检测该任务的堆栈|需要堆栈清零
								(OS_ERR     *)  &err);				//指定一个变量存储错误码

	OSTaskCreate ((OS_TCB*     )  &LineWalkTaskTCB,						//任务控制块
								(CPU_CHAR   *)  "LineWalk_task",		//任务名称,该名字能被调试器显示出来
								(OS_TASK_PTR )   LineWalk_task,			//任务函数
								(void       *)   0,						//传入函数的参数
								(OS_PRIO     )   LineWalk_TASK_PRIO,	//任务的优先级
								(CPU_STK    *)   &LineWalk_TASK_STK[0],	//任务堆栈基地址
								(CPU_STK_SIZE)   LineWalk_STK_SIZE/10,	//任务堆栈深度限制,用于堆栈溢出检测
								(CPU_STK_SIZE)   LineWalk_STK_SIZE,		//任务堆栈大小
								(OS_MSG_QTY  )   0,						//任务内部消息队列接收的最大消息数目,0则是禁止消息队列
								(OS_TICK     )   0,						//使能时间片轮转调度时设定当前任务的时间片长度,0为默认长度
								(void       *)   0,						//用户补充的存储区,一般为一个数据结构,用来扩展任务的TCB,如:放浮点运算内容
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //包含任务的特定选项：检测该任务的堆栈|需要堆栈清零
								(OS_ERR     *)  &err);					//指定一个变量存储错误码				

	OSTaskCreate ((OS_TCB*     )  &HandleControlTaskTCB,					//任务控制块
								(CPU_CHAR   *)  "HandleControl_task",		//任务名称,该名字能被调试器显示出来
								(OS_TASK_PTR )   HandleControl_task,		//任务函数
								(void       *)   0,							//传入函数的参数
								(OS_PRIO     )   HandleControl_TASK_PRIO,	//任务的优先级
								(CPU_STK    *)   &HandleControl_TASK_STK[0],//任务堆栈基地址
								(CPU_STK_SIZE)   HandleControl_STK_SIZE/10,	//任务堆栈深度限制,用于堆栈溢出检测
								(CPU_STK_SIZE)   HandleControl_STK_SIZE,	//任务堆栈大小
								(OS_MSG_QTY  )   0,							//任务内部消息队列接收的最大消息数目,0则是禁止消息队列
								(OS_TICK     )   0,							//使能时间片轮转调度时设定当前任务的时间片长度,0为默认长度
								(void       *)   0,							//用户补充的存储区,一般为一个数据结构,用来扩展任务的TCB,如:放浮点运算内容
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //包含任务的特定选项：检测该任务的堆栈|需要堆栈清零
								(OS_ERR     *)  &err);						//指定一个变量存储错误码		
								
	OSTaskCreate ((OS_TCB*     )  &OffsetControlTaskTCB,					//任务控制块
								(CPU_CHAR   *)  "OffsetControl_task",		//任务名称,该名字能被调试器显示出来
								(OS_TASK_PTR )   OffsetControl_task,		//任务函数
								(void       *)   0,							//传入函数的参数
								(OS_PRIO     )   OffsetControl_TASK_PRIO,	//任务的优先级
								(CPU_STK    *)   &OffsetControl_TASK_STK[0],//任务堆栈基地址
								(CPU_STK_SIZE)   OffsetControl_STK_SIZE/10,	//任务堆栈深度限制,用于堆栈溢出检测
								(CPU_STK_SIZE)   OffsetControl_STK_SIZE,	//任务堆栈大小
								(OS_MSG_QTY  )   0,							//任务内部消息队列接收的最大消息数目,0则是禁止消息队列
								(OS_TICK     )   0,							//使能时间片轮转调度时设定当前任务的时间片长度,0为默认长度
								(void       *)   0,							//用户补充的存储区,一般为一个数据结构,用来扩展任务的TCB,如:放浮点运算内容
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //包含任务的特定选项：检测该任务的堆栈|需要堆栈清零
								(OS_ERR     *)  &err);						//指定一个变量存储错误码			

		LED0(0);LED1(1);delay_ms(500);
		LED0(0);LED1(1);delay_ms(500);							
	OS_CRITICAL_EXIT();		//退出临界区
	OS_TaskSuspend((OS_TCB*)&StarTaskTCB,&err);		//挂起开始任务	
}

/*********************************************************************************************************************
 **
	*	@brief		led_task
	*	@describe	闪灯程序,辅助系统调试
	*
**********************************************************************************************************************/
extern int throw_flag;
extern int v_flag;
extern int speed_real;
//extern  int32_t Encoder_TIM2;	//通过TIM2编码器模式捕捉的计数
void led_task(void *p_arg)
{
	CPU_SR_ALLOC();
	p_arg = p_arg;
//	BEED_ON;delay_ms(500);BEED_OFF;delay_ms(500);
//	BEED_ON;delay_ms(500);BEED_OFF;delay_ms(500);										
	while(1)
	{
	//	OS_CRITICAL_ENTER();		//临界区代码
		if(fire_throw==1)
	{
		 Motor_SetSpeed(&UNDER6_MLCB,throw_speed);
		if(v_flag==0)TIM_Cmd(TIM3, ENABLE);
		if(throw_flag==1){
			fire_throw=0;
			TIM_Cmd(TIM7, ENABLE);
		//  BEED_ON;delay_ms(500);BEED_OFF;delay_ms(500);
		}
	}
	//	OS_CRITICAL_EXIT();
	  delay_ms(10);
	
	}

}


/*********************************************************************************************************************
 **
	*	@brief		throw_task
	*	@describe	抛掷任务，检测抛掷点
	*
**********************************************************************************************************************/
//extern  int32_t Encoder_TIM2;	//通过TIM2编码器模式捕捉的计数 
int throw_flag=0;  //抛掷标志位
int init_distance=0; //抛掷点脉冲记录

void throw_task(void *p_arg)
{
	int hole_circle=0;
	CPU_SR_ALLOC();	
	p_arg = p_arg;		
	OS_TaskSuspend((OS_TCB*)&throwTaskTCB,&throwTaskErr);
	 while(1)
	{	
  OS_CRITICAL_ENTER();		//临界区代码
	hole_circle=Encoder_TIM2*65536+TIM2->CNT+60678*3;
	while((Encoder_TIM2*65536+TIM2->CNT)!=hole_circle){}
	while(((( Encoder_TIM2*65536+TIM2->CNT)%60678)<=(throw_point-3))||((( Encoder_TIM2*65536+TIM2->CNT)%60678)>=(throw_point+3))){}
  throw_flag=1;
		RL1_ON;
		init_distance=Encoder_TIM2*65536+TIM2->CNT;hole_circle=0;
		BEED_ON;
		OS_CRITICAL_EXIT();
		OS_TaskSuspend((OS_TCB*)&throwTaskTCB,&throwTaskErr);
		 delay_ms(20);
	}	  		
  //
	

 
}

/*********************************************************************************************************************
 **
	*	@brief		KEY_task
	*	@describe KEY控制任务,辅助系统调试
	*
**********************************************************************************************************************/
extern int sped[2000];//速度数组，串口打印使用
void KEY_task(void *p_arg)
{
	uint16_t i=0;
	CPU_SR_ALLOC();   //使用临界区代码时需要
	p_arg = p_arg;//任务函数输入参数，此写法是防止编译器因为未使用变量而进行警告
//	OS_CRITICAL_ENTER();		//临界区代码
   int speed_real1=0;	
	while(1)
	{ 					
		if(TASK_RUNNING[2]++,TASK_RUNNING[2]>9)		TASK_RUNNING[2] = 0;		//程序运行记录

		switch(Key_Scan())
		{
			case KEY1:	{

				fire_throw=1;
			}
						break;
			
			case KEY2:	{

				fire_throw=0;	speed_real1=speed_real; 
				                   while(speed_real1)
                              {
																 
				                       if(speed_real1>0){speed_real1-=5;}
			                          else{speed_real1+=5;}
				                        UNDER6_MOVE(speed_real1,30);
																if(speed_real1<=6)UNDER6_MOVE(0,30);
																delay_ms(200);
			                        }	break;
					}
			case KEY3:	{

				UNDER6_MOVE(20,20);
				delay_ms(100);
				while(((Encoder_TIM2*65536 + TIM2->CNT)%60660)>1){}
					Motor_SetSpeed(&UNDER6_MLCB,0);
						break;
					}
			case KEY4:	{
                      RL1_OFF;
//					for(i=0;i<2000;i++)
//				{
////					USART_SendData(UART4,(uint8_t)(48 + (uint8_t)(sped[i]/100000)));
////			      delay_us(500);
////					sped[i] = sped[i]%100000;
////					 USART_SendData(UART4,(uint8_t)(48 + (uint8_t)(sped[i]/10000)));
////			      delay_us(500);
////					sped[i] = sped[i]%10000;
////					 USART_SendData(UART4,(uint8_t)(48 + (uint8_t)(sped[i]/1000)));
////			      delay_us(500);
////					sped[i] = sped[i]%1000;
//	          USART_SendData(UART4,(uint8_t)(48 + (uint8_t)(sped[i]/100)));
//			      delay_us(500);
//		      	sped[i] = sped[i]%100;
//		      	USART_SendData(UART4,(uint8_t)(48 + (uint8_t)(sped[i]/10)));
//		      	delay_us(500);
//		     	  sped[i] = sped[i]%10;
//			      USART_SendData(UART4,(uint8_t)(48 + (uint8_t)(sped[i])));
//			      delay_us(500);		
//            USART_SendData(UART4,10);
//		      	delay_us(500);							
//				}
						break;
					}
			case KEY5:	{

					RL1_ON;
				
			}break;	
			
			default:	break;
		}
			

		LED2(1);LED3(0);delay_ms(100);	/*闪灯*/
		LED2(0);LED3(1);delay_ms(100);	
	}
//	OS_CRITICAL_EXIT();
}

/*********************************************************************************************************************
 **
	*	@brief		LCD_task
	*	@describe LCD显示任务,辅助系统调试
	*
**********************************************************************************************************************/
uint8_t CAN_RE[8] = {0};
extern uint16_t number_DI;
uint16_t flag_exti =0;
int exti_flag15 = 0;
int exti_flag8 = 0;
int exti_flag2 = 0;
int exti_flag13 = 0;
extern int Encoder;
extern int lat_Encoder;
extern int throw_flag;
extern int v_flag;
extern int V_to_set;
extern int control_num;
extern int get_finaldis;
extern int hole_circle;//z相电机看一圈多少脉冲
extern int circle1;
extern int asd;
int speeD,last_speeD=0,acce,target_speed,target_acce;
void LCD_task(void *p_arg)
{
	CPU_SR_ALLOC();
	p_arg = p_arg;
	Clear_LCD();
//	OS_CRITICAL_ENTER();		//临界区代码
//	OS_CRITICAL_EXIT();
	while(1)
	{
		if(TASK_RUNNING[3]++,TASK_RUNNING[3]>9)		TASK_RUNNING[3] = 0;		//程序运行记录		

		speeD=Encoder*200*60/(4000*15);
		acce=(speeD-last_speeD)/0.3;
		last_speeD=speeD;
		
    LCD_Write_string("lat_Encoder",1, 1);
		LCD_Display_int(Encoder_TIM2*65536 + TIM2->CNT,15,1);
		
		LCD_Write_string("init_distance",1, 2);
		LCD_Display_int(init_distance,15,2);
		
		LCD_Write_string("hole_circle",1, 3);
		LCD_Display_int(hole_circle,15,3);
		
		LCD_Write_string("speed_real",1, 4);
		LCD_Display_int(speed_real ,15,4);
			
		LCD_Write_string("get_finaldis",1, 5);
		LCD_Display_int(get_finaldis,15,5);
		
    LCD_Write_string("circle1",1, 6);
		LCD_Display_int(circle1 ,15,6);
//		LCD_Write_string("control_num",1, 6);
//		LCD_Display_int(control_num ,15,6);
//		//LCD_Display_int(asd ,15,6);
		
		delay_ms(100);
	}
}

/*********************************************************************************************************************
 **
	*	@brief		LineWalk_task
	*	@describe 用户自定义主线程任务
	*
**********************************************************************************************************************/
extern Robocon_Handle_Struct Rb_Handle_Control;
void LineWalk_task(void *p_arg)
{
	CPU_SR_ALLOC();
	while(1)
	{
		if(Robocon_Handle_Flag == 1){
		if(TASK_RUNNING[5]++,TASK_RUNNING[5]>9)		TASK_RUNNING[5] = 0;		//程序运行记录
		
//		OS_CRITICAL_ENTER();		//临界区代码
		  
		/*	用户代码	*/
			
		//按键调控
		
		switch(Rb_Handle_Control.KEY)									
		{
				case 0x00:				{
															//不做调控
								}break;
				
				case 0x01:				{
															Model_Aircraft_1 = moc_speed_3m;			//启动无刷电机  3m击盘
					                    Correspond_Receipt();
								}break;
				
				case 0x02:				{		
															Model_Aircraft_1 = moc_speed_6m;			//启动无刷电机  6m击盘
															Correspond_Receipt();
										}break;

				case 0x03:				{
															RL9_ON;																//装盘 
															delay_ms(1000);
															RL9_OFF;	
					                    Correspond_Receipt();
										}break;

				case 0x04:				{
														Model_Aircraft_1 = 1000;
					                  Correspond_Receipt();
										}break;

				case 0x05:				{
														Friction_Falling(moc_speed_3m,speed_3m);
										}break;			
				
				case 0x06:				{														
													  Friction_Falling(moc_speed_6m,speed_6m);
										}break;
				
				case 0x07:				{													
																moc_speed_3m += 5;
										}break;

				case 0x08:				{	
																moc_speed_3m -= 5;
										}break;

				case 0x09:				{
																moc_speed_6m += 5;
										}break;

				case 0x0A:				{													
																moc_speed_3m -= 5;
										}break;

				case 0x0B:				{
//                               Clear_Struct((char*)&tab[4],sizeof(tab));
										}break;

				case 0x0C:				{
											
										}break;

				case 0x0D:				{

										}break;

				case 0x0E:				{
									
										}break;

				case 0x0F:				{
												
										}break;		
				
				default :				{

										}break;		
			}					
		
		Rb_Handle_Control.KEY = 0;
		Robocon_Handle_Flag = 0;				//通信标志位置零 在收到主控指令前 不再执行任务内容
	  } 
			delay_ms(20); 
	}
}

/*********************************************************************************************************************
 **
	*	@brief		HandleControl_task
	*	@describe 手柄控制
	*
**********************************************************************************************************************/
void HandleControl_task(void *p_arg)
{
	int speed_real1=0;
	CPU_SR_ALLOC();
//	unsigned char str[] = "ACTR";
	
	OSSemCreate(	(OS_SEM		*)&Handle_Sem,				//需要创建的信号量(需要先声明)
								(CPU_CHAR	*)"Handle_Sem",				//信号量的名字
								(OS_SEM_CTR)0,									//信号量的初始值
								(OS_ERR   *)&HandleControlTaskErr);		//错误返回代码
	
	while(1)
	{
		OSSemPend(	(OS_SEM	*)&Handle_Sem,						//申请的信号量
								(OS_TICK )0,											//等待时间,0为无限等待,其他为等待的确切时钟节拍数
								(OS_OPT  )OS_OPT_PEND_BLOCKING,		//若没申请到信号量,则任务被挂起
								(CPU_TS *)0,											//获得时间戳
								(OS_ERR *)&HandleControlTaskErr);	//错误代码
		
		if(TASK_RUNNING[6]++,TASK_RUNNING[6]>9)		TASK_RUNNING[6] = 0;		//程序运行记录
		
		/*	用户代码	*/
		OS_CRITICAL_ENTER();		//临界区代码
		
		switch(UART_CH.KEY)
		{
 			case 0x01:				{
				                    fire_throw=1;

							}break;
			
			case 0x02:				{	fire_throw=0;	
														if(speed_real>=5||speed_real<-5)
                              {
																 speed_real1=speed_real;
				                       if(speed_real1>0){speed_real1-=5;}
			                          else{speed_real1+=5;}
				                        UNDER6_MOVE(speed_real1,30);
																delay_ms(200);
			                        }	
                             else {UNDER6_MOVE(0,10);break;	}													
									}

			case 0x03:				{
                    	      TIM_Cmd(TIM7, ENABLE);
									}break;

			case 0x04:				{
				                    target_acce+=5;
									}break;

			case 0x05:				{
				                    target_acce-=5;               
									}break;			
			
			case 0x06:				{
										        UNDER6_MOVE(target_speed,target_acce);
									}break;
			
			case 0x07:				{target_speed+=5;
                     
									}break;

			case 0x08:				{	target_speed-=5;
			                	
									}break;

			case 0x09:				{
							
									} break;

			case 0x0A:				{															
													
									}break;

			case 0x0B:				{
											
									}break;

			case 0x0C:				{
											
									}break;

			case 0x0D:				{
																							
									}break;

			case 0x0E:				{
												
									}break;

			case 0x0F:				{
																					
									}break;		
			case 0x10:				{
																						
									}break;
			case 0x11:				{
				                 																		
									}break;
			case 0x12:				{
																					
									}break;
			case 0x13:				{
											
									}break;
			case 0x14:				{
																
									}break;
			
			default :				{

									}break;
		}
		
		OS_CRITICAL_EXIT();
		
		delay_ms(100);
	}
}

/*********************************************************************************************************************
 **
	*	@brief		OffsetControl_task	---任务挂起模板
	*	@describe 用户自定义主线程任务
	*
**********************************************************************************************************************/
void OffsetControl_task(void *p_arg)
{
	CPU_SR_ALLOC();
	
	while(1)
	{
		if(TASK_RUNNING[7]++,TASK_RUNNING[7]>9)		TASK_RUNNING[7] = 0;		//程序运行记录
		OS_CRITICAL_ENTER();		//临界区代码
		
		/*	用户代码	*/	
		if(flag_off){
			
			RL1_OFF;			
//			while(((Encoder_TIM2*65536+TIM2->CNT)<= fasheplace_345)||(TIM2->CNT >=3000));		//待修改	
//			while(!((TIM2->CNT)>= Launchplace)||(TIM2->CNT >=0));
			while(place = TIM2->CNT,(place<= Launchplace)||(place >=3000));
			RL2_ON;		
			flag_off = 0;
 
		}			
		OS_CRITICAL_EXIT();			

		OS_TaskSuspend((OS_TCB*)&OffsetControlTaskTCB,&OffsetControlTaskErr);		//挂起自身	等待其他模块进行唤醒			
		
//		delay_ms(100);
	}
}








