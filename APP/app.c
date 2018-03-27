/////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************
*										Persional Task
*		�������ȼ��ѷָ�ϵͳ����,����ʹ��
*		���ȼ�0���жϷ������������� 		OS_IntQTask()
		���ȼ�1��ʱ�ӽ������� 				OS_TickTask()
		���ȼ�2����ʱ���� 					OS_TmrTask()
		���ȼ�OS_CFG_PRIO_MAX-2��	ͳ������ OS_StatTask()
		���ȼ�OS_CFG_PRIO_MAX-1��	�������� OS_IdleTask()
		
*		�ٽ��������д
		CPU_SR_ALLOC();
		OS_CRITICAL_ENTER();
		//	�û���Ҫ�����Ĵ���	//
		OS_CRITICAL_EXIT();

////////////////////////////////////////////////////////////////////////////////////////////
*	�����ź���������
	----------------�����źű���----------------
	OS_SEM	Handle_Sem;			
	----------------�����ź���------------------
	OSSemCreate(	(OS_SEM		*)&Handle_Sem,				//��Ҫ�������ź���(��Ҫ������)
								(CPU_CHAR	*)"Handle_Sem",				//�ź���������
								(OS_SEM_CTR)0,									//�ź����ĳ�ʼֵ
								(OS_ERR   *)&HandleControlTaskErr);		//���󷵻ش���
	----------------�����ź���----------------
	OSSemPend(	(OS_SEM	*)&Handle_Sem,						//������ź���
							(OS_TICK )0,											//�ȴ�ʱ��,0Ϊ���޵ȴ�,����Ϊ�ȴ���ȷ��ʱ�ӽ�����
							(OS_OPT  )OS_OPT_PEND_BLOCKING,		//��û���뵽�ź���,�����񱻹���
							(CPU_TS *)0,											//���ʱ���
							(OS_ERR *)&HandleControlTaskErr);	//�������
	----------------�ͷ��ź���----------------
	OSSemPost(	(OS_SEM	*)&Handle_Sem,					//��Ҫ�ͷŵ��ź���
							(OS_OPT	 )OS_OPT_POST_1,		//�ͷŸ��ȴ����ź����������о���������ȼ���
							(OS_ERR	*)&err);						//������뷵��
	�ź������������ĸ����輴�ɽ��м�ʹ�ã������Ҫ�߼��÷��������û����в������ϼ���
////////////////////////////////////////////////////////////////////////////////////////////
*	�������ͽ�Ҳ�����
	----------------��������-----------------
	OS_TaskSuspend((OS_TCB*)&MPU6050TaskTCB, &MPUTaskErr);		
	----------------�������-----------------
	OS_TaskResume((OS_TCB*)&MPU6050TaskTCB, &MPUTaskErr);
	�����������ֻ��ͨ������������н�Ҳ���
////////////////////////////////////////////////////////////////////////////////////////////

*	@attention		�������Աע��ReadMe.text�ļ�	������APP�ļ�����

*	@supplement��	���ȼ�/ջ�ռ��С/����������app.h���
*	@author:	yun
*	@version 	V2.0
*	@data:		2016/12/7
*************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "app.h"
#include "throw.h"
///////////////////////////////////////////////////////////////////////////////////////
/************************************ ���񴴽����� ***********************************/
/*				������ƿ�����							�����ջ�ռ�����			 */
///////////////////////////////////////////////////////////////////////////////////////
//-----star_task
extern OS_TCB StarTaskTCB;			

//-----led_task
OS_TCB LedTaskTCB;					
CPU_STK LED_TASK_STK[LED_STK_SIZE];	

//-----throw_task
OS_TCB throwTaskTCB;					
CPU_STK throw_TASK_STK[throw_STK_SIZE];

//-----LCD_display
OS_TCB	LCDTaskTCB;
CPU_STK LCD_TASK_STK[LCD_STK_SIZE];

//-----KEY_display
OS_TCB	KEYTaskTCB;
CPU_STK KEY_TASK_STK[KEY_STK_SIZE];

//-----LINE_WALKING
OS_TCB	LineWalkTaskTCB;
CPU_STK LineWalk_TASK_STK[LineWalk_STK_SIZE];

//-----Handle_Control
OS_TCB	HandleControlTaskTCB;
CPU_STK HandleControl_TASK_STK[HandleControl_STK_SIZE];

