#ifndef	_LOCATE_FUNTION_H
#define _LOCATE_FUNTION_H

#include "includes.h"


/**************************************����������********************************/
#define LOCATE_BOOL	int

#define LOCATE_ON	1
#define LOCATE_OFF	0

#define SIGN_CLEAR	0

#define TIME_DEFAULT	-1

//ȫ����λϵͳ��������
#define TIM_Piece 20	//����ʱ��Ƭ---20ms



/**
	*	@brief:		λ��֧�ߵĿ��ƺ�
	*/
#define MAX_Speed_x	300
#define MAX_Speed_y	300


#define D_Param	0
#define X_PParam	0.8
#define Y_PParam	1

//���Ӽ��ٿ���б��
#define MAX_SPEED	(2.285 * 120.877)		//		276n/min 120.877n/min��ʾ1m/s
#define MAX_K		(MAX_SPEED/1750)		//		����б�ʣ�1750Ϊms��Ϊһ��ʱ��

//�ٶȵ�λת��
#define SPEED_CONVERT	120.877		//	120.877n/min = 1m/s




/**
	*	@brief:		�Ƕ�֧�ߵĿ��ƺ�
	*/
#define MAX_Speed_w		150

#define	POS_OFFSET_ignore	20	
	
#define ANGLE_TIME 2
#define Angle_P_Adjiust	2
#define Angle_I_Adjiust	0
#define Angle_D_Adjiust	0

#define W_PParam	0.8

#define COLOCKWISE		0	//˳ʱ��
#define AINTCOLOCKWISE	1	//��ʱ��

#define MAX_W		((float)180/2/2)		//		2s 90�� 90/2Ϊ����Ľ��ٶ�
#define	MAX_W2		((float)180/2000)
#define MAX_AK		((float)166/2000)		//		4000	83ת/min
#define	MAX_AK2		((float)166 * 360 /2000)


#define W_SPEED_CONVERT	241.754		//	���ٶȵ�λת��




/**
	*	@brief:		ȫ����λϵͳ�����
	*/


//ȫ����λϵͳ����ӿڣ��ٶ����	
#define LOCATE_control_output(output_vx, output_vy, output_vw)	Move_XY_Rotate_Freedom(output_vx, output_vy, output_vw, 300)

//ȫ����λ�������ƣ�ֹͣ��������	*/
#define LOCATE_control_stop		LOCATE_control_output(0, 0, 0)
/*	����ȫ����λ���ݽ��հ�	*/




/**************************************��ؽṹ����ƽṹ�����********************************/
typedef struct
{
	float ZAngle;				//ƫ����
	float XAngle;				//������
	float YAngle;				//�����
	int X;						//x�᷽���λ��
	int Y;						//y�᷽���λ��
	float W;					//���ٶ�
	uint8_t Riceve_flag;		//���ձ�־
}Locate_DATE;


/**
	*	@brief	ȫ����λϵͳ���ƽṹ��
	*/
/*	ȫ����λ���߷�ʽ����	*/
typedef enum
{
	straight_walk = 0,		//ֱ������
	arc_walk,				//Բ������
}LOCATE_WALK;

/*	ȫ����λ��������	*/
typedef enum
{
	System_Exit = 0,	//�˳�ϵͳ
	Scram_Jerk,			//��ͣ
	Pre_Deal,			//Ԥ����
	Deal_With,			//����
}LOCATE_Command;

/*	ȫ����λϵͳ������������*/
typedef struct
{
	float *ZAngle;				//ƫ����
	float *XAngle;				//������
	float *YAngle;				//�����
	int *X;						//x�᷽���λ��
	int *Y;						//y�᷽���λ��
	float *W;					//���ٶȴ�С
	uint8_t *Riceve_flag;		//���ձ�־
}LOCATE_RTIME;

/*	ȫ����λϵͳ������ư�ϸ��	*/
typedef struct
{
	float target_pos;	//Ŀ��λ�ơ�X����λmm
	int max_speed;		//����ٶ�-X����λn/min(ת/��)
	int accel_time;		//���ٿ���ʱ��
}LOCATE_CONTROL_IN_T;


/*	ȫ����λϵͳ������ư�	*/
typedef struct
{
	LOCATE_CONTROL_IN_T	control_x;
	LOCATE_CONTROL_IN_T	control_y;
	LOCATE_CONTROL_IN_T	control_angle;
	LOCATE_BOOL Rotate_flag;	//��ת���� 0��ʱ��  1˳ʱ��
	LOCATE_WALK walk_flag;		//���߷�ʽѡ��
	LOCATE_BOOL System_Flag;	//ȫ����λϵͳ�����ж�
	LOCATE_BOOL Update_Flag;	//ȫ����λϵͳ���ݸ����ж�
	LOCATE_Command	Command;	//ȫ����λ����
}LOCATE_CONTROL_IN;

/*	ȫ����λϵͳʵʱ���ư�ϸ��	*/
typedef struct
{
	int init_pos;		
	int init_speed;		
	int end_pos;		
	int end_speed;		
}LOCATE_CONTROL_tail;

