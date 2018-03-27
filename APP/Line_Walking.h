#ifndef	_LINE_WALKING_H
#define _LINE_WALKING_H

#include "includes.h"

/*	������ת�뾶	*/
#define UNDER_Rotate_R	0.45f
/*	Ѳ�߲���	*/
#define P_PARAM_PD					0.8f
#define D_PARAM_PD					0

#define LINE_WALK_SPEED_X		0
#define LINE_WALK_SPEED_Y		100
#define LINE_WALK_ACCEL			30

#define OFFSET_ignore				10	//Ѳ���������,�����ڴ������е���
#define Line_Rotate_MAX			45
#define Line_Rotate_MIN			(-Line_Rotate_MAX)


/*	�ǶȾ�ƫ����	*/
#define P_ANGLE_PD					10
#define D_ANGLE_PD					0

#define ANGLE_OFFSET_ignore	0.1	//�ǶȾ�ƫ�������
#define ANGLE_Rotate_MAX		150
#define ANGLE_Rotate_MIN		(-ANGLE_Rotate_MAX)
/*	Ѳ��PD����	*/
void PD_Offset_Walking( int16_t now_offset,
						float P_param, 
						float D_param, 
						int32_t Speed_VX, 
						int32_t Speed_VY, 
						int32_t Speed_accel);

/*	�ǶȾ�ƫ����	*/
void PD_Angle_Retify(	float	now_angle, 						//��ǰ�Ƕ�
						float target_angle, 				//Ŀ��Ƕ�
						float P_param, 							//P����
						float D_param, 							//D����
						int32_t Speed_VX, 					//x������Ҫ���ٶ�
						int32_t Speed_VY, 					//Y������Ҫ���ٶ�
						int32_t Speed_Accel	);			//��Ҫ�ļ��ٶ�










#endif








