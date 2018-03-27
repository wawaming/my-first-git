/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "throw.h"
#include "includes.h"
#include "positioner.h"
#include "CAN_RECIVE.h"
#include "Locate_function.h"
#include "led.h"

/*	信号量申明	*/
extern OS_SEM	LineWalk_task_Sem1;
extern OS_SEM	LineWalk_task_Sem2;

extern SERVO_MLCB UNDER6_MLCB;

/*		变量定义	*/

int32_t  bru_speed =0;										//无刷电机自转速度	-编码器
float  rotate_speed =0;									//飞盘出射速度      -光电对管
volatile int32_t	 Enc_TIM4[2] ={0};			//进入中断前后两次存储编码器计数值  用于计算无刷自转速度


//外部中断 
uint8_t flag_DI =0;
uint16_t number_DI =0;




//定时器2  
extern  int32_t Encoder_TIM2;	//通过TIM2编码器模式捕捉的计数


//定时器4
extern  int32_t Encoder_TIM4;

extern  int32_t Encoder_TIM8;



/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

///**
//  * @brief  This function handles PendSVC exception.
//  * @param  None
//  * @retval None
//  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
////  TimingDelay_Decrement();
//}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
/*************************************************EXTI	INTERRUPT************************************************/
/*************************************************EXTI	INTERRUPT************************************************/
/*************************************************EXTI	INTERRUPT************************************************/
//extern int exti_flag15;
//extern int exti_flag8;
//extern int exti_flag2;
//extern int exti_flag4;
//extern int exti_flag13;

/**
	*	@brief	Encoder1 interrupt
	*	@param	NONE
	*	@retval	NONE
	*/
void EXTI0_IRQHandler(void)
{
	OSIntEnter();		//进入中断处理程序
	if(EXTI_GetFlagStatus(EXTI_Line0) != RESET)
	{
		/*	用户函数	*/
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	OSIntExit();			//退出临界区
}

uint8_t off_flag;
int a=0;
int hole_circle=0;
int circle1=0;
int first_offset=0;//补偿编码器第一圈数据
int pay_first=0,pay_second=0;
void EXTI2_IRQHandler(void)
{
	int a=0;
	OSIntEnter();		//进入中断处理程序
	if(EXTI_GetFlagStatus(EXTI_Line2) != RESET)
	{
		/*	用户函数	*/
		EXTI_ClearITPendingBit(EXTI_Line2);
		circle1+=1;
		if(circle1%28==0)
		{
			Motor_SetSpeed(&UNDER6_MLCB,0);
		}
//		if(circle1==2)
//		{
//		first_offset=Encoder_TIM2*65536 + TIM2->CNT;TIM2->CNT=0;
//		}
//		if(circle1>2)
//		{
//		if((circle1%4)!=0)
//		{
//			pay_first=Encoder_TIM2*65536 + TIM2->CNT;
//		}
//		if((circle1%4)==0)
//		{
//			pay_second=Encoder_TIM2*65536 + TIM2->CNT-pay_first-4000;
//			a=TIM2->CNT-pay_second;
//			if(a>=65536)
//			{
//			Encoder_TIM2+=1;	TIM2->CNT=a-65536;
//			}
//			else TIM2->CNT-=pay_second;
		
	}
//		if((circle1%28)==0)
//		{
//		hole_circle=(Encoder_TIM2*65536 + TIM2->CNT);
//		}
	OSIntExit();			//退出临界区
}

void EXTI3_IRQHandler(void)
{
	OSIntEnter();		//进入中断处理程序
	if(EXTI_GetFlagStatus(EXTI_Line3) != RESET)
	{
 
		/*	用户函数	*/
		
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
	OSIntExit();			//退出临界区
}


void EXTI4_IRQHandler(void)
{
	OSIntEnter();		//进入中断处理程序
	if(EXTI_GetFlagStatus(EXTI_Line4) != RESET)
	{
 
		/*	用户函数	*/

		EXTI_ClearITPendingBit(EXTI_Line4);
	}
	OSIntExit();			//退出临界区
}

/**
	*	@brief	Encoder2 interrupt
	*	@param	NONE
	*	@retval	NONE
	*/
extern uint16_t flag_exti;
void EXTI9_5_IRQHandler(void)
{
	OSIntEnter();		//进入中断处理程序
	if(EXTI_GetFlagStatus(EXTI_Line5) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	if(EXTI_GetFlagStatus(EXTI_Line6) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if(EXTI_GetFlagStatus(EXTI_Line7) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if(EXTI_GetFlagStatus(EXTI_Line8) != RESET)
	{
		/*	用户函数	*/
		EXTI_ClearITPendingBit(EXTI_Line8);
    hole_circle=Encoder_TIM2*65536 + TIM2->CNT;

	}
	if(EXTI_GetFlagStatus(EXTI_Line9) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	OSIntExit();			//退出临界区
}



/**
	*	@brief	EXTI15_10_IRQHandler()
	*	@param	NONE
	*	@retval	NONE
	*/
void EXTI15_10_IRQHandler(void)
{
	OSIntEnter();		//进入中断处理程序
	
	if(EXTI_GetFlagStatus(EXTI_Line10) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line11) != RESET)
	{
		/*	用户代码	*/
//		exti_flag2++;
//		QBEED;
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line12) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line12);
		
			
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line13) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line13);
		if(flag_DI == 0)
		{
//			TIM2->CNT =0;				//计数器清零
//			TIM_Cmd(TIM3, ENABLE);		
//			flag_DI =1;
//			rotate_speed =0;
//			number_DI =0;
		}
		else				
		{										
//			TIM_Cmd(TIM3, DISABLE);
//			rotate_speed = 2500/((float)number_DI);     //时间不得超过6.5536秒 否则数据溢出  单位m/s
//			flag_DI =0;
		}		
		
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line14) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line15) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
	
	OSIntExit();			//退出临界区
}
/************************************************TIMX	INTERRUPT**************************************************/
/************************************************TIMX	INTERRUPT**************************************************/
/************************************************TIMX	INTERRUPT**************************************************/
/**
	*	@brief	TIM1_UP interrupt
	*	@param	NONE
	*	@retval	NONE
	*/