//-----Offset_Control
OS_TCB	OffsetControlTaskTCB;
CPU_STK OffsetControl_TASK_STK[OffsetControl_STK_SIZE];
///////////////////////////////////////////////////////////////////////////////////////
/*									ERROR											 */
/********************************������뷵������*************************************/
/******************************���û����������������*********************************/
/*****************************���ڼ�������Ĵ���ԭ��********************************/
///////////////////////////////////////////////////////////////////////////////////////
OS_ERR	StarTaskErr = OS_ERR_NONE;			/*	��ʼ����	OS_ERR_NONEΪ��ʼ�޴���	*/
OS_ERR	LedTaskErr  = OS_ERR_NONE;			/*	��������	*/
OS_ERR	throwTaskErr  = OS_ERR_NONE;    /*  ��������  */
OS_ERR	LCDTaskErr	= OS_ERR_NONE;			/*	LCD����		*/
OS_ERR	KeyTaskErr	= OS_ERR_NONE;			/*	KEY����		*/
OS_ERR	LineWalkTaskErr	 = OS_ERR_NONE;		/*	LineWalk����*/
OS_ERR	HandleControlTaskErr	 = OS_ERR_NONE;	/*	�ֱ�ң������	*/
OS_ERR	OffsetControlTaskErr	 = OS_ERR_NONE;	/*	ƫ��ɼ�����	�ư�	����ͷ*/
/*	���󷵻ص�ַ	*/

/*	������������ж�����	��Ҫ�˵��Ը���ʱ	����LCD�򴮿ڵȻ�ȡ��������Ϣ	*/
/*	��������ʾ������������	�Լ�������Ƿ�����	*/
/*	������Ŀ�ı�ʱ	��Ҫ�ı������С	*/
uint8_t	TASK_RUNNING[TASK_COUNT] = {0};

///////////////////////////////////////////////////////////////////////////////////////
/**********************************ȫ�ֱ�������***************************************/
///////////////////////////////////////////////////////////////////////////////////////
/*ENCODERʾ��*/
volatile int32_t Encoder1_Count=0;
volatile int32_t Encoder2_Count=0;
volatile int32_t Encoder_TIM2=0;	//65536*ʾ��+TIMx->CNT���������־
volatile int32_t Encoder_TIM4=0;
volatile int32_t Encoder_TIM8=0;
/*������ƿ�*/
extern SERVO_MLCB UNDER1_MLCB;
extern SERVO_MLCB UNDER2_MLCB;
extern SERVO_MLCB UNDER3_MLCB;
extern SERVO_MLCB UNDER4_MLCB;
extern SERVO_MLCB UNDER5_MLCB;
extern SERVO_MLCB UNDER6_MLCB;

/*	���̵���ٶȷ���	*/
int Under_Motor1[2] = {0};
int Under_Motor2[2] = {0};
int Under_Motor3[2] = {0};
int Under_Motor4[2] = {0};

/*	���Կ�ʼ��־	*/
uint8_t ROBOCON_Start=0;

/*	�ֱ�����	*/
HandlesStruct UART_CH;
uint8_t send_flag = 0;
uint8_t Line_Walk_Flag = 0;
int debug_symbol = 1;
int fire_throw=0;
Robocon_Handle_Struct Rb_Handle_Control = {0XFF, 0XFE, 0XFD, 0XF2};
Robocon_Handle_Struct tab[31];
uint8_t Robocon_Handle_Flag = 0;

/*	����ͷģ��	*/
int16_t Camera_Offset = 1;	//����ͷƫ��

/*	Ѳ��ģ��	*/
uint8_t Line_Walk_Start = 0;
uint8_t Line_Speed_val = 0;
float	Line_Angle = 0;
uint8_t Accel_Line_Walk_Flag = 0;
uint8_t	Angle_Line_Walk_Flag = 0;
float Adjust_P = 8;
float Adjust_Time = 2;
									
/*	���Բ��ּ���	*/
uint8_t Error_Count = 0;
int Init_Time = 0;
int End_Time = 0;
int Direction = 1;
extern int a;
/*	ȫ����λ��������㷨�ٶȱ���	*/
int Time_5S;

/*	�������̽�������	*/
uint8_t Locate_Alldate[24] = {0};


/*   ����ģ��		*/
uint8_t flag_off =0;
uint8_t flag_line =0;

int place;
extern uint16_t  safeplace_345;					//
extern uint16_t Launchplace;					//���� �����������ͷ�λ��

