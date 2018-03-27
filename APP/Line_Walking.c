//////////////////////////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************************************
**
	*		@brief	Ѳ��ģ���㷨
	*
******************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Line_Walking.h"

/**********************************************************
**
	*	@brief	A����������B��������
	*
**********************************************************/
//void 


/*********************************************************/
/**	
	*	@brief	PD�ջ�Ѳ��-----ƫ�����ͷ/�ư�
	*/
/**********************************************************/
void PD_Offset_Walking( int16_t now_offset,
												float P_param, 
												float D_param, 
												int32_t Speed_VX, 
												int32_t Speed_VY, 
												int32_t Speed_accel)
{
	static int16_t last_offset=0;
	int32_t PD_Result = 0;
	
	if(ABS_int(now_offset)<OFFSET_ignore)
		PD_Result = 0;
	else{
		/*	������ת��ƫ	*/
		LCD_Display_int(now_offset,0,1);
		PD_Result = (int32_t)(UNDER_Rotate_R * (P_param*now_offset + D_param*(now_offset-last_offset)));
//		LCD_Display_int(PD_Result,0,4);
		if(PD_Result>Line_Rotate_MAX)				PD_Result = Line_Rotate_MAX;
		if(PD_Result<Line_Rotate_MIN)				PD_Result = Line_Rotate_MIN;
//		LCD_Display_int(PD_Result,0,2);
		last_offset = now_offset;
			
	}
	/*	���������	*/
		Move_XY_Rotate_Freedom(Speed_VX, Speed_VY, PD_Result, Speed_accel);
}


/**************************************************************
**
	*	@brief	PD��ƫ���Ƕ�ƫ���ƫ-----MPU6050�Ƕ�����
	*
***************************************************************/
void PD_Angle_Retify(	float	now_angle, 
											float target_angle, 
											float P_param, 
											float D_param, 
											int32_t Speed_VX, 
											int32_t Speed_VY, 
											int32_t Speed_Accel	)
{
	int32_t PD_angle_result=0;
	static float last_angle_offset=0;
	float angle_offset=0;
	
	angle_offset = target_angle-now_angle;
	if(ABS_int(angle_offset)<ANGLE_OFFSET_ignore)
		angle_offset = 0;
	else{
		PD_angle_result = (int32_t)(UNDER_Rotate_R*(P_param*angle_offset + D_param*(angle_offset-last_angle_offset)));
		if(PD_angle_result>ANGLE_Rotate_MAX)		PD_angle_result = ANGLE_Rotate_MAX;
		if(PD_angle_result<ANGLE_Rotate_MIN)		PD_angle_result = ANGLE_Rotate_MIN;
		last_angle_offset = angle_offset;
	}
	/*	���������	*/
		Move_XY_Rotate_Freedom(Speed_VX, Speed_VY, PD_angle_result, Speed_Accel);
	
}






















