#ifndef	ML_ORDER
#define ML_ORDER


/*******************************铭朗控制器指令集*********************************/

/////-------------------------写数据下发到控制器中----------------------------------//////
/*	读取驱动器产品信息指令	*/
#define	ML_SADR 				0x04 	//	设置驱动器ID地址，数值范围：2~127
#define ML_SADR_LEN			0X06	//

/*	设置CAN通讯波特率	*/
#define ML_SCBD					0X07	//	设置驱动器的CAN通讯比特率
#define ML_SCBD_LEN			0x08

/**	@brief	电机、编码器相关配置	*/
#define ML_SSP					0X16	//	设置电机的允许最高速度，适用于任何模式
#define ML_SSP_LEN			0X08	//	

#define ML_SMV					0X18	//	设置最低速度，当设定速度小于最小速度时，电机将制动
#define ML_SMV_LEN			0X06

#define ML_SPC					0X1A	//	设置电机的峰值电流，数值范围：0~40000毫安
#define ML_SPC_LEN			0X08	//

#define ML_SCC					0X1C	//	设置电机的连续电流值，数值范围：0~20000毫安
#define ML_SCC_LEN			0X08	//

#define ML_ENC					0X1E	//	设置编码器的分辨率,参数为编码器实际线数的4倍
#define	ML_ENC_LEN			0X08	//

/**	@brief	驱动器工作模式	*/
#define	ML_SMOD					0X2A	//	设置当前的工作模式和信号源
#define	ML_SMOD_LEN			0X06	//

/**	@brief	驱动器各工作模式工作参数设定	*/
#define ML_SMAV					0X31	//	设置死区电压，数值范围：0~1000mv
#define ML_SMAV_LEN			0X06	//

#define	ML_SPH					0X33	//	设置软件限位的位置上限，电机正向最大位置范围：0~2100000000
#define ML_SPH_LEN			0X08	//

#define ML_SPL					0X35	//	设置软件限位的位置下限
#define	ML_SPL_LEN			0X08	//

#define	ML_SPE					0X3F	//	设置软件限位。1：启用软件限位，0：取消软件限位
#define ML_SPE_LEN			0X06	//

/**	@brief	PID调试设置指令	*/
#define ML_A						0X58	//	设置加速度，数值范围：1~30000
#define ML_A_LEN				0X06	//

#define	ML_AP						0X5A	//	设置电流环比例系数
#define ML_AP_LEN				0X06	//

#define	ML_AI						0X5C	//	设置电流环积分系数
#define ML_AI_LEN				0X06	//

#define ML_AD						0X5E	//	设置电流环微分系数
#define ML_AD_LEN				0X06	//

#define	ML_P						0X60	//	设置速度环比例系数
#define ML_P_LEN				0X06	//	

#define ML_I						0X62	//	设置速度环积分系数
#define	ML_I_LEN				0X06	//

#define ML_D						0X64	//	设置速度环微分系数
#define	ML_D_LEN				0X06	//

#define ML_MP						0X66	//	设置位置环比例系数
#define ML_MP_LEN				0X06	//

#define ML_MI						0X6A	//	设置位置环积分系数
#define ML_MI_LEN				0X06	//

#define ML_MD						0X6C	//	设置位置微分系数
#define	ML_MD_LEN				0X06	//

/**	@brief	设置开机寻找零位指令	*/
#define	ML_SPOF					0X75	//	设置寻找零位标志	1：开机寻找零位	1：开机不寻找零位
#define	ML_SPOF_LEN			0X06	//

/**	@brief	驱动器系统操作指令	*/
#define	ML_ESA					0X82	//	保存设置
#define	ML_ESA_LEN			0X06	//

#define	ML_SBS					0X84	//	软件急停，在任何模式下应用该指令驱动器会立即制动电机
#define	ML_SBS_LEN			0X06	//

#define	ML_CBS					0X85	//	解除软件急停
#define	ML_GBS_LEN			0X06	//

/**	@breif	驱动器反馈方式	*/
#define	ML_SSFT					0X8E	//	设置反馈方式	0：光电编码器反馈 1：测速机反馈
#define	ML_SSFT_LEN			0X06	//

#define	ML_V						0X90	//	设定运行速度
#define	ML_V_LEN				0X08	//

