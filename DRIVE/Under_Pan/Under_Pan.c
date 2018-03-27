//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************************
**
	*	@brief														�����˵��������㷨
	*	@supplement		��ͬ�ĵ��̰�װ��ʽ,����ģ�ͻ�������,�������ӵ��ٶȷ�����Ҫ����ʵ�ʰ�װ����
	*								���㷨�ǻ��������ķ��,��װ��ʽΪ���ֳ�����
	*
**********************************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Under_Pan.h"

/*	���̵������	*/
SERVO_MLCB UNDER1_MLCB;			//��һ���޵��
SERVO_MLCB UNDER2_MLCB;			//�ڶ����޵��
SERVO_MLCB UNDER3_MLCB;			//�������޵��
SERVO_MLCB UNDER4_MLCB;			//�������޵��
SERVO_MLCB UNDER5_MLCB;
SERVO_MLCB UNDER6_MLCB;

/**
	*
	*/
void Underpan_Motor_Init(void)
{
	/*	���̵�����ƿ�����	*/
	Create_MLServo_CB(	 (SERVO_MLCB	*)&UNDER1_MLCB,					
						 (char   	*)"underpan_motor1",			
						 (uint8_t      )DEVICE1_ID,											
						 (uint8_t      )RATIO1,								
						 (uint16_t     )CODER1);
	
	Create_MLServo_CB(	 (SERVO_MLCB	*)&UNDER2_MLCB,					
						 (char   	*)"underpan_motor2",			
						 (uint8_t      )DEVICE2_ID,											
						 (uint8_t      )RATIO2,								
						 (uint16_t     )CODER2);
	
	Create_MLServo_CB(	 (SERVO_MLCB	*)&UNDER3_MLCB,					
						 (char   	*)"underpan_motor3",			
						 (uint8_t      )DEVICE3_ID,											
						 (uint8_t      )RATIO3,								
						 (uint16_t     )CODER3);
	
	Create_MLServo_CB(	 (SERVO_MLCB	*)&UNDER4_MLCB,					
						 (char   	*)"underpan_motor4",			
						 (uint8_t      )DEVICE4_ID,											
						 (uint8_t      )RATIO4,								
						 (uint16_t     )CODER4);
	Create_MLServo_CB(	 (SERVO_MLCB	*)&UNDER5_MLCB,					
						 (char   	*)"underpan_motor5",			
						 (uint8_t      )DEVICE5_ID,											
						 (uint8_t      )RATIO5,								
						 (uint16_t     )CODER5);
	Create_MLServo_CB(	 (SERVO_MLCB	*)&UNDER6_MLCB,					
						 (char   	*)"underpan_motor6",			
						 (uint8_t      )DEVICE6_ID,											
						 (uint8_t      )RATIO6,								
						 (uint16_t     )CODER6);						 
											 
	/*	�����ʼ��	*/
//	Motor_P_SET(&UNDER1_MLCB,5100);
//	Motor_Code_Set(&UNDER1_MLCB,1000);
//	Motor_Speed_Mode(&UNDER1_MLCB);
//	Motor_MastSpeed(&UNDER1_MLCB,RE40_MaxSpeed);
//	delay_ms(10);
//	Motor_AccelSpeed(&UNDER1_MLCB,100);
//	Motor_Save_Param(&UNDER1_MLCB);
//	delay_ms(10);
//	
//	Motor_P_SET(&UNDER2_MLCB,5100);
//	Motor_Code_Set(&UNDER2_MLCB,1000);
//	Motor_Speed_Mode(&UNDER2_MLCB);
//	Motor_MastSpeed(&UNDER2_MLCB,RE40_MaxSpeed);
//	delay_ms(10);
//	Motor_AccelSpeed(&UNDER2_MLCB,100);
//	Motor_Save_Param(&UNDER2_MLCB);
//	delay_ms(10);
//	
//	Motor_P_SET(&UNDER3_MLCB,5100);
//	Motor_Code_Set(&UNDER3_MLCB,1000);
//	Motor_Speed_Mode(&UNDER3_MLCB);
//	Motor_MastSpeed(&UNDER3_MLCB,RE40_MaxSpeed);
//	delay_ms(10);
//	Motor_AccelSpeed(&UNDER3_MLCB,100);
//	Motor_Save_Param(&UNDER3_MLCB);
//	delay_ms(10);
//	
//	Motor_P_SET(&UNDER4_MLCB,5100);
//	Motor_Code_Set(&UNDER4_MLCB,1000);
//	Motor_Speed_Mode(&UNDER4_MLCB);
//	Motor_MastSpeed(&UNDER4_MLCB,RE40_MaxSpeed);
//	delay_ms(10);
//	Motor_AccelSpeed(&UNDER4_MLCB,100);
//	Motor_Save_Param(&UNDER4_MLCB);
//	delay_ms(10);
//	
	Motor_P_SET(&UNDER5_MLCB,5100);
	Motor_Code_Set(&UNDER5_MLCB,1000);
	Motor_Speed_Mode(&UNDER5_MLCB);
	Motor_MastSpeed(&UNDER5_MLCB,RE40_MaxSpeed);
	delay_ms(10);
	Motor_AccelSpeed(&UNDER5_MLCB,100);
	Motor_Save_Param(&UNDER5_MLCB);
	delay_ms(10);
	
	Motor_P_SET(&UNDER6_MLCB,5100);
	Motor_Code_Set(&UNDER6_MLCB,1000);
	Motor_Speed_Mode(&UNDER6_MLCB);
	Motor_MastSpeed(&UNDER6_MLCB,RE40_MaxSpeed);
	delay_ms(10);
	Motor_AccelSpeed(&UNDER6_MLCB,100);
	Motor_Save_Param(&UNDER6_MLCB);
	delay_ms(10);

}

