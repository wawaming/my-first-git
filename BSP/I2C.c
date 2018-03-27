/*************************************************************************************
*	@file			DMA.c
* @author	 	yun
*	@version 	V1.0
*	@date			2016/3/27
* @brief		NONE
*************************************************************************************/
#include "I2C.h"


/**
	*	@brief	I2C初始化函数
	*/
void IIC_Init(void)
{
	GPIO_Config(IIC_GPIOX, IIC_SCL_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);	//SCL引脚，推挽输出
	GPIO_Config(IIC_GPIOX, IIC_SDA_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);	//SDA引脚，开漏输出
}



/**
	*	@brief	I2C的空闲状态
	*	@param	NONE
	*	@retval	NONE
	*	@supplement	数据线和时钟线都处于高电平状态
	*/
void IIC_leisure(void)
{
	IIC_SDL_OUT_SET();
	IIC_SDA_OUT(1);
	IIC_Delay;
	IIC_SCL_OUT(1);
	IIC_Delay;
}

/**
	*	@brief	IIC开始信号
	*	@param	NONE
	*	@retval	NONE
	*	@supplement	SCL为高电平时,SDA从高电平跳变为低电平(即下降沿)
	*/
void IIC_Star(void)
{
	IIC_SDL_OUT_SET();
	IIC_SDA_OUT(1);
	IIC_SCL_OUT(1);
	IIC_Delay;
	IIC_SDA_OUT(0);//下降沿
	IIC_Delay;
	IIC_SCL_OUT(0);//钳住SCL,准备发送数据
}

/**
	*	@brief	IIC停止信号
	*	@param	NONE
	*	@retval	NONE
	* @supplement	SCL为高电平时,SDA从低电平跳变为高电平(即上升沿)
	*/
void IIC_Stop(void)
{
	IIC_SDL_OUT_SET();
	IIC_SCL_OUT(0);
	IIC_SDA_OUT(0);
	IIC_Delay;
	IIC_SCL_OUT(1);
//	IIC_Delay;
	IIC_SDA_OUT(1);//上升沿
	IIC_Delay;
}

/**
	*	@brief		IIC发送应答
	*	@param		ack		0,	应答
										1,	非应答
	*	@retval
	*/
void IIC_SendAck(uint8_t ack)
{
	
	IIC_SCL_OUT(0);
	IIC_SDL_OUT_SET();
//	IIC_Delay;
	IIC_SDA_OUT(ack);
	IIC_Delay;
	IIC_SCL_OUT(1);
	IIC_Delay;
	IIC_SCL_OUT(0);
//	IIC_Delay;
}

/**
	*	@brief		IIC接收应答信号
	*	@param
	*	@retval		u8类型	0，	应答成功
											1，	应答失败
	*/
u8 IIC_RiceveAck(void)
{
	uint8_t i=0;
	IIC_SDL_IN_SET();
	IIC_SDA_OUT(1);
	IIC_Delay;
	IIC_SCL_OUT(1);
	IIC_Delay;
	while(IIC_SDA_IN())
	{
			i++;
//			IIC_Delay;
			if(i>=250)		
			{
				IIC_Stop();
				return 1;
			}
	}
	IIC_SCL_OUT(0);
	//IIC_Delay;
	return 0;
	
}

/**
	*	@brief		IIC发送一个字节
	*	@param		data	需要发送的数据
	*	@retval
	*/
void IIC_SendByte(uint8_t data)
{
	uint8_t i=0,IIC_date;
	IIC_SDL_OUT_SET();
	IIC_SCL_OUT(0);
	for(;i<8;i++)
	{
			IIC_date=(data&0x80)>>7;			
			IIC_SDA_OUT(IIC_date);
			IIC_Delay;
			IIC_SCL_OUT(1);
			IIC_Delay;
			data<<=1;
			IIC_SCL_OUT(0);
			IIC_Delay;
	}
//	IIC_SCL_OUT(0);
}

/**
	*	@brief		接收一个字节
	*	@param		ack 0,应答成功,   1,应答不成功			
	*	@retval		u8类型	data	返回接收到的字节
	*/
u8 IIC_RiceveByte(uint8_t ack)
{
		uint8_t i=0,data=0;
		IIC_SDL_IN_SET();
		IIC_SDA_OUT(1);
		for(;i<8;i++)
		{
				IIC_SCL_OUT(0);
				IIC_Delay;
				IIC_SCL_OUT(1);
				IIC_Delay;
				data<<=1;
				if(IIC_SDA_IN())
				data++;	
				IIC_Delay;
		}
		IIC_SendAck(ack);
		return data;
}

/**
	*	@brief		向目标地址写一个8位数据
	*	@param
	*	@retval		
	*/
void IIC_SendData(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data)
{
	IIC_Star();
	IIC_SendByte(SlaveAddress);					//发送设备ID地址，最低位为0为写信号
	IIC_RiceveAck();
	IIC_SendByte(REG_Address);					//发送需要操作的寄存器地址
	IIC_RiceveAck();
	IIC_SendByte(REG_data);							//发送需要写入的数据
	IIC_RiceveAck();
	IIC_Stop();
}

/**
	*	@brief		向目标地址接收一个数据
	*	@param
	*	@retval		u8类型		data		接收到的数据
	*/
u8 IIC_RiceveData(uint8_t SlaveAddress, uint8_t REG_Address)
{
	uint8_t	REG_Data;
	IIC_Star();
	IIC_SendByte(SlaveAddress);				//发送设备ID地址，最低位为0为写信号
	IIC_RiceveAck();
	IIC_SendByte(REG_Address);				//发送需要操作的寄存器地址
	IIC_RiceveAck();
	IIC_Star();
	IIC_SendByte(SlaveAddress+1);			//发送设备ID地址，最低位为1为读信号
	IIC_RiceveAck();
//#if UT_USE			
	delay_us(80);		//超声波必须延时50us以上才能准确的收到数据
//#endif
	//while(!IIC_SCL_IN);
	REG_Data = IIC_RiceveByte(1);
	IIC_Stop();
	return REG_Data;
}



/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/








