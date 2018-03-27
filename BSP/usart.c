/*************************************************************************************
*	@file			usart.c
* @author	 	yun
*	@supplement		
*	@version 	V1.0
*	@date			2016/5/27
* @brief		NONE
*************************************************************************************/

#include "usart.h"

/**
	*@brief void Usart_Config()	串口配置
	*@param	USART_TypeDef* USARTx		需要初始化的串口
					uint32_t bound					串口波特率
	*@supplement		如果串口需要使能中断函数,还需配置中断优先级
	*@retval NONE
	*/
void Usart_Config(USART_TypeDef* USARTx, uint32_t bound)
{
	USART_InitTypeDef USART_InitStruct;
	
	if(USARTx==USART1){	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
		
		GPIO_Config(GPIOA, GPIO_Pin_9, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
		GPIO_Config(GPIOA, GPIO_Pin_10, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	}
	if(USARTx==USART2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		
		GPIO_Config(GPIOA, GPIO_Pin_2, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
		GPIO_Config(GPIOA, GPIO_Pin_3, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	}
	if(USARTx==USART3){	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		
		GPIO_Config(GPIOB, GPIO_Pin_10, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
		GPIO_Config(GPIOB, GPIO_Pin_11, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);

	}
	
	USART_DeInit(USARTx);						//复位串口,一般情况下,单片机初始化配置时,都需要复位外设
	USART_InitStruct.USART_BaudRate = bound;		//波特率设置
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;		//收发模式(全双工模式)
	USART_InitStruct.USART_Parity     =   USART_Parity_No;			  //关闭奇偶校验位
	USART_InitStruct.USART_StopBits   =   USART_StopBits_1;		    //一个停止位
	USART_InitStruct.USART_WordLength =   USART_WordLength_8b;		//8位数据
	USART_Init(USARTx,&USART_InitStruct);
	
	USART_ITConfig(USARTx,USART_IT_RXNE,ENABLE);		//打开串口接收中断
	USART_Cmd(USARTx,ENABLE);												//使能串口
}

/**
	*	@brief	fputc
	*	@param	NONE
	*	@retval NONE
*/
int fputc(int ch,FILE *f)
{
	/*将数据发到相应的串口*/
	USART_SendData(UART4,(unsigned char)ch);
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);
	return(ch);
}


/***************************************************用户自定义模块**********************************************/
/***************************************************用户自定义模块**********************************************/
/***************************************************用户自定义模块**********************************************/
/**	
	*	@brief	A车串口初始化
	*	@supplement	A车串口数据
								USART1:				TX1(PA9)		RX1(PA10)				Default			
								UART2	:				TX2(PA2)		RX2(PA3)				Default
								UART3	:				TX3(PB10)		RX3(PB11)				Default
								UART4	:				TX4(PA0)		RX4(PA1)					Default
								USART6:				TX6(PC6)		RX6(PC7)				Default
中断说明	：	UART5	1 0 					USART2	1 1 					USART3	 1 2 				UART4		1 3 
	*	@param	NONE
	*	@retval	NONE
	*/
#define		USART1_OPEN	OFF
#define		USART2_OPEN	OFF
#define		USART3_OPEN	ON
#define		UART4_OPEN	ON
#define		USART6_OPEN	OFF
void A_Usart_Config(void)
{
	USART_InitTypeDef USART_InitStruct;
	
#if			USART1_OPEN
	/********************		UART1_INIT(UART5)		***********************
	*	baud	115200					*
	*	开中断								*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	GPIO_Config(GPIOA, GPIO_Pin_9, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOA, GPIO_Pin_10, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	
	USART_DeInit(UART5);						//复位串口
	USART_InitStruct.USART_BaudRate = 115200;		//波特率设置
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;		//收发模式(全双工模式)
	USART_InitStruct.USART_Parity     =   USART_Parity_No;			  //关闭奇偶校验位
	USART_InitStruct.USART_StopBits   =   USART_StopBits_1;		    //一个停止位
	USART_InitStruct.USART_WordLength =   USART_WordLength_8b;		//8位数据
	USART_Init(UART5,&USART_InitStruct);
	
	NVIC_Config(UART5_IRQn, 1, 0);
	
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);		//打开串口接收中断
	USART_Cmd(UART5,ENABLE);												//使能串口
#endif
	
	
	
#if			USART2_OPEN
	/********************		UART2_INIT      		***********************
	*	baud	115200					*
	*	开中断								*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	GPIO_Config(GPIOA, GPIO_Pin_2, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOA, GPIO_Pin_3, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	
	USART_DeInit(USART2);						//复位串口
	USART_InitStruct.USART_BaudRate = 115200;		//波特率设置
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;		//收发模式(全双工模式)
	USART_InitStruct.USART_Parity     =   USART_Parity_No;			  //关闭奇偶校验位
	USART_InitStruct.USART_StopBits   =   USART_StopBits_1;		    //一个停止位
	USART_InitStruct.USART_WordLength =   USART_WordLength_8b;		//8位数据
	USART_Init(USART2,&USART_InitStruct);
	
	NVIC_Config(USART2_IRQn, 1, 6);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);		//打开串口接收中断
	USART_Cmd(USART2,ENABLE);												//使能串口
#endif
	
	
#if			USART3_OPEN
	/********************		UART3_INIT      		***********************
	*	baud	115200					*
	*	开中断								*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	
	GPIO_Config(GPIOB, GPIO_Pin_10, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOB, GPIO_Pin_11, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	
	USART_DeInit(USART3);						//复位串口
	USART_InitStruct.USART_BaudRate = 115200;		//波特率设置
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;		//收发模式(全双工模式)
	USART_InitStruct.USART_Parity     =   USART_Parity_No;			  //关闭奇偶校验位
	USART_InitStruct.USART_StopBits   =   USART_StopBits_1;		    //一个停止位
	USART_InitStruct.USART_WordLength =   USART_WordLength_8b;		//8位数据
	USART_Init(USART3,&USART_InitStruct);
	
	NVIC_Config(USART3_IRQn, 1, 1);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);		//打开串口接收中断
	USART_Cmd(USART3,ENABLE);												//使能串口
#endif
	
	
#if			UART4_OPEN
	/********************		UART4_INIT      		***********************
	*	baud	115200					*
	*	开中断								*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);
	
	GPIO_Config(GPIOC, GPIO_Pin_10, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOC, GPIO_Pin_11, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	
	USART_DeInit(UART4);						//复位串口
	USART_InitStruct.USART_BaudRate = 115200;		//波特率设置
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;		//收发模式(全双工模式)
	USART_InitStruct.USART_Parity     =   USART_Parity_No;			  //关闭奇偶校验位
	USART_InitStruct.USART_StopBits   =   USART_StopBits_1;		    //一个停止位
	USART_InitStruct.USART_WordLength =   USART_WordLength_8b;		//8位数据
	USART_Init(UART4,&USART_InitStruct);
	
	NVIC_Config(UART4_IRQn, 1, 3);
	
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);		//打开串口接收中断
	USART_Cmd(UART4,ENABLE);												//使能串口
#endif
}



extern Robocon_Handle_Struct Rb_Handle_Control;
extern uint8_t Robocon_Handle_Flag;
/*通信回执*/
void Correspond_Receipt()
{
	/*帧头*/
  USART_SendData(UART4,0XFF);
	delay_us(100);
	USART_SendData(UART4,0XFE);
	delay_us(100);
	USART_SendData(UART4,0XFD);
	delay_us(100);
	USART_SendData(UART4,0XF2);
	delay_us(100);
	
	/*按键*/
	USART_SendData(UART4,Rb_Handle_Control.KEY);
	delay_us(100);
	
	/*坐标X交流*/
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_X._u8_data[0]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_X._u8_data[1]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_X._u8_data[2]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_X._u8_data[3]);
	delay_us(100);
	
	/*坐标Y交流*/
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_Y._u8_data[0]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_Y._u8_data[1]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_Y._u8_data[2]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_Y._u8_data[3]);
	delay_us(100);
	
	/*坐标W交流*/
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_W._u8_data[0]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_W._u8_data[1]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_W._u8_data[2]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_W._u8_data[3]);
	delay_us(100);
	
	/*摩擦速度*/
	USART_SendData(UART4,Rb_Handle_Control.Rub_Speed._u8_data[0]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Rub_Speed._u8_data[1]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Rub_Speed._u8_data[2]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Rub_Speed._u8_data[3]);
	delay_us(100);
	
	/*仿生释放位置*/
	USART_SendData(UART4,Rb_Handle_Control.Simulate_Position._u8_data[0]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Simulate_Position._u8_data[1]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Simulate_Position._u8_data[2]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Simulate_Position._u8_data[3]);
	delay_us(100);
	
	/*仿生仰角位置*/
	USART_SendData(UART4,Rb_Handle_Control.Elevation_Position._u8_data[0]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Elevation_Position._u8_data[1]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Elevation_Position._u8_data[2]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Elevation_Position._u8_data[3]);
	delay_us(100);
	
	/*回执信息*/
	USART_SendData(UART4,0X01);
	delay_us(100);
	
	/*帧尾*/
	USART_SendData(UART4,0X02);
	delay_us(100);
	
}

/*通信请求*/
void Communication_request()
{
	/*帧头*/
  USART_SendData(UART4,0XFF);
	delay_us(100);
	USART_SendData(UART4,0XFE);
	delay_us(100);
	USART_SendData(UART4,0XFD);
	delay_us(100);
	USART_SendData(UART4,0XF2);
	delay_us(100);
	
	/*按键*/
	USART_SendData(UART4,Rb_Handle_Control.KEY);
	delay_us(100);
	
	/*坐标X交流*/
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_X._u8_data[0]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_X._u8_data[1]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_X._u8_data[2]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_X._u8_data[3]);
	delay_us(100);
	
	/*坐标Y交流*/
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_Y._u8_data[0]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_Y._u8_data[1]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_Y._u8_data[2]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_Y._u8_data[3]);
	delay_us(100);
	
	/*坐标W交流*/
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_W._u8_data[0]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_W._u8_data[1]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_W._u8_data[2]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Locat_Coord_W._u8_data[3]);
	delay_us(100);
	
	/*摩擦速度*/
	USART_SendData(UART4,Rb_Handle_Control.Rub_Speed._u8_data[0]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Rub_Speed._u8_data[1]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Rub_Speed._u8_data[2]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Rub_Speed._u8_data[3]);
	delay_us(100);
	
	/*仿生释放位置*/
	USART_SendData(UART4,Rb_Handle_Control.Simulate_Position._u8_data[0]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Simulate_Position._u8_data[1]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Simulate_Position._u8_data[2]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Simulate_Position._u8_data[3]);
	delay_us(100);
	
	/*仿生仰角位置*/
	USART_SendData(UART4,Rb_Handle_Control.Elevation_Position._u8_data[0]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Elevation_Position._u8_data[1]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Elevation_Position._u8_data[2]);
	delay_us(100);
	USART_SendData(UART4,Rb_Handle_Control.Elevation_Position._u8_data[3]);
	delay_us(100);
	
	/*回执信息*/
	USART_SendData(UART4,0X00);
	delay_us(100);
	
	/*帧尾*/
	USART_SendData(UART4,0X02);
	delay_us(100);
}