/****************************************************************************************************/
/*																			*����ʻ�㷨*																							*/
/****************************************************************************************************/
/**
	*	@brief	Move_XY_Speed			�Թ̶��ٶȺͼ��ٶ��������᷽����ʻ
	*	@param	move_speed	��ʻ�ٶ�	
						move_acc		��ʻ���ٶ�
						direction		0			x+������ʻ
												1			y+������ʻ
												2			x-������ʻ
												3			y-������ʻ
	*	@retval	NONE
	*/

void fasheceshi(int32_t fashe_speed,int32_t fashe_acc, uint8_t direction)
{
			switch(direction)
			{
				
				/*	x+����	*/	case 1:	UNDER5_MOVE(fashe_speed,fashe_acc);			
			
				/*	x-����	*/	case 0: UNDER5_MOVE(-fashe_speed,fashe_acc);			
			
			}

}

void Move_XY_Speed(int32_t move_speed, int32_t move_acc, uint8_t direction)
{
	switch(direction)
	{
		case	0:					{
	/*	x+����	*/				UNDER1_MOVE(-move_speed,move_acc);
												UNDER2_MOVE(-move_speed,move_acc);
												UNDER3_MOVE(+move_speed,move_acc);
												UNDER4_MOVE(+move_speed,move_acc);
											}break;
		
		case	1:					{
	/*	y+����	*/				UNDER1_MOVE(move_speed,move_acc);
												UNDER2_MOVE(-move_speed,move_acc);
												UNDER3_MOVE(-move_speed,move_acc);
												UNDER4_MOVE(move_speed,move_acc);
											}break;
		
		case	2:					{
	/*	x-����	*/				UNDER1_MOVE(move_speed,move_acc);
												UNDER2_MOVE(move_speed,move_acc);
												UNDER3_MOVE(-move_speed,move_acc);
												UNDER4_MOVE(-move_speed,move_acc);
											}break;

		case	3:					{
	/*	y-����	*/				UNDER1_MOVE(-move_speed,move_acc);
												UNDER2_MOVE(move_speed,move_acc);
												UNDER3_MOVE(move_speed,move_acc);
												UNDER4_MOVE(-move_speed,move_acc);
											}break;			
	}
}

/**
	*	@brief	Move_XY_Speed_Angle
	*	@param	move_speed	��ʻ�ٶ�
						move_acc		��ʻ���ٶ�
						angle				��ʻ�ĽǶ�
	*	@retval	NONE
	*/
