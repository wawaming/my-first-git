/*************************************************************************************
*	@file			myflash.c
*	@brief		对闪存flash进行写或读操作
*	@supplete	需注意,flash内部均以32位为操作单位(与falsh的供电电压有关),注意存储时字节的个数转换为32位半字
* @author	 	yun
*	@version 	V1.0
*	@date			2016/3/27
* @brief		NONE
*************************************************************************************/

#include "myflash.h"


/**
	*@beief：获取某个地址所在的flash扇区
	*@retval：返回：0-11，及addr所在的扇区
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
	*	@param	Flash_Adr				写入flash的初始位置
						Flash_Buff			需要写入的数据
						NumByte					需要写入的数据量(半字数，即16位数的个数)
	*	@retval	NONE
	*/
uint8_t My_FlashWrite(uint32_t Flash_Adr, uint32_t *Flash_Buff, uint32_t NumWord)
{
	FLASH_Status status = FLASH_COMPLETE;
	uint32_t setor_position;		//扇区位置
	uint32_t setor_offset;			//扇区偏移地址(32位字计算)

	/*	check up the Flash_Adr		*/
	if(Flash_Adr<FLASH_BASE||(Flash_Adr%4))	return 0;
	
	/*	calculate the space of the Flash	*/
	setor_position = Flash_Adr;
	setor_offset = Flash_Adr + NumWord * 4;

	FLASH_Unlock();//解锁flash空间
	FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
	
	if(setor_position<0X1FFF0000)
	{
		while(setor_position<setor_offset)
		{
			if(FLASH_READ_VALUE(setor_position) != 0XFFFFFFFF)
			{
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(setor_position),VoltageRange_3);//VCC=2.7~3.6V之间!!
				if(status!=FLASH_COMPLETE)break;	//发生错误了
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

	FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	FLASH_Lock();//上锁
	return 1;
}


/**
	*	@brief	读取指定地址的flash数据(16位字为单位)
	*	@param	Flash_Adr				读取flash的初始位置
						Flash_Buff			需要读取的数据
						NumByte					需要读取的数据量(半字数，即16位数的个数)
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






/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/***************************************************用户自定义函数********************************************************/
/**
	*	@brief
	*	@param
	* @retval
	*/








