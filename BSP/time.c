/*************************************************************************************
*	@file			time.c
* @author	 	yun
*	@version 	V1.0
*	@date			2017/3/3
* @brief		NONE
*************************************************************************************/

#include "time.h"

/**
	*@brief		ͨ�ö�ʱ���Ķ�ʱ��������
	*@param		TIMx		TIM1~TIM5	��Ӧ�Ķ�ʱ�����	TIM2~TIM5Ϊͨ�ö�ʱ��
				arr						��ʱ����װֵ
				TIM_Size	TIM_MIN,	��С��ʱ��		
				TIM_US ,				΢�붨ʱ��
				TIM_MS,					���붨ʱ��
				TIM_S,					�붨ʱ��
	*@equation					time=(arr+1)*(Prescaler+1)/72M
	*@retval	NONE
	*/
void TIM_Timing_Config(TIM_TypeDef *TIMx, uint16_t  arr, TIM_Size_TypeDef TIM_Size, uint32_t on_or_off)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		if(TIMx == TIM1){
			
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		
			switch(TIM_Size)
			{
				case TIM_MIN:       	TIM_TimeBaseStructure.TIM_Prescaler=0; 					//Ԥ��Ƶֵ
															TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 	//ʱ�ӷָ�
															TIM_TimeBaseStructure.TIM_Period=arr-1; 				//�Զ���װ��ֵ
															break;
				
				case TIM_US :					TIM_TimeBaseStructure.TIM_Prescaler=167;  
															TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
															TIM_TimeBaseStructure.TIM_Period=arr-1; 	
															break;
				
				case TIM_MS:					TIM_TimeBaseStructure.TIM_Prescaler=41999; 
															TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
															TIM_TimeBaseStructure.TIM_Period=arr*2-1; 	
															break;
				
				case TIM_S:						TIM_TimeBaseStructure.TIM_Prescaler=41999; 
															TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
															TIM_TimeBaseStructure.TIM_Period=arr*2000-1; 	
															break;
			}
		
		}
		else
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2<<(((uint32_t)TIMx-TIM2_BASE)/0x0400), ENABLE);
					switch(TIM_Size)
			{
				case TIM_MIN:       	TIM_TimeBaseStructure.TIM_Prescaler=0; 					//Ԥ��Ƶֵ
															TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 	//ʱ�ӷָ�
															TIM_TimeBaseStructure.TIM_Period=arr-1; 				//�Զ���װ��ֵ
															break;
				
				case TIM_US :					TIM_TimeBaseStructure.TIM_Prescaler=84;  
															TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
															TIM_TimeBaseStructure.TIM_Period=arr-1; 	
															break;
				
				case TIM_MS:					TIM_TimeBaseStructure.TIM_Prescaler=41999; 
															TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
															TIM_TimeBaseStructure.TIM_Period=arr*2-1; 	
															break;
				
				case TIM_S:						TIM_TimeBaseStructure.TIM_Prescaler=41999; 
															TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
															TIM_TimeBaseStructure.TIM_Period=arr*2000-1; 	
															break;
			}
		
		}
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;	//���ϼ���
		TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);					//����ȱʡֵ������Ӧ�Ķ�ʱ��
		TIM_ClearFlag(TIMx, TIM_FLAG_Update);							//�����Ӧ�Ķ�ʱ�����±�־
		TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE ); 						//��������ж�
		if(on_or_off == ON)
			TIM_Cmd(TIMx, ENABLE);											//ʹ����Ӧ�Ķ�ʱ��
		else
			;
}
	
/**
	*	@brief		��ʱ���Ĳ�������
	*	@param		TIMx						TIM1~TIM5						��Ӧ�Ķ�ʱ�����	
					TIM_Channel_x				TIM_Channel_1~TIM_Channel_4		��Ӧ�Ķ�ʱ��ͨ��
					TIM_ICPolarity_Mode	:
					#define  TIM_ICPolarity_Rising             ((uint16_t)0x0000)//�����ز���
					#define  TIM_ICPolarity_Falling            ((uint16_t)0x0002)//�½��ز���
					#define  TIM_ICPolarity_BothEdge           ((uint16_t)0x000A)//�����ز���
	*	@retval		NONE
	*/
void TIM_Capture_Config(TIM_TypeDef *TIMx, uint16_t TIM_Channel_x, uint16_t TIM_ICPolarity_Mode)
{
		TIM_ICInitTypeDef TIM_ICInitStructure;
	
//		TIM_DeInit(TIMx);	//��ʼ��TIM���Ĵ���
		TIM_ClearFlag(TIMx, TIM_FLAG_Update);		
	
		TIM_ICInitStructure.TIM_Channel		 	= 	TIM_Channel_x;			//ѡ��Ķ�ʱ��ͨ��
		TIM_ICInitStructure.TIM_ICPolarity 	= 	TIM_ICPolarity_Mode;		//����ģʽ
		TIM_ICInitStructure.TIM_ICSelection = 	TIM_ICSelection_DirectTI;	//ӳ��ͨ��(��ӳ��)
		TIM_ICInitStructure.TIM_ICPrescaler = 	TIM_ICPSC_DIV1;				//ʱ�ӷָ�(�˲�Ƶ��)
		TIM_ICInitStructure.TIM_ICFilter 		= 	0x00;					//�˲�����(�ɼ�������ƽ�Ĵ���)
	
		TIM_ICInit(TIMx, &TIM_ICInitStructure);
		TIM_ITConfig(TIMx, TIM_IT_CC1<<((uint16_t)(TIM_Channel_x-TIM_Channel_1)/4) , ENABLE);	//���ж�
		TIM_ClearFlag(TIMx, TIM_FLAG_CC1<<((uint16_t)(TIM_Channel_x-TIM_Channel_1)/4));
		TIM_Cmd(TIMx, ENABLE);       //ʹ�ܶ�ʱ��

}
	

