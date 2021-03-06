/************************************************************************************************************************
*	调试辅助
	*	系统			UCOSIII操作系统
	*	主核(CPU)		cortex-m3
	*	主控(MCU)		STM31F103ZET6	
	*	FLASH			512k
	*	CAN资源			只具有CAN1模块


**	机器参数	**	**	硬件资源分配	**	**	软件资源分配	**	**	调试任务分配	**
/**
  *@brief	机器参数
  */
  @	底盘参数：			四个麦克纳姆轮，半径45mm
	
  @	底盘电机：			减速比		速度上限		加速度上限
    爬杆电机：			减速比		速度上限		加速度上限
	伸缩电机：			减速比		速度上限		加速度上限
	升降电机：			减速比		速度上限		加速度上限
  
  @	编码器：			1000线		4000个跳变沿
  
  @	航模电机：			油门最低点1000	油门最高点2000	定时器周期20000	即控制范围为10%~20%
						初始状态为低油门——直接启动
						初始状态为高油门——重新设置油门范围后直接启动
						
  @	舵机：				
  
  @	摄像头：
  
  @	CCD：
  
  @	灯板：
  
  @	激光雷达：
  
  @	激光：
  
  
  
/**
  *@brief	硬件资源分配			
  */
  @定时器资源分配：
	1、		航模电机：		TIM1	CH1~CH4		PWM波模式
	2、		编码器：		TIM2	CH1\CH2		编码器模式
	3、		舵机：			TIM3	CH1~CH2		PWM波模式
	4、		编码器2：		TIM4	CH1\CH2		编码器模式
	5、		陀螺仪采集周期	TIM5	5ms			定时器模式
	
  @CAN标准ID分配
	底盘电机：	
	爬杆电机：
	伸缩电机：
	升降电机
  
  @串口分配：
	USART1：	
	USART2：	
	USART3：
	UART4		重定向为printf 的输出口
	UART5
	
  @中断分配：		中断向量组	2
  高频率->低频率	响应需要优先级高->低		
	0	0			TIM5定时器中断				触发陀螺仪采集开始
	0	1			TIM4定时中断(编码器模式)	编码器计数溢出控制
	0	2			TIM2定时中断(编码器模式)	编码器计数溢出控制
	
	0	3			CAN——RX中断					控制器参数反馈
	1	0			CAN——error中断
	
	1	1			USART3						手柄中断
	
	
 
  
  
/**
  *@brief	软件资源分配		调试任务分配
  */
  @优先级分配		Systick优先级分配	(Systick任务会触发中断,优先级配置高则时间计时更准确,优先级配置低则降低对其他高频任务)
										(的干扰,暂时配置优先级为1(中断级任务优先级默认为0,故不能使用),因为延时函数为截取滴答)
										(定时器的计数进行计算,延时精准,可以将Systick优先级降低，而不影响精度)
										
					任务优先级请根据任务执行频率和重要性进行配置,如：显示任务为低优先级,巡线任务为中优先级,传感器反馈信息为高优先级
					
  @delay_ms / delay_us	同样会触发任务切换,若需关闭,可以在delay.h中修改,或加临界段代码(	CPU_SR_ALLOC();		/*申请栈变量
																						OS_CRITICAL_ENTER();/*进入临界区
																						/*
																							用户需要保护的代码段  
																												  */
																						OS_CRITICAL_EXIT();)/*退出临界区
																						

/******************************************用户调试参数记录**************************************************/		
/******************************************用户调试参数记录**************************************************/	
/******************************************用户调试参数记录**************************************************/	
/**
  *@brief：若PID参数
  */
  
  
  
  
  
  
  
  
  