/*************************************************************************************
*	@file			motor_control.c
*	@supplement	�����ŷ�����ָ��ͺ��������ڿ������ʿ�����
* @author	 	yun
*	@version 	V1.0
*	@date			2016/7/25
* @brief		NONE
*************************************************************************************/

#include "motor_control.h"

/**
	*@brief �ŷ���������������
	*@param 	servo_id		�ŷ���������id
						order				�ŷ�������������
						len					���͵������ֽ���
						data				��Ҫ���͵�����(ָ��4�ֽ����ݣ�����������)
	*@retval	0�����ͳɹ�
						1����Ϣ����ʧ��
						2�����������Ϣ����ʧ��
	*/
uint8_t Servo_Param_Set(uint8_t servo_id,uint8_t order,uint8_t len,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//��׼֡��ʽ
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//����֡
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
	*@brief	�ŷ�����������ָ���
	*@param	*Servo_Structure		��Ҫ���ŷ����Ʒ��͵�ָ�����
	*@retval	0�����ͳɹ�
						1����Ϣ����ʧ��
						2�����������Ϣ����ʧ��
	*/
uint8_t Servo_Set_Function(Servo_Struct *Servo_Structure)
{
	uint16_t i;
	uint8_t msgbox;
	CanTxMsg	TxMessage;
	
	TxMessage.IDE		= Servo_Structure->Data_Format;						//��׼֡��ʽ
	TxMessage.RTR		= Servo_Structure->Data_Type;							//����֡
	TxMessage.StdId = Servo_Structure->Servo_ID;							//��׼ID
	TxMessage.ExtId = 0;																			//��չID
	TxMessage.DLC 	= Servo_Structure->Data_Len;							//���͵����ݳ���,�ֽ���
	for(i=0;i<4;i++){																					//��������
		TxMessage.Data[i]	= Servo_Structure->Front_Data[i];								
		TxMessage.Data[i+4] = ((Servo_Structure->Last_Data)>>(i*8))&0xff;
	}
	/******���������Ϣ���Ӳ������������******/
	i=0;
	while(msgbox = CAN_Transmit(CAN1,&TxMessage),(msgbox == CAN_TxStatus_NoMailBox ))
	{
		delay_us(200);
//		i++;
	}
//	if(i==5)	return 2;		//���������Ϣ����ʧ��
	
	/******��ѯ��Ϣ�������******/
//	i=0;
	while(!(CAN_TransmitStatus(CAN1, msgbox)==CAN_TxStatus_Ok));
//	i++;
//	while(CAN_TransmitStatus(CAN1, msgbox)==CAN_TxStatus_Pending );
//	if(i==1000)	return 1;	//��Ϣ����ʧ��
	
	/******��Ϣ���ͳɹ�*****/
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************Ϊ��Ӧ���ָ������,������ʿ�������װ����ָ��**************************************/
/**					@brief��	�û���ʹ�ô˺���ǰ����Ҫ�����������ƿ�,��������ز���
									
**************************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
	*@brief �ŷ�������������ʼ��
	*@param
	*@retval
	*/
void Create_MLServo_CB(SERVO_MLCB		*servo_mlcb,					
											 char       	*servo_mlcb_name,			
											 uint8_t     	 star_id,											
											 uint8_t       ratio,								
											 uint16_t      coder)				  
{
	servo_mlcb->Servo_Mlcb_Name   = servo_mlcb_name;			//�û��Զ�����������
	servo_mlcb->Star_Id						= star_id;							//��ʶ��ID
	servo_mlcb->Device_Id					= star_id;							//�豸ID
	servo_mlcb->Ratio							= ratio;								//���������
	servo_mlcb->Coder 						= coder;								//����������
	servo_mlcb->Servo_Error				= Write_Success;				//���󷵻ر�־
}
/**
	*@brief �ŷ�����������д��
	*@retval	0�����ͳɹ�
						1����Ϣ����ʧ��
						2�����������Ϣ����ʧ��
	*/
uint8_t MLServo_Write(SERVO_MLCB *Servo_MLCB, uint8_t order, uint8_t len, int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//��׼֡��ʽ
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//����֡
	Servo_Structure.Servo_ID    = Servo_MLCB->Star_Id;
	Servo_Structure.Data_Len		= len;
	Servo_Structure.Front_Data[0] = len;
	Servo_Structure.Front_Data[1] = Servo_MLCB->Device_Id;
	Servo_Structure.Front_Data[2] = order;
	Servo_Structure.Front_Data[3] = 0;
	if				(		order==ML_SSP		/*��������ٶ�*/\
							||order==ML_SMV		/*������С�ٶ�*/\
							||order==ML_A			/* ���ü��ٶ� */\
							||order==ML_V			)/*���������ٶ�*/
	Servo_Structure.Last_Data = (data*(Servo_MLCB->Ratio));
	
	else if		(		order==ML_PO		/*���õ�ǰλ��Ϊ����λ��*/\
							||order==ML_M			/* ������趨λ���ƶ�   */\
							||order==ML_MR		)/*������趨λ���ƶ�(���λ��)*/
	Servo_Structure.Last_Data = (data*(Servo_MLCB->Ratio)*(4*(Servo_MLCB->Coder))/360);
	
	else
	Servo_Structure.Last_Data = data;
		
	error = Servo_Set_Function(&Servo_Structure);
	return error;
}

/*************************************************����ٶ�����************************************************/
/*																								����ٶ�����																							 */
/*************************************************����ٶ�����************************************************/
/**
	*	@brief	�������ٶ�����
	*	@param	*Servo_MLCB	��Ҫ���Ƶĵ�����ƿ�
	*					data				�ٶȣ�ת/��
	*	@retval	error:	0	���ݷ��ͳɹ�	,���������ݷ���ʧ��
	*/
uint8_t Motor_MastSpeed(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//��׼֡��ʽ
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//����֡
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
	*	@brief	�����С�ٶ�����
	*	@param	*Servo_MLCB	��Ҫ���Ƶĵ�����ƿ�
	*					data				�ٶȣ�ת/��
	*	@retval	error:	0	���ݷ��ͳɹ�	,���������ݷ���ʧ��
	*/
uint8_t Motor_MinSpeed(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//��׼֡��ʽ
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//����֡
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
	*	@brief	��������ٶ�����,	������Դ��ٶȿ�ʼ����
	*	@param	*Servo_MLCB	��Ҫ���Ƶĵ�����ƿ�
	*					data				�ٶȣ�ת/��
	*	@retval	error:	0	���ݷ��ͳɹ�	,���������ݷ���ʧ��
	*/
uint8_t Motor_SetSpeed(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//��׼֡��ʽ
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//����֡
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

/************************************************������ٶ�����**************************************************/
/*																							 ������ٶ�����																									*/
/************************************************������ٶ�����**************************************************/
/**
	*	@brief	���ٶ�����	
	*	@param	*Servo_MLCB	��Ҫ���Ƶĵ�����ƿ�
						data				������	ת/(��*��)
	*	@retval	error	:	0	���ݷ��ͳɹ�,		����	���ݷ���ʧ��
	*/
uint8_t Motor_AccelSpeed(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//��׼֡��ʽ
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//����֡
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

/***********************************************���λ�ÿ���**************************************************/
/*																							���λ�ÿ���																								 */
/***********************************************���λ�ÿ���**************************************************/
/**
	*	@brief	�������λ������	���õ�ǰλ��Ϊ�������λ��
	*	@param	*Servo_MLCB	������ƿ�
						data				������	�Ƕ�
	*	@retval	error	0�����ݷ�������		���������ݷ���ʧ��
	*/
uint8_t Motor_AbsolutePlace(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//��׼֡��ʽ
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//����֡
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
	*	@brief	�������λ������	������ɺ�������˾���λ������
	*	@param	*Servo_MLCB	������ƿ�
						data				������	�Ƕ�
	*	@retval	error	0�����ݷ�������		���������ݷ���ʧ��
	*/
uint8_t Motor_ToAbsolutePlace(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//��׼֡��ʽ
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//����֡
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
	*	@brief	������λ������	������趨λ�ÿ�ʼ�ƶ�
	*	@param	*Servo_MLCB	������ƿ�
						data				������	�Ƕ�
	*	@attention	���λ��Ϊ,������ϴε��趨λ��,������ڵ�����ڵ�ʵʱλ��
	*	@retval	error	0�����ݷ�������		���������ݷ���ʧ��
	*/
uint8_t Motor_ToRelativePlace(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//��׼֡��ʽ
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//����֡
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

/********************************************����ĳ�ʼ������**********************************************/
/*																					 ����ĳ�ʼ������																							*/
/********************************************����ĳ�ʼ������**********************************************/
/**
	*	@brief	���λ�ñ�������	P
	*	@paaram	*Servo_MLCB	������ƿ�
						data				������
	*	@retval	error	0���������ݳɹ�	���������ݷ���ʧ��
	*/
uint8_t Motor_P_SET(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//��׼֡��ʽ
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//����֡
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
	*	@brief	����ķֱ�������	����������
	*	@param	*Servo_MLCB		��Ҫ���õĿ��ƿ�
						data					�������ķֱ���
	*	@retval	error	0���������ݳɹ�	���������ݷ���ʧ��
	*/
uint8_t Motor_Code_Set(SERVO_MLCB *Servo_MLCB,int32_t data)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//��׼֡��ʽ
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//����֡
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
	*	@brief	����Ĺ���ģʽ����	�ٶ�ģʽ
	*	@param	*Servo_MLCB		��Ҫ���õĿ��ƿ�
	*	@retval	error	0���������ݳɹ�	���������ݷ���ʧ��
	*/
uint8_t Motor_Speed_Mode(SERVO_MLCB *Servo_MLCB)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//��׼֡��ʽ
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//����֡
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
	*	@brief	�������ģʽ����	λ��(�Ƕ�)ģʽ
	*	@param	*Servo_MLCB		��Ҫ���õĿ��ƿ�
	*	@retval	error	0���������ݳɹ�	���������ݷ���ʧ��
	*/
uint8_t Motor_Position_Mode(SERVO_MLCB *Servo_MLCB)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//��׼֡��ʽ
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//����֡
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
	*	@brief	�����������ò���
	*	@param	*Servo_MLCB		��Ҫ���õĿ��ƿ�
	*	@retval	error	0���������ݳɹ�	���������ݷ���ʧ��
	*/
uint8_t Motor_Save_Param(SERVO_MLCB *Servo_MLCB)
{
	uint8_t error;
	Servo_Struct Servo_Structure;
	Servo_Structure.Data_Format = CAN_Id_Standard;					//��׼֡��ʽ
	Servo_Structure.Data_Type   = CAN_RTR_Data;							//����֡
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