/*	ȫ����λϵͳʵʱ���ư�ϸ�ڽǶ�	*/
typedef struct
{
	float init_angle;		
	int	init_speed;			
	float end_angle;		
	int end_speed;		
}LOCATE_CONTROL_TAngle;

/*	ȫ����λϵͳʵʱ���ư�	*/
typedef struct
{
	LOCATE_CONTROL_tail	control_x;
	LOCATE_CONTROL_tail	control_y;
	LOCATE_CONTROL_TAngle control_angle;
}LOCATE_CONTROL;

/*	ȫ����λϵͳ���ܲ�����ƽṹ��	*/
typedef struct
{
	float	P_param;
	float	P_param_mid;
}Locate_Property;

typedef struct
{
	Locate_Property	X_Property;
	Locate_Property	Y_Property;
	Locate_Property	Z_Property;
}LOCATE_Property;
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*****************************************	����̬�㷨���	*********************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*	ȫ����λ�ڲ��㷨����ָ���Զ���	*/

typedef int (*Func_Slove)(int, float);
typedef float (*Func_ASlove)(float, float);
typedef float (*Func_Slove2)(int, int);
/*	ȫ����λ�ڲ�����ṹ��	*/
typedef struct
{
	int speed_x;
	int speed_y;
	int speed_w;
}Locate_Output;

/*	ȫ����λ���ƿ�	*/

typedef struct _LOCATE_TCB
{	
	int base_speed;	
	int base_dist;	
	int add_speed;		
	int add_dist;			
	float adv_speed;
	float adv_pos;
	int better_time;
	int accel_time_count;	//��Ҫ��ʱ�����Ŀ
	LOCATE_BOOL	Clear;
	int (*TCB_Func)(struct _LOCATE_TCB control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_tail *locate_controlT, int distance, float P_param, int *result);
}LOCATE_TCB;


typedef struct _LOCATE_TCB_angle
{	
	int base_speed;	
	float base_angle;
	int add_speed;
	float add_angle;
	float adv_speed;
	float adv_angle;
	int direction;		
	int better_time;
	int accel_time_count;	//��Ҫ��ʱ�����Ŀ
	LOCATE_BOOL	Clear;
	int (*TCB_Func)(struct _LOCATE_TCB_angle* control_tcb, Locate_Output loacate_output, float angle, float P_param, int *result);
}LOCATE_TCB_angle;

/*	ȫ����λ�ڲ�ʹ�ú���ָ��	*/
typedef void (*TCB_Func)(LOCATE_TCB stop_tcb);


/*	ȫ����λ��չ����	*/
typedef struct
{
	Tlist_node	locate_plist;
	LOCATE_TCB	date_passage;
}Locate_Tlist;

/*	ȫ����λ��չ����	*/
typedef struct
{
	Tlist_node			locate_plist;
	LOCATE_TCB_angle	date_passage;
}Locate_Tlist_angle;


//typedef int(*Walk_Func)(LOCATE_TCB_angle control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_tail *locate_controlT, int distance, float P_param, int *result);

typedef int (*tcb_Func)(struct _LOCATE_TCB_angle control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_TAngle *locate_controlT, float angle, float P_param, int *result);

typedef int (*tcb_pFunc)(struct _LOCATE_TCB control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_tail *locate_controlT, int distance, float P_param, int *result);
///////////////////////////////////////////////////////////////////////////////////////////////////////

/*****************************************	ȫ����λ��غ������	**********************************/

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	ȫ����λϵͳ�����źŹ���
*/
void LOCATE_KEY(LOCATE_BOOL key);

void LOCATE_KEY_clear(void);

void LOCATE_Ready(void);



/*	
	ȫ����λ���������
*/
void Locate_MM_Clear(void *p, int size);
void Locate_handnode_init(Locate_Tlist *Amend_node);
void Locate_Ahandnode_init(Locate_Tlist_angle *Amend_node);
							
int Locate_node_inster_after(Locate_Tlist *Amend_node);	
int Locate_Anode_inster_after(Locate_Tlist_angle *Amend_node);

int Locate_node_clear(Locate_Tlist *Amend_node);
int Locate_Anode_clear(Locate_Tlist_angle *Amend_node);

							

/*
	���̼������
*/
int V_K_T(int speed, float slope);
							
float T_K_V(int star_time, float slope);
	
float T_P_V(int star_time, float position);

float T_P_W(int star_time, float position);
	
int V_T_P(int speed_mid, float time);	

int W_AK_T(int speed, float slope);	

float T_W_P(int speed, int star_time);

int P_defaultK_T(int position, float slope);

int AP_defaultK_T(int position, float slope);

void Limit_Pos(float *pos, float slope, float speed, float max_speed);

void Limit_Pos_angle(float *pos, float slope, float speed, float max_speed);

/*
	ȫ����λϵͳ ������Ʋ���
*/

void Locate_stop(LOCATE_CONTROL_tail Control_tail, Locate_Tlist *Control_list, tcb_pFunc MyFunc);

void Locate_Astop(LOCATE_CONTROL_TAngle Control_tail, Locate_Tlist_angle *Control_list);

