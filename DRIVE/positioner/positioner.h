#ifndef	_POSITIONER_H
#define _POSITIONER_H

#include "includes.h"

/*			东大全场定位接收数据包		*/
typedef struct	
{
	float pos_x;
	float pos_y;
	float	z_angle;
	float x_angle;
	float y_angle;
	float pos_w;
	uint8_t read;
}POSStructure;





#endif








