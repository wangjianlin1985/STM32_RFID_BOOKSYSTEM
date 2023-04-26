#include "common.h"

#include "led.h"
#include "bsp_dht11.h"


/************************************************
*
*  作者：daodanjishui
************************************************/

/*
 * ATK-ESP8266 AP+STA模式测试
 * 用于测试TCP/UDP连接
 * 返回值:0,正常
 *    其他,错误代码
 */
u8 atk_8266_apsta_test1( void )
{
	/*
	 * u8 temperature;
	 * u8 humidity;
	 */
	DHT11_Data_TypeDef DHT11_Data;
/*
 * u8 netpro;
 * u8 key=0;
 * u8 timex=0;
 * u8 ipbuf[16];        //IP缓存
 */
	u8 p[100] = "AT+CIPSTART=\"TCP\",\"192.168.0.123\",8080";

	u16 t = 2;                      /* 加速第一次获取链接状态 */

	u16	rlen		= 0;
	u8	constate	= 0;    /* 连接状态 */

	printf( "ATK-ESP AP+STA模式测试\r\n" );

	delay_ms( 500 );

	delay_ms( 500 );

	printf( "正在准备连接服务器,请稍等...\r\n" );

	while ( atk_8266_send_cmd( p, "OK", 200 ) )
	{
		printf( "正在测试是否连接服务器成功\r\n" );
	}


	atk_8266_get_ip( 15, 65 ); /* 获取 IP */


	strcpy( p, "AT+CIPMODE=1" );
	printf( "正在开启透传,请稍等...\r\n" );
	while ( atk_8266_send_cmd( p, "OK", 200 ) )
	{
		printf( "正在测试开启透传是否成功\r\n" );

	}
	printf( "透传成功\r\n" );
	strcpy( p, "AT+CIPSEND" );
	printf( "正在准备发送数据,请稍等...\r\n" );
	while ( atk_8266_send_cmd( p, "OK", 200 ) )
	{
		printf( "正在测试准备发送数据是否成功\r\n" );
	
	}
	printf( "准备发送数据成功\r\n" );


	USART2_RX_STA = 0;

	while ( 1 )
	{
		/*
		 *
		 *  /*调用Read_DHT11读取温湿度，若成功则输出该信息*/
		if ( Read_DHT11( &DHT11_Data ) != SUCCESS )
		{
			printf( "\r\n读取DHT11成功!\r\n\r\n湿度为%d.%d ％RH ，温度为 %d.%d℃ \r\n", \
				DHT11_Data.humi_int, DHT11_Data.humi_deci, DHT11_Data.temp_int, DHT11_Data.temp_deci );

			sprintf( (char *) p, "GET http://192.168.0.123:8080/test?signname=%d HTTP/1.1\r\nHost:192.168.0.123:8080\r\nConnection: keep-alive\r\n\r\n", DHT11_Data.humi_int );     /* 测试数据 */
			/* sprintf((char*)p,"POST http://192.168.253.1:8080/test_TCP/ChatServlet?action=sendMessage&user=Robot1&speak=Humidity:%d.%dRH---Temperature:%d.%d'C HTTP/1.1\r\nHost:192.168.253.1:8080\r\nConnection: keep-alive\r\n\r\n",DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);//测试数据 */
		}else  {
			printf( "Read DHT11 ERROR!\r\n" );
			sprintf( (char *) p, "hello,T=0 ,H=0\r\n" );                                                                                                                            /* 返回温湿度给服务器 */
		}


		//发送测试数据
		printf( "正在发送数据,请稍等...\r\n" );
		strcpy( p, "GET /test" );

		atk_8266_send_cmd( p, "OK", 200 );


		t++;
		//获取返回的数据
		delay_ms( 1000 );			
		rlen			= USART2_RX_STA & 0X7FFF;  
		printf( "收到%d字节,内容如下\r\n", rlen );
		printf( "---%s\r\n", USART2_RX_BUF );
			
	}


}


