#ifndef _SHANGWEIJI_H_
#define _SHANGWEIJI_H_

#include "includes.h"
//extern volatile struct  u16 OutData[7]= {0};

void OutPut_Data(void);
//void vcan_sendware(uint8_t *wareaddr, uint32_t waresize);
void vcan_sendware(uint16_t waresize);
static unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);


#endif  
