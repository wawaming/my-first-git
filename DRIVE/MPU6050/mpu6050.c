/*************************************************************************************
*	@file			mpu6050.c
*	@supplement	1、DMP开启的模式下FIFO的输出频率是200hz，官方为100hz
							2、若要读取数据，需要建立相应的数据包
							mpu_dmp_get_data(&pitch,&roll,&yaw) 也能使用此函数获得角度(四元数算法)
* @author	 	yun
*	@version 	V1.2
*	@date			2016/8/4
* @brief		NONE
*************************************************************************************/
#include "mpu6050.h"



static signed char gyro_vector[9] = 			{1, 0, 0,						//陀螺仪方向设置
                                           0, 1, 0,
                                           0, 0, 1};																	 
									
#if	MPU_DMP_STAR ==	MPU_OFF																					 
int16_t pitch_last=0,roll_last=0,yaw_last=0;			
#endif																					 
/**
	*	@brief	mpu6050初始化
	*	@param	void
	*	@retval	0：初始化成功；1：初始化失败
	*/
u8 MPU_Init(void)
{ 
#if	MPU_DMP_STAR ==	MPU_OFF
	unsigned	char pitch_gah,pitch_gal;
	unsigned  char roll_gah,roll_gal;
	unsigned  char yaw_gah,yaw_gal;
#endif
	/*-----设置AD0的引脚的接入电平-----*/
//	GPIO_Config(GPIOA, GPIO_Pin_8, GPIO_Mode_Out_PP);
//	#if	MPU6050_SLAVEADRASS==0x68
//	GPIO_ResetBits(GPIOA, GPIO_Pin_8);									//输入电平0 ，即地址为0x68
//	#else
//	GPIO_SetBits(GPIOA, GPIO_Pin_8);									//输入电平1 ，即地址为0x69
//	#endif
	
	/*----复位mpu6050-----*/
	IIC_leisure();
	delay_ms(100);
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_PWR_MGMT1_REG, 0x80);	//重启内部寄存器到默认值，完成后自动清零
	delay_ms(100);	
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_PWR_MGMT1_REG, 0x00);	//唤醒mpu6050
	delay_ms(100);
	/*----设置mpu6050相关参数----*/
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_GYRO_CFG_REG, 0x03<<3);	//设置陀螺仪精度+-2000度/秒
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_ACCEL_CFG_REG, 0x00<<3);	//设置加速度计精度+-2g
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_SAMPLE_RATE_REG, (1000/MPU_RATE-1));		//设置采样频率
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_CFG_REG, DLPF_CFG);										//设置低通滤波器。近似值为采样频率的一半
	
	/*----设置相关模式-----*/
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_INT_EN_REG, 0);					//设置中断寄存器，关闭所有中断
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_USER_CTRL_REG, 0);				//I2C主模式关闭，失能FIFO缓冲区，不允许读写
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_FIFO_EN_REG, 0);					//失能FIFO，不允许加载
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_INTBP_CFG_REG, 0x08);		//INT引脚低电平有效

	/*----检查初始化是否成功-----*/
	if(IIC_RiceveData(MPU6050_SLAVEADRASS<<1, MPU_DEVICE_ID_REG) != (MPU6050_SLAVEADRASS))
		return	10;
	else
	{
		IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_PWR_MGMT1_REG, 0x01);	//设置mpu6050参考频率为PLL_X轴
		IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_PWR_MGMT2_REG, 0x00);	//打开陀螺仪和加速度计
	}
	
	/*----DMP是否开启-----*/
	#if	MPU_DMP_STAR ==	MPU_ON	
			if(!mpu_init()){
					if(mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL))			return 1;					//设置需要的传感器
					if(mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL))	return 2;					//设置FIFO	
					if(mpu_set_sample_rate(DEFAULT_MPU_HZ))							return 3;					//设置采样率
					if(dmp_load_motion_driver_firmware())								return 4;					//加载dmp固件
					if(dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_vector)))return 5;//设置陀螺仪方向
					if(dmp_enable_feature(DMP_MODE))										return 6;					//设置DMP功能
					if(dmp_set_fifo_rate(DEFAULT_MPU_HZ))								return 7;					//设置DMP输出速率
					if(run_self_test())																	return 8;					//设置自检
					if(mpu_set_dmp_state(1))														return 9;					//使能DMP
			}
			else return 11;
	#endif	
	delay_ms(800);
	delay_ms(800);
			
	/*	读取陀螺仪的数值	*/
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 1, &pitch_gah);
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTL_REG, 1, &pitch_gal);
//	
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_YOUTH_REG, 1, &roll_gah);
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_YOUTL_REG, 1, &roll_gal);
#if	MPU_DMP_STAR ==	MPU_OFF
	MPU_Read_Len(MPU_ADDR, MPU_GYRO_ZOUTH_REG, 1, &yaw_gah);
	MPU_Read_Len(MPU_ADDR, MPU_GYRO_ZOUTL_REG, 1, &yaw_gal);
	
	/*	采集的数值差	*/