//Ħ��
extern int32_t  bru_speed;			//��ˢ�����ת�ٶ�	-������
extern float  rotate_speed;	  //������ת�ٶ�      -���Թ�
uint16_t moc_speed_3m = 1200;
uint16_t moc_speed_6m = 1300;
uint16_t speed_3m = 2000;
uint16_t speed_6m = 3000;
/*����*/
int Launchplace_3m1 = 800;
int Launchplace_6m2 = 850;
int Launchplace_6m3 = 900;
int Launchplace_6m4 = 950;
int Launchplace_6m5 = 900;
int Launchplace_6m6 = 850;
int Launchplace_9m7 = 1000;

/*������*/
extern int speed;
extern float C_angleoffset;
extern float C_nowangle;//����������ǰ�Ƕ�
extern int16_t C_differedge;
extern int16_t C_nowedge;//����������ǰ�߳�
extern int16_t C_targetedge;
extern int speed_adjust;
///////////////////////////////////////////////////////////////////////////////////////
/**********************************�ź�������*****************************************/
///////////////////////////////////////////////////////////////////////////////////////
OS_SEM	Handle_Sem;
OS_SEM	LineWalk_task_Sem1;
OS_SEM	LineWalk_task_Sem2;

///////////////////////////////////////////////////////////////////////////////////////
/******************************** ���������� ***************************************/
///////////////////////////////////////////////////////////////////////////////////////

/**
	*@brief	star_task
	*/ 