void Move_XY_Speed_Angle(int32_t move_speed, int32_t move_acc, int32_t angle)
{
	float speed_x,speed_y;
	short int wheel1,wheel2,wheel3,wheel4;
	speed_x = move_speed*cos((float)angle*3.14f/180);
	speed_y = move_speed*sin((float)angle*3.14f/180);
	
	wheel1 = (int32_t)(-speed_x + speed_y);
	wheel2 = (int32_t)(-speed_x - speed_y);
	wheel3 = (int32_t)(speed_x - speed_y);
	wheel4 = (int32_t)(speed_x + speed_y);
	
	UNDER1_MOVE(wheel1,move_acc);
	UNDER2_MOVE(wheel2,move_acc);
	UNDER3_MOVE(wheel3,move_acc);
	UNDER4_MOVE(wheel4,move_acc);
	
}

/**
	*	@brief	Move_Z_Rotate_Speed	��Z����ת
	*	@param	move_speed	��ת�����ٶ�
						move_acc		���Ӽ��ٶ�
						direction		0��	��ʱ��
												1��	˳ʱ����ת
	*	@retval	NONE
	*/
void Move_Z_Rotate_Speed(int32_t move_speed, int32_t move_acc, uint8_t direction)
{
	if(direction){
		UNDER1_MOVE(-move_speed,move_acc);
		UNDER2_MOVE(-move_speed,move_acc);
		UNDER3_MOVE(-move_speed,move_acc);
		UNDER4_MOVE(-move_speed,move_acc);
	}
	else
	{
		UNDER1_MOVE(move_speed,move_acc);
		UNDER2_MOVE(move_speed,move_acc);
		UNDER3_MOVE(move_speed,move_acc);
		UNDER4_MOVE(move_speed,move_acc);
	}
}



/**
	*	@brief	Move_XY_Freedom		����x��y�����ٶ�ƽ��
	*	@param	move_speed_x��	x�᷽���ٶ�	
						move_speed_y��	y�᷽���ٶ�
						move_acc		��	���ٶ�
	*	@retvlal	NONE
	*/
void Move_XY_Freedom(int32_t move_speed_x, int32_t move_speed_y, int32_t move_acc)
{
	int32_t wheel1,wheel2,wheel3,wheel4;
	
	wheel1 = (int32_t)(-move_speed_x + move_speed_y);
	wheel2 = (int32_t)(-move_speed_x - move_speed_y);
	wheel3 = (int32_t)(move_speed_x - move_speed_y);
	wheel4 = (int32_t)(move_speed_x + move_speed_y);
	
	UNDER1_MOVE(wheel1,move_acc);
	UNDER2_MOVE(wheel2,move_acc);
	UNDER3_MOVE(wheel3,move_acc);
	UNDER4_MOVE(wheel4,move_acc);
	
}


/**
	*	@brief	Move_XY_Freedom		����x��y�����ٶ�ƽ��
	*	@param	move_speed_x��	x�᷽���ٶ�	
						move_speed_y��	y�᷽���ٶ�
						rotate_angle��	�Դ��ٶ�
						move_acc		��	���ٶ�
	*	@retvlal	NONE
	*/
void Move_XY_Rotate_Freedom(int32_t move_speed_x, int32_t move_speed_y, int32_t rotate_angle, int32_t move_acc)
{
	int32_t wheel1,wheel2,wheel3,wheel4;
	
	wheel1 = (int32_t)(-move_speed_x + move_speed_y + rotate_angle);
	wheel2 = (int32_t)(-(move_speed_x + move_speed_y - rotate_angle));
	wheel3 = (int32_t)(-(-move_speed_x + move_speed_y - rotate_angle));
	wheel4 = (int32_t)(move_speed_x + move_speed_y + rotate_angle);
	
	if(wheel1>WHEEL_SPEED_MAX)		wheel1 = WHEEL_SPEED_MAX;
	if(wheel1<WHEEL_SPEED_MIN)		wheel1 = WHEEL_SPEED_MIN;
	
	if(wheel2>WHEEL_SPEED_MAX)		wheel2 = WHEEL_SPEED_MAX;
	if(wheel2<WHEEL_SPEED_MIN)		wheel2 = WHEEL_SPEED_MIN;
	
	if(wheel3>WHEEL_SPEED_MAX)		wheel3 = WHEEL_SPEED_MAX;
	if(wheel3<WHEEL_SPEED_MIN)		wheel3 = WHEEL_SPEED_MIN;
	
	if(wheel4>WHEEL_SPEED_MAX)		wheel4 = WHEEL_SPEED_MAX;
	if(wheel4<WHEEL_SPEED_MIN)		wheel4 = WHEEL_SPEED_MIN;
	
	UNDER1_MOVE(wheel1,move_acc);
	UNDER2_MOVE(wheel2,move_acc);
	UNDER3_MOVE(wheel3,move_acc);
	UNDER4_MOVE(wheel4,move_acc);
	
}


