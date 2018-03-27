//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************************
**
	*	@brief									全场定位算法
	*	@supplement		不同的底盘安装方式,物理模型基础不变,但是轮子的速度方向需要根据实际安装修正
	*								此算法是基于麦克纳姆轮,安装方式为米字长方形
	*								硬件环境基础：东大全场定位模块 Stm32f103
	*
**********************************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Locate_function.h"

/*	全场定位数据包裹	*/
volatile Locate_DATE	Locate_Date;	//全场定位传感器数据包
volatile LOCATE_RTIME	Locate_RTime;	//全场定位实时实际数据包
LOCATE_CONTROL	Locate_Control;	//全场定位控制数据储存包

volatile float angle_P = 0;			//辅助PID计算结果
volatile float angle_last = 0;		//上周期角度
volatile float angle_help_last = 0;	//上次目标角变
volatile float last_angle_offset=0;	//上周起角度偏差
volatile float angle_offset_offset_help = 0;	//辅助角度偏差的偏差
extern float Adjust_P;
extern float Adjust_Time;

/*	全场定位系统调用开关	*/
static LOCATE_BOOL LOCATE_key = 0;
static int LOCATE_ready_flag = LOCATE_ON;

//*TCB_handp[0]	为X链表, *TCB_handp[1]	为Y链表, *TCB_handp_w	为W链表, 
 Locate_Tlist *TCB_handp[2] = {NULL};
 Locate_Tlist_angle *TCB_handp_w = NULL;
 Locate_Tlist X_Handp;
 Locate_Tlist Y_Handp;
 Locate_Tlist_angle W_Handp;


/*·全场定位最终输出参数	*/
Locate_Output Locate_output;

/*	全场定位系统性能参数设计	*/
LOCATE_Property	LOCATE_PPT;
 
/*	计时测试参数	*/
extern int Init_Time;
extern int End_Time;
extern int Time_5S;

/******************************************************************************************************
										全场定位算法制定
						基于正弦函数无限可导的特性，制作全场定位上层算法
						可根据不同的的数据来源进行移植
						数据来源需提供 位置信息，角度信息
******************************************************************************************************/

/**
	*	@brief:		void LOCATE_KEY		全场定位系统启动信号管理
	*	@param:		
	*	@retva:		NONE
	*	@supplement:	全场定位前必须执行的程序，作为全场定位启动的数据接口
	*/

void LOCATE_KEY(LOCATE_BOOL key)
{
	LOCATE_key = key;
}

/**
*@brief:	LOCATE_KEY_clear	清楚定位启动开关
*/
void LOCATE_KEY_clear(void)
{
	LOCATE_key = SIGN_CLEAR;
}

/**
*@brief：	LOCATE_Ready		全场定位系统就绪标志
*/
void LOCATE_Ready(void)
{
	LOCATE_ready_flag = LOCATE_ON;
}



/**********************************************************************************************/
/***********************************		链表程序		***********************************/
/*								此链表程序是在链表文件支持下实现						  	  */
/*									可以自由修改链表的数据段								  */
/**********************************************************************************************/

/**
	*@brief：	Locate_MM_Clear		快速清零小内存区域
	*/
void Locate_MM_Clear(void *p, int size)
{
	char *ptr = NULL;
	ptr = (char *)p;
	while(size--)
	{
		*ptr = 0;
		ptr++;
	}
}




/**
	*@brief：	Locate_handnode_init	初始化链表头
				Locate_Ahandnode_init	初始化链表头
	*/

void Locate_handnode_init(Locate_Tlist *Amend_node)
{
	hand_node_init(&(Amend_node->locate_plist));
}

void Locate_Ahandnode_init(Locate_Tlist_angle *Amend_node)
{
	hand_node_init(&(Amend_node->locate_plist));
}





/**
	*@brief	Locate_node_inster_after	在当前节点后插入节点
			Locate_Anode_inster_after	在当前节点后插入节点
	*/
int Locate_node_inster_after(Locate_Tlist *Amend_node)
{
	Locate_Tlist *pnode = NULL;
	
	pnode = (Locate_Tlist *)malloc(sizeof(Locate_Tlist));
	if(NULL == pnode)
		return 1;	//申请未成功
	
	Locate_MM_Clear(pnode, sizeof(*pnode));
	insert_node_after(&(Amend_node->locate_plist), &(pnode->locate_plist));
	return 0;
}

int Locate_Anode_inster_after(Locate_Tlist_angle *Amend_node)
{
	Locate_Tlist_angle *pnode = NULL;
	
	pnode = (Locate_Tlist_angle *)malloc(sizeof(Locate_Tlist_angle));
	if(NULL == pnode)
		return 1;	//申请未成功
	
	Locate_MM_Clear(pnode, sizeof(*pnode));
	insert_node_after(&(Amend_node->locate_plist), &(pnode->locate_plist));
	return 0;
}





/**	
	*@brief：	Locate_node_clear	删除当前节点
				Locate_Anode_clear	删除当前节点
	*/
int Locate_node_clear(Locate_Tlist *Amend_node)
{
	Locate_Tlist *pnode = Amend_node;
	while(!(NULL == (pnode->locate_plist).nexttcb))
	{
		pnode =(Locate_Tlist *)((pnode->locate_plist).nexttcb);
	}
	while(!(pnode->locate_plist.pretcb == (Tlist_node *)pnode))
	{
		pnode =(Locate_Tlist *)(pnode->locate_plist.pretcb);
		free((Locate_Tlist *)(pnode->locate_plist.nexttcb));
		
	}
	Amend_node->locate_plist.nexttcb = NULL;
	return 0;
}

int Locate_Anode_clear(Locate_Tlist_angle *Amend_node)
{
	Locate_Tlist_angle *pnode = Amend_node;
	while(!(NULL == (pnode->locate_plist).nexttcb))
	{
		pnode =(Locate_Tlist_angle *)((pnode->locate_plist).nexttcb);
	}
	while(!(pnode->locate_plist.pretcb == (Tlist_node *)pnode))
	{
		pnode =(Locate_Tlist_angle *)(pnode->locate_plist.pretcb);
		free((Locate_Tlist_angle *)(pnode->locate_plist.nexttcb));
		
	}
	Amend_node->locate_plist.nexttcb = NULL;
	return 0;
}


/**********************************************************************************************/
/***********************************		方程计算程序		*******************************/
/*									此程序是实现简单方程的计算								  */
/*							 主要是针对以此方程：时间 速度 位移 的计算				  		  */
/**********************************************************************************************/
/**
	*	@brief:		V_K_T	通过速度V 和斜率K 得到时间T
	*	@param:		speed:	峰值速度	slope：斜率
	*	@retval:	star_time	返回时间 单位：ms
	*/
int V_K_T(int speed, float slope)
{
	float star_speed = 0;
	int star_time = 0;
	speed = ABS_int(speed);
	star_speed = (float)speed;
//	star_speed = ABS_float((float)speed/SPEED_CONVERT);
	star_time = (int)(star_speed / MAX_K );
	
	return star_time;	
}





/**
	*	@brief：	T_K_V	通过时间 斜率 得到速度
	*	@param：	star_time：时间		slope：斜率
	*	@retval：	star_speed	返回速度
	*/
float T_K_V(int star_time, float slope)
{
	float star_speed = 0;
	
	star_speed = ((float)star_time * slope);
	
	return star_speed;	
}





/**
	*	@brief：	T_P_V	通过时间 位移 得到速度
	*	@param：	star_time：时间		position：位移
	*	@retval：	star_speed	返回速度
	*/
float T_P_V(int star_time, float position)
{
	float star_speed = 0;
	
	star_speed = (float)(position * 2 / star_time *SPEED_CONVERT);
	
	return star_speed;	
}





/**
	*	@brief：	T_P_W	通过时间 位移 得到速度
	*	@param：	star_time：时间		position：位移
	*	@retval：	star_speed	返回速度
	*/
float T_P_W(int star_time, float position)
{
	float star_speed = 0;
	
	star_speed = (float)(position/360 * 2 * 3.1416f / star_time * 1000 * SPEED_CONVERT * 0.875f);
	
	return star_speed;	
}




/**
	*	@brief:		V_T_P	通过速度V 和时间T 得到位移P
	*	@param:		speed_mid	平均速度
	*	@retva:		return star_time	返回时间：ms
	*/
int V_T_P(int speed_mid, float time)
{
	float star_speed = 0;
	int star_position = 0;
	speed_mid = ABS_int(speed_mid);
	star_speed = (float)speed_mid/SPEED_CONVERT;
	star_position = (int)(star_speed * time);
	
	return star_position;	
}





/**
	*	@brief:		W_K_T	通过角速度W 和斜率K 得到时间T
	*	@param:		speed	角速度		slope：斜率
	*	@retva:		return star_time	返回时间：ms
	*/
int W_AK_T(int speed, float slope)
{
	float star_speed = 0;
	int star_time = 0;
	
	star_speed = ABS_float((float)speed);
	star_time = (int)(star_speed / slope);
	
	return star_time;	
}




/**
	*	@brief:		W_K_T	通过角速度W 和斜率K 得到时间T
	*	@param:		speed	角速度		slope：斜率
	*	@retva:		return star_time	返回时间：ms
	*/
float T_W_P(int speed, int star_time)
{
	float star_speed = 0;
	float star_APos = 0;
	
	star_speed = ABS_float((float)speed/SPEED_CONVERT);
	star_APos = (float)star_speed / 0.875f *  (float)star_time / 2 /3.1416f * 360 / 1000 / 2 ;
	
	return star_APos;	
}





/**	
	*	@brief：	P_defaultK_T	通过位移 斜率 得到时间
	*	@param：	position	位移	slope	斜率
	*	@retval：	star_time	返回时间：ms
	*/
int P_defaultK_T(int position, float slope)
{
	int star_time = 0;
	
	star_time = (int)(sqrt(ABS_float((float)position) * 2 / slope * SPEED_CONVERT));
	
	return star_time;
}



/**	
	*	@brief：	AP_defaultK_T	通过位移 斜率 得到时间
	*	@param：	position	位移	slope	斜率
	*	@retval：	star_time	返回时间：ms
	*/
int AP_defaultK_T(int position, float slope)
{
	int star_time = 0;
	
	star_time = (int)(sqrt(ABS_float((float)position) * 2 / slope ) * 1000);
	
	return star_time;
}




/**	
	*	@brief：	Limit_Pos	通过位移 斜率 得到时间
	*	@param：	position	位移	slope	斜率
	*	@retval：	star_time	返回时间：ms
	*/
void Limit_Pos(float *pos, float slope, float speed, float max_speed)
{
	speed = ABS_float(speed);
	max_speed = (max_speed)/SPEED_CONVERT;
	*pos = (speed + max_speed) * ((max_speed - speed) / slope * SPEED_CONVERT) / 2 +  max_speed * max_speed / slope * SPEED_CONVERT / 2 ;
}



/**	
	*	@brief：	Limit_Pos_angle	通过位移 斜率 得到时间
	*	@param：	position	位移	slope	斜率
	*	@retval：	star_time	返回时间：ms
	*/
void Limit_Pos_angle(float *pos, float slope, float speed, float max_speed)
{
	float angle_limit = 0;
	speed = ABS_float(speed);
	max_speed = (max_speed)/SPEED_CONVERT;
	angle_limit = (speed + max_speed) * ((max_speed - speed) * SPEED_CONVERT / slope ) / 2  +  max_speed * max_speed / slope * SPEED_CONVERT / 2 ;
	*pos = angle_limit * 360 / 2 / 3.1416f / 0.875f / 1000;
	
}
/**********************************************************************************************/
/***********************************	全场定位预处理程序		*******************************/
/*							此程序主要是实现位移曲线，速度曲线的自动设计					  */
/*							 		是全场定位系统中的主要算法部分			  		  		  */
/**********************************************************************************************/
/**	
	*	@brief:		void Locate_stop_rapidly	急停函数设计
	*	@param:		float now_angle,			当前角度
	*	@supplement:		V = f(x) = sin(x - 3.14/2) + 1			即为速度控制曲线
							distance = f(x) = -1/A*cos(Ax - 3.14/2) + x		为位移控制曲线，即速度的积分
	*/
void Locate_stop(LOCATE_CONTROL_tail Control_tail, Locate_Tlist *Control_list, tcb_pFunc MyFunc)
{	
	Func_Slove pfunc = NULL;
	
	Control_list->date_passage.base_speed = Control_tail.end_speed;
	Control_list->date_passage.base_dist  = Control_tail.end_pos;
	Control_list->date_passage.add_speed  = 0 - Control_tail.end_speed;
	
	pfunc = V_K_T;
	Control_list->date_passage.better_time = pfunc(Control_tail.end_speed, MAX_K);
	Control_list->date_passage.accel_time_count = (int)(Control_list->date_passage.better_time / TIM_Piece);
	
	pfunc = V_T_P;
	Control_list->date_passage.add_dist = ((Control_tail.end_speed>0)?-1:1) * V_T_P(Control_list->date_passage.add_speed/2, Control_list->date_passage.better_time) / 3.1416 * 4;
	Control_list->date_passage.adv_pos = (float)Control_tail.end_speed / SPEED_CONVERT * Control_list->date_passage.better_time;
	
	/*	急停算法执行	*/
	Control_list->date_passage.TCB_Func = MyFunc;
//	LCD_Display_int(Control_list->date_passage.better_time, 0, 6);
//	LCD_Display_int(Control_list->date_passage.add_dist, 6, 6);
//	LCD_Display_int(Control_list->date_passage.add_speed, 12, 6);
//	LCD_Display_int(Control_list->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(Control_list->date_passage.base_speed, 10, 7);
}






/**	
	*	@brief:		void Locate_stop_rapidly	急停函数设计
	*	@param:		float now_angle,			当前角度
	*	@supplement:		V = f(x) = sin(x - 3.14/2) + 1			即为速度控制曲线
							distance = f(x) = -1/A*cos(Ax - 3.14/2) + x		为位移控制曲线，即速度的积分
	*/
void Locate_Astop(LOCATE_CONTROL_TAngle Control_tail, Locate_Tlist_angle *Control_list)
{
	Func_Slove pfunc = NULL;
	Func_Slove2 pfunc2 = NULL;
	
	Control_list->date_passage.base_angle = Control_tail.end_angle;
	Control_list->date_passage.base_speed = Control_tail.end_speed;
	Control_list->date_passage.add_speed = 0 - Control_tail.end_speed;
	
	pfunc = W_AK_T;
	Control_list->date_passage.better_time = pfunc(Control_tail.end_speed, MAX_AK);
	Control_list->date_passage.accel_time_count = Control_list->date_passage.better_time / TIM_Piece;
	
	pfunc2 = T_W_P;
	Control_list->date_passage.add_angle = (Control_tail.end_speed>0?-1:1) * pfunc2(Control_tail.end_speed, Control_list->date_passage.better_time)/3.1416 *4;
	Control_list->date_passage.TCB_Func = SinAccelPOS_Stop_angle;
	Control_list->date_passage.adv_angle = (float)Control_tail.end_speed * Control_list->date_passage.better_time/SPEED_CONVERT * 360 / 2 /3.1416 /1000 / 0.875;
//	LCD_Display_int(Control_list->date_passage.better_time, 0, 6);
//	LCD_Display_int(Control_list->date_passage.add_angle, 6, 6);
//	LCD_Display_int(Control_list->date_passage.add_speed, 12, 6);
//	LCD_Display_int(Control_list->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(Control_list->date_passage.base_speed, 10, 7);
	
	
}



/**	
	*	@brief:		void Locate_stop_rapidly	急停函数设计
	*	@param:		Control_tail	上周期控制最终参数传入
	*	@supplement:		
	*/
int Locate_stop_rapidly(LOCATE_CONTROL_tail Control_tail, Locate_Tlist *Control_list, tcb_pFunc MyFunc)
{
	Locate_stop(Control_tail, Control_list, MyFunc);
//	return Control_list->date_passage.TCB_Func(Control_list->date_passage, Locate_output, Control_tail, distance, P_param, result);
	return 0;
}




