/*************************************************************************************
*	@file			motor_control.c
*	@supplement	以下伺服控制指令发送函数均用于控制铭朗控制器
* @author	 	yun
*	@version 	V1.0
*	@date			2016/7/25
* @brief		NONE
*************************************************************************************/

#include "motor_control.h"

/**
	*@brief 伺服控制器参数设置
	*@param 	servo_id		伺服控制器的id
						order				伺服控制器的命令
						len					发送的数据字节数
						data				需要发送的数据(指低4字节数据，即不加命令)
	*@retval	0：发送成功
						1：消息发送失败
						2：申请空闲消息盒子失败
	*/
uint8_t Servo_Param_Set(uint8_t servo_id,uint8_t order,uint8_t len,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//标准帧格式
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//数据帧
	Servo_Structure.Servo_ID    = servo_id;
	Servo_Structure.Data_Len		= len;
	Servo_Structure.Front_Data[0] = len;
	Servo_Structure.Front_Data[1] = servo_id;
	Servo_Structure.Front_Data[2] = order;
	Servo_Structure.Front_Data[3] = 0;
	Servo_Structure.Last_Data = data;
	error = Servo_Set_Function(&Servo_Structure);
	return error;
}

	
/**
	*@brief	伺服控制器发送指令函数
	*@param	*Servo_Structure		需要向伺服控制发送的指令参数
	*@retval	0：发送成功
						1：消息发送失败
						2：申请空闲消息盒子失败
	*/
