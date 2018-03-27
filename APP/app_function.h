#ifndef	_APP_FUNCTION_H
#define _APP_FUNCTION_H

#include "includes.h"


typedef struct
{
	float	P_param;
	float D_param;
	int 	Speed_Vx;
	int 	Speed_Vy;
	int		Speed_Accel;
	int		Accel_ADD_time;
	int   Accel_DEC_time;
}Angle_WalkStruct;

typedef struct
{
	float P_param;
	float P_TrsParam;
	int   position_x;
	int   position_y;
	int   walk_time;
}Locat_ParamStruct;








#endif