/**	
	*	@brief:		void Locate_stop_rapidly	急停函数设计
	*	@param:		Control_tail	上周期控制最终参数传入
	*	@supplement:		
	*/
int Locate_Astop_rapidly(LOCATE_CONTROL_TAngle Control_tail, Locate_Tlist_angle *Control_list)
{
	Locate_Astop(Control_tail, Control_list);
	return 0;
}




/**	
	*	@brief:	Locate_comman	普通正弦加减速曲线，完整曲线
	*	@param:	add_pos		需要增加的位移
				base_pos	当前位移初始点
				*Control_List	将要操作的控制链表
	*/
void Locate_comman(int add_pos, int base_pos, Locate_Tlist *Control_List, tcb_pFunc MyFunc)
{
	//新建链表节点
	Locate_Tlist *new_p;
	float add_dist = 0;
	float better_time = 0;
	float add_speed = 0;
	
	add_dist = (float)add_pos/2;
	better_time = P_defaultK_T(add_dist, MAX_K);
	add_speed = ((add_pos>0)?1:-1) * T_K_V(better_time, MAX_K);
	
	
	new_p = Control_List;
	new_p->date_passage.add_dist = add_dist;
	new_p->date_passage.better_time = better_time;
	new_p->date_passage.add_speed = add_speed;
	new_p->date_passage.base_dist = base_pos;
	new_p->date_passage.base_speed = 0;
	new_p->date_passage.adv_speed = 0;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.adv_pos = 0;
	new_p->date_passage.TCB_Func = MyFunc;
	
//	LCD_Display_int(new_p->date_passage.better_time, 0, 6);
//	LCD_Display_int(new_p->date_passage.add_dist, 6, 6);
//	LCD_Display_int(new_p->date_passage.add_speed, 12, 6);
//	LCD_Display_int(new_p->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(new_p->date_passage.base_speed, 6, 7);
//	LCD_Display_int(new_p->date_passage.base_dist, 12, 7);
	
	Locate_node_inster_after(Control_List);
	new_p = (Locate_Tlist *)((Control_List->locate_plist).nexttcb);
	new_p->date_passage.add_dist = -add_dist;
	new_p->date_passage.better_time = better_time;
	new_p->date_passage.add_speed = ((add_pos>0)?-1:1) * add_speed;
	new_p->date_passage.base_dist = add_dist + base_pos;
	new_p->date_passage.base_speed = -new_p->date_passage.add_speed;
	new_p->date_passage.adv_speed = ((add_pos>0)?1:-1) * add_speed;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.adv_pos = add_pos;
	new_p->date_passage.TCB_Func = MyFunc;

//	LCD_Display_int(new_p->date_passage.better_time, 0, 6);
//	LCD_Display_int(new_p->date_passage.add_dist, 6, 6);
//	LCD_Display_int(new_p->date_passage.add_speed, 12, 6);
//	LCD_Display_int(new_p->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(new_p->date_passage.base_speed, 6, 7);
//	LCD_Display_int(new_p->date_passage.base_dist, 12, 7);
//	BEED_ON;
//	delay_ms(100);
//	BEED_OFF;
//	delay_ms(100);
//	BEED_ON;
//	delay_ms(100);
//	BEED_OFF;
	
}




void Locate_comman_limit(int add_pos, int base_pos, float speed_max, float limit_pos, Locate_Tlist *Control_List, tcb_pFunc MyFunc)
{
	//新建链表节点
	Locate_Tlist *new_p;
	float add_dist = 0;
	float better_time = 0;
	float add_speed = 0;
	int add_limit_pos = ABS_int(add_pos) - limit_pos;
	int limit_time = add_limit_pos/(speed_max / SPEED_CONVERT);
	add_dist = ((add_pos>0)?1:-1) * (float)(limit_pos/2);
	better_time = P_defaultK_T(add_dist, MAX_K);
	add_speed = ((add_pos>0)?1:-1) * T_K_V(better_time, MAX_K);

	new_p = Control_List;
	new_p->date_passage.add_dist = add_dist;
	new_p->date_passage.better_time = better_time;
	new_p->date_passage.add_speed = add_speed;
	new_p->date_passage.base_dist = base_pos;
	new_p->date_passage.base_speed = 0;
	new_p->date_passage.adv_speed = 0;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.adv_pos = 0;
	new_p->date_passage.TCB_Func = MyFunc;
	
//	LCD_Display_int(new_p->date_passage.better_time, 0, 6);
//	LCD_Display_int(new_p->date_passage.add_dist, 6, 6);
//	LCD_Display_int(new_p->date_passage.add_speed, 12, 6);
//	LCD_Display_int(new_p->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(new_p->date_passage.base_speed, 6, 7);
//	LCD_Display_int(new_p->date_passage.base_dist, 12, 7);
	
	Locate_node_inster_after(Control_List);
	new_p = (Locate_Tlist *)((Control_List->locate_plist).nexttcb);
	new_p->date_passage.add_dist = 0;
	new_p->date_passage.better_time = limit_time;
	new_p->date_passage.add_speed = 0;
	new_p->date_passage.base_dist = add_dist + base_pos;
	new_p->date_passage.base_speed = ((add_pos>0)?1:-1) * add_speed;
	new_p->date_passage.adv_speed = ((add_pos>0)?1:-1) * add_speed;
	new_p->date_passage.accel_time_count = (int)(limit_time/TIM_Piece);
	new_p->date_passage.adv_pos = add_limit_pos;
	new_p->date_passage.TCB_Func = MyFunc;

	Locate_node_inster_after((Locate_Tlist *)Control_List->locate_plist.nexttcb);
	new_p = (Locate_Tlist *)(((Locate_Tlist *)(Control_List->locate_plist.nexttcb))->locate_plist.nexttcb);
	new_p->date_passage.add_dist = -add_dist;
	new_p->date_passage.better_time = better_time;
	new_p->date_passage.add_speed = ((add_pos>0)?-1:1) * add_speed;
	new_p->date_passage.base_dist = add_dist + base_pos +((add_pos>0)?1:-1) * add_limit_pos;
	new_p->date_passage.base_speed = -new_p->date_passage.add_speed;
	new_p->date_passage.adv_speed = ((add_pos>0)?1:-1) * add_speed;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.adv_pos = ((add_pos>0)?1:-1) * limit_pos;
	new_p->date_passage.TCB_Func = MyFunc;

//	LCD_Display_int(new_p->date_passage.better_time, 0, 6);
//	LCD_Display_int(new_p->date_passage.add_dist, 6, 6);
//	LCD_Display_int(new_p->date_passage.add_speed, 12, 6);
//	LCD_Display_int(new_p->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(new_p->date_passage.base_speed, 6, 7);
//	LCD_Display_int(new_p->date_passage.base_dist, 12, 7);
//	BEED_ON;
//	delay_ms(100);
//	BEED_OFF;
//	delay_ms(100);
//	BEED_ON;
//	delay_ms(100);
//	BEED_OFF;
	
}





/**	
	*	@brief:	Locate_Acomman	普通正弦加减速曲线，完整曲线
	*	@param:	add_pos		需要增加的位移
				base_pos	当前位移初始点
				*Control_List	将要操作的控制链表
	*/
void Locate_Acomman(float add_pos, float base_pos, Locate_Tlist_angle *Control_List)
{
	//新建链表节点
	Locate_Tlist_angle *new_p;
	float add_angle = 0;
	float better_time = 0;
	float add_speed = 0;
	
	add_angle = (float)add_pos/2;
	better_time = AP_defaultK_T(add_angle, MAX_W);
	add_speed = ((add_pos>0)?1:-1) * T_K_V(better_time, MAX_AK);

	new_p = Control_List;
	new_p->date_passage.add_angle = add_angle;
	new_p->date_passage.better_time = better_time;
	new_p->date_passage.add_speed = add_speed;
	new_p->date_passage.base_angle = base_pos;
	new_p->date_passage.base_speed = 0;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.TCB_Func = SinAccelPOS_ADD_angle;
	new_p->date_passage.adv_speed = 0;
	new_p->date_passage.adv_angle = 0;
//	LCD_Display_int(new_p->date_passage.better_time, 0, 6);
//	LCD_Display_int(new_p->date_passage.add_angle, 6, 6);
//	LCD_Display_int(new_p->date_passage.add_speed, 12, 6);
//	LCD_Display_int(new_p->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(new_p->date_passage.base_speed, 6, 7);
//	LCD_Display_int(new_p->date_passage.base_angle, 12, 7);
	
	Locate_Anode_inster_after(Control_List);
	new_p = (Locate_Tlist_angle *)((Control_List->locate_plist).nexttcb);
	new_p->date_passage.add_angle = -add_angle;
	new_p->date_passage.better_time = better_time;
	new_p->date_passage.add_speed = ((add_pos>0)?-1:1) * add_speed;
	new_p->date_passage.base_angle = add_angle + base_pos;
	new_p->date_passage.base_speed = ((add_pos>0)?1:-1) * add_speed;
	new_p->date_passage.adv_speed = add_speed;
	new_p->date_passage.adv_angle = add_pos;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.TCB_Func = SinAccelPOS_ADD_angle;

//	LCD_Display_int(add_angle, 0, 6);
//	LCD_Display_int(new_p->date_passage.add_angle, 6, 6);
//	LCD_Display_int(new_p->date_passage.add_speed, 12, 6);
//	LCD_Display_int(new_p->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(new_p->date_passage.base_speed, 6, 7);
//	LCD_Display_int(new_p->date_passage.base_angle, 12, 7);
//	BEED_ON;
//	delay_ms(100);
//	BEED_OFF;
//	delay_ms(100);
//	BEED_ON;
//	delay_ms(100);
//	BEED_OFF;
	
}




/**	
	*	@brief:	Locate_Acomman_limit	普通正弦加减速曲线，完整曲线
	*	@param:	add_pos		需要增加的位移
				base_pos	当前位移初始点
				*Control_List	将要操作的控制链表
	*/
void Locate_Acomman_limit(float add_pos, float base_pos, float speed_max, float limit_pos, Locate_Tlist_angle *Control_List)
{
	//新建链表节点
	Locate_Tlist_angle *new_p;
	float add_angle = 0;
	float better_time = 0;
	float add_speed = 0;
	float add_limit_pos = ABS_float(add_pos) - limit_pos;
	float limit_time = add_limit_pos/(speed_max ) * SPEED_CONVERT * 2 * 3.1416f * 0.875f * 1000 /360;
	add_angle = (float)((add_pos>0)?1:-1) * (float)limit_pos/2;
	better_time = AP_defaultK_T(add_angle, MAX_W);
	add_speed = (float)((add_pos>0)?1:-1) * T_K_V(better_time, MAX_AK);

	new_p = Control_List;
	new_p->date_passage.add_angle = add_angle;
	new_p->date_passage.better_time = better_time;
	new_p->date_passage.add_speed = add_speed;
	new_p->date_passage.base_angle = base_pos;
	new_p->date_passage.base_speed = 0;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.TCB_Func = SinAccelPOS_ADD_angle;
	new_p->date_passage.adv_speed = 0;
	new_p->date_passage.adv_angle = 0;
//	LCD_Display_int(new_p->date_passage.better_time, 0, 6);
//	LCD_Display_int(new_p->date_passage.add_angle, 6, 6);
//	LCD_Display_int(new_p->date_passage.add_speed, 12, 6);
//	LCD_Display_int(new_p->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(new_p->date_passage.base_speed, 6, 7);
//	LCD_Display_int(new_p->date_passage.base_angle, 12, 7);
	
	
	
	Locate_Anode_inster_after(Control_List);
	new_p = (Locate_Tlist_angle *)((Control_List->locate_plist).nexttcb);
	new_p->date_passage.add_angle = 0;
	new_p->date_passage.better_time = limit_time;
	new_p->date_passage.add_speed = 0;
	new_p->date_passage.base_angle = add_angle + base_pos;
	new_p->date_passage.base_speed = (float)((add_pos>0)?1:-1) * add_speed;
	new_p->date_passage.adv_speed = (float)((add_pos>0)?1:-1) * add_speed;
	new_p->date_passage.accel_time_count = (int)(limit_time/TIM_Piece);
	new_p->date_passage.TCB_Func = SinAccelPOS_ADD_angle;
	new_p->date_passage.adv_angle = add_limit_pos;
	
	Locate_Anode_inster_after((Locate_Tlist_angle *)Control_List->locate_plist.nexttcb);
	new_p = (Locate_Tlist_angle *)(((Locate_Tlist_angle *)(Control_List->locate_plist.nexttcb))->locate_plist.nexttcb);
	new_p->date_passage.add_angle = ((add_pos>0)?-1:1) * add_angle;
	new_p->date_passage.better_time = better_time;
	new_p->date_passage.add_speed = ((add_pos>0)?-1:1) * speed_max;
	new_p->date_passage.base_angle = add_angle + base_pos + ((add_pos>0)?1:-1) * add_limit_pos;
	new_p->date_passage.base_speed = -new_p->date_passage.add_speed;
	new_p->date_passage.adv_speed = speed_max;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.TCB_Func = SinAccelPOS_ADD_angle;
	new_p->date_passage.adv_angle = limit_pos;
//	LCD_Display_float(better_time, 10, 6);
	
//	LCD_Display_int(new_p->date_passage.add_angle, 6, 6);
//	LCD_Display_int(new_p->date_passage.add_speed, 12, 6);
//	LCD_Display_int(new_p->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(new_p->date_passage.base_speed, 6, 7);
//	LCD_Display_int(new_p->date_passage.base_angle, 12, 7);
//	BEED_ON;
//	delay_ms(100);
//	BEED_OFF;
//	delay_ms(100);
//	BEED_ON;
//	delay_ms(100);
//	BEED_OFF;
	
}





/**	
	*	@brief:	Locate_comman_Diy	普通正弦加减速曲线，完整曲线，自定义时间
	*	@param:	int add_pos	需要增加的位移
				ControlINT	输入的控制参数
				ControlT	系统存储的控制参数
				*Control_List	将要改变的控制链表
	*/
void Locate_comman_Diy(int add_pos, LOCATE_CONTROL_IN_T	ControlINT, LOCATE_CONTROL_tail ControlT, Locate_Tlist *Control_List, tcb_pFunc MyFunc)
{
	//新建链表节点
	Locate_Tlist *new_p;
	float star_speed = 0;
	float add_speed = 0;
	int star_time = 0;
	new_p = Control_List;
	new_p->date_passage.add_dist = add_pos/2;
	new_p->date_passage.better_time = (int)((ControlINT.accel_time)/2);
	
	/*	验证需要的时间是否正确，即是否超过设定的最大加速度	*/
	star_speed = T_K_V(Control_List->date_passage.better_time, MAX_K);
	add_speed = T_P_V((int)(new_p->date_passage.better_time), new_p->date_passage.add_dist);
	if(ABS_int(star_speed) < ABS_int(add_speed) )
	{
		new_p->date_passage.better_time = P_defaultK_T(new_p->date_passage.add_dist, MAX_K);
		star_speed = T_K_V(new_p->date_passage.better_time, (float)MAX_K);
		add_speed = ((add_pos>0)?1:-1) * star_speed;
	}
	star_time = new_p->date_passage.better_time;
	new_p->date_passage.add_speed = add_speed;
	new_p->date_passage.base_dist = ControlT.end_pos;
	new_p->date_passage.base_speed = 0;
	new_p->date_passage.adv_speed = 0;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.adv_pos = 0;
	new_p->date_passage.TCB_Func = MyFunc;
	star_speed = add_speed;
	
//	LCD_Display_float(add_speed, 0, 6);
//	LCD_Display_int(new_p->date_passage.add_dist, 6, 6);
//	LCD_Display_int(new_p->date_passage.add_speed, 12, 6);
//	LCD_Display_int(new_p->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(new_p->date_passage.base_speed, 10, 7);
	
	Locate_node_inster_after(Control_List);
	new_p = (Locate_Tlist *)((Control_List->locate_plist).nexttcb);
	new_p->date_passage.add_dist = -add_pos/2;
	new_p->date_passage.add_speed = ((add_pos>0)?-1:1) * add_speed;
	new_p->date_passage.better_time = star_time;
	new_p->date_passage.base_dist = (int)(add_pos/2) + ControlT.end_pos;
	new_p->date_passage.base_speed = add_speed;
	new_p->date_passage.adv_speed = add_speed;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.adv_pos = add_pos;
	new_p->date_passage.TCB_Func = MyFunc;
	
	
//	LCD_Display_int(new_p->date_passage.better_time, 0, 6);
//	LCD_Display_int(new_p->date_passage.add_dist , 6, 6);
//	LCD_Display_int(new_p->date_passage.add_speed, 12, 6);
//	LCD_Display_int(new_p->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(new_p->date_passage.add_dist + new_p->date_passage.base_dist, 10, 7);
}




