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

/*	�ź�������	*/
extern OS_SEM	LineWalk_task_Sem1;
extern OS_SEM	LineWalk_task_Sem2;

extern SERVO_MLCB UNDER6_MLCB;

/*		��������	*/

int32_t  bru_speed =0;										//��ˢ�����ת�ٶ�	-������
float  rotate_speed =0;									//���̳����ٶ�      -���Թ�
volatile int32_t	 Enc_TIM4[2] ={0};			//�����ж�ǰ�����δ洢����������ֵ  ���ڼ�����ˢ��ת�ٶ�


//�ⲿ�ж� 
uint8_t flag_DI =0;
uint16_t number_DI =0;




//��ʱ��2  
extern  int32_t Encoder_TIM2;	//ͨ��TIM2������ģʽ��׽�ļ���


//��ʱ��4
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
	OSIntEnter();		//�����жϴ������
	if(EXTI_GetFlagStatus(EXTI_Line0) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	OSIntExit();			//�˳��ٽ���
}

uint8_t off_flag;
int a=0;
int hole_circle=0;
int circle1=0;
int first_offset=0;//������������һȦ����
int pay_first=0,pay_second=0;
void EXTI2_IRQHandler(void)
{
	int a=0;
	OSIntEnter();		//�����жϴ������
	if(EXTI_GetFlagStatus(EXTI_Line2) != RESET)
	{
		/*	�û�����	*/
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
	OSIntExit();			//�˳��ٽ���
}

void EXTI3_IRQHandler(void)
{
	OSIntEnter();		//�����жϴ������
	if(EXTI_GetFlagStatus(EXTI_Line3) != RESET)
	{
 
		/*	�û�����	*/
		
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
	OSIntExit();			//�˳��ٽ���
}


void EXTI4_IRQHandler(void)
{
	OSIntEnter();		//�����жϴ������
	if(EXTI_GetFlagStatus(EXTI_Line4) != RESET)
	{
 
		/*	�û�����	*/

		EXTI_ClearITPendingBit(EXTI_Line4);
	}
	OSIntExit();			//�˳��ٽ���
}

/**
	*	@brief	Encoder2 interrupt
	*	@param	NONE
	*	@retval	NONE
	*/
extern uint16_t flag_exti;
void EXTI9_5_IRQHandler(void)
{
	OSIntEnter();		//�����жϴ������
	if(EXTI_GetFlagStatus(EXTI_Line5) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	if(EXTI_GetFlagStatus(EXTI_Line6) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if(EXTI_GetFlagStatus(EXTI_Line7) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if(EXTI_GetFlagStatus(EXTI_Line8) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line8);
    hole_circle=Encoder_TIM2*65536 + TIM2->CNT;

	}
	if(EXTI_GetFlagStatus(EXTI_Line9) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	OSIntExit();			//�˳��ٽ���
}



/**
	*	@brief	EXTI15_10_IRQHandler()
	*	@param	NONE
	*	@retval	NONE
	*/
void EXTI15_10_IRQHandler(void)
{
	OSIntEnter();		//�����жϴ������
	
	if(EXTI_GetFlagStatus(EXTI_Line10) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line11) != RESET)
	{
		/*	�û�����	*/
//		exti_flag2++;
//		QBEED;
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line12) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line12);
		
			
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line13) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line13);
		if(flag_DI == 0)
		{
//			TIM2->CNT =0;				//����������
//			TIM_Cmd(TIM3, ENABLE);		
//			flag_DI =1;
//			rotate_speed =0;
//			number_DI =0;
		}
		else				
		{										
//			TIM_Cmd(TIM3, DISABLE);
//			rotate_speed = 2500/((float)number_DI);     //ʱ�䲻�ó���6.5536�� �����������  ��λm/s
//			flag_DI =0;
		}		
		
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line14) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line15) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
	
	OSIntExit();			//�˳��ٽ���
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
	OSIntEnter();		//�����жϴ������
	if(TIM_GetFlagStatus(TIM5,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM5, TIM_FLAG_Update);
		Encoder=Encoder_TIM2*65536 + TIM2->CNT-lat_Encoder;
		lat_Encoder=Encoder_TIM2*65536 + TIM2->CNT;
		//speed_real=(int)(Encoder*2000*60/(4000*15));//40����ٶȼ���
		speed_real=(int)(Encoder*2000*60/(4000*15));//35����ٶȼ���
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
	OSIntEnter();		//�����жϴ������
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
/*�����Ƕ�ʱ��7�жϣ���Ҫ���ܣ����������ʱ���������ٽ׶Σ�����Ϊ20ms*/
extern int control_num;	
extern int init_distance; //��ʼλ�� ������
extern int diantance_offset_last;		
extern int throw_flag;
/*�������������ٲ������������ٶȴ�ӡ�������ڷ��Ͳ���*/
int time1=0;
int speed2=0;
int sped[2000]={0};
int Encoder1=0,lat_Encoder1=0;
int get_finaldis=0;//���ļ��پ���
void TIM7_IRQHandler(void)
{
	OS_ERR err;
	OSIntEnter();		//�����жϴ������
	if(TIM_GetFlagStatus(TIM7,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM7, TIM_FLAG_Update);
		if(control_num < control_num_max)
      {
		throw_ball();           //ִ�к�����ÿ20ms����һ�ε��ת�٣�ʵ�ֱջ��ٶ�sin���߶��㹦��
		Encoder1=Encoder_TIM2*65536 + TIM2->CNT-lat_Encoder1;//ÿ20ms�߹�������
		lat_Encoder1=Encoder_TIM2*65536 + TIM2->CNT;
	  speed2=(int)(Encoder1*50*60/(4000*15));  //ʵʱ�ٶȣ�20ms��
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
			TIM_Cmd(TIM7, DISABLE);TIM_Cmd(TIM3, DISABLE);//�ض�ʱ���ж�
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
	OSIntEnter();					//�����жϴ������
	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);		
		asd=TIM2->CNT;
		//BEED_ON;
		/*	�û�����	*/
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
//	OSIntEnter();					//�����жϴ������
//	if(TIM_GetFlagStatus(TIM1,TIM_FLAG_Update)!=RESET)
//	{
//		TIM_ClearFlag(TIM1, TIM_FLAG_Update);
//		/*	�û�����	*/
//		
//	}
//	OSIntExit();
//}

/**
	*	@brief	void TIM4_IRQHandler()
	*	@param	NONE
	*	@retval	NONE
	*/
extern int32_t Encoder_TIM4;			//ͨ��TIM4������ģʽ��׽�ļ���

void TIM4_IRQHandler(void)
{
	OSIntEnter();					//�����жϴ������
	if(TIM_GetFlagStatus(TIM4,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		/*	�û�����	*/
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

extern int32_t Encoder_TIM8;			//ͨ��TIM4������ģʽ��׽�ļ���

void TIM8_UP_TIM13_IRQHandler(void)
{
	OSIntEnter();					//�����жϴ������
	if(TIM_GetFlagStatus(TIM8,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM8, TIM_FLAG_Update);
		/*	�û�����	*/
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
//		/*	�û�����	*/
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
	OSIntEnter();		//�����жϴ������
	
	/**********************��������ж�************************/
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		{
			/*		�û�����		*/
		
		}
		USART_ClearITPendingBit(USART1, USART_IT_TC);
	}
	/**********************��������ж�*****************************/
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		{
			/*		�û�����		*/
			ch = USART_ReceiveData(USART1);
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	OSIntExit();			//�˳��ٽ���
}


/**
  * @brief	USART2_IRQHandler
	*	@param	NONE
	*	@retval	NONE
  */ 
void USART2_IRQHandler(void)
{
	uint8_t ch;
	OSIntEnter();		//�����жϴ������
	
	/**********************��������ж�************************/
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		{
			/*		�û�����		*/
		
		}
		USART_ClearITPendingBit(USART2, USART_IT_TC);
	}
	/**********************��������ж�*****************************/
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		{
			/*		�û�����		*/
			ch = USART_ReceiveData(USART2);
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
	
	OSIntExit();			//�˳��ٽ���
}


/**
  * @brief	USART3_IRQHandler
	*	@param	NONE
	*	@retval	NONE
  */
extern HandlesStruct UART_CH;	//�ֱ����ݰ�
extern OS_SEM	Handle_Sem;			//�ֱ������ź���
void USART3_IRQHandler(void)
{
	uint8_t ch;
	static uint8_t i=0;
	static uint8_t step =0;
	static uint8_t usart_ch[4]={0};
	OS_ERR err;
	OSIntEnter();		//�����жϴ������
	
	/**********************��������ж�************************/
	if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
		{
			/*		�û�����		*/
		
		}
		USART_ClearITPendingBit(USART3, USART_IT_TC);
	}
	/**********************��������ж�*****************************/
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		{
			/*		�û�����		*/
			ch = USART_ReceiveData(USART3);
			switch(step)
			{
				case 0:									{	
			/*	֡ͷ	*/								if(ch==0x40)		step++;
																}break;
				
				case 1:									{
			/*	����	*/								if(ch==0x40)		;
																	else{
																		usart_ch[0] = ch;
																		step++;
																	}
																}break;
				
				case 2:									{
			/*	����ҡ��	*/						if(ch==0x40)		step=1;
																	else{
																		usart_ch[1] = ch;
																		step++;
																	}
																}break;

				case 3:									{
			/*	֡β	*/								if(ch==0x40)			step=1;
																	else if(ch!=0x23)	step=0;
																	else if(ch==0x23)		
																	{
																		UART_CH.KEY = usart_ch[0];
																		UART_CH.LEFT_RIGHT =  usart_ch[1];
																		UART_CH.UP_DOWN    =  usart_ch[2];
																		UART_CH.BEFORE_RIGHT = usart_ch[3];
																		UART_CH.finish_flag = 1;//������Ч
																		OSSemPost(	(OS_SEM	*)&Handle_Sem,					//��Ҫ�ͷŵ��ź���
																								(OS_OPT	 )OS_OPT_POST_1,		//�ͷŸ��ȴ����ź����������о���������ȼ���
																								(OS_ERR	*)&err);						//������뷵��
																	}
																}break;
				
				default:								{}break;
			}
		}
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
	
	OSIntExit();			//�˳��ٽ���
}


/**
  * @brief	UART4_IRQHandler
	*	@param	NONE
	*	@retval	NONE
  */ 

/*   ����ģ��		*/
extern Robocon_Handle_Struct Rb_Handle_Control;
extern uint8_t Robocon_Handle_Flag;
void UART4_IRQHandler(void)
{
	uint8_t ch;
	static uint8_t step = 0;
	static uint8_t i = 0;
	static uint8_t j = 0;
	static uint8_t FH_date[4] = {0, 0, 0, 0};
	static uint8_t FH_STD_date[4] = {0XFF, 0XFE, 0XFD, 0XF2};				//���޸�Ϊ����ģ�� ֡ͷ
	static uint8_t FH_judge_flag = 0;
	
	OSIntEnter();		//�����жϴ������
	
	/**********************��������ж�************************/
	if(USART_GetITStatus(UART4, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_TC);
		{
			/*		�û�����		*/
		}
		
	}
	/**********************��������ж�*****************************/
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		{						
												
			/*		�û�����		*/
			ch = USART_ReceiveData(UART4);
						//------------֡ͷ�жϲ���
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
			//------------֡ͷ�жϽ���

			switch(step)
			{
				/*	�ȴ�֡ͷ�жϳɹ�	*/
				case 0:	{}break;
				/*	����������Ϣ����	*/
				case 1:	{
									Rb_Handle_Control.KEY = ch;
									step++;
								}break;
				/*	��λX������Ϣ����	*/
				case 2:	{
									Rb_Handle_Control.Locat_Coord_X._u8_data[j] = ch;
									j++;
									if(4 == j)
									{
										j = 0;
										step++;
									}
								}break;
				/*	��λY������Ϣ����	*/
				case 3:	{
									Rb_Handle_Control.Locat_Coord_Y._u8_data[j] = ch;
									j++;
									if(4 == j)
									{
										j = 0;
										step++;
									}
								}break;
				/*	��λ�Ƕ�W��Ϣ����	*/
				case 4:	{
									Rb_Handle_Control.Locat_Coord_W._u8_data[j] = ch;
									j++;
									if(4 == j)
									{
										j = 0;
										step++;
									}
								}break;
				/*	Ħ�������ٶȴ���	*/
				case 5:	{
									Rb_Handle_Control.Rub_Speed._u8_data[j] = ch;
									j++;
									if(4 == j)
									{
										j = 0;
										step++;
									}
								}break;
				/*	�����ͷ�λ�ô���	*/
				case 6:	{
									Rb_Handle_Control.Simulate_Position._u8_data[j] = ch;
									j++;
									if(4 == j)
									{
										j = 0;
										step++;
									}
								}break;
				/*	��������λ�ô���	*/
				case 7:	{
									Rb_Handle_Control.Elevation_Position._u8_data[j] = ch;
									j++;
									if(4 == j)
									{
										j = 0;
										step++;
									}
								}break;
				/*	��ִ��Ϣ��־	*/
				case 8:	{
									Rb_Handle_Control.Receipt = ch;
									step++;
								}break;
				/*	֡β	*/
				case 9:	{
									Rb_Handle_Control.FT = ch;
									if(0X02 == ch)										//���޸�Ϊ ����ģ��֡ͷ
									{
										Robocon_Handle_Flag = 1;		//�ֱ����ճɹ�
										step = 0;
									}
								}break;
				default:{}break;
			}
		}
		
	}
	
	OSIntExit();			//�˳��ٽ���
}

/**
  * @brief	USART1_IRQHandler
	*	@param	NONE
	*	@retval	NONE
  */ 
void UART5_IRQHandler(void)
{
	uint8_t ch;
	OSIntEnter();		//�����жϴ������
	
	/**********************��������ж�************************/
	if(USART_GetITStatus(UART5, USART_IT_TC) != RESET)
	{
		{
			/*		�û�����		*/
		
		}
		USART_ClearITPendingBit(UART5, USART_IT_TC);
	}
	/**********************��������ж�*****************************/
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		{
			/*		�û�����		*/
			ch = USART_ReceiveData(UART5);
		}
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
	}
	
	OSIntExit();			//�˳��ٽ���
}

/**********************************************************************************************************/
/*																*	CAN_HANDLER	*																													*/
/**********************************************************************************************************/
/**
	*
	*/
void CAN1_RX0_IRQHandler(void)
{
	OSIntEnter();					//�����жϴ������
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0)!=RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_Recive_handle();
	}
	OSIntExit();
}

void CAN1_SCE_IRQHandler(void)
{
	OSIntEnter();					//�����жϴ������
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