void star_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	CPU_Init();
	p_arg = p_arg;//��ֹ����������
	if(TASK_RUNNING[0]++,TASK_RUNNING[0]>9)		TASK_RUNNING[0] = 0;		//�������м�¼	
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	OSTaskCreate ((OS_TCB*     )  &LedTaskTCB,						//������ƿ�
								(CPU_CHAR   *)  "led_task",			//��������,�������ܱ���������ʾ����
								(OS_TASK_PTR )   led_task,			//������
								(void       *)   0,					//���뺯���Ĳ���
								(OS_PRIO     )   LED_TASK_PRIO,		//��������ȼ�
								(CPU_STK    *)   &LED_TASK_STK[0],	//�����ջ����ַ
								(CPU_STK_SIZE)   LED_STK_SIZE/10,	//�����ջ�������,���ڶ�ջ������
								(CPU_STK_SIZE)   LED_STK_SIZE,		//�����ջ��С
								(OS_MSG_QTY  )   0,					//�����ڲ���Ϣ���н��յ������Ϣ��Ŀ,0���ǽ�ֹ��Ϣ����
								(OS_TICK     )   0,					//ʹ��ʱ��Ƭ��ת����ʱ�趨��ǰ�����ʱ��Ƭ����,0ΪĬ�ϳ���
								(void       *)   0,					//�û�����Ĵ洢��,һ��Ϊһ�����ݽṹ,������չ�����TCB,��:�Ÿ�����������
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����������ض�ѡ���������Ķ�ջ|��Ҫ��ջ����
								(OS_ERR     *)  &err);				//ָ��һ�������洢������
								
					
	OSTaskCreate ((OS_TCB*     )  &throwTaskTCB,						//������ƿ�
								(CPU_CHAR   *)  "throw_task",			//��������,�������ܱ���������ʾ����
								(OS_TASK_PTR )   throw_task,			//������
								(void       *)   0,					//���뺯���Ĳ���
								(OS_PRIO     )   throw_TASK_PRIO,		//��������ȼ�
								(CPU_STK    *)   &throw_TASK_STK[0],	//�����ջ����ַ
								(CPU_STK_SIZE)   throw_STK_SIZE/10,	//�����ջ�������,���ڶ�ջ������
								(CPU_STK_SIZE)   throw_STK_SIZE,		//�����ջ��С
								(OS_MSG_QTY  )   0,					//�����ڲ���Ϣ���н��յ������Ϣ��Ŀ,0���ǽ�ֹ��Ϣ����
								(OS_TICK     )   0,					//ʹ��ʱ��Ƭ��ת����ʱ�趨��ǰ�����ʱ��Ƭ����,0ΪĬ�ϳ���
								(void       *)   0,					//�û�����Ĵ洢��,һ��Ϊһ�����ݽṹ,������չ�����TCB,��:�Ÿ�����������
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����������ض�ѡ���������Ķ�ջ|��Ҫ��ջ����
								(OS_ERR     *)  &err);				//ָ��һ�������洢������
								
	OSTaskCreate ((OS_TCB*     )  &LCDTaskTCB,						//������ƿ�
								(CPU_CHAR   *)  "LCD_task",			//��������,�������ܱ���������ʾ����
								(OS_TASK_PTR )   LCD_task,			//������
								(void       *)   0,					//���뺯���Ĳ���
								(OS_PRIO     )   LCD_TASK_PRIO,		//��������ȼ�
								(CPU_STK    *)   &LCD_TASK_STK[0],	//�����ջ����ַ
								(CPU_STK_SIZE)   LCD_STK_SIZE/10,	//�����ջ�������,���ڶ�ջ������
								(CPU_STK_SIZE)   LCD_STK_SIZE,		//�����ջ��С
								(OS_MSG_QTY  )   0,					//�����ڲ���Ϣ���н��յ������Ϣ��Ŀ,0���ǽ�ֹ��Ϣ����
								(OS_TICK     )   0,					//ʹ��ʱ��Ƭ��ת����ʱ�趨��ǰ�����ʱ��Ƭ����,0ΪĬ�ϳ���
								(void       *)   0,					//�û�����Ĵ洢��,һ��Ϊһ�����ݽṹ,������չ�����TCB,��:�Ÿ�����������
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����������ض�ѡ���������Ķ�ջ|��Ҫ��ջ����
								(OS_ERR     *)  &err);				//ָ��һ�������洢������				

	OSTaskCreate ((OS_TCB*     )  &KEYTaskTCB,						//������ƿ�
								(CPU_CHAR   *)  "KEY_task",			//��������,�������ܱ���������ʾ����
								(OS_TASK_PTR )   KEY_task,			//������
								(void       *)   0,					//���뺯���Ĳ���
								(OS_PRIO     )   KEY_TASK_PRIO,		//��������ȼ�
								(CPU_STK    *)   &KEY_TASK_STK[0],	//�����ջ����ַ
								(CPU_STK_SIZE)   KEY_STK_SIZE/10,	//�����ջ�������,���ڶ�ջ������
								(CPU_STK_SIZE)   KEY_STK_SIZE,		//�����ջ��С
								(OS_MSG_QTY  )   0,					//�����ڲ���Ϣ���н��յ������Ϣ��Ŀ,0���ǽ�ֹ��Ϣ����
								(OS_TICK     )   0,					//ʹ��ʱ��Ƭ��ת����ʱ�趨��ǰ�����ʱ��Ƭ����,0ΪĬ�ϳ���
								(void       *)   0,					//�û�����Ĵ洢��,һ��Ϊһ�����ݽṹ,������չ�����TCB,��:�Ÿ�����������
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����������ض�ѡ���������Ķ�ջ|��Ҫ��ջ����
								(OS_ERR     *)  &err);				//ָ��һ�������洢������

	OSTaskCreate ((OS_TCB*     )  &LineWalkTaskTCB,						//������ƿ�
								(CPU_CHAR   *)  "LineWalk_task",		//��������,�������ܱ���������ʾ����
								(OS_TASK_PTR )   LineWalk_task,			//������
								(void       *)   0,						//���뺯���Ĳ���
								(OS_PRIO     )   LineWalk_TASK_PRIO,	//��������ȼ�
								(CPU_STK    *)   &LineWalk_TASK_STK[0],	//�����ջ����ַ
								(CPU_STK_SIZE)   LineWalk_STK_SIZE/10,	//�����ջ�������,���ڶ�ջ������
								(CPU_STK_SIZE)   LineWalk_STK_SIZE,		//�����ջ��С
								(OS_MSG_QTY  )   0,						//�����ڲ���Ϣ���н��յ������Ϣ��Ŀ,0���ǽ�ֹ��Ϣ����
								(OS_TICK     )   0,						//ʹ��ʱ��Ƭ��ת����ʱ�趨��ǰ�����ʱ��Ƭ����,0ΪĬ�ϳ���
								(void       *)   0,						//�û�����Ĵ洢��,һ��Ϊһ�����ݽṹ,������չ�����TCB,��:�Ÿ�����������
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����������ض�ѡ���������Ķ�ջ|��Ҫ��ջ����
								(OS_ERR     *)  &err);					//ָ��һ�������洢������				

	OSTaskCreate ((OS_TCB*     )  &HandleControlTaskTCB,					//������ƿ�
								(CPU_CHAR   *)  "HandleControl_task",		//��������,�������ܱ���������ʾ����
								(OS_TASK_PTR )   HandleControl_task,		//������
								(void       *)   0,							//���뺯���Ĳ���
								(OS_PRIO     )   HandleControl_TASK_PRIO,	//��������ȼ�
								(CPU_STK    *)   &HandleControl_TASK_STK[0],//�����ջ����ַ
								(CPU_STK_SIZE)   HandleControl_STK_SIZE/10,	//�����ջ�������,���ڶ�ջ������
								(CPU_STK_SIZE)   HandleControl_STK_SIZE,	//�����ջ��С
								(OS_MSG_QTY  )   0,							//�����ڲ���Ϣ���н��յ������Ϣ��Ŀ,0���ǽ�ֹ��Ϣ����
								(OS_TICK     )   0,							//ʹ��ʱ��Ƭ��ת����ʱ�趨��ǰ�����ʱ��Ƭ����,0ΪĬ�ϳ���
								(void       *)   0,							//�û�����Ĵ洢��,һ��Ϊһ�����ݽṹ,������չ�����TCB,��:�Ÿ�����������
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����������ض�ѡ���������Ķ�ջ|��Ҫ��ջ����
								(OS_ERR     *)  &err);						//ָ��һ�������洢������		
								
	OSTaskCreate ((OS_TCB*     )  &OffsetControlTaskTCB,					//������ƿ�
								(CPU_CHAR   *)  "OffsetControl_task",		//��������,�������ܱ���������ʾ����
								(OS_TASK_PTR )   OffsetControl_task,		//������
								(void       *)   0,							//���뺯���Ĳ���
								(OS_PRIO     )   OffsetControl_TASK_PRIO,	//��������ȼ�
								(CPU_STK    *)   &OffsetControl_TASK_STK[0],//�����ջ����ַ
								(CPU_STK_SIZE)   OffsetControl_STK_SIZE/10,	//�����ջ�������,���ڶ�ջ������
								(CPU_STK_SIZE)   OffsetControl_STK_SIZE,	//�����ջ��С
								(OS_MSG_QTY  )   0,							//�����ڲ���Ϣ���н��յ������Ϣ��Ŀ,0���ǽ�ֹ��Ϣ����
								(OS_TICK     )   0,							//ʹ��ʱ��Ƭ��ת����ʱ�趨��ǰ�����ʱ��Ƭ����,0ΪĬ�ϳ���
								(void       *)   0,							//�û�����Ĵ洢��,һ��Ϊһ�����ݽṹ,������չ�����TCB,��:�Ÿ�����������
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����������ض�ѡ���������Ķ�ջ|��Ҫ��ջ����
								(OS_ERR     *)  &err);						//ָ��һ�������洢������			

		LED0(0);LED1(1);delay_ms(500);
		LED0(0);LED1(1);delay_ms(500);							
	OS_CRITICAL_EXIT();		//�˳��ٽ���
	OS_TaskSuspend((OS_TCB*)&StarTaskTCB,&err);		//����ʼ����	
}

