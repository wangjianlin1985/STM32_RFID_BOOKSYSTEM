#include "wifi.h"
#include "bsp_dht11.h"
#include "glr_adc.h"     
extern __IO u16 data_value[6];
extern u32 AD1_value,hy,trq,yw;	  
/*
 * ATK-ESP8266发送命令后,检测接收到的应答
 * str:期待的应答结果
 * 返回值:0,没有得到期待的应答结果
 *    其他,期待应答结果的位置(str的位置)
 */
u8* esp8266_check_cmd( u8 *str )
{
	char *strx = 0;
	if ( USART2_RX_STA & 0X8000 )                           /* 接收到一次数据了 */
	{
		USART2_RX_BUF[USART2_RX_STA & 0X7FFF]	= 0;    /* 添加结束符 */
		printf("\r\n---开始-- 接收到单片机返回：%s ---结束--\r\n-\n\n",USART2_RX_BUF);
		strx	= strstr( (const char *) USART2_RX_BUF, (const char *) str );
//		printf("\r\n 查询到的strx：%c,%d,%s\r\n-\n\n",strx,strx,strx);
	}
	return( (u8 *) strx);
}


/*
 * 向ATK-ESP8266发送命令
 * cmd:发送的命令字符串
 * ack:期待的应答结果,如果为空,则表示不需要等待应答
 * waittime:等待时间(单位:10ms)
 * 返回值:0,发送成功(得到了期待的应答结果)
 *       1,发送失败
 */
u8 esp8266_send_cmd( u8 *cmd, u8 *ack, u16 waittime )
{
	u8 res = 0;
	USART2_RX_STA = 0;
	u2_printf( "%s\r\n", cmd );                     /* 发送命令 */
	if ( ack && waittime )                          /* 需要等待应答 */
	{
		while ( --waittime )                    /* 等待倒计时 */
		{
			delay_ms( 10 );
			if ( USART2_RX_STA & 0X8000 )   /* 接收到期待的应答结果 */
			{
				if ( esp8266_check_cmd( ack ) )
				{
					printf( "%s得到期待回应ack:%s\r\n",  (u8 *) cmd ,(u8 *) ack );
					break;          /* 得到有效数据 */
				}
				else {
					printf( "没得到期待回应ack:%s\r\n", (u8 *) ack );
					
				}
				
				USART2_RX_STA = 0;
			}
		}
		if ( waittime == 0 ){
			res = 1;
			printf("超时\n");
		}
	}
		printf("res:%d \n",res);
	return(res);
}


/*
 * 向ATK-ESP8266发送指定数据
 * data:发送的数据(不需要添加回车了)
 * ack:期待的应答结果,如果为空,则表示不需要等待应答
 * waittime:等待时间(单位:10ms)
 * 返回值:0,发送成功(得到了期待的应答结果)luojian
 */
u8 esp8266_send_data( u8 *data, u8 *ack, u16 waittime )
{
	u8 res = 0;
	USART2_RX_STA = 0;
	u2_printf( "%s", data );                        /* 发送命令 */
	if ( ack && waittime )                          /* 需要等待应答 */
	{
		while ( --waittime )                    /* 等待倒计时 */
		{
			delay_ms( 10 );
			if ( USART2_RX_STA & 0X8000 )   /* 接收到期待的应答结果 */
			{
				if ( esp8266_check_cmd( ack ) )
					break;          /* 得到有效数据 */
				USART2_RX_STA = 0;
			}
		}
		if ( waittime == 0 ){
			res = 1;
			printf("超时\n");
		}

	}
	
	printf("res:%d \n",res);
	return(res);
}


/*
 * ATK-ESP8266退出透传模式
 * 返回值:0,退出成功;
 *       1,退出失败
 */
u8 esp8266_quit_trans( void )
{
	while ( (USART2->SR & 0X40) == 0 )
		;                                       /* 等待发送空 */
	USART2->DR = '+';
	delay_ms( 15 );                                 /* 大于串口组帧时间(10ms) */
	while ( (USART2->SR & 0X40) == 0 )
		;                                       /* 等待发送空 */
	USART2->DR = '+';
	delay_ms( 15 );                                 /* 大于串口组帧时间(10ms) */
	while ( (USART2->SR & 0X40) == 0 )
		;                                       /* 等待发送空 */
	USART2->DR = '+';
	delay_ms( 500 );                                /* 等待500ms */
	return(esp8266_send_cmd( "AT", "OK", 20 ) );   /* 退出透传判断. */
}




/*
 * 获取ATK-ESP8266模块的连接状态
 * 返回值:0,未连接;1,连接成功.
 */
