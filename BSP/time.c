/*************************************************************************************
*	@file			time.c
* @author	 	yun
*	@version 	V1.0
*	@date			2017/3/3
* @brief		NONE
*************************************************************************************/

#include "time.h"

/**
	*@brief		通用定时器的定时功能配置
	*@param		TIMx		TIM1~TIM5	相应的定时器编号	TIM2~TIM5为通用定时器
				arr						定时器重装值
				TIM_Size	TIM_MIN,	最小定时器		
				TIM_US ,				微秒定时器
				TIM_MS,					毫秒定时器
				TIM_S,					秒定时器
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
				case TIM_MIN:       	TIM_TimeBaseStructure.TIM_Prescaler=0; 					//预分频值
															TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 	//时钟分割
															TIM_TimeBaseStructure.TIM_Period=arr-1; 				//自动重装载值
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
				case TIM_MIN:       	TIM_TimeBaseStructure.TIM_Prescaler=0; 					//预分频值
															TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 	//时钟分割
															TIM_TimeBaseStructure.TIM_Period=arr-1; 				//自动重装载值
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
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;	//向上计数
		TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);					//根据缺省值配置相应的定时器
		TIM_ClearFlag(TIMx, TIM_FLAG_Update);							//清除相应的定时器更新标志
		TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE ); 						//允许更新中断
		if(on_or_off == ON)
			TIM_Cmd(TIMx, ENABLE);											//使能相应的定时器
		else
			;
}
	
/**
	*	@brief		定时器的捕获设置
	*	@param		TIMx						TIM1~TIM5						相应的定时器编号	
					TIM_Channel_x				TIM_Channel_1~TIM_Channel_4		相应的定时器通道
					TIM_ICPolarity_Mode	:
					#define  TIM_ICPolarity_Rising             ((uint16_t)0x0000)//上升沿捕获
					#define  TIM_ICPolarity_Falling            ((uint16_t)0x0002)//下降沿捕获
					#define  TIM_ICPolarity_BothEdge           ((uint16_t)0x000A)//跳变沿捕获
	*	@retval		NONE
	*/
void TIM_Capture_Config(TIM_TypeDef *TIMx, uint16_t TIM_Channel_x, uint16_t TIM_ICPolarity_Mode)
{
		TIM_ICInitTypeDef TIM_ICInitStructure;
	
//		TIM_DeInit(TIMx);	//初始化TIM各寄存器
		TIM_ClearFlag(TIMx, TIM_FLAG_Update);		
	
		TIM_ICInitStructure.TIM_Channel		 	= 	TIM_Channel_x;			//选择的定时器通道
		TIM_ICInitStructure.TIM_ICPolarity 	= 	TIM_ICPolarity_Mode;		//捕获模式
		TIM_ICInitStructure.TIM_ICSelection = 	TIM_ICSelection_DirectTI;	//映射通道(不映射)
		TIM_ICInitStructure.TIM_ICPrescaler = 	TIM_ICPSC_DIV1;				//时钟分割(滤波频率)
		TIM_ICInitStructure.TIM_ICFilter 		= 	0x00;					//滤波周期(采集连续电平的次数)
	
		TIM_ICInit(TIMx, &TIM_ICInitStructure);
		TIM_ITConfig(TIMx, TIM_IT_CC1<<((uint16_t)(TIM_Channel_x-TIM_Channel_1)/4) , ENABLE);	//打开中断
		TIM_ClearFlag(TIMx, TIM_FLAG_CC1<<((uint16_t)(TIM_Channel_x-TIM_Channel_1)/4));
		TIM_Cmd(TIMx, ENABLE);       //使能定时器

}
	

/**
	*	@brief	相关定时器的PWM配置
	*	@param			TIMx	TIM1~TIM5			相应的定时器编号	
						Pulse1~Pulse4				相应通道的PWM比较值
	*	@retval			NONE		
	*/