/*********************************************************************************************************************
 **
	*	@brief		led_task
	*	@describe	���Ƴ���,����ϵͳ����
	*
**********************************************************************************************************************/
extern int throw_flag;
extern int v_flag;
extern int speed_real;
//extern  int32_t Encoder_TIM2;	//ͨ��TIM2������ģʽ��׽�ļ���
void led_task(void *p_arg)
{
	CPU_SR_ALLOC();
	p_arg = p_arg;
//	BEED_ON;delay_ms(500);BEED_OFF;delay_ms(500);
//	BEED_ON;delay_ms(500);BEED_OFF;delay_ms(500);										
	while(1)
	{
	//	OS_CRITICAL_ENTER();		//�ٽ�������
		if(fire_throw==1)
	{
		 Motor_SetSpeed(&UNDER6_MLCB,throw_speed);
		if(v_flag==0)TIM_Cmd(TIM3, ENABLE);
		if(throw_flag==1){
			fire_throw=0;
			TIM_Cmd(TIM7, ENABLE);
		//  BEED_ON;delay_ms(500);BEED_OFF;delay_ms(500);
		}
	}
	//	OS_CRITICAL_EXIT();
	  delay_ms(10);
	
	}

}


/*********************************************************************************************************************
 **
	*	@brief		throw_task
	*	@describe	�������񣬼��������
	*
**********************************************************************************************************************/
//extern  int32_t Encoder_TIM2;	//ͨ��TIM2������ģʽ��׽�ļ��� 
int throw_flag=0;  //������־λ
int init_distance=0; //�����������¼

