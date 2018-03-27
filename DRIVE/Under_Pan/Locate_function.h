#ifndef	_LOCATE_FUNTION_H
#define _LOCATE_FUNTION_H

#include "includes.h"


/**************************************辅助参数宏********************************/
#define LOCATE_BOOL	int

#define LOCATE_ON	1
#define LOCATE_OFF	0

#define SIGN_CLEAR	0

#define TIME_DEFAULT	-1

//全场定位系统控制周期
#define TIM_Piece 20	//调节时间片---20ms



/**
	*	@brief:		位移支线的控制宏
	*/
#define MAX_Speed_x	300
#define MAX_Speed_y	300


#define D_Param	0
#define X_PParam	0.8
#define Y_PParam	1

//最快加减速控制斜率
#define MAX_SPEED	(2.285 * 120.877)		//		276n/min 120.877n/min表示1m/s
#define MAX_K		(MAX_SPEED/1750)		//		计算斜率，1750为ms，为一半时间

//速度单位转换
#define SPEED_CONVERT	120.877		//	120.877n/min = 1m/s




/**
	*	@brief:		角度支线的控制宏
	*/
#define MAX_Speed_w		150

#define	POS_OFFSET_ignore	20	
	
#define ANGLE_TIME 2
#define Angle_P_Adjiust	2
#define Angle_I_Adjiust	0
#define Angle_D_Adjiust	0

#define W_PParam	0.8

#define COLOCKWISE		0	//顺时针
#define AINTCOLOCKWISE	1	//逆时针

#define MAX_W		((float)180/2/2)		//		2s 90度 90/2为虚拟的角速度
#define	MAX_W2		((float)180/2000)
#define MAX_AK		((float)166/2000)		//		4000	83转/min
#define	MAX_AK2		((float)166 * 360 /2000)


#define W_SPEED_CONVERT	241.754		//	角速度单位转换




/**
	*	@brief:		全场定位系统输出宏
	*/


//全场定位系统输出接口，速度输出	
#define LOCATE_control_output(output_vx, output_vy, output_vw)	Move_XY_Rotate_Freedom(output_vx, output_vy, output_vw, 300)

//全场定位结束控制，停止底盘运行	*/
#define LOCATE_control_stop		LOCATE_control_output(0, 0, 0)
/*	东大全场定位数据接收包	*/




/**************************************相关结构体控制结构体设计********************************/
typedef struct
{
	float ZAngle;				//偏航角
	float XAngle;				//俯仰角
	float YAngle;				//横滚角
	int X;						//x轴方向的位移
	int Y;						//y轴方向的位移
	float W;					//角速度
	uint8_t Riceve_flag;		//接收标志
}Locate_DATE;


/**
	*	@brief	全场定位系统控制结构体
	*/
/*	全场定位行走方式控制	*/
typedef enum
{
	straight_walk = 0,		//直线行走
	arc_walk,				//圆弧行走
}LOCATE_WALK;

/*	全场定位控制命令	*/
typedef enum
{
	System_Exit = 0,	//退出系统
	Scram_Jerk,			//急停
	Pre_Deal,			//预处理
	Deal_With,			//处理
}LOCATE_Command;

/*	全场定位系统反馈数据输入*/
typedef struct
{
	float *ZAngle;				//偏航角
	float *XAngle;				//俯仰角
	float *YAngle;				//横滚角
	int *X;						//x轴方向的位移
	int *Y;						//y轴方向的位移
	float *W;					//角速度大小
	uint8_t *Riceve_flag;		//接收标志
}LOCATE_RTIME;

/*	全场定位系统输入控制包细节	*/
typedef struct
{
	float target_pos;	//目标位移—X，单位mm
	int max_speed;		//最大速度-X，单位n/min(转/分)
	int accel_time;		//加速控制时间
}LOCATE_CONTROL_IN_T;