/**	
	*	@brief:	Locate_comman_Diy_limit	普通正弦加减速曲线，完整曲线，自定义时间
	*	@param:	int add_pos	需要增加的位移
				ControlINT	输入的控制参数
				ControlT	系统存储的控制参数
				*Control_List	将要改变的控制链表
	*/
void Locate_comman_Diy_limit(int add_pos, float speed_max, float limit_pos, LOCATE_CONTROL_IN_T	ControlINT, LOCATE_CONTROL_tail ControlT, Locate_Tlist *Control_List, tcb_pFunc MyFunc)
{
	//新建链表节点
	Locate_Tlist *new_p;
	float star_speed = 0;
	float add_speed = 0;
	float add_pos_limit = 0;
	float add_pos_limit_offset = 0;
	int star_time = 0;
	int limit_flag = 0;
	new_p = Control_List;
	new_p->date_passage.better_time = (int)((ControlINT.accel_time)/2);
	add_pos_limit = add_pos/2;
	
	/*	验证需要的时间是否正确，即是否超过设定的最大加速度	*/
	star_speed = T_K_V(Control_List->date_passage.better_time, MAX_K);
	add_speed = T_P_V((int)(new_p->date_passage.better_time), add_pos_limit);
	if(ABS_int(star_speed) < ABS_int(add_speed) )
	{
		new_p->date_passage.better_time = P_defaultK_T(add_pos_limit, MAX_K);
		star_speed = T_K_V(new_p->date_passage.better_time, (float)MAX_K);
		add_speed = ((add_pos>0)?1:-1) * star_speed;
	}

	if(ABS_float(add_speed)>speed_max)
	{
		limit_flag = 1;
	
		new_p->date_passage.better_time = speed_max / ABS_float(add_speed) * new_p->date_passage.better_time;
		star_speed = speed_max;
		add_speed = ((add_pos>0)?1:-1) * star_speed;
		add_pos_limit = limit_pos / 2;
		add_pos_limit_offset = ABS_int(add_pos) - limit_pos;
	}
	
	star_time = new_p->date_passage.better_time;
	new_p->date_passage.add_speed = add_speed;
	new_p->date_passage.add_dist = add_pos_limit;
	new_p->date_passage.base_dist = ControlT.end_pos;
	new_p->date_passage.base_speed = 0;
	new_p->date_passage.adv_speed = 0;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.adv_pos = 0;
	new_p->date_passage.TCB_Func = MyFunc;
	star_speed = add_speed;
	
	
	
//	LCD_Display_int(new_p->date_passage.add_dist, 6, 6);
//	LCD_Display_int(new_p->date_passage.add_speed, 12, 6);
//	LCD_Display_int(new_p->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(new_p->date_passage.base_speed, 10, 7);
	if(limit_flag)
	{
		Locate_node_inster_after(Control_List);
		new_p = (Locate_Tlist *)((Control_List->locate_plist).nexttcb);
		new_p->date_passage.add_dist = 0;
		new_p->date_passage.add_speed = 0;
		new_p->date_passage.better_time = add_pos_limit_offset / (ABS_float(add_speed)/SPEED_CONVERT);
		new_p->date_passage.base_dist = (int)(add_pos_limit) + ControlT.end_pos;
		new_p->date_passage.base_speed = add_speed;
		new_p->date_passage.adv_speed = add_speed;
		new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
		new_p->date_passage.adv_pos = ((add_pos>0)?1:-1) * add_pos_limit_offset;
		new_p->date_passage.TCB_Func = MyFunc;
	}
	
	Locate_node_inster_after(new_p);
	new_p = (Locate_Tlist *)((new_p->locate_plist).nexttcb);
	new_p->date_passage.add_dist = -add_pos_limit;
	new_p->date_passage.add_speed = ((add_pos>0)?-1:1) * add_speed;
	new_p->date_passage.better_time = star_time;
	new_p->date_passage.base_dist = (int)(add_pos_limit) + ControlT.end_pos + ((add_pos>0)?1:-1) * add_pos_limit_offset;
	new_p->date_passage.base_speed = add_speed;
	new_p->date_passage.adv_speed = add_speed;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.adv_pos = ((add_pos>0)?1:-1) * limit_pos;
	new_p->date_passage.TCB_Func = MyFunc;
	
	LCD_Display_float(star_time, 0, 4);
//	LCD_Display_int(new_p->date_passage.better_time, 0, 6);
//	LCD_Display_int(new_p->date_passage.add_dist , 6, 6);
//	LCD_Display_int(new_p->date_passage.add_speed, 12, 6);
//	LCD_Display_int(new_p->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(new_p->date_passage.add_dist + new_p->date_passage.base_dist, 10, 7);
	

}





void Locate_Acomman_Diy(int add_pos, LOCATE_CONTROL_IN_T ControlINT, LOCATE_CONTROL_TAngle ControlT, Locate_Tlist_angle *Control_List)
{
	//新建链表节点
	Locate_Tlist_angle *new_p;
	float star_speed = 0;
	float add_speed = 0;
	int star_time = 0;
	new_p = Control_List;
	new_p->date_passage.add_angle = (float)add_pos/2;
	new_p->date_passage.better_time = (int)((ControlINT.accel_time)/2);
	
	/*	验证需要的时间是否正确，即是否超过设定的最大加速度	*/
//	delay_ms(100);
//	BEED_ON;
//	delay_ms(100);
//	BEED_OFF;
//	delay_ms(100);

	star_speed = T_K_V(Control_List->date_passage.better_time, MAX_AK);
	add_speed = T_P_W((new_p->date_passage.better_time), new_p->date_passage.add_angle);
	if(ABS_int(star_speed) < ABS_int(add_speed) )
	{
		new_p->date_passage.better_time = AP_defaultK_T(new_p->date_passage.add_angle, MAX_W);
		star_speed = T_K_V(new_p->date_passage.better_time, MAX_AK);
		add_speed = ((add_pos>0)?1:-1) * star_speed;
	}
	star_time = new_p->date_passage.better_time;
	new_p->date_passage.base_angle = ControlT.end_angle;
	new_p->date_passage.base_speed = 0;
	new_p->date_passage.add_speed = add_speed;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.TCB_Func = SinAccelPOS_ADD_angle;
//	star_speed = add_speed;
	new_p->date_passage.adv_speed = 0;
	new_p->date_passage.adv_angle = 0;
//	LCD_Display_int(new_p->date_passage.better_time, 0, 6);
//	LCD_Display_int(new_p->date_passage.add_angle, 6, 6);
//	LCD_Display_int(new_p->date_passage.add_speed, 12, 6);
//	LCD_Display_int(new_p->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(new_p->date_passage.base_speed, 10, 7);
	
	Locate_Anode_inster_after(Control_List);
	new_p = (Locate_Tlist_angle *)((Control_List->locate_plist).nexttcb);
	new_p->date_passage.add_angle = -((float)add_pos/2);
	new_p->date_passage.add_speed = ((add_pos>0)?-1:1) * add_speed;
	new_p->date_passage.better_time = star_time;
	new_p->date_passage.base_angle = (int)(add_pos/2) + ControlT.end_angle;
	new_p->date_passage.base_speed = add_speed;
	new_p->date_passage.adv_speed = add_speed;
	new_p->date_passage.adv_angle = add_pos;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.TCB_Func = SinAccelPOS_ADD_angle;
}




void Locate_Acomman_Diy_limit(int add_pos, float speed_max, float limit_pos, LOCATE_CONTROL_IN_T ControlINT, LOCATE_CONTROL_TAngle ControlT, Locate_Tlist_angle *Control_List)
{
	//新建链表节点
	Locate_Tlist_angle *new_p;
	float star_speed = 0;
	float add_speed = 0;
	float add_pos_limit = 0;
	float add_pos_limit_offset = 0;
	int star_time = 0;
	int limit_flag = 0;
	new_p = Control_List;
	new_p->date_passage.better_time = (int)((ControlINT.accel_time)/2);
	add_pos_limit = (float)add_pos/2;
	/*	验证需要的时间是否正确，即是否超过设定的最大加速度	*/
	star_speed = T_K_V(new_p->date_passage.better_time, MAX_AK);
	add_speed = T_P_W((new_p->date_passage.better_time), add_pos_limit);
	
	if(ABS_int(star_speed) < ABS_int(add_speed) )
	{
		new_p->date_passage.better_time = AP_defaultK_T(add_pos_limit, MAX_W);
		star_speed = T_K_V(new_p->date_passage.better_time, MAX_AK);
		add_speed = ((add_pos>0)?1:-1) * star_speed;
	}
	if(ABS_float(add_speed)>speed_max)
	{
		limit_flag = 1;
		new_p->date_passage.better_time = speed_max / ABS_float(add_speed) * new_p->date_passage.better_time;
		star_speed = speed_max;
		add_speed = ((add_pos>0)?1:-1) * star_speed;
		add_pos_limit = (float)limit_pos/ 2;
		add_pos_limit_offset = ABS_float(add_pos) - 2 * add_pos_limit;
	}
	
	star_time = new_p->date_passage.better_time;
	new_p->date_passage.add_angle = add_pos_limit;
	new_p->date_passage.base_angle = ControlT.end_angle;
	new_p->date_passage.base_speed = 0;
	new_p->date_passage.add_speed = add_speed;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.TCB_Func = SinAccelPOS_ADD_angle;
//	star_speed = add_speed;
	new_p->date_passage.adv_speed = 0;
	new_p->date_passage.adv_angle = 0;
//	LCD_Display_int(new_p->date_passage.add_angle, 6, 6);
//	LCD_Display_int(new_p->date_passage.add_speed, 12, 6);
//	LCD_Display_int(limit_flag, 0, 7);
//	LCD_Display_int(new_p->date_passage.base_speed, 10, 7);

	if(limit_flag)
	{
		Locate_Anode_inster_after(Control_List);
		new_p = (Locate_Tlist_angle *)((Control_List->locate_plist).nexttcb);
		new_p->date_passage.add_angle = 0;
		new_p->date_passage.add_speed = 0;
		new_p->date_passage.better_time = add_pos_limit_offset / 360 * 2*3.1416f *0.875f *1000  * SPEED_CONVERT/ add_speed;
		new_p->date_passage.base_angle = add_pos_limit + ControlT.end_angle;
		new_p->date_passage.base_speed = add_speed;
		new_p->date_passage.adv_speed = add_speed;
		new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
		new_p->date_passage.TCB_Func = SinAccelPOS_ADD_angle;
		new_p->date_passage.adv_angle = add_pos_limit_offset;
//		LCD_Display_int(new_p->date_passage.better_time, 10, 6);

	}
	
	Locate_Anode_inster_after(new_p);
	new_p = (Locate_Tlist_angle *)((new_p->locate_plist).nexttcb);
	new_p->date_passage.add_angle = -(add_pos_limit);
	new_p->date_passage.add_speed = ((add_pos>0)?-1:1) * add_speed;
	new_p->date_passage.better_time = star_time;
	new_p->date_passage.base_angle = add_pos_limit + ControlT.end_angle + ((add_pos>0)?1:-1) * add_pos_limit_offset;
	new_p->date_passage.base_speed = add_speed;
	new_p->date_passage.adv_speed = add_speed;
	new_p->date_passage.accel_time_count = (int)((new_p->date_passage.better_time)/TIM_Piece);
	new_p->date_passage.adv_angle = limit_pos;
	new_p->date_passage.TCB_Func = SinAccelPOS_ADD_angle;
//	LCD_Display_int(add_pos_limit_offset, 10, 6);
//	LCD_Display_int(add_speed, 0, 6);
//	LCD_Display_int(new_p->date_passage.add_angle, 6, 6);
//	LCD_Display_int(new_p->date_passage.add_speed, 12, 6);
//	LCD_Display_int(limit_flag, 0, 7);
//	LCD_Display_int(new_p->date_passage.base_speed, 10, 7);
}





/**
	*@brief	Locate_run_adv	推进函数，继续前进
	*@param	int add_pos, 		需要增加的位移
			float star_speed	当前的初始速度
	*/
void Locate_run_adv(int add_pos, float speed_in, LOCATE_CONTROL_tail ControlT, Locate_Tlist *Control_List, tcb_pFunc MyFunc)
{
	Locate_Tlist *new_p;
	float equation_a = 0;	//a值
	float equation_b = 0;	//b值
	float equation_c = 0;	//c值	
	float atime1 = 0;
	float atime2 = 0;
	float apos1 = 0;
	float apos2 = 0;
	float speed = 0;
	float star_speed = ABS_float(speed_in);
	int star_pos = ABS_int(add_pos);
	
	equation_a = (float)(MAX_K)/SPEED_CONVERT * 1000;
	equation_b = (float)(star_speed * 2);
	equation_c = star_speed * star_speed / (float)((float)(MAX_K)/SPEED_CONVERT*1000 * 2) - (float)star_pos/1000;
	//（b*b - 4ac）求得
	atime1 = ((float)(-equation_b + sqrt((equation_b * equation_b - 4 * equation_a * equation_c)))/(2 * equation_a) * 1000);
	atime2 = (star_speed/((float)MAX_K /SPEED_CONVERT * 1000) * 1000) + atime1;
	apos1 = ((float)atime1 * (star_speed + (float)((float)MAX_K / SPEED_CONVERT * atime1 / 2 )));
	apos2 = ((float)((float)atime2 * (float)((float)MAX_K / SPEED_CONVERT *  atime2) / 2 ));	
	
//	LCD_Display_float(equation_a, 0, 6);
//	LCD_Display_float(equation_b, 8, 6);
//	LCD_Display_float(equation_c, 15, 6);
	speed = T_P_V(atime1, apos1 - (star_speed * atime1));
	
	new_p = Control_List;
	new_p->date_passage.add_dist = ((speed_in>0)?1:-1) * apos1 - (star_speed * atime1);
	new_p->date_passage.better_time = atime1;
	new_p->date_passage.add_speed = ((speed_in>0)?1:-1) * speed;
	new_p->date_passage.base_dist = ControlT.end_pos;
	new_p->date_passage.base_speed = ((speed_in>0)?1:-1) * star_speed * SPEED_CONVERT;
	new_p->date_passage.accel_time_count = (int)((atime1)/TIM_Piece);
	new_p->date_passage.TCB_Func = MyFunc;
	new_p->date_passage.adv_speed = ((speed_in>0)?1:-1) * star_speed * SPEED_CONVERT;
	new_p->date_passage.adv_pos = ((speed_in>0)?1:-1) * star_speed  * atime1;
//	LCD_Display_int((star_speed * atime1), 0, 6);
//	LCD_Display_int((int)new_p->date_passage.base_speed, 6, 6);
//	LCD_Display_int((int)apos1, 12, 6);
//	LCD_Display_int((int)apos2, 0, 7);
//	LCD_Display_int((int)speed, 10, 7);
	
	while(Locate_node_inster_after(Control_List));

	new_p = (Locate_Tlist *)((Control_List->locate_plist).nexttcb);
	new_p->date_passage.add_dist = ((speed_in>0)?-1:1) * apos2;
	new_p->date_passage.better_time = atime2;
	new_p->date_passage.add_speed = ((speed_in>0)?-1:1) * (speed + star_speed * SPEED_CONVERT);
	new_p->date_passage.base_dist = apos1 + ControlT.end_pos;
	new_p->date_passage.base_speed = -new_p->date_passage.add_speed;
	new_p->date_passage.adv_speed = ((speed_in>0)?1:-1) * (speed + star_speed * SPEED_CONVERT);
	new_p->date_passage.accel_time_count = (int)(atime2/TIM_Piece);
	new_p->date_passage.adv_speed = new_p->date_passage.base_speed;
	new_p->date_passage.adv_pos = ((speed_in>0)?1:-1) * apos2 * 2;
	new_p->date_passage.TCB_Func = MyFunc;
}




