#ifndef	_CAN_H
#define _CAN_H

#include "includes.h"

#define CAN_BAUDRATE	500		//CANͨ�ŵĲ�����
#define CAN_REMAP			1			//�Ƿ�ʹ��CAN����ӳ��PD0/PD1�˿�
/**
	*@brief	����CANģ��Ĺ�����
	*@supplement	�û��ɸ����Լ������������˲����飬ÿ���˲�������Ե�������(ʹ�ܷ�ʽ��#define CAN_MY_Filterx			ENABLE)
								�˲������Ѿ��ֱ������FIFO�������û�����.
	*@param	CAN_FILTERx_MODE	CAN_FilterMode_IdMask		����λģʽ
														CAN_FilterMode_IdList		��ʶ��ģʽ
	*/
/********����Filter0**********/
#define CAN_MY_Filter0			ENABLE			
#define CAN_Filter0_IDHigh	0x0000			//��ʶ���ĸ�16λID(0x0000~0xffff),������CAN_FxR0�Ĵ���
#define CAN_Filter0_IDLow		0x0000			//��ʶ���ĵ�16λID(0x0000~0xffff),������CAN_FxR0�Ĵ���
#define CAN_Filter0_MaskIDHigh	0x0000				//����ĸ�16λID(0x0000~0xffff),������CAN_FxR1�Ĵ���
#define CAN_Filter0_MaskIDLow		0				//����ĵ�16λID(0x0000~0xffff),������CAN_FxR1�Ĵ���
#define CAN_FILTER0_MODE		CAN_FilterMode_IdMask	

/********����Filter1**********/
#define CAN_MY_Filter1			DISABLE
#define CAN_Filter1_IDHigh	0x0000
#define CAN_Filter1_IDLow		0x0000
#define CAN_Filter1_MaskIDHigh	0
#define CAN_Filter1_MaskIDLow		0
#define CAN_FILTER1_MODE		CAN_FilterMode_IdMask

/********����Filter2**********/
#define CAN_MY_Filter2			DISABLE
#define CAN_Filter2_IDHigh	0x0000
#define CAN_Filter2_IDLow		0x0000
#define CAN_Filter2_MaskIDHigh	0
#define CAN_Filter2_MaskIDLow		0
#define CAN_FILTER2_MODE		CAN_FilterMode_IdMask

/********����Filter3**********/
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






/************************************************�û��Զ��庯������********************************************************/
/************************************************�û��Զ��庯������********************************************************/
/************************************************�û��Զ��庯������********************************************************/





#endif