extern OS_TCB	OffsetControlTaskTCB;
extern int Time_5S;
int Encoder=0,lat_Encoder=0;
int speed_real=0;
//int time1=0;
//int speed2=0;
//int sped[5000]={0};
void TIM5_IRQHandler(void)
{
	OS_ERR err;
	OSIntEnter();		//进入中断处理程序
	if(TIM_GetFlagStatus(TIM5,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM5, TIM_FLAG_Update);
		Encoder=Encoder_TIM2*65536 + TIM2->CNT-lat_Encoder;
		lat_Encoder=Encoder_TIM2*65536 + TIM2->CNT;
		//speed_real=(int)(Encoder*2000*60/(4000*15));//40电机速度计算
		speed_real=(int)(Encoder*2000*60/(4000*15));//35电机速度计算
//   if(speed_real>=throw_speed) 
//	 {
//   if(time1<5000)	{sped[time1]=lat_Encoder;time1++;}
//}
	}	
	OSIntExit();
}

int v_flag=0; 
extern OS_TCB	throwTaskTCB;
void TIM3_IRQHandler(void)
{
	OS_ERR err;
	OSIntEnter();		//进入中断处理程序
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	//	number_DI++;	
  if(v_flag==0)
	{		
	if(speed_real>=throw_speed)
   {
	 v_flag=1;
	 OSTaskResume(&throwTaskTCB, &err);
	 }
//  if(v_flag==1&&((( Encoder_TIM2*65536+TIM2->CNT)%60000)>=(50000-50 ))&&((( Encoder_TIM2*65536+TIM2->CNT)%60000)<=(50000+50)))distace_flag=1;
//  if(v_flag && distace_flag) {throw_flag=1;init_distance=Encoder_TIM2*65536+TIM2->CNT;}
	}
	else TIM_Cmd(TIM3, DISABLE);
}
	OSIntExit();
}
/*以下是定时器7中断，主要功能：当完成抛掷时进入电机减速阶段，周期为20ms*/
extern int control_num;	
extern int init_distance; //初始位置 脉冲数
extern int diantance_offset_last;		
extern int throw_flag;
/*以上是抛掷减速参数，以下是速度打印，供串口发送参数*/
int time1=0;
int speed2=0;
int sped[2000]={0};
int Encoder1=0,lat_Encoder1=0;
int get_finaldis=0;//最后的减速距离
void TIM7_IRQHandler(void)
{
	OS_ERR err;
	OSIntEnter();		//进入中断处理程序
	if(TIM_GetFlagStatus(TIM7,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM7, TIM_FLAG_Update);
		if(control_num < control_num_max)
      {
		throw_ball();           //执行函数，每20ms控制一次电机转速，实现闭环速度sin曲线定点功能
		Encoder1=Encoder_TIM2*65536 + TIM2->CNT-lat_Encoder1;//每20ms走过的脉冲
		lat_Encoder1=Encoder_TIM2*65536 + TIM2->CNT;
	  speed2=(int)(Encoder1*50*60/(4000*15));  //实时速度（20ms）
    if(time1<2000)	{sped[time1]=speed2;time1++;}
		else{time1=0;}
			}
		else{
			get_finaldis=Encoder_TIM2*65536 + TIM2->CNT-init_distance;
			Motor_SetSpeed(&UNDER6_MLCB,0);
			v_flag=0;throw_flag=0;
			control_num=0;
			diantance_offset_last=0;
			time1=0;
			RL1_OFF;
			BEED_OFF;
			TIM_Cmd(TIM7, DISABLE);TIM_Cmd(TIM3, DISABLE);//关定时器中断
		}
		//OSTaskResume(&OffsetControlTaskTCB, &err);
	}
	OSIntExit();
}