int Locate_Astop_rapidly(LOCATE_CONTROL_TAngle Control_tail, Locate_Tlist_angle *Control_list);

int Locate_stop_rapidly(LOCATE_CONTROL_tail Control_tail, Locate_Tlist *Control_list, tcb_pFunc MyFunc);

void Locate_comman(int add_pos, int base_pos, Locate_Tlist *Control_List, tcb_pFunc MyFunc);

void Locate_comman_limit(int add_pos, int base_pos, float speed_max, float limit_pos, Locate_Tlist *Control_List, tcb_pFunc MyFunc);
	
void Locate_Acomman(float add_pos, float base_pos, Locate_Tlist_angle *Control_List);

void Locate_Acomman_limit(float add_pos, float base_pos, float speed_max, float limit_pos, Locate_Tlist_angle *Control_List);
	
void Locate_comman_Diy(int add_pos, LOCATE_CONTROL_IN_T	ControlINT, LOCATE_CONTROL_tail ControlT, Locate_Tlist *Control_List, tcb_pFunc MyFunc);

void Locate_comman_Diy_limit(int add_pos, float speed_max, float limit_pos, LOCATE_CONTROL_IN_T	ControlINT, LOCATE_CONTROL_tail ControlT, Locate_Tlist *Control_List, tcb_pFunc MyFunc);
	
void Locate_Acomman_Diy(int add_pos, LOCATE_CONTROL_IN_T ControlINT, LOCATE_CONTROL_TAngle ControlT, Locate_Tlist_angle *Control_List);

void Locate_Acomman_Diy_limit(int add_pos, float speed_max, float limit_pos, LOCATE_CONTROL_IN_T ControlINT, LOCATE_CONTROL_TAngle ControlT, Locate_Tlist_angle *Control_List);
	
void Locate_run_adv(int add_pos, float speed_in, LOCATE_CONTROL_tail ControlT, Locate_Tlist *Control_List, tcb_pFunc MyFunc);

void Locate_run_adv_limit(int add_pos, float speed_in, float speed_max, float limit_pos, LOCATE_CONTROL_tail ControlT, Locate_Tlist *Control_List, tcb_pFunc MyFunc);
	
void Locate_Arun_adv(int add_pos, float speed_in, LOCATE_CONTROL_TAngle ControlT, Locate_Tlist_angle *Control_List);

void Locate_Arun_adv_limit(int add_pos, float speed_in, float speed_max, float limit_pos, LOCATE_CONTROL_TAngle ControlT, Locate_Tlist_angle *Control_List);
	
void Locat_Walk_Line(Locate_Tlist *Control_TCB, int target_p, tcb_pFunc MyFunc);

void Locat_Walk_Line_angle(Locate_Tlist_angle *Control_TCB, float target_p);

int SinAccelPOS_ADD_X(LOCATE_TCB control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_tail *locate_controlT, int distance, float P_param, int *result);

int SinAccelPOS_Stop_X(LOCATE_TCB control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_tail *locate_controlT, int distance, float P_param, int *result);

int SinAccelPOS_ADD_Y(LOCATE_TCB control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_tail *locate_controlT, int distance, float P_param, int *result);

int SinAccelPOS_Stop_Y(LOCATE_TCB control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_tail *locate_controlT, int distance, float P_param, int *result);

int SinAccelPOS_ADD_angle(LOCATE_TCB_angle *control_tcb, Locate_Output loacate_output, float angle, float P_param, int *result);

int SinAccelPOS_Stop_angle(LOCATE_TCB_angle* control_tcb, Locate_Output loacate_output, float angle, float P_param, int *result);
	
int CosAccelPOS_DEC(LOCATE_TCB control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_tail locate_controlT, int distance, float P_param, int *result);

int CosAccelPOS_DEC_angle(LOCATE_TCB_angle control_tcb, Locate_Output loacate_output, float angle, float P_param, int *result);
						
void Control_Angle_Walk_All(	int   	Speed_Vx,
								int   	Speed_Vy,
								int     angle_speed);
							
								


/*
	ȫ����λϵͳ �ն˿��Ʋ���
*/
void LOCATE_date_cut(Locate_DATE *locate_rtime_init);
								
void LOCATE_Property_INIT(LOCATE_Property *locate_property, LOCATE_Property *user_property);
								
void LOCATE_INIT(Locate_DATE *locate_rtime);
								
void LOCATE_pre_deal(LOCATE_CONTROL_IN	locate_control_in);
							
void LOCATE_Control_Run_X(Locate_Tlist *controlP, LOCATE_CONTROL_tail *Control_tail, int *end_flag, int *speed_result, int distance, float P_param);
								
void LOCATE_Control_Run_Y(Locate_Tlist *controlP, LOCATE_CONTROL_tail *Control_tail, int *end_flag, int *speed_result, int distance, float P_param);

void LOCATE_AControl_Run(Locate_Tlist_angle *controlP, int *end_flag, int *speed_result, float distance, float P_param);

void LOCATE_Control_In(LOCATE_CONTROL_IN	*locate_control_in);

#endif






