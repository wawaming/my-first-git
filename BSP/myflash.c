/*************************************************************************************
*	@file			myflash.c
*	@brief		������flash����д�������
*	@supplete	��ע��,flash�ڲ�����32λΪ������λ(��falsh�Ĺ����ѹ�й�),ע��洢ʱ�ֽڵĸ���ת��Ϊ32λ����
* @author	 	yun
*	@version 	V1.0
*	@date			2016/3/27
* @brief		NONE
*************************************************************************************/

#include "myflash.h"


/**
	*@beief����ȡĳ����ַ���ڵ�flash����
	*@retval�����أ�0-11����addr���ڵ�����
	*/
uint16_t STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
	return FLASH_Sector_11;	
}

/**
	*	@brief	Program faced with the Flash
	*	@param	Flash_Adr				д��flash�ĳ�ʼλ��
						Flash_Buff			��Ҫд�������
						NumByte					��Ҫд���������(����������16λ���ĸ���)
	*	@retval	NONE
	*/
uint8_t My_FlashWrite(uint32_t Flash_Adr, uint32_t *Flash_Buff, uint32_t NumWord)
{
	FLASH_Status status = FLASH_COMPLETE;
	uint32_t setor_position;		//����λ��
	uint32_t setor_offset;			//����ƫ�Ƶ�ַ(32λ�ּ���)

	/*	check up the Flash_Adr		*/
	if(Flash_Adr<FLASH_BASE||(Flash_Adr%4))	return 0;
	
	/*	calculate the space of the Flash	*/
	setor_position = Flash_Adr;
	setor_offset = Flash_Adr + NumWord * 4;

	FLASH_Unlock();//����flash�ռ�
	FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
	
	if(setor_position<0X1FFF0000)
	{
		while(setor_position<setor_offset)
		{
			if(FLASH_READ_VALUE(setor_position) != 0XFFFFFFFF)
			{
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(setor_position),VoltageRange_3);//VCC=2.7~3.6V֮��!!
				if(status!=FLASH_COMPLETE)break;	//����������
			}else setor_position += 4;
		}
		if(status == FLASH_COMPLETE)
		{
			while(Flash_Adr<setor_offset)
			{
				if(FLASH_ProgramWord(Flash_Adr,*Flash_Buff)!=FLASH_COMPLETE)
				{
					break;
				}
				Flash_Adr += 4;
				Flash_Buff++;
			}
		}
	}

	FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();//����
	return 1;
}


/**
	*	@brief	��ȡָ����ַ��flash����(16λ��Ϊ��λ)
	*	@param	Flash_Adr				��ȡflash�ĳ�ʼλ��
						Flash_Buff			��Ҫ��ȡ������
						NumByte					��Ҫ��ȡ��������(����������16λ���ĸ���)
	*	@retval	NONE
	*/
void MyFlash_ReadHalfWord(uint32_t Flash_Adr, uint32_t *Flash_Buff, uint32_t NumWord)
{
	uint32_t i;

	for(i=0;i<NumWord;i++)
	{
		Flash_Buff[i] = FLASH_READ_VALUE(Flash_Adr);
		Flash_Adr+=4;
	}
}






/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/***************************************************�û��Զ��庯��********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/








