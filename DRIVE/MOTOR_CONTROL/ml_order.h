#ifndef	ML_ORDER
#define ML_ORDER


/*******************************���ʿ�����ָ�*********************************/

/////-------------------------д�����·�����������----------------------------------//////
/*	��ȡ��������Ʒ��Ϣָ��	*/
#define	ML_SADR 				0x04 	//	����������ID��ַ����ֵ��Χ��2~127
#define ML_SADR_LEN			0X06	//

/*	����CANͨѶ������	*/
#define ML_SCBD					0X07	//	������������CANͨѶ������
#define ML_SCBD_LEN			0x08

/**	@brief	������������������	*/
#define ML_SSP					0X16	//	���õ������������ٶȣ��������κ�ģʽ
#define ML_SSP_LEN			0X08	//	

#define ML_SMV					0X18	//	��������ٶȣ����趨�ٶ�С����С�ٶ�ʱ��������ƶ�
#define ML_SMV_LEN			0X06

#define ML_SPC					0X1A	//	���õ���ķ�ֵ��������ֵ��Χ��0~40000����
#define ML_SPC_LEN			0X08	//

#define ML_SCC					0X1C	//	���õ������������ֵ����ֵ��Χ��0~20000����
#define ML_SCC_LEN			0X08	//

#define ML_ENC					0X1E	//	���ñ������ķֱ���,����Ϊ������ʵ��������4��
#define	ML_ENC_LEN			0X08	//

/**	@brief	����������ģʽ	*/
#define	ML_SMOD					0X2A	//	���õ�ǰ�Ĺ���ģʽ���ź�Դ
#define	ML_SMOD_LEN			0X06	//

/**	@brief	������������ģʽ���������趨	*/
#define ML_SMAV					0X31	//	����������ѹ����ֵ��Χ��0~1000mv
#define ML_SMAV_LEN			0X06	//

#define	ML_SPH					0X33	//	���������λ��λ�����ޣ�����������λ�÷�Χ��0~2100000000
#define ML_SPH_LEN			0X08	//

#define ML_SPL					0X35	//	���������λ��λ������
#define	ML_SPL_LEN			0X08	//

#define	ML_SPE					0X3F	//	���������λ��1�����������λ��0��ȡ�������λ
#define ML_SPE_LEN			0X06	//

/**	@brief	PID��������ָ��	*/
#define ML_A						0X58	//	���ü��ٶȣ���ֵ��Χ��1~30000
#define ML_A_LEN				0X06	//

#define	ML_AP						0X5A	//	���õ���������ϵ��
#define ML_AP_LEN				0X06	//

#define	ML_AI						0X5C	//	���õ���������ϵ��
#define ML_AI_LEN				0X06	//

#define ML_AD						0X5E	//	���õ�����΢��ϵ��
#define ML_AD_LEN				0X06	//

#define	ML_P						0X60	//	�����ٶȻ�����ϵ��
#define ML_P_LEN				0X06	//	

#define ML_I						0X62	//	�����ٶȻ�����ϵ��
#define	ML_I_LEN				0X06	//

#define ML_D						0X64	//	�����ٶȻ�΢��ϵ��
#define	ML_D_LEN				0X06	//

#define ML_MP						0X66	//	����λ�û�����ϵ��
#define ML_MP_LEN				0X06	//

#define ML_MI						0X6A	//	����λ�û�����ϵ��
#define ML_MI_LEN				0X06	//

#define ML_MD						0X6C	//	����λ��΢��ϵ��
#define	ML_MD_LEN				0X06	//

/**	@brief	���ÿ���Ѱ����λָ��	*/
#define	ML_SPOF					0X75	//	����Ѱ����λ��־	1������Ѱ����λ	1��������Ѱ����λ
#define	ML_SPOF_LEN			0X06	//

/**	@brief	������ϵͳ����ָ��	*/
#define	ML_ESA					0X82	//	��������
#define	ML_ESA_LEN			0X06	//

#define	ML_SBS					0X84	//	�����ͣ�����κ�ģʽ��Ӧ�ø�ָ���������������ƶ����
#define	ML_SBS_LEN			0X06	//

#define	ML_CBS					0X85	//	��������ͣ
#define	ML_GBS_LEN			0X06	//

/**	@breif	������������ʽ	*/
#define	ML_SSFT					0X8E	//	���÷�����ʽ	0�������������� 1�����ٻ�����
#define	ML_SSFT_LEN			0X06	//

#define	ML_V						0X90	//	�趨�����ٶ�
#define	ML_V_LEN				0X08	//

/**	@brief	����ģʽ����	*/
#define	ML_EC						0X96	//	����Ŀ�����
#define	ML_EC_LEN				0X08	//

