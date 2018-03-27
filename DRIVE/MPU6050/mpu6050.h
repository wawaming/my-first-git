#ifndef __MPU6050_H
#define __MPU6050_H
#include "includes.h"  
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"
 

//#define MPU_ACCEL_OFFS_REG		0X06	//accel_offs寄存器,可读取版本号,寄存器手册未提到
//#define MPU_PROD_ID_REG			0X0C	//prod id寄存器,在寄存器手册未提到
#define MPU_SELF_TESTX_REG		0X0D	//自检寄存器X
#define MPU_SELF_TESTY_REG		0X0E	//自检寄存器Y
#define MPU_SELF_TESTZ_REG		0X0F	//自检寄存器Z
#define MPU_SELF_TESTA_REG		0X10	//自检寄存器A
#define MPU_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define MPU_CFG_REG				0X1A	//配置寄存器
#define MPU_GYRO_CFG_REG		0X1B	//陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG		0X1C	//加速度计配置寄存器
#define MPU_MOTION_DET_REG		0X1F	//运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG			0X23	//FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG		0X24	//IIC主机控制寄存器
#define MPU_I2CSLV0_ADDR_REG	0X25	//IIC从机0器件地址寄存器
#define MPU_I2CSLV0_REG			0X26	//IIC从机0数据地址寄存器
#define MPU_I2CSLV0_CTRL_REG	0X27	//IIC从机0控制寄存器
#define MPU_I2CSLV1_ADDR_REG	0X28	//IIC从机1器件地址寄存器
#define MPU_I2CSLV1_REG			0X29	//IIC从机1数据地址寄存器
#define MPU_I2CSLV1_CTRL_REG	0X2A	//IIC从机1控制寄存器
#define MPU_I2CSLV2_ADDR_REG	0X2B	//IIC从机2器件地址寄存器
#define MPU_I2CSLV2_REG			0X2C	//IIC从机2数据地址寄存器
#define MPU_I2CSLV2_CTRL_REG	0X2D	//IIC从机2控制寄存器
#define MPU_I2CSLV3_ADDR_REG	0X2E	//IIC从机3器件地址寄存器
#define MPU_I2CSLV3_REG			0X2F	//IIC从机3数据地址寄存器
#define MPU_I2CSLV3_CTRL_REG	0X30	//IIC从机3控制寄存器
#define MPU_I2CSLV4_ADDR_REG	0X31	//IIC从机4器件地址寄存器
#define MPU_I2CSLV4_REG			0X32	//IIC从机4数据地址寄存器
#define MPU_I2CSLV4_DO_REG		0X33	//IIC从机4写数据寄存器
#define MPU_I2CSLV4_CTRL_REG	0X34	//IIC从机4控制寄存器
#define MPU_I2CSLV4_DI_REG		0X35	//IIC从机4读数据寄存器

#define MPU_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
#define MPU_INTBP_CFG_REG		0X37	//中断/旁路设置寄存器
#define MPU_INT_EN_REG			0X38	//中断使能寄存器
#define MPU_INT_STA_REG			0X3A	//中断状态寄存器

#define MPU_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH_REG		0X41	//温度值高八位寄存器
#define MPU_TEMP_OUTL_REG		0X42	//温度值低8位寄存器

#define MPU_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器

#define MPU_I2CSLV0_DO_REG		0X63	//IIC从机0数据寄存器
#define MPU_I2CSLV1_DO_REG		0X64	//IIC从机1数据寄存器
#define MPU_I2CSLV2_DO_REG		0X65	//IIC从机2数据寄存器
#define MPU_I2CSLV3_DO_REG		0X66	//IIC从机3数据寄存器

#define MPU_I2CMST_DELAY_REG	0X67	//IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG		0X68	//信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG	0X69	//运动检测控制寄存器
#define MPU_USER_CTRL_REG		0X6A	//用户控制寄存器
#define MPU_PWR_MGMT1_REG		0X6B	//电源管理寄存器1
#define MPU_PWR_MGMT2_REG		0X6C	//电源管理寄存器2 
#define MPU_FIFO_CNTH_REG		0X72	//FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG		0X73	//FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG			0X74	//FIFO读写寄存器
#define MPU_DEVICE_ID_REG		0X75	//器件ID寄存器
 
//如果AD0脚(9脚)接地,IIC地址为0X68(不包含最低位).
//如果接V3.3,则IIC地址为0X69(不包含最低位).
#define MPU_ADDR				0X68


