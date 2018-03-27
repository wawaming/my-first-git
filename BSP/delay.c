/*************************************************************************************
*	@file			delay.c
* @author	 	yun
*	@supplement		#ifdef CPU_CFG_CRITICAL_METHOD	ָ֧��USCOIIIϵͳ,����֧��UCOSIIϵͳ
					��ʱ����:
						�δ�ʱ����һ��Ƶ�ʽ��м�������һ�����ڵļ��������жϣ�һ���ж�
						��Ϊ����ϵͳ��ʱ�ӽ��ģ������õδ�ʱ��������ʱ�жϣ���ʱʱ����
						ʱ�ӽ��ĵ�����������ʹ��ʱʱ�䲻׼ȷ��Ҳ�����ʱ�ӽ��ĵĲ�׼ȷ(��
						Ϊ��ʱ���жϻ�ռ��ʱ��).
						
						��ʱ����ʹ���жϣ������ý�ȡ�δ�ʱ������ֵ�ķ������ﵽЧ������
						Ҫ��ʱʱ����ȡ��ǰ�δ�ʱ���ļ���ֵ�����ʵʱ��ȡ����ֵ��ֱ����
						��֮��ﵽ����Ҫ����ʱ��Ӧ�ļ���ֵʱ�������ʱ
						
						΢�뼶��ʱΪ��׼ȷ����Ҫ�ص�����ϵͳ��������ȣ���ʱ��ɺ���
						
						�δ�ʱ����һ���ݼ���ʱ��
*	@version 	V1.0
*	@date			2016/5/27
* @brief		NONE
*************************************************************************************/

#include "delay.h"

static u8 fac_us=0;			//΢��δ�ʱ����������
static u16 fac_ms=0;		//����δ�ʱ����������






/************************ϵͳ֧�ֺ�****************************/
#if	SYSTEM_SUPPORT_OS			//���ϵͳ֧��os����ϵͳ
//֧��UCOSII
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD������,˵��Ҫ֧��UCOSII				
#define delay_osrunning			OSRunning		//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OSʱ�ӽ���,��ÿ����ȴ���,��ΧΪ10~1000hz
#define delay_osintnesting 	OSIntNesting		//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif

//֧��UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD������,˵��Ҫ֧��UCOSIII	
#define delay_osrunning			OSRunning		//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OSʱ�ӽ���,��ÿ����ȴ���,��ΧΪ10~1000hz
#define delay_osintnesting 	OSIntNestingCtr		//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif



void SysTick_Handler(void)
{	
	if(delay_osrunning==1)						//OS��ʼ����,��ִ�������ĵ��ȴ���
	{
		OSIntEnter();							//�����ж�
		OSTimeTick();       					//����ucos��ʱ�ӷ������               
		OSIntExit();       	 					//���������л����ж�
	}
}




/**
	*@brief		us����ʱʱ,����������ȵ�����,��ֹ�������
	*@param		NONE
	*@retval	NONE
	*@supplement		#ifdef CPU_CFG_CRITICAL_METHOD	ָ֧��USCOIIIϵͳ
	*/
void delay_OSSchedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD				//֧��UCOSIII
	OS_ERR err;
	OSSchedLock(&err);
#else
	OSSchedLock();
#endif
}




/**
	*@brief		us����ʱ��,����������ȵĽ���,���������
	*@param		NONE
	*@retval	NONE
	*@supplement		#ifdef CPU_CFG_CRITICAL_METHOD	ָ֧��USCOIIIϵͳ
	*/
void delay_OSSchedunlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD				//֧��UCOSIII
	OS_ERR err;
	OSSchedUnlock(&err);
#else
	OSSchedUnlock();
#endif
}





/**
	*@brief		OSϵͳ�Դ�����ʱ����
	*@param		u32 ticks	��ʱ�Ľ�����,ʵ����ʱʱ��Ҫ����Ƶ��������
	*@retval	NONE
	*@supplement		#ifdef CPU_CFG_CRITICAL_METHOD	ָ֧��USCOIIIϵͳ
	*/
void delay_OSTimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD							//֧��UCOSIII
	OS_ERR err;
	OSTimeDly(ticks, OS_OPT_TIME_PERIODIC, &err);		//����ģʽ��ʱ,������ʱ >= ʵ��ֵ
#else 
	OSTimeDly(ticks);
#endif
}



#endif

