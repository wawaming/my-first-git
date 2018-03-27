/*************************************************************************************
*	@file			mpu6050.c
*	@supplement	1��DMP������ģʽ��FIFO�����Ƶ����200hz���ٷ�Ϊ100hz
							2����Ҫ��ȡ���ݣ���Ҫ������Ӧ�����ݰ�
							mpu_dmp_get_data(&pitch,&roll,&yaw) Ҳ��ʹ�ô˺�����ýǶ�(��Ԫ���㷨)
* @author	 	yun
*	@version 	V1.2
*	@date			2016/8/4
* @brief		NONE
*************************************************************************************/
#include "mpu6050.h"



static signed char gyro_vector[9] = 			{1, 0, 0,						//�����Ƿ�������
                                           0, 1, 0,
                                           0, 0, 1};																	 
									
#if	MPU_DMP_STAR ==	MPU_OFF																					 
int16_t pitch_last=0,roll_last=0,yaw_last=0;			
#endif																					 
/**
	*	@brief	mpu6050��ʼ��
	*	@param	void
	*	@retval	0����ʼ���ɹ���1����ʼ��ʧ��
	*/
u8 MPU_Init(void)
{ 
#if	MPU_DMP_STAR ==	MPU_OFF
	unsigned	char pitch_gah,pitch_gal;
	unsigned  char roll_gah,roll_gal;
	unsigned  char yaw_gah,yaw_gal;
#endif
	/*-----����AD0�����ŵĽ����ƽ-----*/
//	GPIO_Config(GPIOA, GPIO_Pin_8, GPIO_Mode_Out_PP);
//	#if	MPU6050_SLAVEADRASS==0x68
//	GPIO_ResetBits(GPIOA, GPIO_Pin_8);									//�����ƽ0 ������ַΪ0x68
//	#else
//	GPIO_SetBits(GPIOA, GPIO_Pin_8);									//�����ƽ1 ������ַΪ0x69
//	#endif
	
	/*----��λmpu6050-----*/
	IIC_leisure();
	delay_ms(100);
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_PWR_MGMT1_REG, 0x80);	//�����ڲ��Ĵ�����Ĭ��ֵ����ɺ��Զ�����
	delay_ms(100);	
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_PWR_MGMT1_REG, 0x00);	//����mpu6050
	delay_ms(100);
	/*----����mpu6050��ز���----*/
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_GYRO_CFG_REG, 0x03<<3);	//���������Ǿ���+-2000��/��
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_ACCEL_CFG_REG, 0x00<<3);	//���ü��ٶȼƾ���+-2g
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_SAMPLE_RATE_REG, (1000/MPU_RATE-1));		//���ò���Ƶ��
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_CFG_REG, DLPF_CFG);										//���õ�ͨ�˲���������ֵΪ����Ƶ�ʵ�һ��
	
	/*----�������ģʽ-----*/
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_INT_EN_REG, 0);					//�����жϼĴ������ر������ж�
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_USER_CTRL_REG, 0);				//I2C��ģʽ�رգ�ʧ��FIFO���������������д
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_FIFO_EN_REG, 0);					//ʧ��FIFO�����������
	IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_INTBP_CFG_REG, 0x08);		//INT���ŵ͵�ƽ��Ч

	/*----����ʼ���Ƿ�ɹ�-----*/
	if(IIC_RiceveData(MPU6050_SLAVEADRASS<<1, MPU_DEVICE_ID_REG) != (MPU6050_SLAVEADRASS))
		return	10;
	else
	{
		IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_PWR_MGMT1_REG, 0x01);	//����mpu6050�ο�Ƶ��ΪPLL_X��
		IIC_SendData(MPU6050_SLAVEADRASS<<1, MPU_PWR_MGMT2_REG, 0x00);	//�������Ǻͼ��ٶȼ�
	}
	
	/*----DMP�Ƿ���-----*/
	#if	MPU_DMP_STAR ==	MPU_ON	
			if(!mpu_init()){
					if(mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL))			return 1;					//������Ҫ�Ĵ�����
					if(mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL))	return 2;					//����FIFO	
					if(mpu_set_sample_rate(DEFAULT_MPU_HZ))							return 3;					//���ò�����
					if(dmp_load_motion_driver_firmware())								return 4;					//����dmp�̼�
					if(dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_vector)))return 5;//���������Ƿ���
					if(dmp_enable_feature(DMP_MODE))										return 6;					//����DMP����
					if(dmp_set_fifo_rate(DEFAULT_MPU_HZ))								return 7;					//����DMP�������
					if(run_self_test())																	return 8;					//�����Լ�
					if(mpu_set_dmp_state(1))														return 9;					//ʹ��DMP
			}
			else return 11;
	#endif	
	delay_ms(800);
	delay_ms(800);
			
	/*	��ȡ�����ǵ���ֵ	*/
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 1, &pitch_gah);
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTL_REG, 1, &pitch_gal);
//	
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_YOUTH_REG, 1, &roll_gah);
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_YOUTL_REG, 1, &roll_gal);
#if	MPU_DMP_STAR ==	MPU_OFF
	MPU_Read_Len(MPU_ADDR, MPU_GYRO_ZOUTH_REG, 1, &yaw_gah);
	MPU_Read_Len(MPU_ADDR, MPU_GYRO_ZOUTL_REG, 1, &yaw_gal);
	
	/*	�ɼ�����ֵ��	*/
