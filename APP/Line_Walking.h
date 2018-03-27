#ifndef	_LINE_WALKING_H
#define _LINE_WALKING_H

#include "includes.h"

/*	底盘旋转半径	*/
#define UNDER_Rotate_R	0.45f
/*	巡线参数	*/
#define P_PARAM_PD					0.8f
#define D_PARAM_PD					0

#define LINE_WALK_SPEED_X		0
#define LINE_WALK_SPEED_Y		100
#define LINE_WALK_ACCEL			30

#define OFFSET_ignore				10	//巡线允许误差,即低于此误差不进行调节
#define Line_Rotate_MAX			45
#define Line_Rotate_MIN			(-Line_Rotate_MAX)


/*	角度纠偏参数	*/
#define P_ANGLE_PD					10
#define D_ANGLE_PD					0

#define ANGLE_OFFSET_ignore	0.1	//角度纠偏允许误差
#define ANGLE_Rotate_MAX		150
#define ANGLE_Rotate_MIN		(-ANGLE_Rotate_MAX)
/*	巡线PD函数	*/
void PD_Offset_Walking( int16_t now_offset,
						float P_param, 
						float D_param, 
						int32_t Speed_VX, 
						int32_t Speed_VY, 
						int32_t Speed_accel);

/*	角度纠偏函数	*/
void PD_Angle_Retify(	float	now_angle, 						//当前角度
						float target_angle, 				//目标角度
						float P_param, 							//P参数
						float D_param, 							//D参数
						int32_t Speed_VX, 					//x方向需要的速度
						int32_t Speed_VY, 					//Y方向需要的速度
						int32_t Speed_Accel	);			//需要的加速度










#endif