void throw_task(void *p_arg)
{
	int hole_circle=0;
	CPU_SR_ALLOC();	
	p_arg = p_arg;		
	OS_TaskSuspend((OS_TCB*)&throwTaskTCB,&throwTaskErr);
	 while(1)
	{	
  OS_CRITICAL_ENTER();		//�ٽ�������
	hole_circle=Encoder_TIM2*65536+TIM2->CNT+60678*3;
	while((Encoder_TIM2*65536+TIM2->CNT)!=hole_circle){}
	while(((( Encoder_TIM2*65536+TIM2->CNT)%60678)<=(throw_point-3))||((( Encoder_TIM2*65536+TIM2->CNT)%60678)>=(throw_point+3))){}
  throw_flag=1;
		RL1_ON;
		init_distance=Encoder_TIM2*65536+TIM2->CNT;hole_circle=0;
		BEED_ON;
		OS_CRITICAL_EXIT();
		OS_TaskSuspend((OS_TCB*)&throwTaskTCB,&throwTaskErr);
		 delay_ms(20);
	}	  		
  //
	

 
}

/*********************************************************************************************************************
 **
	*	@brief		KEY_task
	*	@describe KEY��������,����ϵͳ����
	*
**********************************************************************************************************************/
extern int sped[2000];//�ٶ����飬���ڴ�ӡʹ��
void KEY_task(void *p_arg)
{
	uint16_t i=0;
	CPU_SR_ALLOC();   //ʹ���ٽ�������ʱ��Ҫ
	p_arg = p_arg;//�����������������д���Ƿ�ֹ��������Ϊδʹ�ñ��������о���
//	OS_CRITICAL_ENTER();		//�ٽ�������
   int speed_real1=0;	
	while(1)
	{ 					
		if(TASK_RUNNING[2]++,TASK_RUNNING[2]>9)		TASK_RUNNING[2] = 0;		//�������м�¼

		switch(Key_Scan())
		{
			case KEY1:	{

				fire_throw=1;
			}
						break;
			
			case KEY2:	{

				fire_throw=0;	speed_real1=speed_real; 
				                   while(speed_real1)
                              {
																 
				                       if(speed_real1>0){speed_real1-=5;}
			                          else{speed_real1+=5;}
				                        UNDER6_MOVE(speed_real1,30);
																if(speed_real1<=6)UNDER6_MOVE(0,30);
																delay_ms(200);
			                        }	break;
					}
			case KEY3:	{

				UNDER6_MOVE(20,20);
				delay_ms(100);
				while(((Encoder_TIM2*65536 + TIM2->CNT)%60660)>1){}
					Motor_SetSpeed(&UNDER6_MLCB,0);
						break;
					}
			case KEY4:	{
                      RL1_OFF;
//					for(i=0;i<2000;i++)
//				{
////					USART_SendData(UART4,(uint8_t)(48 + (uint8_t)(sped[i]/100000)));
////			      delay_us(500);
////					sped[i] = sped[i]%100000;
////					 USART_SendData(UART4,(uint8_t)(48 + (uint8_t)(sped[i]/10000)));
////			      delay_us(500);
////					sped[i] = sped[i]%10000;
////					 USART_SendData(UART4,(uint8_t)(48 + (uint8_t)(sped[i]/1000)));
////			      delay_us(500);
////					sped[i] = sped[i]%1000;
//	          USART_SendData(UART4,(uint8_t)(48 + (uint8_t)(sped[i]/100)));
//			      delay_us(500);
//		      	sped[i] = sped[i]%100;
//		      	USART_SendData(UART4,(uint8_t)(48 + (uint8_t)(sped[i]/10)));
//		      	delay_us(500);
//		     	  sped[i] = sped[i]%10;
//			      USART_SendData(UART4,(uint8_t)(48 + (uint8_t)(sped[i])));
//			      delay_us(500);		
//            USART_SendData(UART4,10);
//		      	delay_us(500);							
//				}
						break;
					}
			case KEY5:	{

					RL1_ON;
				
			}break;	
			
			default:	break;
		}
			

		LED2(1);LED3(0);delay_ms(100);	/*����*/
		LED2(0);LED3(1);delay_ms(100);	
	}
//	OS_CRITICAL_EXIT();
}

