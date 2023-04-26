#ifndef UART4_H
#define UART4_H


#include "sys.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"

#define USART4_REC_LEN  			200  	//�����������ֽ��� 200
	  	
extern u8  USART4_RX_BUF[USART4_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART4_RX_STA;         		//����״̬���

void uart4_Init(u32 baudrate);
void uart4_test(void);
void u4_printf(char *fmt,...);

#endif
