#ifndef	_CAN_H
#define _CAN_H

#include "includes.h"

#define CAN_BAUDRATE	500		//CAN通信的波特率
#define CAN_REMAP			1			//是否使用CAN的重映射PD0/PD1端口
/**
	*@brief	配置CAN模块的过滤器
	*@supplement	用户可根据自己的需求启动滤波器组，每个滤波器组可以单独配置(使能方式：#define CAN_MY_Filterx			ENABLE)
								滤波器组已经分别关联了FIFO，不用用户操作.
	*@param	CAN_FILTERx_MODE	CAN_FilterMode_IdMask		屏蔽位模式
														CAN_FilterMode_IdList		标识符模式
	*/
/********配置Filter0**********/
#define CAN_MY_Filter0			ENABLE			
#define CAN_Filter0_IDHigh	0x0000			//标识符的高16位ID(0x0000~0xffff),即配置CAN_FxR0寄存器
#define CAN_Filter0_IDLow		0x0000			//标识符的低16位ID(0x0000~0xffff),即配置CAN_FxR0寄存器
#define CAN_Filter0_MaskIDHigh	0x0000				//掩码的高16位ID(0x0000~0xffff),即配置CAN_FxR1寄存器
#define CAN_Filter0_MaskIDLow		0				//掩码的低16位ID(0x0000~0xffff),即配置CAN_FxR1寄存器
#define CAN_FILTER0_MODE		CAN_FilterMode_IdMask	

/********配置Filter1**********/
#define CAN_MY_Filter1			DISABLE
#define CAN_Filter1_IDHigh	0x0000
#define CAN_Filter1_IDLow		0x0000
#define CAN_Filter1_MaskIDHigh	0
#define CAN_Filter1_MaskIDLow		0
#define CAN_FILTER1_MODE		CAN_FilterMode_IdMask

/********配置Filter2**********/
#define CAN_MY_Filter2			DISABLE
#define CAN_Filter2_IDHigh	0x0000
#define CAN_Filter2_IDLow		0x0000
#define CAN_Filter2_MaskIDHigh	0
#define CAN_Filter2_MaskIDLow		0
#define CAN_FILTER2_MODE		CAN_FilterMode_IdMask

/********配置Filter3**********/
#define CAN_MY_Filter3			DISABLE
#define CAN_Filter3_IDHigh	0x0000
#define CAN_Filter3_IDLow		0x0000
#define CAN_Filter3_MaskIDHigh	0
#define CAN_Filter3_MaskIDLow		0
#define CAN_FILTER3_MODE		CAN_FilterMode_IdMask


/*			Persion	Program				*/
void CAN_Config(uint8_t CAN_MODE);
uint8_t CAN_Send_Msg(uint8_t *msg, uint8_t len);
uint8_t CAN_Receive_Msg(CanRxMsg *ReceiveMsg,uint8_t FIFONumber);






/************************************************用户自定义函数定义********************************************************/
/************************************************用户自定义函数定义********************************************************/
/************************************************用户自定义函数定义********************************************************/





#endif