/**	@brief	其他模式控制	*/
#define	ML_EC						0X96	//	设置目标电流
#define	ML_EC_LEN				0X08	//

/**	@brief	位置控制指令	*/
#define	ML_PO						0X98	//	设置绝对位置,设置电机当前位置为绝对位置	在电机停止状态设置会比较精确
#define ML_PO_LEN				0X08	//

#define	ML_M						0X99	//	设置绝对对位置	电机开始向设定的位置运动
#define	ML_M_LEN				0X08	//

#define	ML_MR						0X9A	//	设置相对位置	相对于上次设置位置设定，非电机实时位置	电机开始向设定位置运动
#define	ML_MR_LEN				0X08	//



/////-------------------------从控制器中读数据----------------------------------//////

/*	读取驱动器产品信息指令	*/
#define	ML_GADR					0X05	//	读取驱动器ID地址
#define	ML_GADR_LEN			0X06	//	

/**	@brief	电机、编码器相关配置	*/
#define	ML_GSP					0X17	//	读取设置的电机允许最高速度
#define	ML_GSP_LEN			0X08	//

#define	ML_GMV					0X19	//	读取最低速度值
#define	ML_GMV_LEN			0X06	//

#define	ML_GPC					0X1B	//	读取设置的峰值电流
#define	ML_GPC_LEN			0X08	//

#define	ML_GCC					0X1D	//	读取设置的连续电流值
#define	ML_GCC_LEN			0X08	//

#define	ML_GENC					0X1F	//	读取编码器分辨率
#define	ML_GENC_LEN			0X08	//

/**	@brief	驱动器工作模式	*/
#define	ML_GMOD					0X2B	//	读取当前的工作模式和信号源
#define	ML_GMOD_LEN			0X06	//

/**	@brief	驱动器各工作模式工作参数设定	*/
#define	ML_GMAV					0X32	//	读取死区电压值
#define	ML_GMAV_LEN			0X06	//

#define	ML_GPH					0X34	//	读取软件限位的位置上限
#define	ML_GPH_LEN			0X08	//

#define	ML_GPL					0X36	//	读取软件限位的位置下限
#define	ML_GPL_LEN			0X08	//

#define	ML_GPE					0X40	//	获取软件限位状态
#define	ML_GPE_LEN			0X06	//

/**	@brief	PID调试设置指令	*/
#define	ML_GA						0X59	//	读取电机加速度值
#define	ML_GA_LEN				0X06	//

#define	ML_GAP					0X5B	//	读取电流环比例系数
#define	ML_GPA_LEN			0X06	//

#define	ML_GAI					0X5D	//	读取电流环积分系数
#define	ML_GAI_LEN			0X06	//

#define	ML_GAD					0X5F	//	读取电流环微分系数
#define	ML_GAD_LEN			0X06	//	

#define	ML_GP						0X61	//	读取速度环比例系数
#define	ML_GP_LEN				0X06	//

#define	ML_GI						0X63	//	读取速度环积分系数
#define	ML_GI_LEN				0X06	//

#define	ML_GD						0X65	//	读取速度环微分系数
#define	ML_GD_LEN				0X06	//

#define	ML_GMP					0X67	//	读取位置环比例系数
#define	ML_GMP_LEN			0X06	//

#define	ML_GMI					0X6A	//	读取位置环积分系数
#define	ML_GMI_LEN			0X06	//

#define	ML_GMD					0X6D	//	读取位置环微分系数
#define	ML_GMD_LEN			0X06	//

/**	@brief	设置开机寻找零位指令	*/
#define	ML_GPOF					0X76	//	读取寻找零位标志	1:开机寻找零位	0：开机不寻找零位
#define	ML_GPOF_LEN			0X06	//	

#define	ML_GOV					0X78	//	读取寻找零位速度
#define	ML_GOV_LEN			0X08	//

/**	@breif	驱动器反馈方式	*/
#define	ML_GV						0X91	//	读取平均速度
#define	ML_GV_LEN				0X04	//

#define	ML_GSV					0X93	//	读取设置的速度
#define	ML_GSV_LEN			0X08	//

/**	@brief	位置控制指令	*/
#define	ML_GM						0X9B	//	读取实际位置
#define	ML_GM_LEN				0X08	//	

#define	ML_GC						0XD0	//	读取电流值
#define	ML_GC_LEN				0X06	//	












#endif