/**
	*	@brief	void TIM2_IRQHandler()
	*	@param	NONE
	*	@retval NONE
	*/
int asd=0;
void TIM2_IRQHandler(void)
{
	OSIntEnter();					//进入中断处理程序
	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);		
		asd=TIM2->CNT;
		//BEED_ON;
		/*	用户函数	*/
		if(TIM2->CNT==65535)
		{
			Encoder_TIM2--;
		}
		if((TIM2->CNT)<=10)
		{
			Encoder_TIM2++;
		//	BEED_ON;
		}
	}
	OSIntExit();
}

//void TIM1_IRQHandler(void)
//{
//	OSIntEnter();					//进入中断处理程序
//	if(TIM_GetFlagStatus(TIM1,TIM_FLAG_Update)!=RESET)
//	{
//		TIM_ClearFlag(TIM1, TIM_FLAG_Update);
//		/*	用户函数	*/
//		
//	}
//	OSIntExit();
//}

/**
	*	@brief	void TIM4_IRQHandler()
	*	@param	NONE
	*	@retval	NONE
	*/
extern int32_t Encoder_TIM4;			//通过TIM4编码器模式捕捉的计数

void TIM4_IRQHandler(void)
{
	OSIntEnter();					//进入中断处理程序
	if(TIM_GetFlagStatus(TIM4,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		/*	用户函数	*/
		if(TIM4->CNT==65535)
		{
			Encoder_TIM4--;
		}
		if(TIM4->CNT==0)
		{
			Encoder_TIM4++;
		}
	}
	OSIntExit();
}

extern int32_t Encoder_TIM8;			//通过TIM4编码器模式捕捉的计数

void TIM8_UP_TIM13_IRQHandler(void)
{
	OSIntEnter();					//进入中断处理程序
	if(TIM_GetFlagStatus(TIM8,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM8, TIM_FLAG_Update);
		/*	用户函数	*/
		if(TIM8->CNT==65535)
		{
			Encoder_TIM8--;
		}
		if(TIM8->CNT==0)
		{
			Encoder_TIM8++;
		}
	}
	
	
	OSIntExit();
}





//
//void TIM1_BRK_TIM9_IRQHandler(void)
//{
//		OSIntEnter();	
//	if(TIM_GetFlagStatus(TIM9,TIM_FLAG_Update)!=RESET)
//	{
//		TIM_ClearFlag(TIM9, TIM_FLAG_Update);
//		/*	用户函数	*/
//		if(TIM9->CNT==65535)
//		{
//			Encoder_TIM9--;
//		}
//		if(TIM9->CNT==0)
//		{
//			Encoder_TIM9++;
//		}
//	}

//	OSIntExit();
//}




/*************************************************USART INTERRUPT************************************************/
/*************************************************USART INTERRUPT************************************************/
/*************************************************USART INTERRUPT************************************************/
/**
  * @brief	USART1_IRQHandler
	*	@param	NONE
	*	@retval	NONE
  */ 

void USART1_IRQHandler(void)
{
	uint8_t ch;
	OSIntEnter();		//进入中断处理程序
	
	/**********************发送完成中断************************/
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		{
			/*		用户函数		*/
		
		}
		USART_ClearITPendingBit(USART1, USART_IT_TC);
	}
	/**********************接收完成中断*****************************/
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		{
			/*		用户函数		*/
			ch = USART_ReceiveData(USART1);
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	OSIntExit();			//退出临界区
}


/**
  * @brief	USART2_IRQHandler
	*	@param	NONE
	*	@retval	NONE
  */ 
void USART2_IRQHandler(void)
{
	uint8_t ch;
	OSIntEnter();		//进入中断处理程序
	
	/**********************发送完成中断************************/
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		{
			/*		用户函数		*/
		
		}
		USART_ClearITPendingBit(USART2, USART_IT_TC);
	}
	/**********************接收完成中断*****************************/
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		{
			/*		用户函数		*/
			ch = USART_ReceiveData(USART2);
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
	
	OSIntExit();			//退出临界区
}


/**
  * @brief	USART3_IRQHandler
	*	@param	NONE
	*	@retval	NONE
  */
extern HandlesStruct UART_CH;	//手柄数据包
extern OS_SEM	Handle_Sem;			//手柄任务信号量
void USART3_IRQHandler(void)
{
	uint8_t ch;
	static uint8_t i=0;
	static uint8_t step =0;
	static uint8_t usart_ch[4]={0};
	OS_ERR err;
	OSIntEnter();		//进入中断处理程序
	
	/**********************发送完成中断************************/
	if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
		{
			/*		用户函数		*/
		
		}
		USART_ClearITPendingBit(USART3, USART_IT_TC);
	}
	/**********************接收完成中断*****************************/
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		{
			/*		用户函数		*/
			ch = USART_ReceiveData(USART3);
			switch(step)
			{
				case 0:									{	
			/*	帧头	*/								if(ch==0x40)		step++;
																}break;
				
				case 1:									{
			/*	按键	*/								if(ch==0x40)		;
																	else{
																		usart_ch[0] = ch;
																		step++;
																	}
																}break;
				
				case 2:									{
			/*	左右摇杆	*/						if(ch==0x40)		step=1;
																	else{
																		usart_ch[1] = ch;
																		step++;
																	}
																}break;

				case 3:									{
			/*	帧尾	*/								if(ch==0x40)			step=1;
																	else if(ch!=0x23)	step=0;
																	else if(ch==0x23)		
																	{
																		UART_CH.KEY = usart_ch[0];
																		UART_CH.LEFT_RIGHT =  usart_ch[1];
																		UART_CH.UP_DOWN    =  usart_ch[2];
																		UART_CH.BEFORE_RIGHT = usart_ch[3];
																		UART_CH.finish_flag = 1;//数据有效
																		OSSemPost(	(OS_SEM	*)&Handle_Sem,					//需要释放的信号量
																								(OS_OPT	 )OS_OPT_POST_1,		//释放给等待此信号量的任务中具有最高优先级者
																								(OS_ERR	*)&err);						//错误代码返回
																	}
																}break;
				
				default:								{}break;
			}
		}
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
	
	OSIntExit();			//退出临界区
}