uint8_t Servo_Set_Function(Servo_Struct *Servo_Structure)
{
	uint16_t i;
	uint8_t msgbox;
	CanTxMsg	TxMessage;
	
	TxMessage.IDE		= Servo_Structure->Data_Format;						//标准帧格式
	TxMessage.RTR		= Servo_Structure->Data_Type;							//数据帧
	TxMessage.StdId = Servo_Structure->Servo_ID;							//标准ID
	TxMessage.ExtId = 0;																			//扩展ID
	TxMessage.DLC 	= Servo_Structure->Data_Len;							//传送的数据长度,字节数
	for(i=0;i<4;i++){																					//传输数据
		TxMessage.Data[i]	= Servo_Structure->Front_Data[i];								
		TxMessage.Data[i+4] = ((Servo_Structure->Last_Data)>>(i*8))&0xff;
	}
	/******申请空闲消息盒子并提出发送申请******/
	i=0;
	while(msgbox = CAN_Transmit(CAN1,&TxMessage),(msgbox == CAN_TxStatus_NoMailBox ))
	{
		delay_us(200);
//		i++;
	}
//	if(i==5)	return 2;		//申请空闲消息盒子失败
	
	/******查询消息发送情况******/
//	i=0;
	while(!(CAN_TransmitStatus(CAN1, msgbox)==CAN_TxStatus_Ok));
//	i++;
//	while(CAN_TransmitStatus(CAN1, msgbox)==CAN_TxStatus_Pending );
//	if(i==1000)	return 1;	//消息发送失败
	
	/******消息发送成功*****/
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************为适应大多指令的输出,针对铭朗控制器封装部分指令**************************************/
/**					@brief：	用户在使用此函数前，需要先申请电机控制块,并设置相关参数
									
**************************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
	*@brief 伺服控制器参数初始化
	*@param
	*@retval
	*/
void Create_MLServo_CB(SERVO_MLCB		*servo_mlcb,					
											 char       	*servo_mlcb_name,			
											 uint8_t     	 star_id,											
											 uint8_t       ratio,								
											 uint16_t      coder)				  
{
	servo_mlcb->Servo_Mlcb_Name   = servo_mlcb_name;			//用户自定义驱动名称
	servo_mlcb->Star_Id						= star_id;							//标识符ID
	servo_mlcb->Device_Id					= star_id;							//设备ID
	servo_mlcb->Ratio							= ratio;								//电机传动比
	servo_mlcb->Coder 						= coder;								//编码器线数
	servo_mlcb->Servo_Error				= Write_Success;				//错误返回标志
}
/**
	*@brief 伺服控制器参数写入
	*@retval	0：发送成功
						1：消息发送失败
						2：申请空闲消息盒子失败
	*/
uint8_t MLServo_Write(SERVO_MLCB *Servo_MLCB, uint8_t order, uint8_t len, int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//标准帧格式
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//数据帧
	Servo_Structure.Servo_ID    = Servo_MLCB->Star_Id;
	Servo_Structure.Data_Len		= len;
	Servo_Structure.Front_Data[0] = len;
	Servo_Structure.Front_Data[1] = Servo_MLCB->Device_Id;
	Servo_Structure.Front_Data[2] = order;
	Servo_Structure.Front_Data[3] = 0;
	if				(		order==ML_SSP		/*设置最大速度*/\
							||order==ML_SMV		/*设置最小速度*/\
							||order==ML_A			/* 设置加速度 */\
							||order==ML_V			)/*设置运行速度*/
	Servo_Structure.Last_Data = (data*(Servo_MLCB->Ratio));
	
	else if		(		order==ML_PO		/*设置当前位置为绝对位置*/\
							||order==ML_M			/* 电机向设定位置移动   */\
							||order==ML_MR		)/*电机向设定位置移动(相对位置)*/
	Servo_Structure.Last_Data = (data*(Servo_MLCB->Ratio)*(4*(Servo_MLCB->Coder))/360);
	
	else
	Servo_Structure.Last_Data = data;
		
	error = Servo_Set_Function(&Servo_Structure);
	return error;
}

/*************************************************电机速度设置************************************************/
/*																								电机速度设置																							 */
/*************************************************电机速度设置************************************************/
/**
	*	@brief	电机最大速度设置
	*	@param	*Servo_MLCB	需要控制的电机控制块
	*					data				速度：转/分
	*	@retval	error:	0	数据发送成功	,其他：数据发送失败
	*/
uint8_t Motor_MastSpeed(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//标准帧格式
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//数据帧
	Servo_Structure.Servo_ID    = Servo_MLCB->Star_Id;
	Servo_Structure.Data_Len		= ML_SSP_LEN;
	
	Servo_Structure.Front_Data[0] = ML_SSP_LEN;
	Servo_Structure.Front_Data[1] = Servo_MLCB->Device_Id;
	Servo_Structure.Front_Data[2] = ML_SSP;
	Servo_Structure.Front_Data[3] = 0;
	
	Servo_Structure.Last_Data = (data*(Servo_MLCB->Ratio));
		
	error = Servo_Set_Function(&Servo_Structure);
	return error;
}

/**
	*	@brief	电机最小速度设置
	*	@param	*Servo_MLCB	需要控制的电机控制块
	*					data				速度：转/分
	*	@retval	error:	0	数据发送成功	,其他：数据发送失败
	*/
uint8_t Motor_MinSpeed(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//标准帧格式
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//数据帧
	Servo_Structure.Servo_ID    = Servo_MLCB->Star_Id;
	Servo_Structure.Data_Len		= ML_SMV_LEN;
	
	Servo_Structure.Front_Data[0] = ML_SMV_LEN;
	Servo_Structure.Front_Data[1] = Servo_MLCB->Device_Id;
	Servo_Structure.Front_Data[2] = ML_SMV;
	Servo_Structure.Front_Data[3] = 0;
	
	Servo_Structure.Last_Data = (data*(Servo_MLCB->Ratio));
		
	error = Servo_Set_Function(&Servo_Structure);
	return error;
}


/**
	*	@brief	电机运行速度设置,	电机会以此速度开始运行
	*	@param	*Servo_MLCB	需要控制的电机控制块
	*					data				速度：转/分
	*	@retval	error:	0	数据发送成功	,其他：数据发送失败
	*/
uint8_t Motor_SetSpeed(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//标准帧格式
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//数据帧
	Servo_Structure.Servo_ID    = Servo_MLCB->Star_Id;
	Servo_Structure.Data_Len		= ML_V_LEN;
	
	Servo_Structure.Front_Data[0] = ML_V_LEN;
	Servo_Structure.Front_Data[1] = Servo_MLCB->Device_Id;
	Servo_Structure.Front_Data[2] = ML_V;
	Servo_Structure.Front_Data[3] = 0;
	
	Servo_Structure.Last_Data = (data*(Servo_MLCB->Ratio));
		
	error = Servo_Set_Function(&Servo_Structure);
	return error;
}

/************************************************电机加速度设置**************************************************/
/*																							 电机加速度设置																									*/
/************************************************电机加速度设置**************************************************/
/**
	*	@brief	加速度设置	
	*	@param	*Servo_MLCB	需要控制的电机控制块
						data				控制量	转/(分*分)
	*	@retval	error	:	0	数据发送成功,		其他	数据发送失败
	*/
uint8_t Motor_AccelSpeed(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//标准帧格式
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//数据帧
	Servo_Structure.Servo_ID    = Servo_MLCB->Star_Id;
	Servo_Structure.Data_Len	= ML_A_LEN;
	
	Servo_Structure.Front_Data[0] = ML_A_LEN;
	Servo_Structure.Front_Data[1] = Servo_MLCB->Device_Id;
	Servo_Structure.Front_Data[2] = ML_A;
	Servo_Structure.Front_Data[3] = 0;
	
	Servo_Structure.Last_Data = (data*(Servo_MLCB->Ratio));
		
	error = Servo_Set_Function(&Servo_Structure);
	return error;
}

/***********************************************电机位置控制**************************************************/
/*																							电机位置控制																								 */
/***********************************************电机位置控制**************************************************/
/**
	*	@brief	电机绝对位置设置	设置当前位置为电机绝对位置
	*	@param	*Servo_MLCB	电机控制块
						data				控制量	角度
	*	@retval	error	0：数据发送正常		其他：数据发送失败
	*/
uint8_t Motor_AbsolutePlace(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//标准帧格式
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//数据帧
	Servo_Structure.Servo_ID    = Servo_MLCB->Star_Id;
	Servo_Structure.Data_Len		= ML_PO_LEN;
	
	Servo_Structure.Front_Data[0] = ML_PO_LEN;
	Servo_Structure.Front_Data[1] = Servo_MLCB->Device_Id;
	Servo_Structure.Front_Data[2] = ML_PO;
	Servo_Structure.Front_Data[3] = 0;
	
	Servo_Structure.Last_Data = (data*(Servo_MLCB->Ratio)*(4*(Servo_MLCB->Coder))/360);
		
	error = Servo_Set_Function(&Servo_Structure);
	return error;
}


/**
	*	@brief	电机绝对位置设置	设置完成后电机将向此绝对位置运行
	*	@param	*Servo_MLCB	电机控制块
						data				控制量	角度
	*	@retval	error	0：数据发送正常		其他：数据发送失败
	*/
uint8_t Motor_ToAbsolutePlace(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//标准帧格式
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//数据帧
	Servo_Structure.Servo_ID    = Servo_MLCB->Star_Id;
	Servo_Structure.Data_Len		= ML_M_LEN;
	
	Servo_Structure.Front_Data[0] = ML_M_LEN;
	Servo_Structure.Front_Data[1] = Servo_MLCB->Device_Id;
	Servo_Structure.Front_Data[2] = ML_M;
	Servo_Structure.Front_Data[3] = 0;
	
	Servo_Structure.Last_Data = (data*(Servo_MLCB->Ratio)*(4*(Servo_MLCB->Coder))/360);
		
	error = Servo_Set_Function(&Servo_Structure);
	return error;
}

/**
	*	@brief	电机相对位置设置	电机向设定位置开始移动
	*	@param	*Servo_MLCB	电机控制块
						data				控制量	角度
	*	@attention	相对位置为,相对于上次的设定位置,非相对于电机现在的实时位置
	*	@retval	error	0：数据发送正常		其他：数据发送失败
	*/
uint8_t Motor_ToRelativePlace(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//标准帧格式
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//数据帧
	Servo_Structure.Servo_ID    = Servo_MLCB->Star_Id;
	Servo_Structure.Data_Len		= ML_MR_LEN;
	
	Servo_Structure.Front_Data[0] = ML_MR_LEN;
	Servo_Structure.Front_Data[1] = Servo_MLCB->Device_Id;
	Servo_Structure.Front_Data[2] = ML_MR;
	Servo_Structure.Front_Data[3] = 0;
	
	Servo_Structure.Last_Data = (data*(Servo_MLCB->Ratio)*(4*(Servo_MLCB->Coder))/360);
		
	error = Servo_Set_Function(&Servo_Structure);
	return error;
}

/********************************************电机的初始化设置**********************************************/
/*																					 电机的初始化设置																							*/
/********************************************电机的初始化设置**********************************************/
/**
	*	@brief	电机位置比例设置	P
	*	@paaram	*Servo_MLCB	电机控制块
						data				控制量
	*	@retval	error	0：发送数据成功	其他：数据发送失败
	*/
uint8_t Motor_P_SET(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//标准帧格式
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//数据帧
	Servo_Structure.Servo_ID    = Servo_MLCB->Star_Id;
	Servo_Structure.Data_Len		= ML_MP_LEN;
	
	Servo_Structure.Front_Data[0] = ML_MP_LEN;
	Servo_Structure.Front_Data[1] = Servo_MLCB->Device_Id;
	Servo_Structure.Front_Data[2] = ML_MP;
	Servo_Structure.Front_Data[3] = 0;
	
	Servo_Structure.Last_Data = data;
		
	error = Servo_Set_Function(&Servo_Structure);
	return error;
}

/**
	*	@brief	电机的分辨率设置	编码器线数
	*	@param	*Servo_MLCB		需要设置的控制块
						data					编码器的分辨率
	*	@retval	error	0：发送数据成功	其他：数据发送失败
	*/
uint8_t Motor_Code_Set(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//标准帧格式
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//数据帧
	Servo_Structure.Servo_ID    = Servo_MLCB->Star_Id;
	Servo_Structure.Data_Len		= ML_ENC_LEN;
	
	Servo_Structure.Front_Data[0] = ML_ENC_LEN;
	Servo_Structure.Front_Data[1] = Servo_MLCB->Device_Id;
	Servo_Structure.Front_Data[2] = ML_ENC;
	Servo_Structure.Front_Data[3] = 0;
	
	Servo_Structure.Last_Data = (data*4);
		
	error = Servo_Set_Function(&Servo_Structure);
	return error;
}



/**
	*	@brief	电机的工作模式设置	速度模式
	*	@param	*Servo_MLCB		需要设置的控制块
	*	@retval	error	0：发送数据成功	其他：数据发送失败
	*/
uint8_t Motor_Speed_Mode(SERVO_MLCB *Servo_MLCB)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//标准帧格式
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//数据帧
	Servo_Structure.Servo_ID    = Servo_MLCB->Star_Id;
	Servo_Structure.Data_Len		= ML_SMOD_LEN;
	
	Servo_Structure.Front_Data[0] = ML_SMOD_LEN;
	Servo_Structure.Front_Data[1] = Servo_MLCB->Device_Id;
	Servo_Structure.Front_Data[2] = ML_SMOD;
	Servo_Structure.Front_Data[3] = 0;
	
	Servo_Structure.Last_Data = 0;
		
	error = Servo_Set_Function(&Servo_Structure);
	return error;
}


