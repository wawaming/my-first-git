#include "throw.h"
extern SERVO_MLCB UNDER6_MLCB;
/*抛掷函数：实现固定转动速度和固定点抛掷球
  输入函数：停止点：set_distance
	         减速时间：decelerate_time
           PID调节：P D
*/
 /*实际给定速度的PD控制
	    init_distance               //初始位置脉冲数
	    current_diantance_reality  //当前位置实际值 脉冲数
      current_diantance_theory	//当前位置理论值 脉冲数
	当前 位置 速度 时间 V_max=throw_speed
	X轴时间 time_to_x=π*控制周期*次数/减速总时间 
	Y轴速度 V_to_y =V_Max*（cos(time_to_x)+1）
	当前位置理论值 current_diantance_theory=init_distance+(V_Max/2)*(sin(time_to_x)+time_to_x)
	*/
	//初始化：
int control_num=0;//控制次数0-max
int diantance_offset_last=0;//记入前次偏差值
extern int init_distance; //初始位置 脉冲数
extern  int32_t Encoder_TIM2;	//通过TIM2编码器模式捕捉的计数
void throw_ball(void)
	{
		int V_to_set=0; 	
    int V_to_y=0;
    int current_diantance_reality=0;//当前位置实际值 脉冲数
    int current_diantance_theory=0;	//当前位置理论值 脉冲数
    int diantance_offset=0;	        //距离偏差		
		int decelerate_time=0;//减速时间
		float time_to_x=0.0;
		decelerate_time=set_distance/throw_speed*2;
    time_to_x=Pi*TIME_cyc*control_num/decelerate_time; //ms
    V_to_y=(int)(throw_speed_max/2*(cos(time_to_x)+1)); //当前控制时间理论应该给定的时间 r/min
	  current_diantance_theory=init_distance+(throw_speed_max/2)*(sin(time_to_x)+time_to_x);//r/min=1脉冲/ms 
	  current_diantance_reality=Encoder_TIM2*65536+TIM2->CNT;  //编码器读数	  
	  diantance_offset=current_diantance_theory-current_diantance_reality;	
	  V_to_set=V_to_y+(int)(throw_P*diantance_offset+throw_D*(diantance_offset-diantance_offset_last));
	  diantance_offset_last=diantance_offset;
	  if(V_to_set>throw_speed_max) V_to_set=throw_speed_max;
	  if(V_to_set<throw_speed_min) V_to_set=throw_speed_min;
		Motor_SetSpeed(&UNDER6_MLCB,V_to_set);
		control_num++;
}