/***************************************************************************************************/
/*																			*	���㷨���� *																						 */
/***************************************************************************************************/
/**
	*	@brief	SIN_Control_Speed	�����Һ������ٹ���
	*	@supplement				v = f(x) = sin(x-3.14/2) + 1
											��ʽ�ӻ��ֵõ�	x = -cos(x-3.14/2) + x		��·�̱��ʽ
											��ʽ��΢�ֵõ�	a = cos(x-3.14/2)					�����ٶȱ��ʽ
	*	@param	target_speed	:	Ŀ���ٶ�		accel_time:	����ʱ��(����)Ϊʱ��Ƭ��������
	*	@retval	
	*/
uint8_t SIN_Control_Speed_ADD(int32_t now_speed_x, int32_t now_speed_y, int32_t target_speed_x, int32_t target_speed_y, int32_t accel_time)
{
	uint8_t	i=0;
	float time_piece;		//ÿ�εĵ�����---����---ÿ�ε���ʱ������ʱ��ı���
	float t_to_x = 0;			//������ʱ��tӳ�䵽x����
	int32_t y_to_vx = 0;	//������õ���yֵӳ�䵽�ٶ���(x������ٶ�)
	int32_t y_to_vy = 0;	//������õ���yֵӳ�䵽�ٶ���(y������ٶ�)
	int32_t wheel1,wheel2,wheel3,wheel4;	//�����ٶ�
	int32_t wheel1_accel,wheel2_accel,wheel3_accel,wheel4_accel;//���Ӽ��ٶ�
	time_piece = (float)TIM_Piece/accel_time;
	while(i<(uint8_t)(accel_time/TIM_Piece))
	{
		
		/*	���ƴ���+1	*/
		i++;
		
		/*	����ÿ�ο��Ƶ�xֵ	*/
		t_to_x = time_piece*i*3.1416f;
		
		
		/*	�����ٶ������	*/
		y_to_vx = (int32_t)((float)((target_speed_x - now_speed_x)/2) * (sin(t_to_x - 3.1416/2)+1) + now_speed_x);
		y_to_vy = (int32_t)((float)((target_speed_y - now_speed_y)/2) * (sin(t_to_x - 3.1416/2)+1) + now_speed_y);	
		
		wheel1 = (int32_t)(-y_to_vx + y_to_vy );
		wheel2 = (int32_t)(-(y_to_vx + y_to_vy ));
		wheel3 = (int32_t)(-(-y_to_vx + y_to_vy ));
		wheel4 = (int32_t)(y_to_vx + y_to_vy );
		
//		LCD_Display_int(wheel1,0,4);
//		LCD_Display_int(wheel2,5,4);
//		LCD_Display_int(wheel3,10,4);
//		LCD_Display_int(wheel4,15,4);
		
		wheel1_accel = WHEEL_SET_ACCEL_MAX;
		wheel2_accel = WHEEL_SET_ACCEL_MAX;
		wheel3_accel = WHEEL_SET_ACCEL_MAX;
		wheel4_accel = WHEEL_SET_ACCEL_MAX;
		
		if(wheel1>WHEEL_SET_SPEED_MAX)					wheel1 = WHEEL_SET_SPEED_MAX;
		if(wheel1<WHEEL_SET_SPEED_MIN)					wheel1 = WHEEL_SET_SPEED_MIN;
//		if(wheel1_accel>WHEEL_SET_ACCEL_MAX)		wheel1_accel = WHEEL_SET_ACCEL_MAX;
		
		if(wheel2>WHEEL_SET_SPEED_MAX)					wheel2 = WHEEL_SET_SPEED_MAX;
		if(wheel2<WHEEL_SET_SPEED_MIN)					wheel2 = WHEEL_SET_SPEED_MIN;
//		if(wheel2_accel>WHEEL_SET_ACCEL_MAX)		wheel2_accel = WHEEL_SET_ACCEL_MAX;
		
		if(wheel3>WHEEL_SET_SPEED_MAX)					wheel3 = WHEEL_SET_SPEED_MAX;
		if(wheel3<WHEEL_SET_SPEED_MIN)					wheel3 = WHEEL_SET_SPEED_MIN;
//		if(wheel3_accel>WHEEL_SET_ACCEL_MAX)		wheel3_accel = WHEEL_SET_ACCEL_MAX;
		
		if(wheel4>WHEEL_SET_SPEED_MAX)					wheel4 = WHEEL_SET_SPEED_MAX;
		if(wheel4<WHEEL_SET_SPEED_MIN)					wheel4 = WHEEL_SET_SPEED_MIN;
//		if(wheel4_accel>WHEEL_SET_ACCEL_MAX)		wheel4_accel = WHEEL_SET_ACCEL_MAX;
		
		UNDER1_MOVE(wheel1,wheel1_accel);
		UNDER2_MOVE(wheel2,wheel2_accel);
		UNDER3_MOVE(wheel3,wheel3_accel);
		UNDER4_MOVE(wheel4,wheel4_accel);
//		LCD_Display_int(wheel1,0,4);
//		LCD_Display_int(wheel1,5,4);
//		LCD_Display_int(wheel1,10,4);
//		LCD_Display_int(wheel1,15,4);
		delay_ms(TIM_Piece);
	}
	return 0;
	
}

