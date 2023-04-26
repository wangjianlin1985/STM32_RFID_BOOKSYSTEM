#ifndef UART5_H
#define UART5_H


#include "sys.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"

#define USART5_REC_LEN  			200  	//定义最大接收字节数 200
	  	
extern u8  USART5_RX_BUF[USART5_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART5_RX_STA;         		//接收状态标记

void uart5_Init(u32 baudrate);
void uart5_test(void);
void u5_printf(char *fmt,...);

#endif
