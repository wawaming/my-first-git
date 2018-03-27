#ifndef _CAN_RECIVE_H
#define _CAN_RECIVE_H

#include "includes.h"


/*	定义联合体	*/
typedef	union
{
	int 	int_data;
	char	char_data[4];
}INT_CHAR_UNION;

/*		巡线传感器id	*/
#define CAMERA_LEFT_ID	72
#define CAMERA_RIGHT_ID	71








/*	CAN 接收数据处理	*/
void CAN_Recive_handle(void);































#endif










