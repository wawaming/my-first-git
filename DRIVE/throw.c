#include "throw.h"
extern SERVO_MLCB UNDER6_MLCB;
/*����������ʵ�̶ֹ�ת���ٶȺ͹̶���������
  ���뺯����ֹͣ�㣺set_distance
	         ����ʱ�䣺decelerate_time
           PID���ڣ�P D
*/
 /*ʵ�ʸ����ٶȵ�PD����
	    init_distance               //��ʼλ��������
	    current_diantance_reality  //��ǰλ��ʵ��ֵ ������
      current_diantance_theory	//��ǰλ������ֵ ������
	��ǰ λ�� �ٶ� ʱ�� V_max=throw_speed
	X��ʱ�� time_to_x=��*��������*����/������ʱ�� 
	Y���ٶ� V_to_y =V_Max*��cos(time_to_x)+1��
	��ǰλ������ֵ current_diantance_theory=init_distance+(V_Max/2)*(sin(time_to_x)+time_to_x)
	*/
	//��ʼ����
int control_num=0;//���ƴ���0-max
int diantance_offset_last=0;//����ǰ��ƫ��ֵ
extern int init_distance; //��ʼλ�� ������
extern  int32_t Encoder_TIM2;	//ͨ��TIM2������ģʽ��׽�ļ���
void throw_ball(void)
	{
		int V_to_set=0; 	
    int V_to_y=0;
    int current_diantance_reality=0;//��ǰλ��ʵ��ֵ ������
    int current_diantance_theory=0;	//��ǰλ������ֵ ������
    int diantance_offset=0;	        //����ƫ��		
		int decelerate_time=0;//����ʱ��
		float time_to_x=0.0;
		decelerate_time=set_distance/throw_speed*2;
    time_to_x=Pi*TIME_cyc*control_num/decelerate_time; //ms
    V_to_y=(int)(throw_speed_max/2*(cos(time_to_x)+1)); //��ǰ����ʱ������Ӧ�ø�����ʱ�� r/min
	  current_diantance_theory=init_distance+(throw_speed_max/2)*(sin(time_to_x)+time_to_x);//r/min=1����/ms 
	  current_diantance_reality=Encoder_TIM2*65536+TIM2->CNT;  //����������	  
	  diantance_offset=current_diantance_theory-current_diantance_reality;	
	  V_to_set=V_to_y+(int)(throw_P*diantance_offset+throw_D*(diantance_offset-diantance_offset_last));
	  diantance_offset_last=diantance_offset;
	  if(V_to_set>throw_speed_max) V_to_set=throw_speed_max;
	  if(V_to_set<throw_speed_min) V_to_set=throw_speed_min;
		Motor_SetSpeed(&UNDER6_MLCB,V_to_set);
		control_num++;
}
