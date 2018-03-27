/*************************************************************************************
*	@file			can.c
* @author	 	yun
*	@version 	V1.0
*	@date			2016/7/25
* @brief		NONE
*************************************************************************************/

#include "can.h"

/**
	*@breif	can 通信的配置
	*@param	 CAN_MODE：	CAN_Mode_Normal						正常模式
											CAN_Mode_LoopBack					回环模式
											CAN_Mode_Silent						静默模式
											CAN_Mode_Silent_LoopBack		静默回环模式
	*@retval NONE
	*/
void CAN_Config(uint8_t CAN_MODE)
{
	CAN_InitTypeDef	CAN_InitStructure;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);		//stm32f10x系列只有CAN1模块(除互联版以外)，stm32f40x系列拥有CAN1/CAN2模块
	
	/*****配置CAN通信的IO口*****/
	#if	CAN_REMAP
		GPIO_Config(GPIOD, GPIO_Pin_0, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
		GPIO_Config(GPIOD, GPIO_Pin_1, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);			//重映射为Remap2不是Remap1
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
	#else
		GPIO_Config(GPIOA, GPIO_Pin_11, GPIO_Mode_IPU);				//上拉输入，接CAN_RX
		GPIO_Config(GPIOA, GPIO_Pin_12, GPIO_Mode_Out_PP);		//推挽输出，接CAN_TX
	#endif
	
	CAN_DeInit(CAN1);//重置CAN
	CAN_StructInit(&CAN_InitStructure);
	/*****CAN相关设置，操作的是CAN主控制寄存器(CAN_MCR)*****/
	CAN_InitStructure.CAN_ABOM = ENABLE;		//软件离线管理(can模块从离线模式恢复时，需要软件先申请bxCAN后才会执行硬件检测，恢复离线)
	CAN_InitStructure.CAN_AWUM = DISABLE;		//通过软件进行睡眠模式下的唤醒
	CAN_InitStructure.CAN_Mode = CAN_MODE;	//工作模式：
	CAN_InitStructure.CAN_NART = ENABLE;		//禁止报文发送失败后进行自动重发					DISABLE(OLD PARAM)
	CAN_InitStructure.CAN_RFLM = DISABLE;		//报文不锁定，新的报文会覆盖旧的
	CAN_InitStructure.CAN_TTCM = DISABLE;		//非时间触发模式(时间触发模式下，报文是周期性发送，不是事件触发)，时间触发模式主要用于实时性要求极高的情况
	CAN_InitStructure.CAN_TXFP = ENABLE;		//报文发送的优先级由标识符决定
	
	/*****CAN	波特率设置		CAN_BAUDRATE = 36M/(CAN_Prescaler*(1+CAN_BS1+CAN_BS2))	APB1线时钟为36M*****/
	#if		CAN_BAUDRATE == 1000	/* 1MBPs */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;			//(TS1[3:0]+1)	
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;			//(TS2[2:0]+1)
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;			//(SJW[1:0]+1)		再同步段并不参与波特率计算,参与计算的同步段恒为1tq
	CAN_InitStructure.CAN_Prescaler = 4;					//(BRP[9:0]+1)		此处设置的为BRP的值，因此分频系数为设置值+1
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
	CAN_Init(CAN1, &CAN_InitStructure);        	//初始化CAN1 
	
	/*****CAN 筛选器设置*****/
	#if	CAN_MY_Filter0 == ENABLE	/* 滤波器配置0 */
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;									//使能过滤器
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;		//给过滤器分配FIFO0进行收发
	CAN_FilterInitStructure.CAN_FilterIdHigh = CAN_Filter0_IDHigh<<5;					//标识符的高16位ID(0x0000~0xffff),即配置CAN_FxR0寄存器
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Filter0_IDLow;						//标识符的低16位ID(0x0000~0xffff),即配置CAN_FxR0寄存器
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = CAN_Filter0_MaskIDHigh<<5;	//掩码的高16位ID(0x0000~0xffff),即配置CAN_FxR1寄存器
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Filter0_MaskIDLow;		//掩码的低16位ID(0x0000~0xffff),即配置CAN_FxR1寄存器
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;							//过滤器模式
	CAN_FilterInitStructure.CAN_FilterNumber = 0;													//过滤器组0
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;				//设置过滤器为32位宽
	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
	#endif
	
	#if	CAN_MY_Filter1 == ENABLE	/* 滤波器配置1 */
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;									//使能过滤器
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1;		//给过滤器分配FIFO1进行收发
	CAN_FilterInitStructure.CAN_FilterIdHigh = CAN_Filter1_IDHigh;					//标识符的高16位ID(0x0000~0xffff),即配置CAN_FxR0寄存器
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Filter1_IDLow;						//标识符的低16位ID(0x0000~0xffff),即配置CAN_FxR0寄存器
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = CAN_Filter1_MaskIDHigh;	//掩码的高16位ID(0x0000~0xffff),即配置CAN_FxR1寄存器
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Filter1_MaskIDLow;		//掩码的低16位ID(0x0000~0xffff),即配置CAN_FxR1寄存器
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;							//过滤器模式
	CAN_FilterInitStructure.CAN_FilterNumber = 1;														//过滤器组1
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;				//设置过滤器为32位宽
	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
	#endif
	
	#if	CAN_MY_Filter2 == ENABLE	/* 滤波器配置2 */
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;									//使能过滤器
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;		//给过滤器分配FIFO0进行收发
	CAN_FilterInitStructure.CAN_FilterIdHigh = CAN_Filter2_IDHigh;					//标识符的高16位ID(0x0000~0xffff),即配置CAN_FxR0寄存器
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Filter2_IDLow;						//标识符的低16位ID(0x0000~0xffff),即配置CAN_FxR0寄存器
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = CAN_Filter2_MaskIDHigh;	//掩码的高16位ID(0x0000~0xffff),即配置CAN_FxR1寄存器
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Filter2_MaskIDLow;		//掩码的低16位ID(0x0000~0xffff),即配置CAN_FxR1寄存器
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;							//过滤器模式
	CAN_FilterInitStructure.CAN_FilterNumber = 2;														//过滤器组2
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;				//设置过滤器为32位宽
	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
	#endif
	
	/*	开启中断	*/
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);	/*	打开中断开关	*/
	CAN_ITConfig(CAN1,CAN_IT_BOF, ENABLE);	/* Bus-off */
	CAN_ITConfig(CAN1,CAN_IT_EWG, ENABLE);	/* Error warning */
	CAN_ITConfig(CAN1,CAN_IT_EPV, ENABLE);	/* Error passive */	
}




