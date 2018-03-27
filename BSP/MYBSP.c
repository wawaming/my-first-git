/*************************************************************************************
*	@file			BSP.c
* @author	 	yun
*	@version 	V1.0
*	@date			2016/3/27
* @brief		��ʼ����������
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
	A_Usart_Config();/*B����������*/
	
	/*		Sensor	install		*/
	Encoder_Init();					//��������ʼ��
	Underpan_Motor_Init();			//���̵����ʼ��
	
	/*		MPU6050		*/
	TIM5_Init();
	TIM_Cmd(TIM3, ENABLE);
//	while(MPU_Init());				//MPU6050��̬ģ���ʼ��	

	/*	ȫ����λϵͳ��ʼ��	*/
//	LOCATE_INIT(&Locate_Date);
}


