/**	@brief	λ�ÿ���ָ��	*/
#define	ML_PO						0X98	//	���þ���λ��,���õ����ǰλ��Ϊ����λ��	�ڵ��ֹͣ״̬���û�ȽϾ�ȷ
#define ML_PO_LEN				0X08	//

#define	ML_M						0X99	//	���þ��Զ�λ��	�����ʼ���趨��λ���˶�
#define	ML_M_LEN				0X08	//

#define	ML_MR						0X9A	//	�������λ��	������ϴ�����λ���趨���ǵ��ʵʱλ��	�����ʼ���趨λ���˶�
#define	ML_MR_LEN				0X08	//



/////-------------------------�ӿ������ж�����----------------------------------//////

/*	��ȡ��������Ʒ��Ϣָ��	*/
#define	ML_GADR					0X05	//	��ȡ������ID��ַ
#define	ML_GADR_LEN			0X06	//	

/**	@brief	������������������	*/
#define	ML_GSP					0X17	//	��ȡ���õĵ����������ٶ�
#define	ML_GSP_LEN			0X08	//

#define	ML_GMV					0X19	//	��ȡ����ٶ�ֵ
#define	ML_GMV_LEN			0X06	//

#define	ML_GPC					0X1B	//	��ȡ���õķ�ֵ����
#define	ML_GPC_LEN			0X08	//

#define	ML_GCC					0X1D	//	��ȡ���õ���������ֵ
#define	ML_GCC_LEN			0X08	//

#define	ML_GENC					0X1F	//	��ȡ�������ֱ���
#define	ML_GENC_LEN			0X08	//

/**	@brief	����������ģʽ	*/
#define	ML_GMOD					0X2B	//	��ȡ��ǰ�Ĺ���ģʽ���ź�Դ
#define	ML_GMOD_LEN			0X06	//

/**	@brief	������������ģʽ���������趨	*/
#define	ML_GMAV					0X32	//	��ȡ������ѹֵ
#define	ML_GMAV_LEN			0X06	//

#define	ML_GPH					0X34	//	��ȡ�����λ��λ������
#define	ML_GPH_LEN			0X08	//

#define	ML_GPL					0X36	//	��ȡ�����λ��λ������
#define	ML_GPL_LEN			0X08	//

#define	ML_GPE					0X40	//	��ȡ�����λ״̬
#define	ML_GPE_LEN			0X06	//

/**	@brief	PID��������ָ��	*/
#define	ML_GA						0X59	//	��ȡ������ٶ�ֵ
#define	ML_GA_LEN				0X06	//

#define	ML_GAP					0X5B	//	��ȡ����������ϵ��
#define	ML_GPA_LEN			0X06	//

#define	ML_GAI					0X5D	//	��ȡ����������ϵ��
#define	ML_GAI_LEN			0X06	//

#define	ML_GAD					0X5F	//	��ȡ������΢��ϵ��
#define	ML_GAD_LEN			0X06	//	

#define	ML_GP						0X61	//	��ȡ�ٶȻ�����ϵ��
#define	ML_GP_LEN				0X06	//

#define	ML_GI						0X63	//	��ȡ�ٶȻ�����ϵ��
#define	ML_GI_LEN				0X06	//

#define	ML_GD						0X65	//	��ȡ�ٶȻ�΢��ϵ��
#define	ML_GD_LEN				0X06	//

#define	ML_GMP					0X67	//	��ȡλ�û�����ϵ��
#define	ML_GMP_LEN			0X06	//

#define	ML_GMI					0X6A	//	��ȡλ�û�����ϵ��
#define	ML_GMI_LEN			0X06	//

#define	ML_GMD					0X6D	//	��ȡλ�û�΢��ϵ��
#define	ML_GMD_LEN			0X06	//

/**	@brief	���ÿ���Ѱ����λָ��	*/
#define	ML_GPOF					0X76	//	��ȡѰ����λ��־	1:����Ѱ����λ	0��������Ѱ����λ
#define	ML_GPOF_LEN			0X06	//	

#define	ML_GOV					0X78	//	��ȡѰ����λ�ٶ�
#define	ML_GOV_LEN			0X08	//

/**	@breif	������������ʽ	*/
#define	ML_GV						0X91	//	��ȡƽ���ٶ�
#define	ML_GV_LEN				0X04	//

#define	ML_GSV					0X93	//	��ȡ���õ��ٶ�
#define	ML_GSV_LEN			0X08	//

/**	@brief	λ�ÿ���ָ��	*/
#define	ML_GM						0X9B	//	��ȡʵ��λ��
#define	ML_GM_LEN				0X08	//	

#define	ML_GC						0XD0	//	��ȡ����ֵ
#define	ML_GC_LEN				0X06	//	












#endif