////因为模块AD0默认接GND,所以转为读写地址后,为0XD1和0XD0(如果接VCC,则为0XD3和0XD2)  
//#define MPU_READ    0XD1
//#define MPU_WRITE   0XD0


/*********************************************************************************/

#define MPU_ON	1
#define MPU_OFF	0

#define MPU_DMP_STAR	MPU_ON	//是否使能DMP
#define MPU_RATE	50					//MPU的输出频率
#define DLPF_CFG	4						//低通滤波器
#define MPU6050_SLAVEADRASS		0X68	//mpu6050设备id地址 0x68：AD0端口接GND 0x69：AD0端口接VDD

/*	设置DMP的功能	*/
#define DMP_MODE	(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_TAP|DMP_FEATURE_ANDROID_ORIENT\
									|DMP_FEATURE_SEND_RAW_ACCEL|DMP_FEATURE_SEND_CAL_GYRO|DMP_FEATURE_GYRO_CAL)
									
/*	MPU6050数据包	*/
#define PACK_LENGTH	32		//FIFO原始数据包大小(没有解析的数据包),即PACK_LENGTH = dmp.packet_length，设置DMP功能时会改变其值
													//开启所有功能则数据包长度为32个字节
													/*if (DMP_MODE & DMP_FEATURE_SEND_RAW_ACCEL)
																dmp.packet_length += 6;
														if (mask & DMP_FEATURE_SEND_ANY_GYRO)
																dmp.packet_length += 6;
														if (mask & (DMP_FEATURE_LP_QUAT | DMP_FEATURE_6X_LP_QUAT))
																dmp.packet_length += 16;
														if (mask & (DMP_FEATURE_TAP | DMP_FEATURE_ANDROID_ORIENT))
																dmp.packet_length += 4;*/
									
typedef struct	/*	DMP开启后FIFO数据包	*/
{
	int32_t DMP_FIFO_QUAT[4];
	uint16_t DMP_FIFO_ACCEL[3];
	uint16_t DMP_FIFO_GYRO[3];
}DMP_FIFO_PACK_STRUCT;

typedef struct	/*	MPU的groy与accel原数据寄存器所存储的数据包	*/
{
	uint16_t	groy[3];				//陀螺仪数据
	uint16_t	accel[3];				//加速度计数据
	uint32_t	dmp_quat[4];		//dmp融合数据四元数
}MPU_PACKET;

typedef	struct	/*	计算的结果-角度数据包	*/
{
	float	mpu_pitch;			//绕X轴旋转,即俯仰角
	float	mpu_roll;				//绕Y轴旋转,即横滚角
	float	mpu_yaw;				//绕Z轴旋转,即偏航角
}MPU_ANGLE;

/*	滤波器采集频率	*/
#define COLLECT_TIME	0.005	//5ms

/*	MPU6050计算参数	*/
#define ACCEL_UNIT	16384	//65536/4
#define GROY_UNIT		16.4	//65536/4000
#define Q30	1073741824.0f				//Q30数据格式下的2的30次方值


#define ACCEL_X_ERROR	0		/*初始误差*/
#define ACCEL_Y_ERROR	0
#define ACCEL_Z_ERROR	0

#define GROY_X_ERROR	0		/*初始误差*/
#define GROY_Y_ERROR	0
#define GROY_Z_ERROR	0

u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf);//IIC连续写
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf); //IIC连续读 
u8 MPU_Init(void); 								//初始化MPU6050
void QUAT_Convert(float *pitch,float *roll,float *yaw,int32_t *quat);
int MPU_FIFO_RADE(short *gyro, short *accel, long *quat, uint8_t all_data);

#if	MPU_DMP_STAR ==	MPU_OFF
void MPU_Gather(float *pitch,	float *roll, float *yaw);
#endif
/***********************可用作调试*************************/
/************************原子程序*************************/
u8 MPU_Write_Byte(u8 reg,u8 data);
u8 MPU_Read_Byte(u8 reg);
u8 MPU_Set_Gyro_Fsr(u8 fsr);
u8 MPU_Set_Accel_Fsr(u8 fsr);
u8 MPU_Set_LPF(u16 lpf);
u8 MPU_Set_Rate(u16 rate);
u8 MPU_Set_Fifo(u8 sens);

short MPU_Get_Temperature(void);
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az);



#endif




































