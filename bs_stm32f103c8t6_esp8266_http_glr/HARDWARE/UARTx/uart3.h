#ifndef UART3_H
#define UART3_H


#include "sys.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"

#define USART3_REC_LEN  			200  	//定义最大接收字节数 200
	  	
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA;         		//接收状态标记

void uart3_Init(u32 baudrate);
void uart3_test(void);
void u3_printf(char *fmt,...);

#endif
