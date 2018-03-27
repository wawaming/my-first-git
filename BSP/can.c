/*************************************************************************************
*	@file			can.c
* @author	 	yun
*	@version 	V1.0
*	@date			2016/7/25
* @brief		NONE
*************************************************************************************/

#include "can.h"

/**
	*@breif	can ͨ�ŵ�����
	*@param	 CAN_MODE��	CAN_Mode_Normal						����ģʽ
											CAN_Mode_LoopBack					�ػ�ģʽ
											CAN_Mode_Silent						��Ĭģʽ
											CAN_Mode_Silent_LoopBack		��Ĭ�ػ�ģʽ
	*@retval NONE
	*/
void CAN_Config(uint8_t CAN_MODE)
{
	CAN_InitTypeDef	CAN_InitStructure;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);		//stm32f10xϵ��ֻ��CAN1ģ��(������������)��stm32f40xϵ��ӵ��CAN1/CAN2ģ��
	
	/*****����CANͨ�ŵ�IO��*****/
	#if	CAN_REMAP
		GPIO_Config(GPIOD, GPIO_Pin_0, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
		GPIO_Config(GPIOD, GPIO_Pin_1, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);			//��ӳ��ΪRemap2����Remap1
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
	#else
		GPIO_Config(GPIOA, GPIO_Pin_11, GPIO_Mode_IPU);				//�������룬��CAN_RX
		GPIO_Config(GPIOA, GPIO_Pin_12, GPIO_Mode_Out_PP);		//�����������CAN_TX
	#endif
	
	CAN_DeInit(CAN1);//����CAN
	CAN_StructInit(&CAN_InitStructure);
	/*****CAN������ã���������CAN�����ƼĴ���(CAN_MCR)*****/
	CAN_InitStructure.CAN_ABOM = ENABLE;		//������߹���(canģ�������ģʽ�ָ�ʱ����Ҫ���������bxCAN��Ż�ִ��Ӳ����⣬�ָ�����)
	CAN_InitStructure.CAN_AWUM = DISABLE;		//ͨ���������˯��ģʽ�µĻ���
	CAN_InitStructure.CAN_Mode = CAN_MODE;	//����ģʽ��
	CAN_InitStructure.CAN_NART = ENABLE;		//��ֹ���ķ���ʧ�ܺ�����Զ��ط�					DISABLE(OLD PARAM)
	CAN_InitStructure.CAN_RFLM = DISABLE;		//���Ĳ��������µı��ĻḲ�Ǿɵ�
	CAN_InitStructure.CAN_TTCM = DISABLE;		//��ʱ�䴥��ģʽ(ʱ�䴥��ģʽ�£������������Է��ͣ������¼�����)��ʱ�䴥��ģʽ��Ҫ����ʵʱ��Ҫ�󼫸ߵ����
	CAN_InitStructure.CAN_TXFP = ENABLE;		//���ķ��͵����ȼ��ɱ�ʶ������
	
	/*****CAN	����������		CAN_BAUDRATE = 36M/(CAN_Prescaler*(1+CAN_BS1+CAN_BS2))	APB1��ʱ��Ϊ36M*****/
	#if		CAN_BAUDRATE == 1000	/* 1MBPs */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;			//(TS1[3:0]+1)	
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;			//(TS2[2:0]+1)
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;			//(SJW[1:0]+1)		��ͬ���β������벨���ʼ���,��������ͬ���κ�Ϊ1tq
	CAN_InitStructure.CAN_Prescaler = 4;					//(BRP[9:0]+1)		�˴����õ�ΪBRP��ֵ����˷�Ƶϵ��Ϊ����ֵ+1
	#elif	CAN_BAUDRATE == 800		/* 800KBps */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_Prescaler = 5;
	#elif	CAN_BAUDRATE == 500		/* 500KBps */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_7tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_Prescaler = 6;
	#elif	CAN_BAUDRATE == 400		/* 400KBps */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_Prescaler = 8;
	#elif	CAN_BAUDRATE == 250		/* 250KBps */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_Prescaler = 11;
	#elif	CAN_BAUDRATE == 200		/* 200KBps */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_Prescaler = 11;
	#elif	CAN_BAUDRATE == 125		/* 125KBps */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_Prescaler = 23;
	#elif	CAN_BAUDRATE == 100		/* 100KBps */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_Prescaler = 23;
	#else
	#error	"Please select first the CAN Baudrate in Private defines in CAN.c "
	#endif
	CAN_Init(CAN1, &CAN_InitStructure);        	//��ʼ��CAN1 
	
	/*****CAN ɸѡ������*****/
	#if	CAN_MY_Filter0 == ENABLE	/* �˲�������0 */
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;									//ʹ�ܹ�����
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;		//������������FIFO0�����շ�
	CAN_FilterInitStructure.CAN_FilterIdHigh = CAN_Filter0_IDHigh<<5;					//��ʶ���ĸ�16λID(0x0000~0xffff),������CAN_FxR0�Ĵ���
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Filter0_IDLow;						//��ʶ���ĵ�16λID(0x0000~0xffff),������CAN_FxR0�Ĵ���
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = CAN_Filter0_MaskIDHigh<<5;	//����ĸ�16λID(0x0000~0xffff),������CAN_FxR1�Ĵ���
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Filter0_MaskIDLow;		//����ĵ�16λID(0x0000~0xffff),������CAN_FxR1�Ĵ���
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;							//������ģʽ
	CAN_FilterInitStructure.CAN_FilterNumber = 0;													//��������0
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;				//���ù�����Ϊ32λ��
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	#endif
	
	#if	CAN_MY_Filter1 == ENABLE	/* �˲�������1 */
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;									//ʹ�ܹ�����
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1;		//������������FIFO1�����շ�
	CAN_FilterInitStructure.CAN_FilterIdHigh = CAN_Filter1_IDHigh;					//��ʶ���ĸ�16λID(0x0000~0xffff),������CAN_FxR0�Ĵ���
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Filter1_IDLow;						//��ʶ���ĵ�16λID(0x0000~0xffff),������CAN_FxR0�Ĵ���
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = CAN_Filter1_MaskIDHigh;	//����ĸ�16λID(0x0000~0xffff),������CAN_FxR1�Ĵ���
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Filter1_MaskIDLow;		//����ĵ�16λID(0x0000~0xffff),������CAN_FxR1�Ĵ���
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;							//������ģʽ
	CAN_FilterInitStructure.CAN_FilterNumber = 1;														//��������1
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;				//���ù�����Ϊ32λ��
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	#endif
	
	#if	CAN_MY_Filter2 == ENABLE	/* �˲�������2 */
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;									//ʹ�ܹ�����
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;		//������������FIFO0�����շ�
	CAN_FilterInitStructure.CAN_FilterIdHigh = CAN_Filter2_IDHigh;					//��ʶ���ĸ�16λID(0x0000~0xffff),������CAN_FxR0�Ĵ���
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Filter2_IDLow;						//��ʶ���ĵ�16λID(0x0000~0xffff),������CAN_FxR0�Ĵ���
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = CAN_Filter2_MaskIDHigh;	//����ĸ�16λID(0x0000~0xffff),������CAN_FxR1�Ĵ���
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Filter2_MaskIDLow;		//����ĵ�16λID(0x0000~0xffff),������CAN_FxR1�Ĵ���
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;							//������ģʽ
	CAN_FilterInitStructure.CAN_FilterNumber = 2;														//��������2
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;				//���ù�����Ϊ32λ��
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	#endif
	
	/*	�����ж�	*/
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);	/*	���жϿ���	*/
	CAN_ITConfig(CAN1,CAN_IT_BOF, ENABLE);	/* Bus-off */
	CAN_ITConfig(CAN1,CAN_IT_EWG, ENABLE);	/* Error warning */
	CAN_ITConfig(CAN1,CAN_IT_EPV, ENABLE);	/* Error passive */	
}