//	pitch_last = pitch_gah*256 + pitch_gal;			
//	roll_last	 = roll_gah*256 + roll_gal;
	yaw_last	 = yaw_gah*256 + yaw_gal;
#endif
	return	0;
}


/**
	*	@brief	读取DMP_FIFO包
	*	@param	pta:	需要存储的地址,函数中的操作均以字节为单位,且有一定存储数据
									若需要按照特定位置储存,调用DMP库函数进行处理：
									int dmp_read_fifo(short *gyro, short *accel, long *quat,
																		unsigned long *timestamp, short *sensors, unsigned char *more)
	*	@retval	0:	读取成功，其他：读取失败
	*/
int MPU_FIFO_RADE(short *gyro, short *accel, long *quat, uint8_t all_data)
{
	uint8_t more;
//	uint8_t error;
	uint8_t PACK_buff[32];
	/*----通过IIC,向FIFO读取相应的字节数	即数据包----*/
	if (mpu_read_fifo_stream(PACK_LENGTH, PACK_buff, &more))
			return 1;
	else{
		/*	读取四元数数据	*/
	quat[0] = ((long)PACK_buff[0] << 24) | ((long)PACK_buff[1] << 16) |
						((long)PACK_buff[2] << 8) | PACK_buff[3];
	quat[1] = ((long)PACK_buff[4] << 24) | ((long)PACK_buff[5] << 16) |
						((long)PACK_buff[6] << 8) | PACK_buff[7];
	quat[2] = ((long)PACK_buff[8] << 24) | ((long)PACK_buff[9] << 16) |
						((long)PACK_buff[10] << 8) | PACK_buff[11];
	quat[3] = ((long)PACK_buff[12] << 24) | ((long)PACK_buff[13] << 16) |
						((long)PACK_buff[14] << 8) | PACK_buff[15];}
	if(1){
	/* We can detect a corrupted FIFO by monitoring the quaternion data and
         * ensuring that the magnitude is always normalized to one. This
         * shouldn't happen in normal operation, but if an I2C error occurs,
         * the FIFO reads might become misaligned.
         *
         * Let's start by scaling down the quaternion data to avoid long long
         * math.*/
#ifdef	FIFO_CORRUPTION_CHECK
				int32_t quat_q14[4], quat_mag_sq;
				quat_q14[0] = quat[0] >> 16;
        quat_q14[1] = quat[1] >> 16;
        quat_q14[2] = quat[2] >> 16;
        quat_q14[3] = quat[3] >> 16;
				quat_mag_sq = quat_q14[0] * quat_q14[0] + quat_q14[1] * quat_q14[1] +
											quat_q14[2] * quat_q14[2] + quat_q14[3] * quat_q14[3];
				/* Quaternion is outside of the acceptable threshold. */
				if ((quat_mag_sq < QUAT_MAG_SQ_MIN) ||(quat_mag_sq > QUAT_MAG_SQ_MAX)){
						mpu_reset_fifo();
						return 2;
				}
#endif
			}
	
		/*	读取FIFO中的groy and accel	*/				
		accel[0] = ((short)PACK_buff[16] << 8) | PACK_buff[17];
		accel[1] = ((short)PACK_buff[18] << 8) | PACK_buff[19];
		accel[2] = ((short)PACK_buff[20] << 8) | PACK_buff[21];
		gyro[0] = ((short)PACK_buff[22] << 8) | PACK_buff[23];
		gyro[1] = ((short)PACK_buff[24] << 8) | PACK_buff[25];
		gyro[2] = ((short)PACK_buff[26] << 8) | PACK_buff[27];
	return 0;
}



/**
	*	@brief	四元数处理
	*	@param	pitch		俯仰角存放地址
						roll		横滚角存放地址
						yaw			偏航角存放地址
						quat		四元数地址
	*	@retval	NONE
	*/
void QUAT_Convert(float *pitch,float *roll,float *yaw,int32_t *quat)
{
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	q0 = quat[0] / Q30;	//q30格式转换为浮点数
	q1 = quat[1] / Q30;	
	q2 = quat[2] / Q30;
	q3 = quat[3] / Q30; 
	
	*pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	
	*roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	
	*yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	
}


/**
	*	@brief	MPU采集角度，不通过DMP
	*	@param	*pitch	俯仰角储存位置	*roll	翻滚角储存位置	*yaw	偏航角储存位置
	*	@retval	NONE
	*/
