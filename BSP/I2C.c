/*************************************************************************************
*	@file			DMA.c
* @author	 	yun
*	@version 	V1.0
*	@date			2016/3/27
* @brief		NONE
*************************************************************************************/
#include "I2C.h"


/**
	*	@brief	I2C��ʼ������
	*/
void IIC_Init(void)
{
	GPIO_Config(IIC_GPIOX, IIC_SCL_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);	//SCL���ţ��������
	GPIO_Config(IIC_GPIOX, IIC_SDA_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);	//SDA���ţ���©���
}



/**
	*	@brief	I2C�Ŀ���״̬
	*	@param	NONE
	*	@retval	NONE
	*	@supplement	�����ߺ�ʱ���߶����ڸߵ�ƽ״̬
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
	*	@brief	IIC��ʼ�ź�
	*	@param	NONE
	*	@retval	NONE
	*	@supplement	SCLΪ�ߵ�ƽʱ,SDA�Ӹߵ�ƽ����Ϊ�͵�ƽ(���½���)
	*/
void IIC_Star(void)
{
	IIC_SDL_OUT_SET();
	IIC_SDA_OUT(1);
	IIC_SCL_OUT(1);
	IIC_Delay;
	IIC_SDA_OUT(0);//�½���
	IIC_Delay;
	IIC_SCL_OUT(0);//ǯסSCL,׼����������
}

/**
	*	@brief	IICֹͣ�ź�
	*	@param	NONE
	*	@retval	NONE
	* @supplement	SCLΪ�ߵ�ƽʱ,SDA�ӵ͵�ƽ����Ϊ�ߵ�ƽ(��������)
	*/
void IIC_Stop(void)
{
	IIC_SDL_OUT_SET();
	IIC_SCL_OUT(0);
	IIC_SDA_OUT(0);
	IIC_Delay;
	IIC_SCL_OUT(1);
//	IIC_Delay;
	IIC_SDA_OUT(1);//������
	IIC_Delay;
}

/**
	*	@brief		IIC����Ӧ��
	*	@param		ack		0,	Ӧ��
										1,	��Ӧ��
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
	*	@brief		IIC����Ӧ���ź�
	*	@param
	*	@retval		u8����	0��	Ӧ��ɹ�
											1��	Ӧ��ʧ��
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
	*	@brief		IIC����һ���ֽ�
	*	@param		data	��Ҫ���͵�����
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
	*	@brief		����һ���ֽ�
	*	@param		ack 0,Ӧ��ɹ�,   1,Ӧ�𲻳ɹ�			
	*	@retval		u8����	data	���ؽ��յ����ֽ�
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
	*	@brief		��Ŀ���ַдһ��8λ����
	*	@param
	*	@retval		
	*/
void IIC_SendData(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data)
{
	IIC_Star();
	IIC_SendByte(SlaveAddress);					//�����豸ID��ַ�����λΪ0Ϊд�ź�
	IIC_RiceveAck();
	IIC_SendByte(REG_Address);					//������Ҫ�����ļĴ�����ַ
	IIC_RiceveAck();
	IIC_SendByte(REG_data);							//������Ҫд�������
	IIC_RiceveAck();
	IIC_Stop();
}

/**
	*	@brief		��Ŀ���ַ����һ������
	*	@param
	*	@retval		u8����		data		���յ�������
	*/
u8 IIC_RiceveData(uint8_t SlaveAddress, uint8_t REG_Address)
{
	uint8_t	REG_Data;
	IIC_Star();
	IIC_SendByte(SlaveAddress);				//�����豸ID��ַ�����λΪ0Ϊд�ź�
	IIC_RiceveAck();
	IIC_SendByte(REG_Address);				//������Ҫ�����ļĴ�����ַ
	IIC_RiceveAck();
	IIC_Star();
	IIC_SendByte(SlaveAddress+1);			//�����豸ID��ַ�����λΪ1Ϊ���ź�
	IIC_RiceveAck();
//#if UT_USE			
	delay_us(80);		//������������ʱ50us���ϲ���׼ȷ���յ�����
//#endif
	//while(!IIC_SCL_IN);
	REG_Data = IIC_RiceveByte(1);
	IIC_Stop();
	return REG_Data;
}



/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/