/**
	*@brief		CAN���ͺ���
	*@param		*msg��	��Ҫ���͵������׵�ַ
	*@retval	0�����ͳɹ�
						1����Ϣ����ʧ��
						2�����������Ϣ����ʧ��
	*/
uint8_t CAN_Send_Msg(uint8_t *msg, uint8_t len)
{
	uint16_t i;
	uint8_t msgbox;
	CanTxMsg	TxMessage;
	
	TxMessage.IDE		= CAN_Id_Standard;				//��׼֡��ʽ
	TxMessage.RTR		= CAN_RTR_Data;						//����֡
	TxMessage.StdId = 0x12;										//��׼ID
	TxMessage.ExtId = 0;										//��չID
	TxMessage.DLC 	= len;										//���͵����ݳ���,�ֽ���
	for(i=0;i<TxMessage.DLC;i++)
		TxMessage.Data[i]	= msg[i];								//���������
	
	/******���������Ϣ���Ӳ������������******/
	i=0;
	while(msgbox = CAN_Transmit(CAN1,&TxMessage),(msgbox == CAN_TxStatus_NoMailBox && i<5))
	{
		delay_us(200);
		i++;
	}
	if(i==10)	return 2;		//���������Ϣ����ʧ��
	
	/******��ѯ��Ϣ�������******/
	i=0;
	while(CAN_TransmitStatus(CAN1, msgbox)==CAN_TxStatus_Pending && i<3000)	i++;
	if(i==3000)	return 1;	//��Ϣ����ʧ��
	
	/******��Ϣ���ͳɹ�*****/
	return 0;
}




/**
	*@brief		CAN���պ���
	*@param		msg		���յ������ݴ�ŵ�ַ
						FIFONumber:		CAN_FIFO0		��FIFO0����ȡ����
													CAN_FIFO1		��FIFO1����ȡ����
	*@retval	1:	û�н��յ�����	0���ɹ��ӵ�����
	*/
uint8_t CAN_Receive_Msg(CanRxMsg *ReceiveMsg,uint8_t FIFONumber)
{
	if(CAN_MessagePending(CAN1, FIFONumber) == 0)		return 1;
	CAN_Receive(CAN1, FIFONumber, ReceiveMsg);
	return ReceiveMsg->DLC;
}



/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/