void TIM_PWM_Config(TIM_TypeDef *TIMx,uint16_t Pulse1, uint16_t Pulse2, uint16_t Pulse3, uint16_t Pulse4)
{
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
			
	TIM_OCInitStructure.TIM_OCMode				 = 	TIM_OCMode_PWM1;		//比比较值小时为有效电平
	TIM_OCInitStructure.TIM_OCPolarity		 =	TIM_OCPolarity_High;		//高电平为有效电平
	TIM_OCInitStructure.TIM_OutputState		 = 	TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCIdleState    =  TIM_OCIdleState_Reset;
	
	if(Pulse1)
	{
		TIM_OCInitStructure.TIM_Pulse = Pulse1; 		//设置通道1的比较值
		TIM_OC1Init(TIMx,&TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIMx,TIM_OCPreload_Enable);//使能预装载
	}
	
	if(Pulse2)
	{
		TIM_OCInitStructure.TIM_Pulse = Pulse2;    		//设置通道2的比较值
		TIM_OC2Init(TIMx,&TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIMx,TIM_OCPreload_Enable);
	}
	
	if(Pulse3)
	{
		TIM_OCInitStructure.TIM_Pulse = Pulse3;       	//设置通道3的比较值
		TIM_OC3Init(TIMx,&TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIMx,TIM_OCPreload_Enable);
	}
	
	if(Pulse4)
	{
		TIM_OCInitStructure.TIM_Pulse = Pulse4;       	//设置通道4的比较值
		TIM_OC4Init(TIMx,&TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIMx,TIM_OCPreload_Enable);
	}
	
	TIM_ARRPreloadConfig(TIMx, ENABLE);			 		// 使能TIMx的重装载值
	
	/* TIM1 Main Output Enable 使能TIM1外设的主输出*/
	TIM_CtrlPWMOutputs(TIMx,ENABLE);
	
	TIM_Cmd(TIMx, ENABLE);
	
}


/**
	*	@brief	TIMX编码器模式
	*	@param	channel_choose	编码器通道选择:	0：1、2通道			1：3、4通道
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
	TIM_TimeBaseStructure.TIM_Prescaler=0; 					//预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 	//时钟分割
	TIM_TimeBaseStructure.TIM_Period=65535; 				//自动重装载值
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;	//向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);	 				//根据缺省值配置相应的定时器
	
	TIM_EncoderInterfaceConfig(TIMx, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//上升沿采集
	if(!channel_choose){
		TIMx->CCMR1 |= (0x0A<<12);	//通道2的输入捕捉滤波器设置
		TIMx->CCMR1 |= (0x0A<<4);	//输入捕捉滤波器设置,查参考手册CCMR1寄存器配置
	}
	else{
		TIMx->CCMR2 |= (0x0A<<12);	//通道4输入滤波器配置
		TIMx->CCMR2 |= (0X0A<<4);	//通道3输入滤波器配置
	}
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);	//清除相应的定时器更新标志
	TIMx->CNT = 0;
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIMx, ENABLE);
	}

	else
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2<<(((uint32_t)TIMx-TIM2_BASE)/0x0400), ENABLE);

	TIM_TimeBaseStructure.TIM_Prescaler=0; 					//预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 	//时钟分割
	TIM_TimeBaseStructure.TIM_Period=65535; 				//自动重装载值
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;	//向上计数
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);					//根据缺省值配置相应的定时器
	
	TIM_EncoderInterfaceConfig(TIMx, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//上升沿采集
	if(!channel_choose){
		TIMx->CCMR1 |= (0x0A<<12);	//通道2的输入捕捉滤波器设置
		TIMx->CCMR1 |= (0x0A<<4);	//输入捕捉滤波器设置,查参考手册CCMR1寄存器配置
	}
	else{
		TIMx->CCMR2 |= (0x0A<<12);	//通道4输入滤波器配置
		TIMx->CCMR2 |= (0X0A<<4);	//通道3输入滤波器配置
	}
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);	//清除相应的定时器更新标志
	TIMx->CNT = 0;
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIMx, ENABLE);
}
}
/**/


/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/