/**
	*	@brief	SIN_Control_Speed	�����Һ������ٹ���
	*	@supplement				v = f(x) = sin(x+3.14/2) + 1
											��ʽ�ӻ��ֵõ�	x = -cos(x+3.14/2) + x		��·�̱��ʽ
											��ʽ��΢�ֵõ�	a = cos(x+3.14/2)					�����ٶȱ��ʽ
	*	@param	target_speed	:	Ŀ���ٶ�		accel_time:	����ʱ��(����)Ϊʱ��Ƭ��������
	*	@retval	
	*/
uint8_t SIN_Control_Speed_DEC(int32_t now_speed_x, int32_t now_speed_y, int32_t target_speed_x, int32_t target_speed_y, int32_t accel_time, int32_t position)
{
	uint8_t	i=0;
	float time_piece;		//ÿ�εĵ�����---����---ÿ�ε���ʱ������ʱ��ı���
	float t_to_x = 0;			//������ʱ��tӳ�䵽x����
	int32_t y_to_vx = 0;	//������õ���yֵӳ�䵽�ٶ���(x������ٶ�)
	int32_t y_to_vy = 0;	//������õ���yֵӳ�䵽�ٶ���(y������ٶ�)
	int32_t wheel1,wheel2,wheel3,wheel4;	//�����ٶ�
	int32_t wheel1_accel,wheel2_accel,wheel3_accel,wheel4_accel;//���Ӽ��ٶ�
	time_piece = (float)TIM_Piece/accel_time;
	while(i<(uint8_t)(accel_time/TIM_Piece))
	{
		
		/*	���ƴ���+1	*/
		i++;
		
		/*	����ÿ�ο��Ƶ�xֵ	*/
		t_to_x = time_piece*i*3.1416f;
		
		
		/*	�����ٶ������	*/
		y_to_vx = (int32_t)((float)((now_speed_x - target_speed_x)/2) * (sin(t_to_x + 3.1416/2)+1) );
		y_to_vy = (int32_t)((float)((now_speed_y - target_speed_y)/2) * (sin(t_to_x + 3.1416/2)+1) );	
		
		wheel1 = (int32_t)(-y_to_vx + y_to_vy );
		wheel2 = (int32_t)(-(y_to_vx + y_to_vy ));
		wheel3 = (int32_t)(-(-y_to_vx + y_to_vy ));
		wheel4 = (int32_t)(y_to_vx + y_to_vy );
		
		wheel1_accel = WHEEL_SET_ACCEL_MAX;
		wheel2_accel = WHEEL_SET_ACCEL_MAX;
		wheel3_accel = WHEEL_SET_ACCEL_MAX;
		wheel4_accel = WHEEL_SET_ACCEL_MAX;
		
		if(wheel1>WHEEL_SET_SPEED_MAX)					wheel1 = WHEEL_SET_SPEED_MAX;
		if(wheel1<WHEEL_SET_SPEED_MIN)					wheel1 = WHEEL_SET_SPEED_MIN;
//		if(wheel1_accel>WHEEL_SET_ACCEL_MAX)		wheel1_accel = WHEEL_SET_ACCEL_MAX;
		
		if(wheel2>WHEEL_SET_SPEED_MAX)					wheel2 = WHEEL_SET_SPEED_MAX;
		if(wheel2<WHEEL_SET_SPEED_MIN)					wheel2 = WHEEL_SET_SPEED_MIN;
//		if(wheel2_accel>WHEEL_SET_ACCEL_MAX)		wheel2_accel = WHEEL_SET_ACCEL_MAX;
		
		if(wheel3>WHEEL_SET_SPEED_MAX)					wheel3 = WHEEL_SET_SPEED_MAX;
		if(wheel3<WHEEL_SET_SPEED_MIN)					wheel3 = WHEEL_SET_SPEED_MIN;
//		if(wheel3_accel>WHEEL_SET_ACCEL_MAX)		wheel3_accel = WHEEL_SET_ACCEL_MAX;
		
		if(wheel4>WHEEL_SET_SPEED_MAX)					wheel4 = WHEEL_SET_SPEED_MAX;
		if(wheel4<WHEEL_SET_SPEED_MIN)					wheel4 = WHEEL_SET_SPEED_MIN;
//		if(wheel4_accel>WHEEL_SET_ACCEL_MAX)		wheel4_accel = WHEEL_SET_ACCEL_MAX;
		
		
//		LCD_Display_int(wheel1,0,6);
//		LCD_Display_int(wheel2,5,6);
//		LCD_Display_int(wheel3,10,6);
//		LCD_Display_int(wheel4,15,6);
		
//		LCD_Display_int(wheel1,0,6);
//		LCD_Display_int(wheel1,5,6);
//		LCD_Display_int(wheel1,10,6);
//		LCD_Display_int(wheel1,15,6);
		
		
		UNDER1_MOVE(wheel1,wheel1_accel);
		UNDER2_MOVE(wheel2,wheel2_accel);
		UNDER3_MOVE(wheel3,wheel3_accel);
		UNDER4_MOVE(wheel4,wheel4_accel);
		      
		/*
		if((-(int32_t)(Encoder_TIM4*65536+TIM4->CNT))>=(position-200))
			break;*/
		delay_ms(TIM_Piece);
		
	}
	
		y_to_vx = (int32_t)((now_speed_x-target_speed_x)/10);
		y_to_vy = (int32_t)((now_speed_y-target_speed_y)/10);
		wheel1 = (int32_t)(-y_to_vx + y_to_vy );
		wheel2 = (int32_t)(-(y_to_vx + y_to_vy ));
		wheel3 = (int32_t)(-(-y_to_vx + y_to_vy ));
		wheel4 = (int32_t)(y_to_vx + y_to_vy );

	
//		LCD_Display_int(wheel1_accel,0,7);
//		LCD_Display_int(wheel2_accel,5,7);
//		LCD_Display_int(wheel3_accel,10,7);
//		LCD_Display_int(wheel4_accel,15,7);
		UNDER1_MOVE(wheel1,wheel1_accel);
		UNDER2_MOVE(wheel2,wheel2_accel);
		UNDER3_MOVE(wheel3,wheel3_accel);
		UNDER4_MOVE(wheel4,wheel4_accel);
/*	while((-(int32_t)(Encoder_TIM4*65536+TIM4->CNT))<=(position));*/
//	BEED_ON;
	Move_XY_Freedom(0, 0, WHEEL_SET_ACCEL_MAX);
	return 0;
	
}