/**
	*@brief	Locate_run_adv	推进函数，继续前进
	*@param	int add_pos, 		需要增加的位移
			float star_speed	当前的初始速度
	*/
void Locate_run_adv_limit(int add_pos, float speed_in, float speed_max, float limit_pos, LOCATE_CONTROL_tail ControlT, Locate_Tlist *Control_List, tcb_pFunc MyFunc)
{
	Locate_Tlist *new_p;
	float equation_a = 0;	//a值
	float equation_b = 0;	//b值
	float equation_c = 0;	//c值	
	float atime1 = 0;
	float atime2 = 0;
	float apos1 = 0;
	float apos2 = 0;
	float speed = 0;
	float star_speed = ABS_float(speed_in);
	int star_pos = ABS_int(add_pos);
	int add_limit_pos = add_pos - limit_pos;
	int limit_time = add_limit_pos/(speed_max / SPEED_CONVERT);
	
	equation_a = (float)(MAX_K)/SPEED_CONVERT * 1000;
	equation_b = (float)(star_speed * 2);
	equation_c = star_speed * star_speed / (float)((float)(MAX_K)/SPEED_CONVERT*1000 * 2) - (float)limit_pos/1000;
	//（b*b - 4ac）求得
	atime1 = ((float)(-equation_b + sqrt((equation_b * equation_b - 4 * equation_a * equation_c)))/(2 * equation_a) * 1000);
	atime2 = (star_speed/((float)MAX_K /SPEED_CONVERT * 1000) * 1000) + atime1;
	apos1 = ((float)atime1 * (star_speed + (float)((float)MAX_K / SPEED_CONVERT * atime1 / 2 )));
	apos2 = ((float)((float)atime2 * (float)((float)MAX_K / SPEED_CONVERT *  atime2) / 2 ));	
	
	
//	LCD_Display_float(equation_a, 0, 6);
//	LCD_Display_float(equation_b, 8, 6);
//	LCD_Display_float(equation_c, 15, 6);
	
	speed = T_P_V(atime1, apos1 - (star_speed * atime1));
	
	new_p = Control_List;
	new_p->date_passage.add_dist = ((speed_in>0)?1:-1) * apos1 - (star_speed * atime1);
	new_p->date_passage.better_time = atime1;
	new_p->date_passage.add_speed = ((speed_in>0)?1:-1) * speed;
	new_p->date_passage.base_dist = ControlT.end_pos;
	new_p->date_passage.base_speed = ((speed_in>0)?1:-1) * star_speed * SPEED_CONVERT;
	new_p->date_passage.accel_time_count = (int)((atime1)/TIM_Piece);
	new_p->date_passage.TCB_Func = MyFunc;
	new_p->date_passage.adv_speed = ((speed_in>0)?1:-1) * star_speed * SPEED_CONVERT;
	new_p->date_passage.adv_pos = ((speed_in>0)?1:-1) * star_speed  * atime1;
	LCD_Display_float((new_p->date_passage.better_time), 0, 1);
//	LCD_Display_int((int)new_p->date_passage.base_speed, 6, 6);
//	LCD_Display_int((int)apos1, 12, 6);
//	LCD_Display_int((int)apos2, 0, 7);
//	LCD_Display_int((int)speed, 10, 7);
	
	Locate_node_inster_after(Control_List);
	new_p = (Locate_Tlist *)((Control_List->locate_plist).nexttcb);
	new_p->date_passage.add_dist = 0;
	new_p->date_passage.better_time = limit_time;
	new_p->date_passage.add_speed = 0;
	new_p->date_passage.base_dist = apos1 + ControlT.end_pos;
	new_p->date_passage.base_speed = ((speed_in>0)?1:-1) * (speed + star_speed * SPEED_CONVERT);
	new_p->date_passage.adv_speed = ((speed_in>0)?1:-1) * (speed + star_speed * SPEED_CONVERT);
	new_p->date_passage.accel_time_count = (int)(limit_time/TIM_Piece);
	new_p->date_passage.TCB_Func = MyFunc;
	new_p->date_passage.adv_pos = add_limit_pos;
	
	
	Locate_node_inster_after((Locate_Tlist *)Control_List->locate_plist.nexttcb);
	new_p = (Locate_Tlist *)(((Locate_Tlist *)(Control_List->locate_plist.nexttcb))->locate_plist.nexttcb);
	new_p->date_passage.add_dist = ((speed_in>0)?-1:1) * apos2;
	new_p->date_passage.better_time = atime2;
	new_p->date_passage.add_speed = ((speed_in>0)?-1:1) * (speed + star_speed * SPEED_CONVERT);
	new_p->date_passage.base_dist = apos1 + ControlT.end_pos + add_limit_pos;
	new_p->date_passage.base_speed = -new_p->date_passage.add_speed;
	new_p->date_passage.adv_speed = ((speed_in>0)?1:-1) * (speed + star_speed * SPEED_CONVERT);
	new_p->date_passage.accel_time_count = (int)(atime2/TIM_Piece);
	new_p->date_passage.adv_speed = new_p->date_passage.base_speed;
	new_p->date_passage.adv_pos = ((speed_in>0)?1:-1) * apos2 * 2;
	new_p->date_passage.TCB_Func = MyFunc;
	
//	BEED_ON;
//	delay_ms(100);
//	BEED_OFF;
//	delay_ms(100);
//	BEED_ON;
//	delay_ms(100);
//	BEED_OFF;
}





/**
	*@brief	Locate_run_adv	推进函数，继续前进
	*@param	int add_pos, 		需要增加的位移
			float star_speed	当前的初始速度
	*/
void Locate_Arun_adv(int add_pos, float speed_in, LOCATE_CONTROL_TAngle ControlT, Locate_Tlist_angle *Control_List)
{
	Locate_Tlist_angle *new_p;
	float equation_a = 0;	//a值
	float equation_b = 0;	//b值
	float equation_c = 0;	//c值	
	float atime1 = 0;
	float atime2 = 0;
	float apos1 = 0;
	float apos2 = 0;
	float apos_base  = 0;
	float speed = 0;
	float star_speed = ABS_float(speed_in) ;
	float star_pos = ABS_float((float)add_pos * 2 * 3.1416 * 0.875 /360);
	
	equation_a = (float)(MAX_AK)/SPEED_CONVERT * 1000;
	equation_b = (float)(star_speed * 2);
	equation_c = star_speed * star_speed / (float)((float)(MAX_AK)/SPEED_CONVERT*1000 * 2) - (float)star_pos;
	//（b*b - 4ac）求得
	atime1 = ((float)(-equation_b + sqrt((equation_b * equation_b - 4 * equation_a * equation_c)))/(2 * equation_a) * 1000);
	atime2 = (star_speed/((float)MAX_AK /SPEED_CONVERT * 1000) * 1000) + atime1;
	apos1 = ((float)atime1 / 1000 * (star_speed + (float)((float)MAX_AK / SPEED_CONVERT * atime1 / 2 )));
	apos2 = ((float)((float)atime2 /1000 * (float)((float)MAX_AK / SPEED_CONVERT *  atime2) / 2 ));	
	apos1 = apos1 * 360 / 2 / 3.1416 / 0.875;
	apos2 = apos2 * 360 / 2 / 3.1416 / 0.875;
	
//	LCD_Display_float(equation_a, 0, 6);
//	LCD_Display_float(equation_b, 8, 6);
//	LCD_Display_float(equation_c, 15, 6);
	
	apos_base= star_speed * atime1 / 0.875 /2 / 3.1416 * 360 /1000;
	speed = T_K_V(atime1, MAX_AK);
	
	new_p = Control_List;
	new_p->date_passage.add_angle = ((speed_in>0)?1:-1) * apos1 - ((speed_in>0)?1:-1) * apos_base;
	new_p->date_passage.better_time = atime1;
	new_p->date_passage.add_speed = ((speed_in>0)?1:-1) * speed;
	new_p->date_passage.base_angle = ControlT.end_angle;
	new_p->date_passage.base_speed = ((speed_in>0)?1:-1) * star_speed * SPEED_CONVERT;
	new_p->date_passage.adv_speed = ((speed_in>0)?1:-1) * star_speed * SPEED_CONVERT;
	new_p->date_passage.accel_time_count = (int)((atime1)/TIM_Piece);
	new_p->date_passage.TCB_Func = SinAccelPOS_ADD_angle;
	new_p->date_passage.adv_angle = ((speed_in>0)?1:-1) * star_speed  * atime1 * 360 / 2 / 3.1416f / 1000 / 0.875f;
	
//	LCD_Display_int((int)atime1, 0, 6);
//	LCD_Display_int((int)atime2, 6, 6);
//	LCD_Display_int((int)new_p->date_passage.add_speed, 12, 6);
	
	Locate_Anode_inster_after(Control_List);
	new_p = (Locate_Tlist_angle *)((Control_List->locate_plist).nexttcb);
	new_p->date_passage.add_angle = ((speed_in>0)?-1:1) * apos2;
	new_p->date_passage.better_time = atime2;
	new_p->date_passage.add_speed = ((speed_in>0)?-1:1) * (speed + (int)(star_speed * SPEED_CONVERT));
	new_p->date_passage.base_angle = apos1 + ControlT.end_angle;
	new_p->date_passage.base_speed = -new_p->date_passage.add_speed;
	new_p->date_passage.adv_speed = ((speed_in>0)?1:-1) * (speed + (float)(star_speed * SPEED_CONVERT));
	new_p->date_passage.accel_time_count = (int)(atime2/TIM_Piece);
	new_p->date_passage.TCB_Func = SinAccelPOS_ADD_angle;
	new_p->date_passage.adv_angle = ((speed_in>0)?1:-1) * apos2 * 2;
//	LCD_Display_int((int)new_p->date_passage.add_speed, 0, 7);
//	LCD_Display_int((int)(new_p->date_passage.add_angle), 10, 7);
}






void Locate_Arun_adv_limit(int add_pos, float speed_in, float speed_max, float limit_pos, LOCATE_CONTROL_TAngle ControlT, Locate_Tlist_angle *Control_List)
{
	Locate_Tlist_angle *new_p;
	float equation_a = 0;	//a值
	float equation_b = 0;	//b值
	float equation_c = 0;	//c值	
	float atime1 = 0;
	float atime2 = 0;
	float apos1 = 0;
	float apos2 = 0;
	float apos_base  = 0;
	float speed = 0;
	float star_speed = ABS_float(speed_in) ;
	float star_pos = ABS_float((float)limit_pos * 2 * 3.1416 * 0.875 /360);
	float add_limit_pos = ABS_float(add_pos) - limit_pos;
	int limit_time = add_limit_pos/(speed_max) * SPEED_CONVERT * 2 * 3.1416f * 0.875f * 1000 /360;
	
	equation_a = (float)(MAX_AK)/SPEED_CONVERT * 1000;
	equation_b = (float)(star_speed * 2);
	equation_c = star_speed * star_speed / (float)((float)(MAX_AK)/SPEED_CONVERT*1000 * 2) - (float)star_pos;
	//（b*b - 4ac）求得
	atime1 = ((float)(-equation_b + sqrt((equation_b * equation_b - 4 * equation_a * equation_c)))/(2 * equation_a) * 1000);
	atime2 = (star_speed/((float)MAX_AK /SPEED_CONVERT * 1000) * 1000) + atime1;
	apos1 = ((float)atime1 / 1000 * (star_speed + (float)((float)MAX_AK / SPEED_CONVERT * atime1 / 2 )));
	apos2 = ((float)((float)atime2 /1000 * (float)((float)MAX_AK / SPEED_CONVERT *  atime2) / 2 ));	
	apos1 = apos1 * 360 / 2 / 3.1416 / 0.875;
	apos2 = apos2 * 360 / 2 / 3.1416 / 0.875;
	
//	LCD_Display_float(equation_a, 0, 6);
//	LCD_Display_float(equation_b, 8, 6);
//	LCD_Display_float(equation_c, 15, 6);
//	BEED_ON;
//	delay_ms(100);
//	BEED_OFF;
//	delay_ms(100);
//	BEED_ON;
//	delay_ms(100);
//	BEED_OFF;
	
	
	apos_base= star_speed * atime1 / 0.875 /2 / 3.1416 * 360 /1000;
	speed = T_K_V(atime1, MAX_AK);
	
	new_p = Control_List;
	new_p->date_passage.add_angle = ((speed_in>0)?1:-1) * apos1 - apos_base;
	new_p->date_passage.better_time = atime1;
	new_p->date_passage.add_speed = ((speed_in>0)?1:-1) * speed;
	new_p->date_passage.base_angle = ControlT.end_angle;
	new_p->date_passage.base_speed = ((speed_in>0)?1:-1) * star_speed * SPEED_CONVERT;
	new_p->date_passage.adv_speed = ((speed_in>0)?1:-1) * star_speed * SPEED_CONVERT;
	new_p->date_passage.accel_time_count = (int)((atime1)/TIM_Piece);
	new_p->date_passage.adv_angle = ((speed_in>0)?1:-1) * star_speed  * atime1 * 360 / 2 / 3.1416f / 1000 / 0.875f;
	new_p->date_passage.TCB_Func = SinAccelPOS_ADD_angle;
	
//	LCD_Display_int((int)atime1, 0, 6);
//	LCD_Display_int((int)atime2, 6, 6);
//	LCD_Display_int((int)new_p->date_passage.adv_speed, 12, 6);
//	LCD_Display_int((int)new_p->date_passage.add_angle, 0, 7);
//	LCD_Display_int((int)new_p->date_passage.base_speed, 6, 7);
//	LCD_Display_int((int)new_p->date_passage.adv_speed, 12, 7);
	
	Locate_Anode_inster_after(Control_List);
	new_p = (Locate_Tlist_angle *)((Control_List->locate_plist).nexttcb);
	new_p->date_passage.add_angle = 0;
	new_p->date_passage.better_time = limit_time;
	new_p->date_passage.add_speed = 0;
	new_p->date_passage.base_angle = apos1 + ControlT.end_angle;
	new_p->date_passage.base_speed = ((speed_in>0)?1:-1) * (speed + (float)(star_speed * SPEED_CONVERT));
	new_p->date_passage.adv_speed = ((speed_in>0)?1:-1) * (speed + (float)(star_speed * SPEED_CONVERT));
	new_p->date_passage.accel_time_count = (int)(limit_time/TIM_Piece);
	new_p->date_passage.adv_angle = add_limit_pos;
	new_p->date_passage.TCB_Func = SinAccelPOS_ADD_angle;
	
	Locate_Anode_inster_after((Locate_Tlist_angle *)Control_List->locate_plist.nexttcb);
	new_p = (Locate_Tlist_angle *)(((Locate_Tlist_angle *)(Control_List->locate_plist.nexttcb))->locate_plist.nexttcb);
	new_p->date_passage.add_angle = ((speed_in>0)?-1:1) * apos2;
	new_p->date_passage.better_time = atime2;
	new_p->date_passage.add_speed = ((speed_in>0)?-1:1) * (speed + (int)(star_speed * SPEED_CONVERT));
	new_p->date_passage.base_angle = apos1 + ControlT.end_angle + ((speed_in>0)?1:-1) * add_limit_pos;
	new_p->date_passage.base_speed = -new_p->date_passage.add_speed;
	new_p->date_passage.adv_speed = ((speed_in>0)?1:-1) * (speed + (float)(star_speed * SPEED_CONVERT));
	new_p->date_passage.accel_time_count = (int)(atime2/TIM_Piece);
	new_p->date_passage.adv_angle = ((speed_in>0)?1:-1) * apos2 * 2;
	new_p->date_passage.TCB_Func = SinAccelPOS_ADD_angle;
	
//	LCD_Display_int((int)new_p->date_passage.add_speed, 0, 7);
//	LCD_Display_int((int)(new_p->date_passage.add_angle), 10, 7);
}