/**
	*	@brief	��ض�ʱ����PWM����
	*	@param			TIMx	TIM1~TIM5			��Ӧ�Ķ�ʱ�����	
						Pulse1~Pulse4				��Ӧͨ����PWM�Ƚ�ֵ
	*	@retval			NONE		
	*/

void TIM_PWM_Config(TIM_TypeDef *TIMx,uint16_t Pulse1, uint16_t Pulse2, uint16_t Pulse3, uint16_t Pulse4)
{
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
			
	TIM_OCInitStructure.TIM_OCMode				 = 	TIM_OCMode_PWM1;		//�ȱȽ�ֵСʱΪ��Ч��ƽ
	TIM_OCInitStructure.TIM_OCPolarity		 =	TIM_OCPolarity_High;		//�ߵ�ƽΪ��Ч��ƽ
	TIM_OCInitStructure.TIM_OutputState		 = 	TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCIdleState    =  TIM_OCIdleState_Reset;
	
	if(Pulse1)
	{
		TIM_OCInitStructure.TIM_Pulse = Pulse1; 		//����ͨ��1�ıȽ�ֵ
		TIM_OC1Init(TIMx,&TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIMx,TIM_OCPreload_Enable);//ʹ��Ԥװ��
	}
	
	if(Pulse2)
	{
		TIM_OCInitStructure.TIM_Pulse = Pulse2;    		//����ͨ��2�ıȽ�ֵ
		TIM_OC2Init(TIMx,&TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIMx,TIM_OCPreload_Enable);
	}
	
	if(Pulse3)
	{
		TIM_OCInitStructure.TIM_Pulse = Pulse3;       	//����ͨ��3�ıȽ�ֵ
		TIM_OC3Init(TIMx,&TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIMx,TIM_OCPreload_Enable);
	}
	
	if(Pulse4)
	{
		TIM_OCInitStructure.TIM_Pulse = Pulse4;       	//����ͨ��4�ıȽ�ֵ
		TIM_OC4Init(TIMx,&TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIMx,TIM_OCPreload_Enable);
	}
	
	TIM_ARRPreloadConfig(TIMx, ENABLE);			 		// ʹ��TIMx����װ��ֵ
	
	/* TIM1 Main Output Enable ʹ��TIM1����������*/
	TIM_CtrlPWMOutputs(TIMx,ENABLE);
	
	TIM_Cmd(TIMx, ENABLE);
	
}


/**
	*	@brief	TIMX������ģʽ
	*	@param	channel_choose	������ͨ��ѡ��:	0��1��2ͨ��			1��3��4ͨ��
	*	@retval
	*/

void TIM_ENCODER_Config(TIM_TypeDef *TIMx,uint8_t channel_choose)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	if(TIMx == TIM1)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	if(TIMx == TIM8)
	{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler=0; 					//Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 	//ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_Period=65535; 				//�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;	//���ϼ���
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);	 				//����ȱʡֵ������Ӧ�Ķ�ʱ��
	
	TIM_EncoderInterfaceConfig(TIMx, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//�����زɼ�
	if(!channel_choose){
		TIMx->CCMR1 |= (0x0A<<12);	//ͨ��2�����벶׽�˲�������
		TIMx->CCMR1 |= (0x0A<<4);	//���벶׽�˲�������,��ο��ֲ�CCMR1�Ĵ�������
	}
	else{
		TIMx->CCMR2 |= (0x0A<<12);	//ͨ��4�����˲�������
		TIMx->CCMR2 |= (0X0A<<4);	//ͨ��3�����˲�������
	}
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);	//�����Ӧ�Ķ�ʱ�����±�־
	TIMx->CNT = 0;
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIMx, ENABLE);
	}

	else
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2<<(((uint32_t)TIMx-TIM2_BASE)/0x0400), ENABLE);

	TIM_TimeBaseStructure.TIM_Prescaler=0; 					//Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 	//ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_Period=65535; 				//�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;	//���ϼ���
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);					//����ȱʡֵ������Ӧ�Ķ�ʱ��
	
	TIM_EncoderInterfaceConfig(TIMx, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//�����زɼ�
	if(!channel_choose){
		TIMx->CCMR1 |= (0x0A<<12);	//ͨ��2�����벶׽�˲�������
		TIMx->CCMR1 |= (0x0A<<4);	//���벶׽�˲�������,��ο��ֲ�CCMR1�Ĵ�������
	}
	else{
		TIMx->CCMR2 |= (0x0A<<12);	//ͨ��4�����˲�������
		TIMx->CCMR2 |= (0X0A<<4);	//ͨ��3�����˲�������
	}
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);	//�����Ӧ�Ķ�ʱ�����±�־
	TIMx->CNT = 0;
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIMx, ENABLE);
}
}
/**/


/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/