/**
  * @brief	UART4_IRQHandler
	*	@param	NONE
	*	@retval	NONE
  */ 

/*   发射模块		*/
extern Robocon_Handle_Struct Rb_Handle_Control;
extern uint8_t Robocon_Handle_Flag;
void UART4_IRQHandler(void)
{
	uint8_t ch;
	static uint8_t step = 0;
	static uint8_t i = 0;
	static uint8_t j = 0;
	static uint8_t FH_date[4] = {0, 0, 0, 0};
	static uint8_t FH_STD_date[4] = {0XFF, 0XFE, 0XFD, 0XF2};				//已修改为发射模块 帧头
	static uint8_t FH_judge_flag = 0;
	
	OSIntEnter();		//进入中断处理程序
	
	/**********************发送完成中断************************/
	if(USART_GetITStatus(UART4, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_TC);
		{
			/*		用户函数		*/
		}
		
	}
	/**********************接收完成中断*****************************/
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		{						
												
			/*		用户函数		*/
			ch = USART_ReceiveData(UART4);
						//------------帧头判断部分
			if(4 == i)
			{
				for(i=0;i<4;i++){
					if(FH_STD_date[i] == FH_date[i])
						continue;
					else{
						break;
					}
				}
				if(4 == i)
				{
					step = 1;
					FH_judge_flag = 0;
				}
				i = 0;
			}
			if((0XFF == ch))
			{
				i = 0;
				FH_judge_flag = 1;
			}
			if(1 == FH_judge_flag)
			{
				FH_date[i] = ch;
				i++;
			}
			//------------帧头判断结束

			switch(step)
			{
				/*	等待帧头判断成功	*/
				case 0:	{}break;
				/*	按键调控信息传输	*/
				case 1:	{
									Rb_Handle_Control.KEY = ch;
									step++;
								}break;
				/*	定位X坐标信息传输	*/
				case 2:	{
									Rb_Handle_Control.Locat_Coord_X._u8_data[j] = ch;
									j++;
									if(4 == j)
									{
										j = 0;
										step++;
									}
								}break;
				/*	定位Y坐标信息传输	*/
				case 3:	{
									Rb_Handle_Control.Locat_Coord_Y._u8_data[j] = ch;
									j++;
									if(4 == j)
									{
										j = 0;
										step++;
									}
								}break;
				/*	定位角度W信息传输	*/
				case 4:	{
									Rb_Handle_Control.Locat_Coord_W._u8_data[j] = ch;
									j++;
									if(4 == j)
									{
										j = 0;
										step++;
									}
								}break;
				/*	摩擦机构速度传输	*/
				case 5:	{
									Rb_Handle_Control.Rub_Speed._u8_data[j] = ch;
									j++;
									if(4 == j)
									{
										j = 0;
										step++;
									}
								}break;
				/*	仿生释放位置传输	*/
				case 6:	{
									Rb_Handle_Control.Simulate_Position._u8_data[j] = ch;
									j++;
									if(4 == j)
									{
										j = 0;
										step++;
									}
								}break;
				/*	仿生仰角位置传输	*/
				case 7:	{
									Rb_Handle_Control.Elevation_Position._u8_data[j] = ch;
									j++;
									if(4 == j)
									{
										j = 0;
										step++;
									}
								}break;
				/*	回执信息标志	*/
				case 8:	{
									Rb_Handle_Control.Receipt = ch;
									step++;
								}break;
				/*	帧尾	*/
				case 9:	{
									Rb_Handle_Control.FT = ch;
									if(0X02 == ch)										//已修改为 发射模块帧头
									{
										Robocon_Handle_Flag = 1;		//手柄接收成功
										step = 0;
									}
								}break;
				default:{}break;
			}
		}
		
	}
	
	OSIntExit();			//退出临界区
}