/**
	*@brief		��ʱ������ʼ��
	*@param		NONE
	*@retval	NONE
	*@supplement		�δ�ʱ���Ĵ����ĸ�λĬ��ֵΪ0
									SystemCoreClock:168mhz  ʱ����Ƶ�仯ʱ����Ҫ������Ҫ�޸�
	*/
void delay_init(void)
{
#if	SYSTEM_SUPPORT_OS			//���ϵͳ֧��os����ϵͳ
	u32 reload;
#endif
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//�δ�ʱ����ʱ��ΪAHBʱ�ӵ�8��Ƶ
	fac_us = SystemCoreClock/8000000;						//��ʱ1us����Ҫ�ĵδ�����,��1us����Ҫ�ĵδ����
	
#if	SYSTEM_SUPPORT_OS
	reload = SystemCoreClock/8000000;		//ÿ��ļ������� ��λΪM,1 s = 1M us(΢��)
	reload *= 1000000/delay_ostickspersec;	//ÿ��ʱ�ӽ�������Ҫ�ļ������� delay_ostickspersecΪϵͳʱ�ӽ���Ƶ��
	fac_ms = 1000/delay_ostickspersec;		//һ��ʱ�ӽ�������Ҫ�ĺ�����,����һ��������Ҫ�ĵδ����,
											//��ΪƵ���ڳ��������Բ���ȡ0,����Ϊfac_msΪu16,���������
	
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;			//�����δ�ʱ�����ж�,�����λд1
	SysTick->LOAD = reload;								//ÿ��ʱ�ӽ����ж�һ��
	SysTick->CTRL|= SysTick_CTRL_ENABLE_Msk;			//ʹ�ܵδ�ʱ���������λд1 

#else
	fac_ms = (u16)fac_us*1000;							//ÿ��ms�ĵδ�����,����ָ����һ��������Ҫ�ĵδ����
#endif
}



#if			SYSTEM_SUPPORT_OS


/**
	*@brief	us��ʱ����,֧��os��ֲ
	*@param	u32 nus	��Ҫ��ʱ��usʱ��
	*@retval	NONE
	*/
void delay_us(u32 nus)
{
	u32 ticks;
	u32 told, tnow, tcnt=0;
	u32 reload=SysTick->LOAD;
	
	ticks = nus*fac_us;			//��Ҫ��ʱ�ĵδ��������
	delay_OSSchedlock();		//�ر��������
	told = SysTick->VAL;		//��õδ�ʱ����ǰ����ֵ
	
	while(1)
	{
		tnow = SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)			tcnt+=told-tnow;
			else 							tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;		//������������,���˳�ѭ��
		}
	}
	delay_OSSchedunlock();			//��os������
}





/**
	*@brief	ms��ʱ����,֧��os��ֲ
	*@param	u16 nms	��Ҫ��ʱ��usʱ��
	*@retval	NONE
	*/
void delay_ms(u16 nms)
{
	if(delay_osrunning && delay_osintnesting==0)		//ȷ��ϵͳ�������ж���
	{
		if(nms>fac_ms)	//����ʱ��ʱ�����һ��ʱ�ӽ���
			delay_OSTimedly(nms/fac_ms);	//��ʱ����ʱ�ӽ���,os�Դ�����ʱ����,��ʱ�ӽ��ĵĵ�λΪ��ʱ��С��λ
		nms%=fac_ms;		//ʱ�ӽ��ĵ���ʽ��ʱ�˲��ֺ�ʣ��ĵ���ʱ�ӽ��ĵĶ�Сʱ����us��ִ��
	}
	delay_us((u32)(nms*1000));	//us��ʱ����
	
}


#else

void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//ʱ�����	  		 
	SysTick->VAL=0x00;        					//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					//��ռ�����	 
}

//nms<=0xffffff*8*1000/SystemCoreClock
//SystemCoreClock��λΪHz,nms��λΪms
//��168M������,nms<=798ms 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;							//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;       					//��ռ�����	  	    
} 
//��ʱnms 
//nms:0~65535
void delay_xms(u16 nms)
{	 	 
	u8 repeat=nms/540;						//������540,�ǿ��ǵ�ĳЩ�ͻ����ܳ�Ƶʹ��,
											//���糬Ƶ��248M��ʱ��,delay_xms���ֻ����ʱ541ms������
	u16 remain=nms%540;
	while(repeat)
	{
		delay_ms(540);
		repeat--;
	}
	if(remain)delay_ms(remain);
} 

#endif

/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
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



