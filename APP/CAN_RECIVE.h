#ifndef _CAN_RECIVE_H
#define _CAN_RECIVE_H

#include "includes.h"


/*	����������	*/
typedef	union
{
	int 	int_data;
	char	char_data[4];
}INT_CHAR_UNION;

/*		Ѳ�ߴ�����id	*/
#define CAMERA_LEFT_ID	72
#define CAMERA_RIGHT_ID	71








/*	CAN �������ݴ���	*/
void CAN_Recive_handle(void);































#endif