/**
	*	@brief	电机工作模式设置	位置(角度)模式
	*	@param	*Servo_MLCB		需要设置的控制块
	*	@retval	error	0：发送数据成功	其他：数据发送失败
	*/
uint8_t Motor_Position_Mode(SERVO_MLCB *Servo_MLCB)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//标准帧格式
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//数据帧
	Servo_Structure.Servo_ID    = Servo_MLCB->Star_Id;
	Servo_Structure.Data_Len		= ML_SMOD_LEN;
	
	Servo_Structure.Front_Data[0] = ML_SMOD_LEN;
	Servo_Structure.Front_Data[1] = Servo_MLCB->Device_Id;
	Servo_Structure.Front_Data[2] = ML_SMOD;
	Servo_Structure.Front_Data[3] = 0;
	
	Servo_Structure.Last_Data = 256;
		
	error = Servo_Set_Function(&Servo_Structure);
	return error;
}


/**
	*	@brief	保存以上设置参数
	*	@param	*Servo_MLCB		需要设置的控制块
	*	@retval	error	0：发送数据成功	其他：数据发送失败
	*/
uint8_t Motor_Save_Param(SERVO_MLCB *Servo_MLCB)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//标准帧格式
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//数据帧
	Servo_Structure.Servo_ID    = Servo_MLCB->Star_Id;
	Servo_Structure.Data_Len		= ML_ESA_LEN;
	
	Servo_Structure.Front_Data[0] = ML_ESA_LEN;
	Servo_Structure.Front_Data[1] = Servo_MLCB->Device_Id;
	Servo_Structure.Front_Data[2] = ML_ESA;
	Servo_Structure.Front_Data[3] = 0;
	
	Servo_Structure.Last_Data = 256;
		
	error = Servo_Set_Function(&Servo_Structure);
	return error;
}
