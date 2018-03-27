/*************************************************************************************
*	@file			BSP.c
* @author	 	yun
*	@version 	V1.0
*	@date			2016/3/27
* @brief		初始化所需外设
*************************************************************************************/
#include "MYBSP.h"
//extern Locate_DATE	Locate_Date;

void BSP_Init(void)
{
	/*		System install	*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();	
	
	/*		Communication install	*/
	IIC_Init();
	CAN_Config(CAN_Mode_Normal);			//---CAN_Mode_LoopBack ----CAN_Mode_Normal
	NVIC_Config(CAN1_RX1_IRQn, 2, 1);//---FIFO0 RX_Interrupt
	NVIC_Config(CAN1_SCE_IRQn, 2, 2);		//---Warning/Error	Interrupt
	
	/*		Debug install		*/
	LED_Init();
	BEED_Init();
	ROC_LCD_Init();
	A_Usart_Config();/*B车串口配置*/
	
	/*		Sensor	install		*/
	Encoder_Init();					//编码器初始化
	Underpan_Motor_Init();			//底盘电机初始化
	
	/*		MPU6050		*/
	TIM5_Init();
	TIM_Cmd(TIM3, ENABLE);
//	while(MPU_Init());				//MPU6050姿态模块初始化	

	/*	全场定位系统初始化	*/
//	LOCATE_INIT(&Locate_Date);
}


















