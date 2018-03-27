//////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************/
/**	
	*	@brief	CAN接收数据处理
	*/
/********************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////

#include "CAN_RECIVE.h"


/*******************************************************************************************
*																			*	全局变量段	*																			 *
********************************************************************************************/
/*	摄像头偏差	*/
extern int16_t Camera_Offset;

/*	底盘电机速度	*/
extern int Under_Motor1[2];
extern int Under_Motor2[2];
extern int Under_Motor3[2];
extern int Under_Motor4[2];
extern uint8_t CAN_RE[8];
/**
	*	@brief
	*	@param
	*/
void CAN_Recive_handle(void)
{
	uint8_t i;
	INT_CHAR_UNION under_motor;
	CanRxMsg	CAMERA_Msg;
	CAMERA_Msg.StdId = 0x00;
	CAMERA_Msg.RTR = 0x00;
	CAMERA_Msg.IDE = 0x00;
	CAMERA_Msg.ExtId = 0x00;
	CAMERA_Msg.DLC = 0x00;
	CAMERA_Msg.Data[0] = 0x00;
	CAMERA_Msg.Data[1] = 0x00;
	CAMERA_Msg.Data[2] = 0x00;
	CAMERA_Msg.Data[3] = 0x00;
	CAMERA_Msg.Data[4] = 0x00;
	CAMERA_Msg.Data[5] = 0x00;
	CAMERA_Msg.Data[6] = 0x00;
	CAMERA_Msg.Data[7] = 0x00;
	
	CAN_Receive(CAN1, CAN_FIFO0, &CAMERA_Msg);
#if 1
	/*	*	*	*	*	*	*	*	*	摄像头数据接收	*	*	*	*	*	*	*	*	*	*/
	if(CAMERA_Msg.StdId == CAMERA_LEFT_ID)
	{
		if(CAMERA_Msg.Data[2])
		{
			Camera_Offset = (int16_t)CAMERA_Msg.Data[3];
		}
		else
		{
			Camera_Offset = -(int16_t)CAMERA_Msg.Data[3];
		}
	}
	else if(CAMERA_Msg.StdId == CAMERA_RIGHT_ID)
	{
		if(CAMERA_Msg.Data[2])
		{
			Camera_Offset = (int16_t)CAMERA_Msg.Data[3];
		}
		else
		{
			Camera_Offset = -(int16_t)CAMERA_Msg.Data[3];
		}
	}
	/*	*	*	*	*	*	*	*	*	底盘电机速度反馈	*	*	*	*	*	*	*	*	*	*/
	/*									*	读取底盘电机1速度	*								*/
	else if((CAMERA_Msg.Data[1] == DEVICE1_ID)&&(CAMERA_Msg.Data[2] == ML_GV))
	{
		for(i=0;i<4;i++)
		{
			under_motor.char_data[i] = CAMERA_Msg.Data[4+i];
		}
		Under_Motor1[0] = 1;
		Under_Motor1[1] = under_motor.int_data;
	}
	else if((CAMERA_Msg.Data[1] == DEVICE2_ID)&&(CAMERA_Msg.Data[2] == ML_GV))
	{
		for(i=0;i<4;i++)
		{
			under_motor.char_data[i] = CAMERA_Msg.Data[4+i];
		}
		Under_Motor2[0] = 1;
		Under_Motor2[1] = under_motor.int_data;
	}
	else if((CAMERA_Msg.Data[1] == DEVICE3_ID)&&(CAMERA_Msg.Data[2] == ML_GV))
	{
		for(i=0;i<4;i++)
		{
			under_motor.char_data[i] = CAMERA_Msg.Data[4+i];
		}
		Under_Motor3[0] = 1;
		Under_Motor3[1] = under_motor.int_data;
	}
	else if((CAMERA_Msg.Data[1]== DEVICE4_ID)&&(CAMERA_Msg.Data[2] == ML_GV))
	{
		for(i=0;i<4;i++)
		{
			under_motor.char_data[i] = CAMERA_Msg.Data[4+i];
		}
		Under_Motor4[0] = 1;
		Under_Motor4[1] = under_motor.int_data;
	}
	else{
//		LCD_Display_int(CAMERA_Msg.Data[1],5,3);
	}
#else
	for(i=0; i<8; i++)
	{
		CAN_RE[i] = CAMERA_Msg.Data[i];
		CAN_RE[i] = i;
	}
#endif
}
























