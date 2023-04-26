#ifndef UART5_H
#define UART5_H


#include "sys.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"

#define USART5_REC_LEN  			200  	//�����������ֽ��� 200
	  	
extern u8  USART5_RX_BUF[USART5_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART5_RX_STA;         		//����״̬���

void uart5_Init(u32 baudrate);
void uart5_test(void);
void u5_printf(char *fmt,...);

#endif
