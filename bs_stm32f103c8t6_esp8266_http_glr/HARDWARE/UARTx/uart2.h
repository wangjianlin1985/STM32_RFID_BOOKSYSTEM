#ifndef UART2_H
#define UART2_H

#include "sys.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"

#define USART2_MAX_RECV_LEN  			1024  	//�����������ֽ��� 1024
#define  USART2_MAX_SEND_LEN	  	1024  	//����������ֽ��� 1024
extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         		//����״̬���	

void USART2_INIT(u32 baudrate);
void uart2_test(void);
void u2_printf(char *fmt,...);

void TIM4_Int_Init(u16 arr,u16 psc);

#endif