u8 esp8266_consta_check( void )
{
	u8	*p;
	u8	res;
	if ( esp8266_quit_trans() )
		return(0);                              /* 退出透传 */
	esp8266_send_cmd( "AT+CIPSTATUS", ":", 50 );   /* 发送AT+CIPSTATUS指令,查询连接状态 */
	p	= esp8266_check_cmd( "+CIPSTATUS:" );
	res	= *p;
	return(res);
}






/*
 * 获取AP+STA ip地址并在指定位置显示
 * ipbuf:ip地址输出缓存区
 */
void esp8266_get_ip( u8 x, u8 y )
{
	if ( esp8266_send_cmd( "AT+CIFSR", "OK", 50 ) ) /* 获取WAN IP地址失败 */
	{
		printf( "IP获取失败\r\n" );
	}else {
		printf( "IP获取成功\r\n" );
	}
}


/* ATK-ESP8266模块测试主函数 */
void ESP_INIT( void )
{
	u8 result = 1;
	
	  while(esp8266_send_cmd("AT","OK",20));//检查WIFI模块是否在线
		while(esp8266_send_cmd("ATE0","OK",20));//关闭回显
		while(esp8266_send_cmd("AT","OK",20));//检查WIFI模块是否在线
		/*esp8266_msg_show(32,155,0);
	 
		printf("准备退出透传\n");
		while ( result = esp8266_send_cmd( "+++", "OK", 200 ) )           /* 检查WIFI模块是否在线 */
	/*{
		printf("AT返回：%d \n",result);
		printf( "正在退出透传!!!" );
		delay_ms( 800 );
	}*/
	/*
	u2_printf( "+++\r\n");  
	delay_ms( 800 );
	printf("准备重启\n");*/
	
	
	//while ( result = esp8266_send_cmd( "AT+RST", "OK", 2 ) )           /* 检查WIFI模块是否在线 */
	/*{
		printf("AT重启返回：%d \n",result);
		printf( "正在重启!!!" );
		delay_ms( 800 );
	}
	delay_ms( 800 );*/
	
	
	printf( "WIFI模块测试\r\n" );
	
	
	//esp8266_send_cmd( "AT", "OK", 20 );

	/*
	while ( result = esp8266_send_cmd( "AT", "OK", 2 ) )           /* 检查WIFI模块是否在线  
	{
		printf("AT返回：%d \n",result);
		esp8266_quit_trans();                          /* 退出透传  
		esp8266_send_cmd( "AT+CIPMODE=0", "OK", 200 ); /* 关闭透传模式  
		printf( "未检测到模块!!!" );
		delay_ms( 800 );
	}
	while ( esp8266_send_cmd( "ATE0", "OK", 200 ) );   /* 关闭回显  
	*/

	printf("准备设置从模式！");
	while ( esp8266_send_cmd( "AT+CWMODE=1", "OK", 200 ) );   /* 关闭回显 */
	delay_ms( 10 );
  printf("准备连接wifi\n");
	while ( esp8266_send_cmd( "AT+CWJAP=\"CMCC-KPUD\",\"p675eph9\"", "OK", 3000 ) )   /* 连接wifi */
	{
		printf("正在连接wifi，请稍后。。。\n");
		delay_ms( 1000 );
	}
  printf("连接wifi成功 \n");
	
	printf( "正在准备连接服务器,请稍等...\r\n" );

	
	while ( esp8266_send_cmd( "AT+CIPSTART=\"TCP\",\"192.168.1.4\",9528", "OK", 3000 ) )
	{
		printf( "正在测试是否连接服务器成功\r\n" );
	}

	printf("common112233\n");

	esp8266_get_ip( 15, 65 ); /* 获取 IP */


	//strcpy( p, "AT+CIPMODE=1" );
	printf( "正在开启透传,请稍等...\r\n" );
	while ( esp8266_send_cmd( "AT+CIPMODE=1", "OK", 200 ) )
	{
		printf( "正在测试开启透传是否成功\r\n" );

	}
	printf( "透传成功\r\n" );
	//strcpy( p, "AT+CIPSEND" );
	
	
	printf( "正在准备发送数据,请稍等...\r\n" );
	while ( esp8266_send_cmd( "AT+CIPSEND", "OK", 200 ) )
	{
		printf( "正在测试准备发送数据是否成功\r\n" );
	
	}
	
	printf( "准备发送数据成功\r\n" );
//	//红灯灭
//	GPIO_ResetBits(GPIOB,GPIO_Pin_8);	
	/*
	 * esp8266_at_response(1);//检查ATK-ESP8266模块发送过来的数据,及时上传给电脑
	 * esp8266_wifiap_test();	//WIFI AP测试
	 */
//	esp8266_send_data_web();                           /* ap+sta测试 */
	/* } */
}