#if	MPU_DMP_STAR ==	MPU_OFF
void MPU_Gather(float *pitch,	float *roll, float *yaw)
{
	int16_t pitch_gather,roll_gather,yaw_gather;
	float pitch_data,roll_data,yaw_data;
	unsigned	char pitch_gah,pitch_gal;
	unsigned  char roll_gah,roll_gal;
	unsigned  char yaw_gah,yaw_gal;
	
//	pitch_data = *pitch;
//	roll_data = *roll;
	yaw_data = *yaw;
	/*	读取陀螺仪的数值	*/
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 1, &pitch_gah);
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTL_REG, 1, &pitch_gal);
//	
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_YOUTH_REG, 1, &roll_gah);
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_YOUTL_REG, 1, &roll_gal);
//	
	MPU_Read_Len(MPU_ADDR, MPU_GYRO_ZOUTH_REG, 1, &yaw_gah);
	MPU_Read_Len(MPU_ADDR, MPU_GYRO_ZOUTL_REG, 1, &yaw_gal);
	
	/*	采集的数值差	*/
//	pitch_gather = pitch_gah*256 + pitch_gal - pitch_last;			
//	roll_gather	 = roll_gah*256 + roll_gal - roll_last;
	yaw_gather	 = yaw_gah*256 + yaw_gal -  yaw_last;
	
	/*	开始计算	*/
//	if(ABS_int(pitch_gather)>8){
//		pitch_data -= (float)((float)pitch_gather/GROY_UNIT*0.005);
//		(*pitch) = pitch_data;
////		pitch_last = pitch_gah*256 + pitch_gal;
//	}
//	
//	if(ABS_int(roll_gather)>8){
//		(*roll) -= (float)roll_gather/GROY_UNIT*0.005;
////		roll_last = roll_gah*256 + roll_gal;
//	}
	
	if(ABS_int(yaw_gather)>8){
		(*yaw) -= (float)yaw_gather/GROY_UNIT*0.005;
//		yaw_last = yaw_gah*256 + yaw_gal; 
	}
}
#endif


/**
	*	@brief	MPU通过IIC写任意数量的字节
	*	@param	addr:		IIC地址
						reg：		操作的寄存器地址
						len：		需要写入的字节数目
						buf：		需要写入的数据
	*	@retval	0：写入成功，1：写入失败
	*/
uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	uint8_t i=0;
	IIC_Star();
	IIC_SendByte(addr<<1);	//发送MPU的ID
	if(IIC_RiceveAck())
	{
		IIC_Stop();
		return 1;
	}
	IIC_SendByte(reg);			//发送需要操作的寄存器地址
	IIC_RiceveAck();
	for(;i<len;i++)
	{
		IIC_SendByte(buf[i]);	//向相关寄存器发送数据
		if(IIC_RiceveAck())
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_Stop();
	return 0;
}


/**
	*	@brief	MPU通过IIC读取任意数量的数据
	*	@param	addr	IIC地址
						reg		操作的寄存器地址
						len		需要读取的字节数
						buf		读取数据的储存位置
	*	@retval	0:	读取成功，1:	读取失败
	*/
uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	IIC_Star();
	IIC_SendByte(addr<<1);		//发送MPU的ID(写命令)
	if(IIC_RiceveAck())
	{
		IIC_Stop();
		return 1;
	}
	IIC_SendByte(reg);				//发送需要操作的寄存器地址
	IIC_RiceveAck();
	IIC_Star();
	IIC_SendByte(addr<<1|1);	//发送MPU的ID(读命令)
	IIC_RiceveAck();
	while(len)
	{
		if(len==1)	*buf=IIC_RiceveByte(1);		//以不发应答信号作为停止接收
		else        *buf=IIC_RiceveByte(0);			
		len--;
		buf++;
	}
	IIC_Stop();
	return 0;
}


/*********************************			可用作调试		****************************************/
/****************************************原子程序*********************************************/
/****************************************原子程序*********************************************/
/****************************************原子程序*********************************************/
/****************************************原子程序*********************************************/
/****************************************原子程序*********************************************/


//得到温度值
//返回值:温度值(扩大了100倍)
short MPU_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}


//IIC写一个字节 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{ 
    IIC_Star(); 
	IIC_SendByte((MPU_ADDR<<1)|0);//发送器件地址+写命令	
	if(IIC_RiceveAck())	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_SendByte(reg);	//写寄存器地址
    IIC_RiceveAck();		//等待应答 
	IIC_SendByte(data);//发送数据
	if(IIC_RiceveAck())	//等待ACK
	{
		IIC_Stop();	 
		return 1;		 
	}		 
    IIC_Stop();	 
	return 0;
}
//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
    IIC_Star(); 
	IIC_SendByte((MPU_ADDR<<1)|0);//发送器件地址+写命令	
	IIC_RiceveAck();		//等待应答 
    IIC_SendByte(reg);	//写寄存器地址
    IIC_RiceveAck();		//等待应答
    IIC_Star();
	IIC_SendByte((MPU_ADDR<<1)|1);//发送器件地址+读命令	
    IIC_RiceveAck();		//等待应答 
	res=IIC_RiceveByte(1);//读取数据,发送nACK 
    IIC_Stop();			//产生一个停止条件 
	return res;		
}