/**
  * @brief	USART1_IRQHandler
	*	@param	NONE
	*	@retval	NONE
  */ 
void UART5_IRQHandler(void)
{
	uint8_t ch;
	OSIntEnter();		//进入中断处理程序
	
	/**********************发送完成中断************************/
	if(USART_GetITStatus(UART5, USART_IT_TC) != RESET)
	{
		{
			/*		用户函数		*/
		
		}
		USART_ClearITPendingBit(UART5, USART_IT_TC);
	}
	/**********************接收完成中断*****************************/
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		{
			/*		用户函数		*/
			ch = USART_ReceiveData(UART5);
		}
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
	}
	
	OSIntExit();			//退出临界区
}

/**********************************************************************************************************/
/*																*	CAN_HANDLER	*																													*/
/**********************************************************************************************************/
/**
	*
	*/
void CAN1_RX0_IRQHandler(void)
{
	OSIntEnter();					//进入中断处理程序
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0)!=RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_Recive_handle();
	}
	OSIntExit();
}

void CAN1_SCE_IRQHandler(void)
{
	OSIntEnter();					//进入中断处理程序
	if(CAN_GetITStatus(CAN1,CAN_IT_BOF ) != RESET)	 /* Bus-off */
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_BOF );
	}
	if(CAN_GetITStatus(CAN1, CAN_IT_EWG ) != RESET)	 /* Error warning */
	{	
		CAN_ClearITPendingBit(CAN1, CAN_IT_EWG );
	}
	if(CAN_GetITStatus(CAN1, CAN_IT_EPV) != RESET)  	 /* Error passive */
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_EPV );
	}	
	OSIntExit();	
}












/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
