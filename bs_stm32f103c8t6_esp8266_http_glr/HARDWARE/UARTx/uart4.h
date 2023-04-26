#ifndef UART4_H
#define UART4_H


#include "sys.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"

#define USART4_REC_LEN  			200  	//定义最大接收字节数 200
	  	
extern u8  USART4_RX_BUF[USART4_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART4_RX_STA;         		//接收状态标记

void uart4_Init(u32 baudrate);
void uart4_test(void);
void u4_printf(char *fmt,...);

#endif
