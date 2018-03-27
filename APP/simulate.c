///////////////////////////////////////////////////////////////////
//										*		simulate		*
//				仿生机构落盘动作 共四个动作 		
//										存盘		
//										预装盘
//										装盘
//										发射
//				摩擦机构	两个动作
//										装盘
//										击盘
//						俯仰角调节	分为粗调 、微调
//	*@author		
//	*@vertion		V1.0
//	*@date			2017/3/11
////////////////////////////////////////////////////////////////////

#include "simulate.h"

/*	仿生   参数待修改	*/
uint16_t  safeplace_345 = 500;
uint16_t Launchplace = 1050;

uint16_t SERVO_Zhong_Place  = 500;
uint16_t SERVO_Kai_Place   =  600;
uint16_t SERVO_Guan_Place  =  1000;
uint8_t PWM_adjust;
uint8_t Place_adjust;
extern Robocon_Handle_Struct Rb_Handle_Control;
extern uint8_t flag_off;
extern int Launchplace_3m1;
extern int Launchplace_6m2;
extern int Launchplace_6m3;
extern int Launchplace_6m4;
extern int Launchplace_6m5;
extern int Launchplace_6m6;
extern int Launchplace_9m7;

extern int32_t Encoder_TIM2;				//65536*示数+TIMx->CNT
void  cunpan(void)
{
		RL1_ON;
		delay_ms(2000);
		RL1_OFF;

}

void  yu_zhuangpan(uint8_t fashe_x)
{
		//判断电磁阀3、4、5的状态
		 if(GPIO_ReadOutputDataBit(GPIOG, GPIO_Pin_4)& \
														GPIO_ReadOutputDataBit(GPIOG, GPIO_Pin_4)& GPIO_ReadOutputDataBit(GPIOG, GPIO_Pin_4))
		 {
			 RL6_ON;										//打开夹具气缸
			 delay_ms(20);
		  	switch(fashe_x){
					
					case 1: 
								RL1_ON;
						break;
					case 2: 
						break;
					case 3:
						break;
					case 4: 
						break;
					case 5: 
						break;
					case 6:
						break;
					case 7:
						break;				
					
					default: break;
				}
				
				while(65536*Encoder_TIM2+TIM2->CNT >= safeplace_345);
		 }
		 else{
				switch(fashe_x){
					
					case 1: 
								RL1_ON;
								RL2_OFF;
								RL3_OFF;
						break;
					case 2: 
						break;
					case 3:
						break;						
					
					default: break;			 
				}
				RL6_ON;		 
		 
		 }
		 
				SERVO_PWM1 = SERVO_Zhong_Place;			 
				RL2_ON;
				
				RL2_OFF;
				SERVO_PWM1 = SERVO_Kai_Place ;
		 
}

uint8_t  zhuangpan(uint8_t fashe_x)
{
			uint8_t i_1 =0;
	
			switch(fashe_x){
			
				case 1:
							RL1_OFF;
					break;
				case 2:
							RL2_OFF;
					break;
				case 3:
							RL3_OFF;
					break;
				default:	break;											
			}
			
			while(i_1++,65536*Encoder_TIM2+TIM2->CNT <= safeplace_345)	//待修改
				delay_ms(100);				
					
			RL6_OFF;
			SERVO_PWM1 = SERVO_Kai_Place;
			
			if(i_1 >= 50)
				return 0;
			else
				return 1;
}

uint8_t  fashe(uint8_t fashe_x)
{			
			uint16_t i_2 =0;
			switch(fashe_x){
			
				case 1:
							RL1_ON;
					break;
				case 2:
							RL2_ON;
					break;
				case 3:
							RL3_ON;
					break;
				default:	break;											
			}
			
			while(i_2++, 65536*Encoder_TIM2+TIM2->CNT <= Launchplace)		//待修改
					delay_ms(1);			
			RL6_ON;
			
			if(i_2 >= 2000)
					return 0;   				//时间异常  返回错误信息
			else
					return 1;			
}

/*	摩擦落盘	*/
void qidong(uint8_t jipan_x)
{
		switch (jipan_x){
			
			case 3:
			{
					Model_Aircraft_1 = Model_Aircraft_1_3;				//加速方式待修改
				
			}
			break;
			case 5:
			{
					Model_Aircraft_1 = Model_Aircraft_1_5;				
			}
			break;		
			case 9:
			{
					Model_Aircraft_1 = Model_Aircraft_1_9;
				
			}
			break;
			default :	break;
				
		}

}

void jipan(void)
{
	SERVO_PWM2 = SERVO_KAI;		//打开舵机，装盘	
	RL7_ON;										//打开电磁阀，送盘

}



extern int32_t  bru_speed;
void Friction_Falling(int distance_pwm,int distance_speed)
{
	PWM_adjust = Rb_Handle_Control.Rub_Speed._int_date;
	Model_Aircraft_1 = distance_pwm + PWM_adjust;
	while(!((distance_speed-30)<=(abs(bru_speed))<=(distance_speed+30)));
	RL1_ON;
	delay_ms(1000);
	RL1_OFF;
	Correspond_Receipt();
	
}

/*结构体部分成员清零*/
void Clear_Struct(char *pta,int size)
{
	 uint8_t i=4;
   for(;i<=(size-2);i++)
	 {
	 *pta++=0;
	 }
}




/*仿生*/
void Falling_3m1()
{ 
	Place_adjust = Rb_Handle_Control.Simulate_Position._int_date;
	Launchplace = Launchplace_3m1 + Place_adjust;
	RL3_ON;
	delay_ms(1000);
	
	RL1_ON;
	while((Encoder_TIM2*65536+TIM2->CNT)>=2);
	RL3_OFF;
	RL2_OFF;
	
	RL4_ON;
	RL5_OFF;
	RL6_OFF;
	RL7_OFF;
	RL8_OFF;
	
	flag_off = 1;
	Correspond_Receipt();	
}