/**
*	@brief:		Locat_Walk_Line		巡线函数设计，保持末状态
*	@param:		Control_TCB					需要操控的控制支线
*	@retval:	NONE
	*/
void Locat_Walk_Line(Locate_Tlist *Control_TCB, int target_p, tcb_pFunc MyFunc)
{
	Control_TCB->date_passage.accel_time_count	= 1;
	Control_TCB->date_passage.add_dist			= 0;
	Control_TCB->date_passage.add_speed			= 0;
	Control_TCB->date_passage.adv_speed			= 0;
	Control_TCB->date_passage.base_dist			= target_p;
	Control_TCB->date_passage.base_speed		= 0;
	Control_TCB->date_passage.better_time		= TIM_Piece;
	Control_TCB->date_passage.TCB_Func			= MyFunc;
}





/**
*	@brief:		Locat_Walk_Line_angle		巡线函数设计，保持末状态
*	@param:		Control_TCB					需要操控的控制支线
*	@retval:	NONE
	*/
void Locat_Walk_Line_angle(Locate_Tlist_angle *Control_TCB, float target_p)
{
	Control_TCB->date_passage.accel_time_count	= 1;
	Control_TCB->date_passage.add_angle			= 0;
	Control_TCB->date_passage.add_speed			= 0;
	Control_TCB->date_passage.adv_speed			= 0;
	Control_TCB->date_passage.adv_angle			= 0;
	Control_TCB->date_passage.base_angle		= target_p;
	Control_TCB->date_passage.base_speed		= 0;
	Control_TCB->date_passage.better_time		= TIM_Piece;
	Control_TCB->date_passage.direction			= 0;
	Control_TCB->date_passage.TCB_Func			= SinAccelPOS_ADD_angle;
}





/**	
	*	@brief:		void SinAccelPOS_ADD	动态设计加速曲线 + 位移平移曲线 + 速度曲线 + 自转曲线
	*	@param:		LOCATE_TCB stop_tcb		传入数据以结构体方式进行管理，保证程序的可延展性, 和调试能力
	*	@supplement:		V = f(x) = sin(x - 3.14/2) + 1			即为速度控制曲线
							distance = f(x) = -1/A*cos(Ax - 3.14/2) + x		为位移控制曲线，即速度的积分							
UNDER1_MLCB
	*/
int SinAccelPOS_ADD_X(LOCATE_TCB control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_tail *locate_controlT, int distance, float P_param, int *result)
{
	
	/*	曲线映射参数	*/
	static int piece_count = 0;
	float t_to_x = 0;		//t到x的映射
	
	/*	控制环参数	*/
	//位移环 + 速度
	int speed = 0;
	int distance_offset = 0;
	static int distance_last = 0;
	if(control_tcb.Clear)
		piece_count = 0;
	/*	一个周期开始	*/
	piece_count++;	
	
	if(0 == control_tcb.better_time)
		;
	else
	{
		locate_controlT->init_pos = locate_controlT->end_pos;
		
		/*	算法设计	*/
		t_to_x = 3.1416 * (float)TIM_Piece * (float)piece_count / control_tcb.better_time;		//计算当前所在的控制时间，映射到正弦曲线的X轴向量上
		
		/*	计算模型对应速度	v = B * (cos(x) + 1)	*/
		speed = control_tcb.base_speed + (int)((float)(control_tcb.add_speed/2) * (sin(t_to_x - 3.1416f/2) + 1));
		
		/*	调试程序	*/
		
		/*	计算距离偏差	*/
		distance_offset = locate_controlT->init_pos - distance;


		/*位移环对速度进行纠正 因为频率够高，因此不适用D参数也能进行调节，若频率降低时需调用D参数*/
		speed += (int)(P_param * distance_offset  + 0 * (distance_offset - distance_last));

		*result = speed;
		distance_last = distance_offset;
		
		/*	计算模型对应路程	distance = f(x) = 1/A*sin(Ax) + x	位移曲线之所以晚一个周期，因为偏差是在运行后才能得到*/
		locate_controlT->end_pos = control_tcb.base_dist + (float)control_tcb.adv_pos * (float)TIM_Piece *(float)piece_count/ control_tcb.better_time  + (((float)(control_tcb.add_dist)/(3.1416))*(-cos(t_to_x - 3.1416/2) +  t_to_x));
	}
	/*	循环判断	*/
	if(piece_count >= control_tcb.accel_time_count)
	{
		piece_count = 0;
		return 1;
	}
	else
		return 0;

	/*	一个周期结束	*/
////////////////////////////////////////////////////////////////////////////////////////
}




/**	
	*	@brief:		void SinAccelPOS_Stop_X	急停函数
	*	@param:		LOCATE_TCB stop_tcb		传入数据以结构体方式进行管理，保证程序的可延展性, 和调试能力
	*	@supplement:		V = f(x) = sin(x - 3.14/2) + 1			即为速度控制曲线
							distance = f(x) = -1/A*cos(Ax - 3.14/2) + x		为位移控制曲线，即速度的积分							
UNDER1_MLCB
	*/
int SinAccelPOS_Stop_X(LOCATE_TCB control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_tail *locate_controlT, int distance, float P_param, int *result)
{
	
	/*	曲线映射参数	*/
	static int piece_count = 0;
	float t_to_x = 0;		//t到x的映射
	static int distance_last = 0;
	
	/*	控制环参数	*/
	//位移环 + 速度
	int speed = 0;
	int distance_offset = 0;
	if(control_tcb.Clear)
		piece_count = 0;
	/*	一个周期开始	*/
	piece_count++;	
	
	if(0 == control_tcb.better_time)
		;
	else
	{
		locate_controlT->init_pos = locate_controlT->end_pos;
		
		/*	算法设计	*/
		t_to_x = 3.1416 / 2 * (float)TIM_Piece * (float)piece_count / control_tcb.better_time;		//计算当前所在的控制时间，映射到正弦曲线的X轴向量上
		
		/*	计算模型对应速度	v = B * (cos(x) + 1)	*/
		speed = control_tcb.base_speed + (int)((float)(control_tcb.add_speed) * sin(t_to_x) );
		
		/*	调试程序	*/
//		LCD_Display_int(speed, 0, 6);
//		LCD_Display_int(control_tcb.base_speed, 6, 6);
		
		/*	计算距离偏差	*/
		distance_offset = locate_controlT->init_pos - distance;


		/*位移环对速度进行纠正 因为频率够高，因此不适用D参数也能进行调节，若频率降低时需调用D参数*/
//		speed += (int)(P_param * distance_offset  + 0 * (distance_offset - distance_last));

		*result = speed;
		distance_last = distance_offset;
		
		/*	计算模型对应路程	distance = f(x) = 1/A*sin(Ax) + x	位移曲线之所以晚一个周期，因为偏差是在运行后才能得到*/
//		locate_controlT->end_pos = control_tcb.base_dist + (float)control_tcb.adv_speed * (float)TIM_Piece / SPEED_CONVERT *(float)piece_count + (((float)(control_tcb.add_dist)/(3.1416))*(-cos(t_to_x - 3.1416/2) +  t_to_x));
		locate_controlT->end_pos = control_tcb.base_dist + (float)control_tcb.adv_pos * (float)TIM_Piece *(float)piece_count/ control_tcb.better_time  + ((float)(control_tcb.add_dist)*(-cos(t_to_x)) + 1);
//		LCD_Display_int(control_tcb.base_dist, 0, 7);
//		LCD_Display_int(locate_controlT->end_pos, 12, 7);
	}
	/*	循环判断	*/
	if(piece_count >= control_tcb.accel_time_count)
	{
		piece_count = 0;
		return 1;
	}
	else
		return 0;

	/*	一个周期结束	*/
////////////////////////////////////////////////////////////////////////////////////////
}





/**	
	*	@brief:		void SinAccelPOS_ADD	动态设计加速曲线 + 位移平移曲线 + 速度曲线 + 自转曲线
	*	@param:		LOCATE_TCB stop_tcb		传入数据以结构体方式进行管理，保证程序的可延展性, 和调试能力
	*	@supplement:		V = f(x) = sin(x - 3.14/2) + 1			即为速度控制曲线
							distance = f(x) = -1/A*cos(Ax - 3.14/2) + x		为位移控制曲线，即速度的积分							
UNDER1_MLCB
	*/
int SinAccelPOS_ADD_Y(LOCATE_TCB control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_tail *locate_controlT, int distance, float P_param, int *result)
{
	
	/*	曲线映射参数	*/
	static int piece_count = 0;
	float t_to_x = 0;		//t到x的映射
	
	/*	控制环参数	*/
	//位移环 + 速度
	int speed = 0;
	int distance_offset = 0;
	static int distance_last = 0;
	if(control_tcb.Clear)
		piece_count = 0;
//	LCD_Display_int(piece_count, 0, 0);
	/*	一个周期开始	*/
	piece_count++;	
	
	if(0 == control_tcb.better_time)
		;
	else
	{
		locate_controlT->init_pos = locate_controlT->end_pos;
		
		/*	算法设计	*/
		t_to_x = 3.1416 * (float)TIM_Piece * (float)piece_count / control_tcb.better_time;		//计算当前所在的控制时间，映射到正弦曲线的X轴向量上
		
		/*	计算模型对应速度	v = B * (cos(x) + 1)	*/
		speed = control_tcb.base_speed + (int)((float)control_tcb.add_speed/2 * (sin(t_to_x - 3.1416f/2) + 1));
		
		/*	调试程序	*/
//		LCD_Display_int(control_tcb.base_speed, 8, 0);
//		LCD_Display_int((int)((float)(control_tcb.add_speed/2) * (sin(t_to_x - 3.1416f/2) + 1)), 12, 0);
		
		/*	计算距离偏差	*/
		distance_offset = locate_controlT->init_pos - distance;

		/*位移环对速度进行纠正 因为频率够高，因此不适用D参数也能进行调节，若频率降低时需调用D参数*/
		speed += (int)(P_param * distance_offset  + 0 * (distance_offset - distance_last));
		
		*result = speed;
		distance_last = distance_offset;
		
		/*	计算模型对应路程	distance = f(x) = 1/A*sin(Ax) + x	位移曲线之所以晚一个周期，因为偏差是在运行后才能得到*/
		locate_controlT->end_pos = control_tcb.base_dist + (float)control_tcb.adv_pos * (float)TIM_Piece *(float)piece_count/ control_tcb.better_time  + (((float)(control_tcb.add_dist)/(3.1416))*(-cos(t_to_x - 3.1416/2) +  t_to_x));
//		LCD_Display_int(locate_controlT->end_pos, 0, 0);
//		LCD_Display_int(locate_controlT->end_pos, 12, 7);
	}
	/*	循环判断	*/
	if(piece_count >= control_tcb.accel_time_count)
	{
		piece_count = 0;
		return 1;
	}
	else
		return 0;

	/*	一个周期结束	*/
////////////////////////////////////////////////////////////////////////////////////////
}





/**	
	*	@brief:		void SinAccelPOS_ADD	动态设计加速曲线 + 位移平移曲线 + 速度曲线 + 自转曲线
	*	@param:		LOCATE_TCB stop_tcb		传入数据以结构体方式进行管理，保证程序的可延展性, 和调试能力
	*	@supplement:		V = f(x) = sin(x - 3.14/2) + 1			即为速度控制曲线
							distance = f(x) = -1/A*cos(Ax - 3.14/2) + x		为位移控制曲线，即速度的积分							
UNDER1_MLCB
	*/
int SinAccelPOS_Stop_Y(LOCATE_TCB control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_tail *locate_controlT, int distance, float P_param, int *result)
{
	
	/*	曲线映射参数	*/
	static int piece_count = 0;
	float t_to_x = 0;		//t到x的映射
	
	/*	控制环参数	*/
	//位移环 + 速度
	int speed = 0;
	int distance_offset = 0;
	static int distance_last = 0;
	if(control_tcb.Clear)
		piece_count = 0;
//	LCD_Display_int(piece_count, 0, 0);
	/*	一个周期开始	*/
	piece_count++;	
	
	if(0 == control_tcb.better_time)
		;
	else
	{
		locate_controlT->init_pos = locate_controlT->end_pos;
		
		/*	算法设计	*/
		t_to_x = 3.1416 /2 * (float)TIM_Piece * (float)piece_count / control_tcb.better_time;		//计算当前所在的控制时间，映射到正弦曲线的X轴向量上
		
		/*	计算模型对应速度	v = B * (cos(x) + 1)	*/
		speed = control_tcb.base_speed + (int)((float)(control_tcb.add_speed) * (sin(t_to_x)));
		
		/*	调试程序	*/
//		LCD_Display_int(control_tcb.base_speed, 8, 0);
//		LCD_Display_int((int)((float)(control_tcb.add_speed/2) * (sin(t_to_x - 3.1416f/2) + 1)), 12, 0);
		
		/*	计算距离偏差	*/
		distance_offset = locate_controlT->init_pos - distance;

		/*位移环对速度进行纠正 因为频率够高，因此不适用D参数也能进行调节，若频率降低时需调用D参数*/
//		speed += (int)(P_param * distance_offset  + 0 * (distance_offset - distance_last));
		
		*result = speed;
		distance_last = distance_offset;
		
		/*	计算模型对应路程	distance = f(x) = 1/A*sin(Ax) + x	位移曲线之所以晚一个周期，因为偏差是在运行后才能得到*/
		locate_controlT->end_pos = control_tcb.base_dist + (float)control_tcb.adv_pos * (float)TIM_Piece *(float)piece_count/ control_tcb.better_time  + ((float)(control_tcb.add_dist)*(-cos(t_to_x ) +  1));
//		LCD_Display_int(control_tcb.base_dist, 0, 7);
//		LCD_Display_int(locate_controlT->end_pos, 12, 7);
	}
	/*	循环判断	*/
	if(piece_count >= control_tcb.accel_time_count)
	{
		piece_count = 0;
		return 1;
	}
	else
		return 0;

	/*	一个周期结束	*/
////////////////////////////////////////////////////////////////////////////////////////
}







/**	
	*	@brief:		void SinAccelPOS_ADD	动态设计加速曲线 + 位移平移曲线 + 速度曲线 + 自转曲线
	*	@param:		LOCATE_TCB stop_tcb		传入数据以结构体方式进行管理，保证程序的可延展性, 和调试能力
	*	@supplement:		V = f(x) = sin(x - 3.14/2) + 1			即为速度控制曲线
							distance = f(x) = -1/A*cos(Ax - 3.14/2) + x		为位移控制曲线，即速度的积分							
UNDER1_MLCB
	*/
int SinAccelPOS_ADD_angle(LOCATE_TCB_angle* control_tcb, Locate_Output loacate_output, float angle, float P_param, int *result)
{
	
	/*	曲线映射参数	*/
	static int piece_count = 0;
	float t_to_x = 0;		//t到x的映射
	float base_angle =0;
	
	/*	控制环参数	*/
	//位移环 + 速度
	int angle_speed = 0;
	//  角度/360度*2*3.1416	即转化为弧度进行计算，要和时间单位统一，所以*1000，120.877代表120.877转每分代表1m/s 0.875代表线速度和角速度的转换量
	if(control_tcb->Clear)
		piece_count = 0;
	
	/*	一个周期开始	*/
	piece_count++;	
	if(0 == control_tcb->better_time)
		;
	else
	{
		Locate_Control.control_angle.init_angle = Locate_Control.control_angle.end_angle;
			
		/*	算法设计	*/
		t_to_x = 3.1416 * (float)TIM_Piece * (float)piece_count / control_tcb->better_time;		//计算当前所在的控制时间，映射到正弦曲线的X轴向量上

		/*	计算模型对应速度	v = B * (cos(x) + 1)	*/
		angle_speed = control_tcb->base_speed + (int)((float)(control_tcb->add_speed/2) * (sin(t_to_x - 3.1416f/2) + 1));
		
		*result = angle_speed;

		#if  1
		base_angle = (float)(control_tcb->adv_angle)*((float)piece_count/(control_tcb->accel_time_count));

		/*	计算模型对应路程	distance = f(x) = 1/A*sin(Ax) + x	位移曲线之所以晚一个周期，因为偏差是在运行后才能得到*/
		Locate_Control.control_angle.end_angle = control_tcb->base_angle + base_angle + (control_tcb->add_angle)/(3.1416f)*((-cos(t_to_x - 3.1416f/2) +  t_to_x));

		while(Locate_Control.control_angle.end_angle>180)			Locate_Control.control_angle.end_angle -= 360;
		while(Locate_Control.control_angle.end_angle<-180)			Locate_Control.control_angle.end_angle += 360;

//		if(0 == (int)Locate_Control.control_angle.end_angle)
//		{
//			BEED_ON;
//			delay_ms(200);
//			BEED_OFF;
//		}
		#endif
//		LCD_Display_float(control_tcb->base_speed, 0, 6);
	}
	/*	循环判断	*/
	if(piece_count >= control_tcb->accel_time_count)
	{
		piece_count = 0;
		return 1;
	}
	else
		return 0;

	/*	一个周期结束	*/
////////////////////////////////////////////////////////////////////////////////////////
}