/*********************************************************************************************************************
 **
	*	@brief		LCD_task
	*	@describe LCD��ʾ����,����ϵͳ����
	*
**********************************************************************************************************************/
uint8_t CAN_RE[8] = {0};
extern uint16_t number_DI;
uint16_t flag_exti =0;
int exti_flag15 = 0;
int exti_flag8 = 0;
int exti_flag2 = 0;
int exti_flag13 = 0;
extern int Encoder;
extern int lat_Encoder;
extern int throw_flag;
extern int v_flag;
extern int V_to_set;
extern int control_num;
extern int get_finaldis;
extern int hole_circle;//z������һȦ��������
extern int circle1;
extern int asd;
int speeD,last_speeD=0,acce,target_speed,target_acce;
void LCD_task(void *p_arg)
{
	CPU_SR_ALLOC();
	p_arg = p_arg;
	Clear_LCD();
//	OS_CRITICAL_ENTER();		//�ٽ�������
//	OS_CRITICAL_EXIT();
	while(1)
	{
		if(TASK_RUNNING[3]++,TASK_RUNNING[3]>9)		TASK_RUNNING[3] = 0;		//�������м�¼		

		speeD=Encoder*200*60/(4000*15);
		acce=(speeD-last_speeD)/0.3;
		last_speeD=speeD;
		
    LCD_Write_string("lat_Encoder",1, 1);
		LCD_Display_int(Encoder_TIM2*65536 + TIM2->CNT,15,1);
		
		LCD_Write_string("init_distance",1, 2);
		LCD_Display_int(init_distance,15,2);
		
		LCD_Write_string("hole_circle",1, 3);
		LCD_Display_int(hole_circle,15,3);
		
		LCD_Write_string("speed_real",1, 4);
		LCD_Display_int(speed_real ,15,4);
			
		LCD_Write_string("get_finaldis",1, 5);
		LCD_Display_int(get_finaldis,15,5);
		
    LCD_Write_string("circle1",1, 6);
		LCD_Display_int(circle1 ,15,6);
//		LCD_Write_string("control_num",1, 6);
//		LCD_Display_int(control_num ,15,6);
//		//LCD_Display_int(asd ,15,6);
		
		delay_ms(100);
	}
}

/*********************************************************************************************************************
 **
	*	@brief		LineWalk_task
	*	@describe �û��Զ������߳�����
	*
**********************************************************************************************************************/
extern Robocon_Handle_Struct Rb_Handle_Control;
void LineWalk_task(void *p_arg)
{
	CPU_SR_ALLOC();
	while(1)
	{
		if(Robocon_Handle_Flag == 1){
		if(TASK_RUNNING[5]++,TASK_RUNNING[5]>9)		TASK_RUNNING[5] = 0;		//�������м�¼
		
//		OS_CRITICAL_ENTER();		//�ٽ�������
		  
		/*	�û�����	*/
			
		//��������
		
		switch(Rb_Handle_Control.KEY)									
		{
				case 0x00:				{
															//��������
								}break;
				
				case 0x01:				{
															Model_Aircraft_1 = moc_speed_3m;			//������ˢ���  3m����
					                    Correspond_Receipt();
								}break;
				
				case 0x02:				{		
															Model_Aircraft_1 = moc_speed_6m;			//������ˢ���  6m����
															Correspond_Receipt();
										}break;

				case 0x03:				{
															RL9_ON;																//װ�� 
															delay_ms(1000);
															RL9_OFF;	
					                    Correspond_Receipt();
										}break;

				case 0x04:				{
														Model_Aircraft_1 = 1000;
					                  Correspond_Receipt();
										}break;

				case 0x05:				{
														Friction_Falling(moc_speed_3m,speed_3m);
										}break;			
				
				case 0x06:				{														
													  Friction_Falling(moc_speed_6m,speed_6m);
										}break;
				
				case 0x07:				{													
																moc_speed_3m += 5;
										}break;

				case 0x08:				{	
																moc_speed_3m -= 5;
										}break;

				case 0x09:				{
																moc_speed_6m += 5;
										}break;

				case 0x0A:				{													
																moc_speed_3m -= 5;
										}break;

				case 0x0B:				{
//                               Clear_Struct((char*)&tab[4],sizeof(tab));
										}break;

				case 0x0C:				{
											
										}break;

				case 0x0D:				{

										}break;

				case 0x0E:				{
									
										}break;

				case 0x0F:				{
												
										}break;		
				
				default :				{

										}break;		
			}					
		
		Rb_Handle_Control.KEY = 0;
		Robocon_Handle_Flag = 0;				//ͨ�ű�־λ���� ���յ�����ָ��ǰ ����ִ����������
	  } 
			delay_ms(20); 
	}
}

