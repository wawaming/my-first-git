/*************************************************************************************
*	@file			delay.c
* @author	 	yun
*	@supplement		#ifdef CPU_CFG_CRITICAL_METHOD	指支持USCOIII系统,否则支持UCOSII系统
					延时过程:
						滴答定时器以一定频率进行计数，以一定周期的计数触发中断，一次中断
						作为操作系统的时钟节拍，若再用滴答定时器进行延时中断，延时时间若
						时钟节拍的整数倍，会使延时时间不准确，也会造成时钟节拍的不准确(因
						为延时的中断会占用时间).
						
						延时不能使用中断，所以用截取滴答定时器计数值的方法来达到效果，需
						要延时时，读取当前滴答定时器的计数值，随后实时读取计数值，直到两
						者之差达到所需要的延时对应的计数值时，完成延时
						
						微秒级延时为了准确，需要关掉操作系统的任务调度，延时完成后开启
						
						滴答定时器是一个递减定时器
*	@version 	V1.0
*	@date			2016/5/27
* @brief		NONE
*************************************************************************************/

#include "delay.h"

static u8 fac_us=0;			//微秒滴答定时器计数次数
static u16 fac_ms=0;		//毫秒滴答定时器计数次数






/************************系统支持宏****************************/
#if	SYSTEM_SUPPORT_OS			//如果系统支持os操作系统
//支持UCOSII
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD定义了,说明要支持UCOSII				
#define delay_osrunning			OSRunning		//OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OS时钟节拍,即每秒调度次数,范围为10~1000hz
#define delay_osintnesting 	OSIntNesting		//中断嵌套级别,即中断嵌套次数
#endif

//支持UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD定义了,说明要支持UCOSIII	
#define delay_osrunning			OSRunning		//OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OS时钟节拍,即每秒调度次数,范围为10~1000hz
#define delay_osintnesting 	OSIntNestingCtr		//中断嵌套级别,即中断嵌套次数
#endif



void SysTick_Handler(void)
{	
	if(delay_osrunning==1)						//OS开始跑了,才执行正常的调度处理
	{
		OSIntEnter();							//进入中断
		OSTimeTick();       					//调用ucos的时钟服务程序               
		OSIntExit();       	 					//触发任务切换软中断
	}
}




/**
	*@brief		us级延时时,进行任务调度的上锁,防止任务调度
	*@param		NONE
	*@retval	NONE
	*@supplement		#ifdef CPU_CFG_CRITICAL_METHOD	指支持USCOIII系统
	*/
void delay_OSSchedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD				//支持UCOSIII
	OS_ERR err;
	OSSchedLock(&err);
#else
	OSSchedLock();
#endif
}




/**
	*@brief		us级延时后,进行任务调度的解锁,打开任务调度
	*@param		NONE
	*@retval	NONE
	*@supplement		#ifdef CPU_CFG_CRITICAL_METHOD	指支持USCOIII系统
	*/
void delay_OSSchedunlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD				//支持UCOSIII
	OS_ERR err;
	OSSchedUnlock(&err);
#else
	OSSchedUnlock();
#endif
}





/**
	*@brief		OS系统自带的延时函数
	*@param		u32 ticks	延时的节拍数,实际延时时间要根据频率来计算
	*@retval	NONE
	*@supplement		#ifdef CPU_CFG_CRITICAL_METHOD	指支持USCOIII系统
	*/
void delay_OSTimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD							//支持UCOSIII
	OS_ERR err;
	OSTimeDly(ticks, OS_OPT_TIME_PERIODIC, &err);		//周期模式延时,周期延时 >= 实际值
#else 
	OSTimeDly(ticks);
#endif
}



#endif

/**
	*@brief		延时函数初始化
	*@param		NONE
	*@retval	NONE
	*@supplement		滴答定时器寄存器的各位默认值为0
									SystemCoreClock:168mhz  时钟主频变化时，需要根据需要修改
	*/
void delay_init(void)
{
#if	SYSTEM_SUPPORT_OS			//如果系统支持os操作系统
	u32 reload;
#endif
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//滴答定时器的时钟为AHB时钟的8分频
	fac_us = SystemCoreClock/8000000;						//计时1us所需要的滴答周期,即1us所需要的滴答计数
	
#if	SYSTEM_SUPPORT_OS
	reload = SystemCoreClock/8000000;		//每秒的计数次数 单位为M,1 s = 1M us(微秒)
	reload *= 1000000/delay_ostickspersec;	//每个时钟节拍所需要的计数次数 delay_ostickspersec为系统时钟节拍频率
	fac_ms = 1000/delay_ostickspersec;		//一个时钟节拍所需要的毫秒数,不是一个毫秒需要的滴答计数,
											//因为频率在除数上所以不能取0,且因为fac_ms为u16,最好能整除
	
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;			//开启滴答定时器的中断,即向此位写1
	SysTick->LOAD = reload;								//每个时钟节拍中断一次
	SysTick->CTRL|= SysTick_CTRL_ENABLE_Msk;			//使能滴答定时器，即向此位写1 

#else
	fac_ms = (u16)fac_us*1000;							//每个ms的滴答周期,这里指的是一个毫秒需要的滴答计数
#endif
}



#if			SYSTEM_SUPPORT_OS


/**
	*@brief	us延时函数,支持os移植
	*@param	u32 nus	需要延时的us时间
	*@retval	NONE
	*/
void delay_us(u32 nus)
{
	u32 ticks;
	u32 told, tnow, tcnt=0;
	u32 reload=SysTick->LOAD;
	
	ticks = nus*fac_us;			//需要延时的滴答计数次数
	delay_OSSchedlock();		//关闭任务调度
	told = SysTick->VAL;		//获得滴答定时器当前计数值
	
	while(1)
	{
		tnow = SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)			tcnt+=told-tnow;
			else 							tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;		//计数次数满足,则退出循环
		}
	}
	delay_OSSchedunlock();			//打开os调度器
}





/**
	*@brief	ms延时函数,支持os移植
	*@param	u16 nms	需要延时的us时间
	*@retval	NONE
	*/
void delay_ms(u16 nms)
{
	if(delay_osrunning && delay_osintnesting==0)		//确保系统不是在中断中
	{
		if(nms>fac_ms)	//即延时的时间大于一个时钟节拍
			delay_OSTimedly(nms/fac_ms);	//延时多少时钟节拍,os自带的延时函数,即时钟节拍的单位为延时最小单位
		nms%=fac_ms;		//时钟节拍的形式延时了部分后，剩余的低于时钟节拍的短小时间用us来执行
	}
	delay_us((u32)(nms*1000));	//us延时函数
	
}


#else

void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//时间加载	  		 
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					//清空计数器	 
}

//nms<=0xffffff*8*1000/SystemCoreClock
//SystemCoreClock单位为Hz,nms单位为ms
//对168M条件下,nms<=798ms 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;							//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;       					//清空计数器	  	    
} 
//延时nms 
//nms:0~65535
void delay_xms(u16 nms)
{	 	 
	u8 repeat=nms/540;						//这里用540,是考虑到某些客户可能超频使用,
											//比如超频到248M的时候,delay_xms最大只能延时541ms左右了
	u16 remain=nms%540;
	while(repeat)
	{
		delay_ms(540);
		repeat--;
	}
	if(remain)delay_ms(remain);
} 

#endif

/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/

void delay(int n)
{
//	CPU_SR_ALLOC();
	int i = 0,j;
	for(j = 0; j <=n; j++)
		for(i = 0; i <=6000; i++);
//	OSSched();                                              /* Find next task to run!                                 */
}