/*	全场定位系统输入控制包	*/
typedef struct
{
	LOCATE_CONTROL_IN_T	control_x;
	LOCATE_CONTROL_IN_T	control_y;
	LOCATE_CONTROL_IN_T	control_angle;
	LOCATE_BOOL Rotate_flag;	//旋转方向 0逆时针  1顺时针
	LOCATE_WALK walk_flag;		//行走方式选择
	LOCATE_BOOL System_Flag;	//全场定位系统运行中断
	LOCATE_BOOL Update_Flag;	//全场定位系统数据更新中断
	LOCATE_Command	Command;	//全场定位命令
}LOCATE_CONTROL_IN;

/*	全场定位系统实时控制包细节	*/
typedef struct
{
	int init_pos;		
	int init_speed;		
	int end_pos;		
	int end_speed;		
}LOCATE_CONTROL_tail;

/*	全场定位系统实时控制包细节角度	*/
typedef struct
{
	float init_angle;		
	int	init_speed;			
	float end_angle;		
	int end_speed;		
}LOCATE_CONTROL_TAngle;

/*	全场定位系统实时控制包	*/
typedef struct
{
	LOCATE_CONTROL_tail	control_x;
	LOCATE_CONTROL_tail	control_y;
	LOCATE_CONTROL_TAngle control_angle;
}LOCATE_CONTROL;

/*	全场定位系统性能参数设计结构体	*/
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
/*****************************************	动静态算法结合	*********************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*	全场定位内部算法函数指针自定义	*/

typedef int (*Func_Slove)(int, float);
typedef float (*Func_ASlove)(float, float);
typedef float (*Func_Slove2)(int, int);
/*	全场定位内部输出结构体	*/
typedef struct
{
	int speed_x;
	int speed_y;
	int speed_w;
}Locate_Output;

/*	全场定位控制块	*/

typedef struct _LOCATE_TCB
{	
	int base_speed;	
	int base_dist;	
	int add_speed;		
	int add_dist;			
	float adv_speed;
	float adv_pos;
	int better_time;
	int accel_time_count;	//需要的时间块数目
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
	int accel_time_count;	//需要的时间块数目
	LOCATE_BOOL	Clear;
	int (*TCB_Func)(struct _LOCATE_TCB_angle* control_tcb, Locate_Output loacate_output, float angle, float P_param, int *result);
}LOCATE_TCB_angle;

/*	全场定位内部使用函数指针	*/
typedef void (*TCB_Func)(LOCATE_TCB stop_tcb);


/*	全场定位扩展链表	*/
typedef struct
{
	Tlist_node	locate_plist;
	LOCATE_TCB	date_passage;
}Locate_Tlist;

/*	全场定位扩展链表	*/
typedef struct
{
	Tlist_node			locate_plist;
	LOCATE_TCB_angle	date_passage;
}Locate_Tlist_angle;


//typedef int(*Walk_Func)(LOCATE_TCB_angle control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_tail *locate_controlT, int distance, float P_param, int *result);

typedef int (*tcb_Func)(struct _LOCATE_TCB_angle control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_TAngle *locate_controlT, float angle, float P_param, int *result);

typedef int (*tcb_pFunc)(struct _LOCATE_TCB control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_tail *locate_controlT, int distance, float P_param, int *result);
///////////////////////////////////////////////////////////////////////////////////////////////////////

/*****************************************	全场定位相关函数设计	**********************************/

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	全场定位系统启动信号管理
*/
void LOCATE_KEY(LOCATE_BOOL key);

void LOCATE_KEY_clear(void);

void LOCATE_Ready(void);



/*	
	全场定位链表函数设计
*/
void Locate_MM_Clear(void *p, int size);
void Locate_handnode_init(Locate_Tlist *Amend_node);
void Locate_Ahandnode_init(Locate_Tlist_angle *Amend_node);
							
int Locate_node_inster_after(Locate_Tlist *Amend_node);	
int Locate_Anode_inster_after(Locate_Tlist_angle *Amend_node);

int Locate_node_clear(Locate_Tlist *Amend_node);
int Locate_Anode_clear(Locate_Tlist_angle *Amend_node);

							

/*
	方程计算程序
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
	全场定位系统 曲线设计部分
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
	全场定位系统 终端控制部分
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