/**
	*	@brief	�������A�������ٶ�
	*	@param	
	*	@retval
	*/
MOTOR_SPEED_STRUCT Translation_Speed_Reverse(int32_t wheel1_speed, int32_t wheel2_speed,int32_t wheel3_speed,int32_t wheel4_speed )
{
	MOTOR_SPEED_STRUCT ture_speed;
	ture_speed.MOTOR_SPEED_X = (int32_t)(( - wheel1_speed - wheel2_speed + wheel3_speed + wheel4_speed)/4);
	ture_speed.MOTOR_SPEED_Y = (int32_t)((  wheel1_speed - wheel2_speed - wheel3_speed + wheel4_speed)/4);
	return ture_speed;
}


extern int32_t Encoder_TIM8;
extern Robocon_Handle_Struct Rb_Handle_Control;
int speed = 0;
int speed_adjust;
float C_angleoffset;
float C_nowangle;//����������ǰ�Ƕ�
int16_t C_differedge;//��������C��֮��
int16_t C_nowedge;//����������ǰ�߳�
int16_t C_targetedge;//��������Ŀ��߳�
uint8_t Pitch_Adjustment(float C_targetangle,int Accel_time,float P,float D,uint8_t direction)
{
  int16_t A_edge = 317;//��������A��
  int16_t B_edge = 330;//��������B��


	float C_initangle;//����������ʼ�Ƕ�
	
	float C_theoryangle;

	float C_angleoffset_last;
	float Angle_adjust = Rb_Handle_Control.Elevation_Position._int_date;//��������΢���Ƕ�
	C_targetangle = C_targetangle + Angle_adjust;
	float Distance;
	int piece_count = 0;
	float t1_to_x = 0;		//t��x��ӳ��
	float t2_to_x = 0;
	int control_count = Accel_time/TIM_Piece;
	float Angle_speed_average = 0;
	float Angle_speed = 0;
	C_initangle = (Encoder_TIM8*65536+TIM8->CNT)*360/4000;//�����ʼ�Ƕ�
	
	Angle_speed_average = ((C_targetangle - C_initangle)/360)*2*3.1416f/Accel_time*1000;

	C_nowedge = sqrt((A_edge*A_edge)+(B_edge*B_edge)-2*A_edge*B_edge*(cos(C_initangle)));//���Һ�������C��
	C_targetedge = sqrt((A_edge*A_edge)+(B_edge*B_edge)-2*A_edge*B_edge*(cos(C_targetangle*3.1416f/180)));
	C_differedge = abs(C_targetedge-C_nowedge);

	Distance = (C_differedge/4)*2000;//������Ӧ��������
	/*�������߼����ٶ�*/
	float V_average = ((C_differedge/4)*1000*60)/Accel_time;
	
	while(piece_count < control_count)
	{
	t1_to_x = 2 * 3.1416f * (float)TIM_Piece * (float)piece_count / Accel_time;		//���㵱ǰ���ڵĿ���ʱ�䣬ӳ�䵽�������ߵ�X��������
	t2_to_x = 2 * 3.1416f * (float)TIM_Piece * (float)piece_count / Accel_time;		//���㵱ǰ���ڵĿ���ʱ�䣬ӳ�䵽�������ߵ�X��������

	speed = (int)(V_average * (sin(t1_to_x - 3.1416f/2) + 1));
	Angle_speed = (Angle_speed_average * (sin(t2_to_x - 3.1416f/2) + 1));	
		/*���������߹��Ƕ�*/
	C_theoryangle = (float)C_initangle + (((float)(C_targetangle - C_initangle)/(2*3.1416f))*(-cos(t2_to_x - 3.1416f/2) +  t2_to_x));
		
		/*����ʵ�ʽǶ�*/
	C_nowangle = (((float)(Encoder_TIM8*65536+TIM8->CNT)*360)/4000);//���㵱ǰ�Ƕ�ֵ��	
	
  C_angleoffset = C_theoryangle-C_nowangle ;

  speed_adjust= speed+P*C_angleoffset + D*(C_angleoffset - C_angleoffset_last);		
		
	C_angleoffset_last = C_angleoffset;	
	if(speed_adjust>=speed25_max)    speed_adjust = speed25_max;
	if(speed_adjust<=speed25_min)    speed_adjust = speed25_min;	
		
	switch(direction)
	{
		case	0:					{
	/*	������	*/				UNDER6_MOVE(speed_adjust,150);//y_to_ax

											}break;
		
		case	1:					{
	/*	������	*/				UNDER6_MOVE(-speed_adjust,150);

											}break;		
	}		
		
	piece_count++;
		
	delay_ms(TIM_Piece);
	}
	UNDER6_MOVE(0,100);
	return 0;
}