/**
	*@brief		CAN发送函数
	*@param		*msg：	需要发送的数据首地址
	*@retval	0：发送成功
						1：消息发送失败
						2：申请空闲消息盒子失败
	*/
uint8_t CAN_Send_Msg(uint8_t *msg, uint8_t len)
{
	uint16_t i;
	uint8_t msgbox;
	CanTxMsg	TxMessage;
	
	TxMessage.IDE		= CAN_Id_Standard;				//标准帧格式
	TxMessage.RTR		= CAN_RTR_Data;						//数据帧
	TxMessage.StdId = 0x12;										//标准ID
	TxMessage.ExtId = 0;										//扩展ID
	TxMessage.DLC 	= len;										//传送的数据长度,字节数
	for(i=0;i<TxMessage.DLC;i++)
		TxMessage.Data[i]	= msg[i];								//传输的数据
	
	/******申请空闲消息盒子并提出发送申请******/
	i=0;
	while(msgbox = CAN_Transmit(CAN1,&TxMessage),(msgbox == CAN_TxStatus_NoMailBox && i<5))
	{
		delay_us(200);
		i++;
	}
	if(i==10)	return 2;		//申请空闲消息盒子失败
	
	/******查询消息发送情况******/
	i=0;
	while(CAN_TransmitStatus(CAN1, msgbox)==CAN_TxStatus_Pending && i<3000)	i++;
	if(i==3000)	return 1;	//消息发送失败
	
	/******消息发送成功*****/
	return 0;
}




/**
	*@brief		CAN接收函数
	*@param		msg		接收到的数据存放地址
						FIFONumber:		CAN_FIFO0		从FIFO0中提取数据
													CAN_FIFO1		从FIFO1中提取数据
	*@retval	1:	没有接收到数据	0：成功接到数据
	*/
uint8_t CAN_Receive_Msg(CanRxMsg *ReceiveMsg,uint8_t FIFONumber)
{
	if(CAN_MessagePending(CAN1, FIFONumber) == 0)		return 1;
	CAN_Receive(CAN1, FIFONumber, ReceiveMsg);
	return ReceiveMsg->DLC;
}



/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/