/**	
	*	@brief:		void SinAccelPOS_ADD	动态设计加速曲线 + 位移平移曲线 + 速度曲线 + 自转曲线
	*	@param:		LOCATE_TCB stop_tcb		传入数据以结构体方式进行管理，保证程序的可延展性, 和调试能力
	*	@supplement:		V = f(x) = sin(x - 3.14/2) + 1			即为速度控制曲线
							distance = f(x) = -1/A*cos(Ax - 3.14/2) + x		为位移控制曲线，即速度的积分							
UNDER1_MLCB
	*/
int SinAccelPOS_Stop_angle(LOCATE_TCB_angle* control_tcb, Locate_Output loacate_output, float angle, float P_param, int *result)
{
	
	/*	曲线映射参数	*/
	static int piece_count = 0;
	float t_to_x = 0;		//t到x的映射
	float base_angle =0;
	
	/*	控制环参数	*/
	//位移环 + 速度
	int angle_speed = 0;
	//  角度/360度*2*3.1416	即转化为弧度进行计算，要和时间单位统一，所以*1000，120.877代表120.877转每分代表1m/s 0.875代表线速度和角速度的转换量
	if(control_tcb->Clear)
		piece_count = 0;
	
	/*	一个周期开始	*/
	piece_count++;	
	if(0 == control_tcb->better_time)
		;
	else
	{
		Locate_Control.control_angle.init_angle = Locate_Control.control_angle.end_angle;
			
		/*	算法设计	*/
		t_to_x = 3.1416 / 2 * (float)TIM_Piece * (float)piece_count / control_tcb->better_time;		//计算当前所在的控制时间，映射到正弦曲线的X轴向量上

		/*	计算模型对应速度	v = B * (cos(x) + 1)	*/
		angle_speed = control_tcb->base_speed + (int)((float)(control_tcb->add_speed) * (sin(t_to_x)));
		
		*result = angle_speed;

		#if  1
		base_angle = (float)(control_tcb->adv_angle)*((float)piece_count/(control_tcb->accel_time_count));

		/*	计算模型对应路程	distance = f(x) = 1/A*sin(Ax) + x	位移曲线之所以晚一个周期，因为偏差是在运行后才能得到*/
		Locate_Control.control_angle.end_angle = control_tcb->base_angle + base_angle + (control_tcb->add_angle)*((-cos(t_to_x) +  1));

		while(Locate_Control.control_angle.end_angle>180)			Locate_Control.control_angle.end_angle -= 360;
		while(Locate_Control.control_angle.end_angle<-180)			Locate_Control.control_angle.end_angle += 360;

//		if(0 == (int)Locate_Control.control_angle.end_angle)
//		{
//			BEED_ON;
//			delay_ms(200);
//			BEED_OFF;
//		}
		#endif
//		LCD_Display_float(Locate_Control.control_angle.end_angle, 0, 6);
	}
	/*	循环判断	*/
	if(piece_count >= control_tcb->accel_time_count)
	{
		piece_count = 0;
		return 1;
	}
	else
		return 0;

	/*	一个周期结束	*/
////////////////////////////////////////////////////////////////////////////////////////
}





/**	
	*	@brief:		int CosAccelPOS_DEC()	动态减速曲线设计 + 位移曲线 + 速度曲线 + 自转曲线设计
	*	@param:		LOCATE_TCB stop_tcb		传入数据以结构体方式进行管理，保证程序的可延展性, 和调试能力
	*	@supplement:		V = f(x) = cos(x) + 1			即为速度控制曲线
							distance = f(x) = -1/A*sin(Ax) + x		为位移控制曲线，即速度的积分
UNDER1_MLCB
	*/

int CosAccelPOS_DEC(LOCATE_TCB control_tcb, Locate_Output loacate_output, LOCATE_CONTROL_tail locate_controlT, int distance, float P_param, int *result)
{
	
	/*	曲线映射参数	*/
	static int piece_count = 0;
	float t_to_x = 0;		//t到x的映射
	
	/*	控制环参数	*/
	//位移环 + 速度
	int speed = 0;
	int distance_offset = 0;
	static int distance_last = 0;

	/*	一个周期开始	*/
	piece_count++;	
	locate_controlT.init_pos = locate_controlT.end_pos;
		
	/*	算法设计	*/
	t_to_x = 3.1416 * (float)TIM_Piece * (float)piece_count / control_tcb.better_time;		//计算当前所在的控制时间，映射到正弦曲线的X轴向量上
	
	/*	计算模型对应速度	v = B * (cos(x) + 1)	*/
	speed = control_tcb.base_speed + (int)((float)(control_tcb.add_speed/2) * (cos(t_to_x) + 1));
	
	/*	计算距离偏差	*/
	distance_offset = locate_controlT.init_pos - distance;
	
	/*位移环对速度进行纠正 因为频率够高，因此不适用D参数也能进行调节，若频率降低时需调用D参数*/
	speed += (int)(P_param * distance_offset  + 0 * (distance_offset - distance_last));
	
	*result = speed;
	
	distance_last = distance_offset;	
	
	/*	计算模型对应路程	distance = f(x) = 1/A*sin(Ax) + x	位移曲线之所以晚一个周期，因为偏差是在运行后才能得到*/
	locate_controlT.end_pos = control_tcb.base_dist + (int)(((float)(control_tcb.add_dist)/(3.1416f))*(sin(t_to_x) +  t_to_x));
	
	/*	循环判断	*/
	if(piece_count >= control_tcb.accel_time_count)
	{
		piece_count = 0;
		return 1;
	}
	else
		return 0;

	/*	一个周期结束	*/
////////////////////////////////////////////////////////////////////////////////////////
}





/**	
	*	@brief:		int CosAccelPOS_DEC_angle()	动态减速曲线设计 + 位移曲线 + 速度曲线 + 自转曲线设计
	*	@param:		LOCATE_TCB stop_tcb			传入数据以结构体方式进行管理，保证程序的可延展性, 和调试能力
	*	@supplement:		V = f(x) = cos(x) + 1			即为速度控制曲线
							distance = f(x) = -1/A*sin(Ax) + x		为位移控制曲线，即速度的积分
	*/
int CosAccelPOS_DEC_angle(LOCATE_TCB_angle control_tcb, Locate_Output loacate_output, float angle, float P_param, int *result)
{
	
	/*	曲线映射参数	*/
	static int piece_count = 0;
	float t_to_x = 0;		//t到x的映射
	
	/*	控制环参数	*/
	
	//角度曲线
	float angle_speed = 0;
	//  角度/360度*2*3.1416	即转化为弧度进行计算，要和时间单位统一，所以*1000，120.877代表120.877转每分代表1m/s 0.875代表线速度和角速度的转换量

///////////////////////////////////////////////////////////////////////////////////////////
	/*	一个周期开始	*/
	piece_count++;	
//	locate_controlT.init_angle = 0;
	Locate_Control.control_angle.init_angle = Locate_Control.control_angle.end_angle;
		
	/*	算法设计	*/
	t_to_x = 3.1416 * (float)TIM_Piece * (float)piece_count / control_tcb.better_time;		//计算当前所在的控制时间，映射到正弦曲线的X轴向量上
	
	/*	计算模型对应速度	v = B * (cos(x) + 1)	*/
	angle_speed = control_tcb.base_speed + (int)((float)(control_tcb.add_speed/2) * (cos(t_to_x) + 1));
	*result = angle_speed;
		
	/*	计算模型对应路程	distance = f(x) = 1/A*sin(Ax) + x	位移曲线之所以晚一个周期，因为偏差是在运行后才能得到*/
	Locate_Control.control_angle.end_angle = control_tcb.base_angle + ((float)(control_tcb.add_angle)/(3.1416f))*(sin(t_to_x) +  t_to_x);
	while(Locate_Control.control_angle.end_angle>180)			Locate_Control.control_angle.end_angle -= 360;
	while(Locate_Control.control_angle.end_angle<-180)			Locate_Control.control_angle.end_angle += 360;
////	if(NULL == locate_controlT.end_angle)
////	{
////		BEED_ON;
////		delay_ms(200);
////		BEED_OFF;
////	}
	/*	循环判断	*/
	if(piece_count >= control_tcb.accel_time_count)
	{
		piece_count = 0;
		return 1;
	}
	else
		return 0;

	/*	一个周期结束	*/
////////////////////////////////////////////////////////////////////////////////////////
}





/**
	*	@brief:	自适应角度闭环控制	也是全场定位系统输出
	*	@param:	int   Speed_Vx,						位移环结果X速度
				int   Speed_Vy,						位移环结果Y速度
				int   angle_speed,					位移环结果W角速度
	*	@supplement:		利用陀螺仪的角度数据，进行PID闭环巡线控制，但是因为出现平移误差，因此还需要进行平移的闭环控制
	*	@retval: NONE
	*/
void Control_Angle_Walk_All(	int   	Speed_Vx,
								int   	Speed_Vy,
								int     angle_speed)
{
	int speed_vx = 0;
	int speed_vy = 0;
	/*		辅助PID变量		*/	
	float angle_offset_help_target = 0;	//当前需要的纠偏角度量
	float angle_offset_add = 0;			//上周实际纠偏量

	/*		角度闭环PID		*/
	static float angle_offset_offset_last = 0;	
	int32_t PD_angle_result=0;
	float angle_offset=0;
	
	/*		角度环速度合成参数	*/
	float angle_mid = 0;	//平均角度(下周期)
	angle_offset = Locate_Control.control_angle.end_angle- (*Locate_RTime.ZAngle);
	if(angle_offset>180)	
	{
		angle_offset -= 360;
		angle_mid = Locate_Control.control_angle.init_angle + ABS_float(angle_offset)/2;
	}
	else if(angle_offset<-180)	
	{
		angle_offset += 360;
		angle_mid = Locate_Control.control_angle.init_angle - ABS_float(angle_offset)/2;
	}
	else
		angle_mid = (Locate_Control.control_angle.init_angle + (*Locate_RTime.ZAngle))/2;


	/*			辅助PID调节			*/
	//计算即将需要的角度纠偏量
	angle_offset_help_target = angle_offset / (float)Adjust_Time;	
	//计算上周期的实际角度纠偏量
	angle_offset_add = (*Locate_RTime.ZAngle) - angle_last;
	if(angle_offset_add>180)	angle_offset_add -= 360;
	if(angle_offset_add<-180)	angle_offset_add += 360;
	//计算上周期的角度纠偏量的偏差
	angle_offset_offset_help =(angle_help_last>0?1:-1) * (angle_help_last - angle_offset_add);
	
	
	//辅助pid
	angle_P = (ABS_float(angle_offset_help_target) * 2 + 10 + Adjust_P*angle_offset_offset_help  + Angle_I_Adjiust*angle_offset_offset_help + Angle_D_Adjiust*(angle_offset_offset_help - angle_offset_offset_last));
	
	//更新数据：更新上周期目标纠偏量
	angle_help_last = angle_offset_help_target;
	//更新数据：更新上周期角度纠偏量的偏差
	angle_offset_offset_last = angle_offset_offset_help;
	//更新数据：更新上周期角度数据
	angle_last = (*Locate_RTime.ZAngle);
	
	/*			角度PID调节			*/
	angle_offset = Locate_Control.control_angle.end_angle- (*Locate_RTime.ZAngle);
	if(ABS_float(angle_offset)<ANGLE_OFFSET_ignore)
		angle_offset = 0;
	else{
		PD_angle_result = (int32_t)(UNDER_Rotate_R*(angle_P*angle_offset + D_Param*(angle_offset-last_angle_offset)));
		if(PD_angle_result>ANGLE_Rotate_MAX)		PD_angle_result = ANGLE_Rotate_MAX;
		if(PD_angle_result<ANGLE_Rotate_MIN)		PD_angle_result = ANGLE_Rotate_MIN;
		last_angle_offset = angle_offset;
	}
	
	/*	合成角速度	*/
	angle_speed += PD_angle_result;
	                                                  
	/*	合成平移速度	*/
	angle_mid = angle_mid/360*2*3.1416;
	speed_vx = (int)((float)Speed_Vx * cos( 0 - angle_mid) + (float)Speed_Vy * cos( 3.1416f/2 - angle_mid));
	speed_vy = (int)((float)Speed_Vx * sin( 0 - angle_mid) + (float)Speed_Vy * sin( 3.1416f/2 - angle_mid));
	
	/*	计算结果输出	*/
	LOCATE_control_output(speed_vx, speed_vy, angle_speed);
//	LOCATE_control_output(0, 0, 0);
	/* 	更新全场定位数据包	*/
	Locate_Control.control_x.end_speed = speed_vx;
	Locate_Control.control_y.end_speed = speed_vy;
	Locate_Control.control_angle.end_speed = angle_speed;
	
}
////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************/
/***********************************	全场定位系统设计		*******************************/
/*							此程序主要设计全场定位系统的运行和系统参数设计					  */
/*							 		是全场定位公用户自定的部分								  */
/*										可根据手柄进行修改									  */
/**********************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////
/**
	*	@brief:		void Date_Pack_Cut		全场定位系统反馈数据接入口
	*	@param:		locate_rtime_init		数据来源	用指针传入保证其实时性
	*	@retva:		NONE
	*	@supplement:	全场定位前必须执行的程序
	*/
void LOCATE_date_cut(Locate_DATE *locate_rtime_init)
{

	Locate_RTime.X = &locate_rtime_init->X;
	Locate_RTime.Y = &locate_rtime_init->Y;
	Locate_RTime.W = &locate_rtime_init->W;
	Locate_RTime.XAngle = &locate_rtime_init->XAngle;
	Locate_RTime.YAngle = &locate_rtime_init->YAngle;
	Locate_RTime.ZAngle = &locate_rtime_init->ZAngle;
	Locate_RTime.Riceve_flag = &locate_rtime_init->Riceve_flag;
}



/**
	*	@brief:		void Date_Pack_Cut		全场定位系统反馈数据接入口
	*	@param:		locate_rtime_init		数据来源	用指针传入保证其实时性
	*	@retva:		NONE
	*	@supplement:	全场定位前必须执行的程序
	*/
void LOCATE_Property_INIT(LOCATE_Property *locate_property, LOCATE_Property *user_property)
{

	locate_property = user_property;
}




/**
	*	@brief:		void LOCATE_INIT	全场定位系统的初始化
	*	@param:		locate_rtime		数据来源
	*	@retva:		NONE
	*	@supplement:	全场定位前必须执行的程序
	*/