//	pitch_last = pitch_gah*256 + pitch_gal;			
//	roll_last	 = roll_gah*256 + roll_gal;
	yaw_last	 = yaw_gah*256 + yaw_gal;
#endif
	return	0;
}


/**
	*	@brief	��ȡDMP_FIFO��
	*	@param	pta:	��Ҫ�洢�ĵ�ַ,�����еĲ��������ֽ�Ϊ��λ,����һ���洢����
									����Ҫ�����ض�λ�ô���,����DMP�⺯�����д���
									int dmp_read_fifo(short *gyro, short *accel, long *quat,
																		unsigned long *timestamp, short *sensors, unsigned char *more)
	*	@retval	0:	��ȡ�ɹ�����������ȡʧ��
	*/
int MPU_FIFO_RADE(short *gyro, short *accel, long *quat, uint8_t all_data)
{
	uint8_t more;
//	uint8_t error;
	uint8_t PACK_buff[32];
	/*----ͨ��IIC,��FIFO��ȡ��Ӧ���ֽ���	�����ݰ�----*/
	if (mpu_read_fifo_stream(PACK_LENGTH, PACK_buff, &more))
			return 1;
	else{
		/*	��ȡ��Ԫ������	*/
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
	
		/*	��ȡFIFO�е�groy and accel	*/				
		accel[0] = ((short)PACK_buff[16] << 8) | PACK_buff[17];
		accel[1] = ((short)PACK_buff[18] << 8) | PACK_buff[19];
		accel[2] = ((short)PACK_buff[20] << 8) | PACK_buff[21];
		gyro[0] = ((short)PACK_buff[22] << 8) | PACK_buff[23];
		gyro[1] = ((short)PACK_buff[24] << 8) | PACK_buff[25];
		gyro[2] = ((short)PACK_buff[26] << 8) | PACK_buff[27];
	return 0;
}



/**
	*	@brief	��Ԫ������
	*	@param	pitch		�����Ǵ�ŵ�ַ
						roll		����Ǵ�ŵ�ַ
						yaw			ƫ���Ǵ�ŵ�ַ
						quat		��Ԫ����ַ
	*	@retval	NONE
	*/
void QUAT_Convert(float *pitch,float *roll,float *yaw,int32_t *quat)
{
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	q0 = quat[0] / Q30;	//q30��ʽת��Ϊ������
	q1 = quat[1] / Q30;	
	q2 = quat[2] / Q30;
	q3 = quat[3] / Q30; 
	
	*pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	
	*roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	
	*yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	
}


/**
	*	@brief	MPU�ɼ��Ƕȣ���ͨ��DMP
	*	@param	*pitch	�����Ǵ���λ��	*roll	�����Ǵ���λ��	*yaw	ƫ���Ǵ���λ��
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
	/*	��ȡ�����ǵ���ֵ	*/
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 1, &pitch_gah);
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTL_REG, 1, &pitch_gal);
//	
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_YOUTH_REG, 1, &roll_gah);
//	MPU_Read_Len(MPU_ADDR, MPU_GYRO_YOUTL_REG, 1, &roll_gal);
//	
	MPU_Read_Len(MPU_ADDR, MPU_GYRO_ZOUTH_REG, 1, &yaw_gah);
	MPU_Read_Len(MPU_ADDR, MPU_GYRO_ZOUTL_REG, 1, &yaw_gal);
	
	/*	�ɼ�����ֵ��	*/
