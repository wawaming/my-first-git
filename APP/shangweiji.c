//#include "common.h"
#include "includes.h" 
#include "shangweiji.h"

uint16_t  OutData[7]= {0};
//************************************************
/*
*  功能说明：SCI示波器发送函数
*  参数说明：
OutData[]  需要发送的数值赋予该数组
*  函数返回：无符号结果值
*  修改时间：2013-2-10
*/
//****************************************************
static unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}




void OutPut_Data(void)
{
    int temp[4] = {0};
    unsigned int temp1[4] = {0};
    unsigned char databuf[10] = {0};
    unsigned char i;
    unsigned short CRC16 = 0;
    for(i=0;i<4;i++)
    {

        temp[i]  = (int)OutData[i];
        temp1[i] = (unsigned int)temp[i];

    }

    for(i=0;i<4;i++)
    {
        databuf[i*2]   = (unsigned char)(temp1[i]%256);
        databuf[i*2+1] = (unsigned char)(temp1[i]/256);
    }

    CRC16 = CRC_CHECK(databuf,8);
    databuf[8] = CRC16%256;
    databuf[9] = CRC16/256;

    for(i=0;i<10;i++)
    {
        USART_SendData (UART4,(char)databuf[i]);
    }
}


void vcan_sendware(uint16_t waresize)
{
//#define CMD_WARE     3
//    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
//    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

//    USART_SendData(UART4, cmdf, sizeof(cmdf));    //先发送前命令
//    USART_SendData(UART4, wareaddr, waresize);    //发送数据
//    USART_SendData(UART4, cmdr, sizeof(cmdr));    //发送后命令
		
		/*	K60移植 已修改	*/
		
		USART_SendData(UART4, 0X03);    //发送数据
		USART_SendData(UART4, 0XFC);    //发送数据
	
		USART_SendData(UART4, waresize);    //发送数据
	
		USART_SendData(UART4, 0XFC);    //发送数据	
		USART_SendData(UART4, 0X03);    //发送数据	

}