void LOCATE_INIT(Locate_DATE *locate_rtime)
{
	/*	更新全场定位反馈数据通道	*/
	LOCATE_date_cut(locate_rtime);
	
	/*	复位全场定位系统	*/
	//清除中断信号
	LOCATE_KEY_clear();		
	//清空实时控制包
	Locate_MM_Clear(&Locate_Control, sizeof(Locate_Control));
	Locate_MM_Clear(&Locate_output, sizeof(Locate_output));

	//设计控制链
	Locate_MM_Clear(&X_Handp, sizeof(X_Handp));
	Locate_MM_Clear(&Y_Handp, sizeof(Y_Handp));
	Locate_MM_Clear(&W_Handp, sizeof(W_Handp));
	X_Handp.locate_plist.pretcb = (Tlist_node *)&X_Handp;
	Y_Handp.locate_plist.pretcb = (Tlist_node *)&Y_Handp;
	W_Handp.locate_plist.pretcb = (Tlist_node *)&W_Handp;
	X_Handp.locate_plist.nexttcb = NULL;
	Y_Handp.locate_plist.nexttcb = NULL;
	W_Handp.locate_plist.nexttcb = NULL;
	TCB_handp[0] = &X_Handp;
	TCB_handp[1] = &Y_Handp;
	TCB_handp_w = &W_Handp;
	
	/*	系统准备完成信号	*/
	LOCATE_Ready();
	
}


/**
	*	@brief:		void LOCATE_pre_handle	全场定位预处理
	*	@param:		locate_control_in		上周期控制数据	
	*	@retva:		NONE
	*	@supplement:	全场定位算法之一：对数据进行预处理即判定运动情况，制定合理的曲线分配
	*	@hehe:	((Locate_Tlist *)((TCB_handp[0]->locate_plist).nexttcb))->date_passage;
		负责各支线链表的设计和参数设定
	*/
void LOCATE_pre_deal(LOCATE_CONTROL_IN	locate_control_in)
{	
	
	//增量
	int add_x = locate_control_in.control_x.target_pos - Locate_Control.control_x.end_pos;
	int add_y = locate_control_in.control_y.target_pos - Locate_Control.control_y.end_pos;
	float add_angle = 0;
	int base_x = 0;
	int base_y = 0;
	float base_angle = 0;
	//赋值方程中的常数值
	float star_speed_x = (float)Locate_Control.control_x.end_speed/ SPEED_CONVERT;
	float forecast_pos_x = star_speed_x * star_speed_x / (MAX_K / SPEED_CONVERT )/ 2 ;
	float star_speed_y = (float)Locate_Control.control_y.end_speed/ SPEED_CONVERT;
	float forecast_pos_y = star_speed_y * star_speed_y / (MAX_K / SPEED_CONVERT )/ 2 ;
	float star_speed_w = (float)Locate_Control.control_angle.end_speed/ SPEED_CONVERT;
	float forecast_pos_w = star_speed_w * star_speed_w / (MAX_AK / SPEED_CONVERT ) / 2 * 360 /1000 /2 /3.1416f /0.875f;
	//限制最大速度位移
	float Limit_pos_x = 0;
	float Limit_pos_y = 0;
	float Limit_pos_w = 0;
	
//	BEED_ON;
//	delay_ms(100);
//	BEED_OFF;
//	delay_ms(100);
//	BEED_ON;
//	delay_ms(100);
//	BEED_OFF;
//	delay_ms(100);
	
#if 1
	Limit_Pos(&Limit_pos_x, MAX_K, star_speed_x, (float)MAX_Speed_x);
//	LCD_Display_float(Limit_pos_x, 10, 3);

	/*	如果X的初速度为0	*/
	if(!Locate_Control.control_x.end_speed)
	{
		if(TIME_DEFAULT == locate_control_in.control_x.accel_time)
		{
			if(ABS_int(add_x)>Limit_pos_x)
				Locate_comman_limit(add_x, Locate_Control.control_x.end_pos, MAX_Speed_x, Limit_pos_x, TCB_handp[0], SinAccelPOS_ADD_X);
			else
				Locate_comman(add_x, Locate_Control.control_x.end_pos, TCB_handp[0], SinAccelPOS_ADD_X);
		}
		else
		{			
			if(ABS_int(add_x)>Limit_pos_x)
				Locate_comman_Diy_limit(add_x, MAX_Speed_x, Limit_pos_x, locate_control_in.control_x, Locate_Control.control_x, TCB_handp[0], SinAccelPOS_ADD_X);
			else
				Locate_comman_Diy(add_x, locate_control_in.control_x, Locate_Control.control_x, TCB_handp[0], SinAccelPOS_ADD_X);
		}
	}
	/*	如果X的初速度不为0	*/
	else
	{
		
		/*	如果两次运动方向相同	*/
		//正方向
		if(add_x>0 && Locate_Control.control_x.end_speed>0)	
		{
			if(add_x>forecast_pos_x)
			{
				if(ABS_int(add_x)>Limit_pos_x)
					Locate_run_adv_limit(add_x, star_speed_x, MAX_Speed_x, Limit_pos_x, Locate_Control.control_x, TCB_handp[0], SinAccelPOS_ADD_X);
				else
					Locate_run_adv(add_x, star_speed_x, Locate_Control.control_x, TCB_handp[0], SinAccelPOS_ADD_X);
			}
			else
			{
				/*	急停 并反向	*/
				Locate_stop(Locate_Control.control_x, TCB_handp[0], SinAccelPOS_Stop_X);
				add_x = add_x - (TCB_handp[0]->date_passage).add_dist;
				base_x = Locate_Control.control_x.end_pos + (TCB_handp[0]->date_passage).add_dist;
				Locate_node_inster_after(TCB_handp[0]);
				if(ABS_int(add_x)>Limit_pos_x)
					Locate_comman_limit(add_x, base_x, MAX_Speed_x, Limit_pos_x, (Locate_Tlist*)(TCB_handp[0]->locate_plist.nexttcb), SinAccelPOS_ADD_X);
				else
					Locate_comman(add_x, base_x, (Locate_Tlist*)(TCB_handp[0]->locate_plist.nexttcb), SinAccelPOS_ADD_X);
			}
		}
		//负方向
		else if(add_x<0 && Locate_Control.control_x.end_speed<0)
		{
			
			if(add_x<forecast_pos_x)
			{
				if(ABS_int(add_x)>Limit_pos_x)
					Locate_run_adv_limit(add_x, star_speed_x, MAX_Speed_x, Limit_pos_x, Locate_Control.control_x, TCB_handp[0], SinAccelPOS_ADD_X);
				else
					Locate_run_adv(add_x, star_speed_x, Locate_Control.control_x, TCB_handp[0], SinAccelPOS_ADD_X);
			}
			else
			{
				
				/*	急停 并反向	*/
				Locate_stop(Locate_Control.control_x, TCB_handp[0], SinAccelPOS_Stop_X);
				add_x = add_x - (TCB_handp[0]->date_passage).add_dist;
				base_x = Locate_Control.control_x.end_pos + (TCB_handp[0]->date_passage).add_dist;
				Locate_node_inster_after(TCB_handp[0]);
				if(ABS_int(add_x)>Limit_pos_x)
					Locate_comman_limit(add_x, base_x, MAX_Speed_x, Limit_pos_x, (Locate_Tlist*)(TCB_handp[0]->locate_plist.nexttcb), SinAccelPOS_ADD_X);
				else
					Locate_comman(add_x, base_x, (Locate_Tlist*)(TCB_handp[0]->locate_plist.nexttcb), SinAccelPOS_ADD_X);
			}
		}
		/*	如果两次运行方向不相同	*/
		//正方向
		else if(add_x>0 && Locate_Control.control_x.end_speed<0)
		{
			/*	急停 并反向	*/
			Locate_stop(Locate_Control.control_x, TCB_handp[0], SinAccelPOS_Stop_X);
			add_x = add_x - (TCB_handp[0]->date_passage).add_dist;
			base_x = Locate_Control.control_x.end_pos + (TCB_handp[0]->date_passage).add_dist;
			Locate_node_inster_after(TCB_handp[0]);
			if(ABS_int(add_x)>Limit_pos_x)
				Locate_comman_limit(add_x, base_x, MAX_Speed_x, Limit_pos_x, (Locate_Tlist*)(TCB_handp[0]->locate_plist.nexttcb), SinAccelPOS_ADD_X);
			else
				Locate_comman(add_x, base_x, (Locate_Tlist*)(TCB_handp[0]->locate_plist.nexttcb), SinAccelPOS_ADD_X);

		}
		//负方向
		else if(add_x<0 && Locate_Control.control_x.end_speed>0)
		{
			/*	急停 并反向	*/
			
			Locate_stop(Locate_Control.control_x, TCB_handp[0], SinAccelPOS_Stop_X);
			add_x = add_x - (TCB_handp[0]->date_passage).add_dist;
			base_x = Locate_Control.control_x.end_pos + (TCB_handp[0]->date_passage).add_dist;
			Locate_node_inster_after(TCB_handp[0]);
			if(ABS_int(add_x)>Limit_pos_x)
				Locate_comman_limit(add_x, base_x, MAX_Speed_x, Limit_pos_x, (Locate_Tlist*)(TCB_handp[0]->locate_plist.nexttcb), SinAccelPOS_ADD_X);
			else
				Locate_comman(add_x, base_x, (Locate_Tlist*)(TCB_handp[0]->locate_plist.nexttcb), SinAccelPOS_ADD_X);
		}
	}
	
#endif
	
#if	1
	Limit_Pos(&Limit_pos_y, MAX_K, star_speed_y, (float)MAX_Speed_y);

	LCD_Display_float(Limit_pos_y, 10, 5);

	/*	如果Y的初速度为0	*/
	if(!Locate_Control.control_y.end_speed)
	{
		if(TIME_DEFAULT == locate_control_in.control_y.accel_time)
		{
			if(ABS_int(add_y)>Limit_pos_y)
				Locate_comman_limit(add_y, Locate_Control.control_y.end_pos, MAX_Speed_y, Limit_pos_y, TCB_handp[1], SinAccelPOS_ADD_Y);
			else
				Locate_comman(add_y, Locate_Control.control_y.end_pos, TCB_handp[1], SinAccelPOS_ADD_Y);
		}
		else
		{
			if(ABS_int(add_y)>Limit_pos_y)
				Locate_comman_Diy_limit(add_y, MAX_Speed_y, Limit_pos_y, locate_control_in.control_y, Locate_Control.control_y, TCB_handp[1], SinAccelPOS_ADD_Y);
			else
				Locate_comman_Diy(add_y, locate_control_in.control_y, Locate_Control.control_y, TCB_handp[1], SinAccelPOS_ADD_Y);
		}
	}
	/*	判断Y的初速度不为0*/
	else
	{
		/*	如果两次运动方向相同	*/
		//正方向
		if(add_y>0 && Locate_Control.control_y.end_speed>0)
		{
//			LCD_Display_int(Locate_Control.control_y.end_pos, 15, 0);
			//如果需要位移大于急停位移
			if(add_y>forecast_pos_y)
			{
				if(ABS_int(add_y)>Limit_pos_y)
					Locate_run_adv_limit(add_y, star_speed_y, MAX_Speed_y, Limit_pos_y, Locate_Control.control_y, TCB_handp[1], SinAccelPOS_ADD_Y);
				else
					Locate_run_adv(add_y, star_speed_y, Locate_Control.control_y, TCB_handp[1], SinAccelPOS_ADD_Y);
			}
			else
			{
				/*	急停 并反向·*/
				Locate_stop(Locate_Control.control_y, TCB_handp[1], SinAccelPOS_Stop_Y);
				add_y = add_y - (TCB_handp[1]->date_passage).add_dist;
				base_y = Locate_Control.control_y.end_pos + (TCB_handp[1]->date_passage).add_dist;
				Locate_node_inster_after(TCB_handp[1]);
				if(ABS_int(add_y)>Limit_pos_y)
					Locate_comman_limit(add_y, base_y, MAX_Speed_y, Limit_pos_y, (Locate_Tlist*)(TCB_handp[1]->locate_plist.nexttcb), SinAccelPOS_ADD_Y);
				else
					Locate_comman(add_y, base_y, (Locate_Tlist*)(TCB_handp[1]->locate_plist.nexttcb), SinAccelPOS_ADD_Y);
			}
		}
		//负方向
		else if(add_y<0 && forecast_pos_y<0)
		{
			if(add_y<forecast_pos_y)
			{
				if(ABS_int(add_y)>Limit_pos_y)
					Locate_run_adv_limit(add_y, star_speed_y, MAX_Speed_y, Limit_pos_y, Locate_Control.control_y, TCB_handp[1], SinAccelPOS_ADD_Y);
				else
					Locate_run_adv(add_y, star_speed_y, Locate_Control.control_y, TCB_handp[1], SinAccelPOS_ADD_Y);
			}
			else
			{
				/*	急停 并反向	*/
				Locate_stop(Locate_Control.control_y, TCB_handp[1], SinAccelPOS_Stop_Y);
				add_y = add_y - (TCB_handp[1]->date_passage).add_dist;
				base_y = Locate_Control.control_y.end_pos + (TCB_handp[1]->date_passage).add_dist;
				Locate_node_inster_after(TCB_handp[1]);
				if(ABS_int(add_y)>Limit_pos_y)
					Locate_comman_limit(add_y, base_y, MAX_Speed_y, Limit_pos_y, (Locate_Tlist*)(TCB_handp[1]->locate_plist.nexttcb), SinAccelPOS_ADD_Y);
				else
					Locate_comman(add_y, base_y, (Locate_Tlist*)(TCB_handp[1]->locate_plist.nexttcb), SinAccelPOS_ADD_Y);
			}
		}
		/*	如果两次运动方向不相同	*/
		//正方向
		else if(add_y>0 && Locate_Control.control_y.end_speed<0)
		{
			/*	急停 并反向	*/
			Locate_stop(Locate_Control.control_y, TCB_handp[1], SinAccelPOS_Stop_Y);
			add_y = add_y - (TCB_handp[1]->date_passage).add_dist;
			base_y = Locate_Control.control_y.end_pos + (TCB_handp[1]->date_passage).add_dist;
			Locate_node_inster_after(TCB_handp[1]);
			if(ABS_int(add_y)>Limit_pos_y)
				Locate_comman_limit(add_y, base_y, MAX_Speed_y, Limit_pos_y, (Locate_Tlist*)(TCB_handp[1]->locate_plist.nexttcb), SinAccelPOS_ADD_Y);
			else
				Locate_comman(add_y, base_y, (Locate_Tlist*)(TCB_handp[1]->locate_plist.nexttcb), SinAccelPOS_ADD_Y);
		}
		//负方向
		else
		{
			/* 急停 并反向	*/
			Locate_stop(Locate_Control.control_y, TCB_handp[1], SinAccelPOS_Stop_Y);
			add_y = add_y - (TCB_handp[1]->date_passage).add_dist;
			base_y = Locate_Control.control_y.end_pos + (TCB_handp[1]->date_passage).add_dist;
			Locate_node_inster_after(TCB_handp[1]);
			if(ABS_int(add_y)>Limit_pos_y)
				Locate_comman_limit(add_y, base_y, MAX_Speed_y, Limit_pos_y, (Locate_Tlist*)(TCB_handp[1]->locate_plist.nexttcb), SinAccelPOS_ADD_Y);
			else
				Locate_comman(add_y, base_y, (Locate_Tlist*)(TCB_handp[1]->locate_plist.nexttcb), SinAccelPOS_ADD_Y);
		}
	}
	
#endif
		
#if 1
	Limit_Pos_angle(&Limit_pos_w, MAX_AK, star_speed_w,(float)MAX_Speed_w);
//	LCD_Display_float(Limit_pos_w, 10, 6);
	while(locate_control_in.control_angle.target_pos>180)	locate_control_in.control_angle.target_pos -= 360;
	while(locate_control_in.control_angle.target_pos<-180)	locate_control_in.control_angle.target_pos += 360;
	add_angle = locate_control_in.control_angle.target_pos - Locate_Control.control_angle.end_angle;
	LCD_Display_float(add_angle,0,5);
	if(!locate_control_in.Rotate_flag)
	{
		while((int)add_angle<0)	
			add_angle += 360;
	}
	else
	{
		while((int)add_angle>0)
			add_angle -= 360;
	}
	LCD_Display_float(add_angle,10,6);
	/*	自转初速度为0	*/
	if(!Locate_Control.control_angle.end_speed)
	{
		/*	默认时间	*/
		if(TIME_DEFAULT == locate_control_in.control_angle.accel_time)
		{
			if(ABS_float(add_angle)>Limit_pos_w)
				Locate_Acomman_limit(add_angle, Locate_Control.control_angle.end_angle, MAX_Speed_w, Limit_pos_w, TCB_handp_w);
			else
				Locate_Acomman(add_angle, Locate_Control.control_angle.end_angle, TCB_handp_w);
		}
		/*	用户自定义时间	*/
		else
		{
			
			if(((ABS_float(add_angle))>(Limit_pos_w) ))
			{
				
//				BEED_ON;
//				delay_ms(100);
//				BEED_OFF;
//				delay_ms(100);
//				BEED_ON;
//				delay_ms(100);
//				BEED_OFF;
				Locate_Acomman_Diy_limit(add_angle, MAX_Speed_w, Limit_pos_w, locate_control_in.control_angle, Locate_Control.control_angle, TCB_handp_w);
			}
				
			else
			{
				
			Locate_Acomman_Diy(add_angle, locate_control_in.control_angle, Locate_Control.control_angle, TCB_handp_w);
			}
				
		}
	}
	/*	判断角初速度和后续运动方向是否相同	*/
	else
	{
		/*	两次运动方向相同	*/
		//正方向 - 逆时针
		if(add_angle>0 && Locate_Control.control_angle.end_speed>0)
		{
			//推进
			if(add_angle>forecast_pos_w)
			{
				if(ABS_float(add_angle)>Limit_pos_w)
				{
					Locate_Arun_adv_limit(add_angle, star_speed_w, MAX_Speed_w, Limit_pos_w, Locate_Control.control_angle, TCB_handp_w);
				}
				else
				{
					Locate_Arun_adv(add_angle, star_speed_w, Locate_Control.control_angle, TCB_handp_w);
				}
			}
			else
			{
				/*	急停并反向	*/
				Locate_Astop(Locate_Control.control_angle, TCB_handp_w);
				add_angle = add_angle - TCB_handp_w->date_passage.add_angle;
				base_angle = Locate_Control.control_angle.end_angle + TCB_handp_w->date_passage.add_angle;
				Locate_Anode_inster_after(TCB_handp_w);
				if(ABS_float(add_angle)>Limit_pos_w)
					Locate_Acomman_limit(add_angle, base_angle, MAX_Speed_w, Limit_pos_w, TCB_handp_w);
				else
					Locate_Acomman(add_angle, base_angle, TCB_handp_w);
			}
		}
		//顺时针
		if(add_angle<0 && Locate_Control.control_angle.end_speed>0)
		{
			//推进
			if(add_angle<forecast_pos_w)
			{
				if(ABS_float(add_angle)>Limit_pos_w)
					Locate_Arun_adv_limit(add_angle, star_speed_w, MAX_Speed_w, Limit_pos_w, Locate_Control.control_angle, TCB_handp_w);
				else
					Locate_Arun_adv(add_angle, star_speed_w, Locate_Control.control_angle, TCB_handp_w);
			}
			else
			{
				/*	急停并反向	*/
				Locate_Astop(Locate_Control.control_angle, TCB_handp_w);
				add_angle = add_angle - TCB_handp_w->date_passage.add_angle;
				base_angle = Locate_Control.control_angle.end_angle + TCB_handp_w->date_passage.add_angle;
				Locate_Anode_inster_after(TCB_handp_w);
				if(ABS_float(add_angle)>Limit_pos_w)
					Locate_Acomman_limit(add_angle, base_angle, MAX_Speed_w, Limit_pos_w, TCB_handp_w);
				else
					Locate_Acomman(add_angle, base_angle, TCB_handp_w);
			}
		}
		/*	两次运动方向不同	*/
		//顺时针旋转
		else if(add_angle<0 && Locate_Control.control_angle.end_speed>0)
		{
			/*	急停并反向	*/
			Locate_Astop(Locate_Control.control_angle, TCB_handp_w);
			add_angle = add_angle - TCB_handp_w->date_passage.add_angle;
			base_angle = Locate_Control.control_angle.end_angle + TCB_handp_w->date_passage.add_angle;
			Locate_Anode_inster_after(TCB_handp_w);
			if(ABS_float(add_angle)>Limit_pos_w)
				Locate_Acomman_limit(add_angle, base_angle, MAX_Speed_w, Limit_pos_w, TCB_handp_w);
			else
				Locate_Acomman(add_angle, base_angle, TCB_handp_w);
		}
		/*	两次运动方向不相同	*/
		//逆时针旋转
		else if(add_angle>0 && Locate_Control.control_angle.end_speed<0)
		{
			/*	急停并反向	*/
			Locate_Astop(Locate_Control.control_angle, TCB_handp_w);
			add_angle = add_angle - TCB_handp_w->date_passage.add_angle;
			base_angle = Locate_Control.control_angle.end_angle + TCB_handp_w->date_passage.add_angle;
			Locate_Anode_inster_after(TCB_handp_w);
			if(ABS_float(add_angle)>Limit_pos_w)
				Locate_Acomman_limit(add_angle, base_angle, MAX_Speed_w, Limit_pos_w, TCB_handp_w);
			else
				Locate_Acomman(add_angle, base_angle, TCB_handp_w);
		}
	}
#endif
}