/*********************************************************************************************************************
 **
	*	@brief		HandleControl_task
	*	@describe �ֱ�����
	*
**********************************************************************************************************************/
void HandleControl_task(void *p_arg)
{
	int speed_real1=0;
	CPU_SR_ALLOC();
//	unsigned char str[] = "ACTR";
	
	OSSemCreate(	(OS_SEM		*)&Handle_Sem,				//��Ҫ�������ź���(��Ҫ������)
								(CPU_CHAR	*)"Handle_Sem",				//�ź���������
								(OS_SEM_CTR)0,									//�ź����ĳ�ʼֵ
								(OS_ERR   *)&HandleControlTaskErr);		//���󷵻ش���
	
	while(1)
	{
		OSSemPend(	(OS_SEM	*)&Handle_Sem,						//������ź���
								(OS_TICK )0,											//�ȴ�ʱ��,0Ϊ���޵ȴ�,����Ϊ�ȴ���ȷ��ʱ�ӽ�����
								(OS_OPT  )OS_OPT_PEND_BLOCKING,		//��û���뵽�ź���,�����񱻹���
								(CPU_TS *)0,											//���ʱ���
								(OS_ERR *)&HandleControlTaskErr);	//�������
		
		if(TASK_RUNNING[6]++,TASK_RUNNING[6]>9)		TASK_RUNNING[6] = 0;		//�������м�¼
		
		/*	�û�����	*/
		OS_CRITICAL_ENTER();		//�ٽ�������
		
		switch(UART_CH.KEY)
		{
 			case 0x01:				{
				                    fire_throw=1;

							}break;
			
			case 0x02:				{	fire_throw=0;	
														if(speed_real>=5||speed_real<-5)
                              {
																 speed_real1=speed_real;
				                       if(speed_real1>0){speed_real1-=5;}
			                          else{speed_real1+=5;}
				                        UNDER6_MOVE(speed_real1,30);
																delay_ms(200);
			                        }	
                             else {UNDER6_MOVE(0,10);break;	}													
									}

			case 0x03:				{
                    	      TIM_Cmd(TIM7, ENABLE);
									}break;

			case 0x04:				{
				                    target_acce+=5;
									}break;

			case 0x05:				{
				                    target_acce-=5;               
									}break;			
			
			case 0x06:				{
										        UNDER6_MOVE(target_speed,target_acce);
									}break;
			
			case 0x07:				{target_speed+=5;
                     
									}break;

			case 0x08:				{	target_speed-=5;
			                	
									}break;

			case 0x09:				{
							
									} break;

			case 0x0A:				{															
													
									}break;

			case 0x0B:				{
											
									}break;

			case 0x0C:				{
											
									}break;

			case 0x0D:				{
																							
									}break;

			case 0x0E:				{
												
									}break;

			case 0x0F:				{
																					
									}break;		
			case 0x10:				{
																						
									}break;
			case 0x11:				{
				                 																		
									}break;
			case 0x12:				{
																					
									}break;
			case 0x13:				{
											
									}break;
			case 0x14:				{
																
									}break;
			
			default :				{

									}break;
		}
		
		OS_CRITICAL_EXIT();
		
		delay_ms(100);
	}
}

/*********************************************************************************************************************
 **
	*	@brief		OffsetControl_task	---�������ģ��
	*	@describe �û��Զ������߳�����
	*
**********************************************************************************************************************/
void OffsetControl_task(void *p_arg)
{
	CPU_SR_ALLOC();
	
	while(1)
	{
		if(TASK_RUNNING[7]++,TASK_RUNNING[7]>9)		TASK_RUNNING[7] = 0;		//�������м�¼
		OS_CRITICAL_ENTER();		//�ٽ�������
		
		/*	�û�����	*/	
		if(flag_off){
			
			RL1_OFF;			
//			while(((Encoder_TIM2*65536+TIM2->CNT)<= fasheplace_345)||(TIM2->CNT >=3000));		//���޸�	
//			while(!((TIM2->CNT)>= Launchplace)||(TIM2->CNT >=0));
			while(place = TIM2->CNT,(place<= Launchplace)||(place >=3000));
			RL2_ON;		
			flag_off = 0;
 
		}			
		OS_CRITICAL_EXIT();			

		OS_TaskSuspend((OS_TCB*)&OffsetControlTaskTCB,&OffsetControlTaskErr);		//��������	�ȴ�����ģ����л���			
		
//		delay_ms(100);
	}
}








