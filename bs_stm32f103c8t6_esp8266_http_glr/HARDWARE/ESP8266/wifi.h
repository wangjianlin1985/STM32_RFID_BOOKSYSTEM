#ifndef __COMMON_H__
#define __COMMON_H__	 
#include "sys.h"
#include "usart.h"		
#include "delay.h"	  	  	 
#include "malloc.h"
#include "string.h"    	
#include "uart2.h"

u8* esp8266_check_cmd(u8 *str);
u8 esp8266_send_data(u8 *data,u8 *ack,u16 waittime);
u8 esp8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8 esp8266_quit_trans(void);
u8 esp8266_consta_check(void);
void esp8266_get_ip(u8 x,u8 y);
void ESP_INIT(void);

u8 esp8266_send_data_web(void);//

#endif





