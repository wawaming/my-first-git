#include "BSP.h"
void BSP_Init(void)
{	
	/*		System install	*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	delay_init();	
	
	/*		Communication install	*/
	IIC_Init();
	CAN_Config(CAN_Mode_Normal);			//---CAN_Mode_LoopBack ----CAN_Mode_Normal
	NVIC_Config(CAN1_RX1_IRQn, 2, 1);//---FIFO0 RX_Interrupt
//	NVIC_Config(CAN1_SCE_IRQn, 2, 2);		//---Warning/Error	Interrupt
	
	Underpan_Motor_Init();
	
	/*		Debug install		*/
	LED_Init();
	KEY_Init();
	ROC_LCD_Init();
	RELAVY_Init();
	RL6_OFF;
	A_Usart_Config();
	TIM5_Init();
	TIM7_Init();
	TIM3_Init();
	BEED_Init();
 SERVO_Init();
	/*		Sensor	install		*/
	Encoder_Init();					//±àÂëÆ÷³õÊ¼»¯
	Model_Aircraft_Init();	
	DI_Init();
}





