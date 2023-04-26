#ifndef UART2_H
#define UART2_H

#include "sys.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"

#define USART2_MAX_RECV_LEN  			1024  	//定义最大接收字节数 1024
#define  USART2_MAX_SEND_LEN	  	1024  	//定义最大发送字节数 1024
extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记	

void USART2_INIT(u32 baudrate);
void uart2_test(void);
void u2_printf(char *fmt,...);

void TIM4_Int_Init(u16 arr,u16 psc);

#endif