/**	
	*	@brief:		void LOCATE_Control_Run()	控制链表的执行	全场定位的曲线执行
	*	@param:		Locate_Tlist *controlP		控制直线链表
					int 		 *end_flag		执行标记(是否执行完)
					int 		 *speed_result	执行结果(速度输出)
	*	@retval:	NONE
	*/
void LOCATE_Control_Run_X(Locate_Tlist *controlP, LOCATE_CONTROL_tail *Control_tail, int *end_flag, int *speed_result, int distance, float P_param)
{
	static Locate_Tlist* controlp = NULL;
	
	if(controlP->date_passage.Clear)
	{
		controlp = controlP;
//		controlP->date_passage.Clear = 0;
	}
		
	if(!(NULL == controlp))
	{
		*end_flag = (controlp->date_passage).TCB_Func(controlp->date_passage, Locate_output, Control_tail, distance, P_param, speed_result);
		if(*end_flag)
		{
			if(!(NULL == controlp->locate_plist.nexttcb))
			{
				controlp = (Locate_Tlist *)controlp->locate_plist.nexttcb;
//				*end_flag = (controlp->date_passage).TCB_Func(controlp->date_passage, Locate_output, Control_tail, distance, P_param, speed_result);
				*end_flag = 0;
			}
			else
			{
				*speed_result = 0;
				*end_flag = 1;
				controlp = NULL;
			}
		}
		
	}
	else
	{
		*speed_result = 0;
		controlP->date_passage.Clear =  1;
		*end_flag = 1;
	}
	if(*end_flag)
	{
		controlP->date_passage.Clear =  1;
	}
	/*	调试显示程序	*/
}





/**	
	*	@brief:		void LOCATE_Control_Run()	控制链表的执行	全场定位的曲线执行
	*	@param:		Locate_Tlist *controlP		控制直线链表
					int 		 *end_flag		执行标记(是否执行完)
					int 		 *speed_result	执行结果(速度输出)
	*	@retval:	NONE
	*/
void LOCATE_Control_Run_Y(Locate_Tlist *controlP, LOCATE_CONTROL_tail *Control_tail, int *end_flag, int *speed_result, int distance, float P_param)
{
	static Locate_Tlist* controlp = NULL;
	
	if(controlP->date_passage.Clear)
	{
		controlp = controlP;
	}
		
	if(!(NULL == controlp))
	{
		*end_flag = (controlp->date_passage).TCB_Func(controlp->date_passage, Locate_output, Control_tail, distance, P_param, speed_result);
		if(*end_flag)
		{
			if(!(NULL == controlp->locate_plist.nexttcb))
			{
				controlp = (Locate_Tlist *)controlp->locate_plist.nexttcb;
				*end_flag = 0;
			}
			else
			{
				*speed_result = 0;
				*end_flag = 1;
				controlp = NULL;
			}
		}
		
	}
	else
	{
		*speed_result = 0;
		controlP->date_passage.Clear =  1;
		*end_flag = 1;
	}
	if(*end_flag)
	{
		controlP->date_passage.Clear =  1;
	}
	/*	调试显示程序	*/
//	LCD_Display_int(*speed_result, 0, 6);
//	LCD_Display_int(*end_flag, 6, 6);
//	LCD_Display_int(run_flag, 12, 6);
//	LCD_Display_int(TCB_handp[0]->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(TCB_handp[0]->date_passage.base_speed, 10, 7);
}






/**	
	*	@brief:		void LOCATE_Control_Run()	控制链表的执行	全场定位的曲线执行
	*	@param:		Locate_Tlist *controlP		控制直线链表
					int 		 *end_flag		执行标记(是否执行完)
					int 		 *speed_result	执行结果(速度输出)
	*	@retval:	NONE
	*/
void LOCATE_AControl_Run(Locate_Tlist_angle *controlP, int *end_flag, int *speed_result, float distance, float P_param)
{
	static Locate_Tlist_angle* controlp = NULL;
	if(controlP->date_passage.Clear)
	{
		controlp = controlP;
//		controlP->date_passage.Clear = 0;
	}
		
	if(!(NULL == controlp))
	{
		*end_flag = (controlp->date_passage).TCB_Func(&controlp->date_passage, Locate_output, (*Locate_RTime.ZAngle), (float)W_PParam, &(Locate_output.speed_w));
		if(*end_flag)
		{
			if(!(NULL == controlp->locate_plist.nexttcb))
			{
				controlp = (Locate_Tlist_angle *)controlp->locate_plist.nexttcb;
//				*end_flag = (controlp->date_passage).TCB_Func(controlp->date_passage, Locate_output, Control_tail, (*Locate_RTime.ZAngle), (float)W_PParam, &(Locate_output.speed_w));
				*end_flag = 0;
			}
			else
			{
				*speed_result = 0;
				*end_flag = 1;
				controlp = NULL;				
			}
		}
	}
	else
	{
		*speed_result = 0;
		*end_flag = 1;
		controlP->date_passage.Clear = 1;
	}
	if(*end_flag)
	{
		controlP->date_passage.Clear = 1;
	}
	/*	调试显示程序	*/
//	LCD_Display_int(*speed_result, 0, 6);
//	LCD_Display_int(*end_flag, 6, 6);
//	LCD_Display_int(run_flag, 12, 6);
//	LCD_Display_int(TCB_handp[0]->date_passage.accel_time_count, 0, 7);
//	LCD_Display_int(TCB_handp[0]->date_passage.base_speed, 10, 7);
}





/**
	*	@brief:		void LOCATE_Control_In	全场定位系统输入	也是全场定位曲线的控制终端
	*	@param:		locate_control_in	全场定位控制参数传入	为用户的控制目标
	*	@retva:		NONE
	*	@supplement:	全场定位算法：先将运行速度和将要运行的速度进行控制，以此来进行参数设定
	*/
void LOCATE_Control_In(LOCATE_CONTROL_IN	*locate_control_in)
{
	static LOCATE_BOOL end_flag_x = 1;
	static LOCATE_BOOL end_flag_y = 1;
	static LOCATE_BOOL end_flag_w = 1;

	static LOCATE_BOOL Walk_flag_x = 0;
	static LOCATE_BOOL Walk_flag_y = 0;
	static LOCATE_BOOL Walk_flag_w = 0;
	static int x_leave = 0;
	static int y_leave = 0;
	static float w_leave = 0;

	/*	进入全场定位系统控制	*/
		if(locate_control_in->Update_Flag)
		{
			BEED_ON;
			delay_ms(100);
			BEED_OFF;
			delay_ms(100);
			BEED_ON;
			delay_ms(100);
			BEED_OFF;
			delay_ms(100);
			//	释放
			Locate_node_clear(TCB_handp[0]);
			Locate_node_clear(TCB_handp[1]);
			Locate_Anode_clear(TCB_handp_w);
			TCB_handp[0]->date_passage.Clear = 1;
			TCB_handp[1]->date_passage.Clear = 1;
			TCB_handp_w->date_passage.Clear = 1;
			Locate_Control.control_x.end_pos = (*Locate_RTime.X);
			Locate_Control.control_y.end_pos = (*Locate_RTime.Y);
			Locate_Control.control_angle.end_angle = (*Locate_RTime.ZAngle);
			Walk_flag_x = 0;
			Walk_flag_y = 0;
			Walk_flag_w = 0;
			
			switch(locate_control_in->Command)
			{
				
				case System_Exit:		{
											end_flag_x = Locate_stop_rapidly(Locate_Control.control_x, TCB_handp[0], SinAccelPOS_Stop_X);
											end_flag_y = Locate_stop_rapidly(Locate_Control.control_y, TCB_handp[1], SinAccelPOS_Stop_Y);
											end_flag_w = Locate_Astop_rapidly(Locate_Control.control_angle, TCB_handp_w);
										}break;
				
				case Scram_Jerk:		{
											end_flag_x = Locate_stop_rapidly(Locate_Control.control_x, TCB_handp[0], SinAccelPOS_Stop_X);
											end_flag_y = Locate_stop_rapidly(Locate_Control.control_y, TCB_handp[1], SinAccelPOS_Stop_Y);
											end_flag_w = Locate_Astop_rapidly(Locate_Control.control_angle, TCB_handp_w);
										}break;
				
				case Pre_Deal:			{
											LOCATE_pre_deal(*locate_control_in);
//											locate_control_in->Command = Deal_With;
										}break;
				
				case Deal_With:			{
											
										}break;
				
				default:				{
											
											
										}break;
						
			}
			locate_control_in->Update_Flag = 0;
			
			//开始时间
			Init_Time = Time_5S * 5000 + TIM5->CNT;
		}

		/*	按照曲线设计，开始进行控制	*/
		LOCATE_Control_Run_X(TCB_handp[0], &Locate_Control.control_x, &end_flag_x, &(Locate_output.speed_x), (*Locate_RTime.X), (float)X_PParam);
		LOCATE_Control_Run_Y(TCB_handp[1], &Locate_Control.control_y, &end_flag_y, &(Locate_output.speed_y), (*Locate_RTime.Y), (float)Y_PParam);
		LOCATE_AControl_Run(TCB_handp_w, &end_flag_w, &(Locate_output.speed_w), (*Locate_RTime.ZAngle), (float)W_PParam);

		/*	曲线最终速度输出	*/
		Control_Angle_Walk_All(	Locate_output.speed_x,
								Locate_output.speed_y,
								Locate_output.speed_w);

		/*	数据刷新完成，清零标志	*/
		TCB_handp[0]->date_passage.Clear = 0;
		TCB_handp[1]->date_passage.Clear = 0;
		TCB_handp_w->date_passage.Clear = 0;

		/*	曲线已跟踪完，保持最终位置，用PID保持	*/
		if(1 == end_flag_x)		
		{
			if(0 == Walk_flag_x)
			{
				x_leave = Locate_Control.control_x.end_pos;
			}
			Locate_node_clear(TCB_handp[0]);
			Locat_Walk_Line(TCB_handp[0], x_leave, SinAccelPOS_ADD_X);
			TCB_handp[0]->date_passage.Clear = 1;
			Walk_flag_x = 1;
		}
		if(1 == end_flag_y )		
		{
			if(0 == Walk_flag_y)
			{
				y_leave = Locate_Control.control_y.end_pos;
			}
			Locate_node_clear(TCB_handp[1]);
			Locat_Walk_Line(TCB_handp[1], y_leave, SinAccelPOS_ADD_Y);
			TCB_handp[1]->date_passage.Clear = 1;
			Walk_flag_y = 1;
		}
		if(1 == end_flag_w)
		{
			if(0 == Walk_flag_w)
			{
				w_leave = Locate_Control.control_angle.end_angle;
			}
			Locate_Anode_clear(TCB_handp_w);
			Locat_Walk_Line_angle(TCB_handp_w, w_leave);
			TCB_handp_w->date_passage.Clear = 1;
			Walk_flag_w = 1;
		}
//		delay_ms(0);
//		i++;
	if(1 == end_flag_x && 1 == end_flag_y && 1 == end_flag_w)
	{
		End_Time = Time_5S * 5000 + TIM5->CNT;
		TIM_Cmd(TIM5, DISABLE);
//		LCD_Display_int(i, 0, 3);
//		LCD_Display_int(End_Time, 10, 7);
	}
	

}	





