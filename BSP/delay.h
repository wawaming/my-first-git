#ifndef _DELAY_H
#define _DELAY_H

#include "includes.h"

#define SYSTEM_SUPPORT_OS		1						//1,֧��ϵͳ.  0,��֧��ϵͳ


/*************** Enabled Sched or Disabled Sched of system task **************/
//void delay_OSSchedlock(void);				//�����������
//void delay_OSSchedunlock(void);			//������Ƚ���

//void delay_OSTimedly(u32 ticks);			//ϵͳ��ʱ����
//void SysTick_Handler(void);				//�δ�ʱ���ж�
/*************** user program	*******************************/
void delay_init(void);			//��ʱ������ʼ��
void delay_us(u32 nus);			//΢�뼶��ʱ
void delay_ms(u16 nms);			//���뼶��ʱ
void delay_xms(u16 nms);		//��ʱ�������
////����Ϊ��ຯ��
//void WFI_SET(void);		//ִ��WFIָ��
//void INTX_DISABLE(void);	//�ر������ж�
//void INTX_ENABLE(void);	//���������ж�
//void MSR_MSP(u32 addr);	//���ö�ջ��ַ


/************************************************�û��Զ��庯������********************************************************/
/************************************************�û��Զ��庯������********************************************************/
/************************************************�û��Զ��庯������********************************************************/

void delay(int n);

#endif