//	pitch_gather = pitch_gah*256 + pitch_gal - pitch_last;			
//	roll_gather	 = roll_gah*256 + roll_gal - roll_last;
	yaw_gather	 = yaw_gah*256 + yaw_gal -  yaw_last;
	
	/*	��ʼ����	*/
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
	*	@brief	MPUͨ��IICд�����������ֽ�
	*	@param	addr:		IIC��ַ
						reg��		�����ļĴ�����ַ
						len��		��Ҫд����ֽ���Ŀ
						buf��		��Ҫд�������
	*	@retval	0��д��ɹ���1��д��ʧ��
	*/
uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	uint8_t i=0;
	IIC_Star();
	IIC_SendByte(addr<<1);	//����MPU��ID
	if(IIC_RiceveAck())
	{
		IIC_Stop();
		return 1;
	}
	IIC_SendByte(reg);			//������Ҫ�����ļĴ�����ַ
	IIC_RiceveAck();
	for(;i<len;i++)
	{
		IIC_SendByte(buf[i]);	//����ؼĴ�����������
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
	*	@brief	MPUͨ��IIC��ȡ��������������
	*	@param	addr	IIC��ַ
						reg		�����ļĴ�����ַ
						len		��Ҫ��ȡ���ֽ���
						buf		��ȡ���ݵĴ���λ��
	*	@retval	0:	��ȡ�ɹ���1:	��ȡʧ��
	*/
uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	IIC_Star();
	IIC_SendByte(addr<<1);		//����MPU��ID(д����)
	if(IIC_RiceveAck())
	{
		IIC_Stop();
		return 1;
	}
	IIC_SendByte(reg);				//������Ҫ�����ļĴ�����ַ
	IIC_RiceveAck();
	IIC_Star();
	IIC_SendByte(addr<<1|1);	//����MPU��ID(������)
	IIC_RiceveAck();
	while(len)
	{
		if(len==1)	*buf=IIC_RiceveByte(1);		//�Բ���Ӧ���ź���Ϊֹͣ����
		else        *buf=IIC_RiceveByte(0);			
		len--;
		buf++;
	}
	IIC_Stop();
	return 0;
}


/*********************************			����������		****************************************/
/****************************************ԭ�ӳ���*********************************************/
/****************************************ԭ�ӳ���*********************************************/
/****************************************ԭ�ӳ���*********************************************/
/****************************************ԭ�ӳ���*********************************************/
/****************************************ԭ�ӳ���*********************************************/


//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
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
//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
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
//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
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


//IICдһ���ֽ� 
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{ 
    IIC_Star(); 
	IIC_SendByte((MPU_ADDR<<1)|0);//����������ַ+д����	
	if(IIC_RiceveAck())	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_SendByte(reg);	//д�Ĵ�����ַ
    IIC_RiceveAck();		//�ȴ�Ӧ�� 
	IIC_SendByte(data);//��������
	if(IIC_RiceveAck())	//�ȴ�ACK
	{
		IIC_Stop();	 
		return 1;		 
	}		 
    IIC_Stop();	 
	return 0;
}
//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
    IIC_Star(); 
	IIC_SendByte((MPU_ADDR<<1)|0);//����������ַ+д����	
	IIC_RiceveAck();		//�ȴ�Ӧ�� 
    IIC_SendByte(reg);	//д�Ĵ�����ַ
    IIC_RiceveAck();		//�ȴ�Ӧ��
    IIC_Star();
	IIC_SendByte((MPU_ADDR<<1)|1);//����������ַ+������	
    IIC_RiceveAck();		//�ȴ�Ӧ�� 
	res=IIC_RiceveByte(1);//��ȡ����,����nACK 
    IIC_Stop();			//����һ��ֹͣ���� 
	return res;		
}

