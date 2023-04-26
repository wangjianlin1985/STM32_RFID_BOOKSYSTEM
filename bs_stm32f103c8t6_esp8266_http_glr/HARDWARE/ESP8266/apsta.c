#include "common.h"

#include "led.h"
#include "bsp_dht11.h"


/************************************************
*
*  ���ߣ�daodanjishui
************************************************/

/*
 * ATK-ESP8266 AP+STAģʽ����
 * ���ڲ���TCP/UDP����
 * ����ֵ:0,����
 *    ����,�������
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
 * u8 ipbuf[16];        //IP����
 */
	u8 p[100] = "AT+CIPSTART=\"TCP\",\"192.168.0.123\",8080";

	u16 t = 2;                      /* ���ٵ�һ�λ�ȡ����״̬ */

	u16	rlen		= 0;
	u8	constate	= 0;    /* ����״̬ */

	printf( "ATK-ESP AP+STAģʽ����\r\n" );

	delay_ms( 500 );

	delay_ms( 500 );

	printf( "����׼�����ӷ�����,���Ե�...\r\n" );

	while ( atk_8266_send_cmd( p, "OK", 200 ) )
	{
		printf( "���ڲ����Ƿ����ӷ������ɹ�\r\n" );
	}


	atk_8266_get_ip( 15, 65 ); /* ��ȡ IP */


	strcpy( p, "AT+CIPMODE=1" );
	printf( "���ڿ���͸��,���Ե�...\r\n" );
	while ( atk_8266_send_cmd( p, "OK", 200 ) )
	{
		printf( "���ڲ��Կ���͸���Ƿ�ɹ�\r\n" );

	}
	printf( "͸���ɹ�\r\n" );
	strcpy( p, "AT+CIPSEND" );
	printf( "����׼����������,���Ե�...\r\n" );
	while ( atk_8266_send_cmd( p, "OK", 200 ) )
	{
		printf( "���ڲ���׼�����������Ƿ�ɹ�\r\n" );
	
	}
	printf( "׼���������ݳɹ�\r\n" );


	USART2_RX_STA = 0;

	while ( 1 )
	{
		/*
		 *
		 *  /*����Read_DHT11��ȡ��ʪ�ȣ����ɹ����������Ϣ*/
		if ( Read_DHT11( &DHT11_Data ) != SUCCESS )
		{
			printf( "\r\n��ȡDHT11�ɹ�!\r\n\r\nʪ��Ϊ%d.%d ��RH ���¶�Ϊ %d.%d�� \r\n", \
				DHT11_Data.humi_int, DHT11_Data.humi_deci, DHT11_Data.temp_int, DHT11_Data.temp_deci );

			sprintf( (char *) p, "GET http://192.168.0.123:8080/test?signname=%d HTTP/1.1\r\nHost:192.168.0.123:8080\r\nConnection: keep-alive\r\n\r\n", DHT11_Data.humi_int );     /* �������� */
			/* sprintf((char*)p,"POST http://192.168.253.1:8080/test_TCP/ChatServlet?action=sendMessage&user=Robot1&speak=Humidity:%d.%dRH---Temperature:%d.%d'C HTTP/1.1\r\nHost:192.168.253.1:8080\r\nConnection: keep-alive\r\n\r\n",DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);//�������� */
		}else  {
			printf( "Read DHT11 ERROR!\r\n" );
			sprintf( (char *) p, "hello,T=0 ,H=0\r\n" );                                                                                                                            /* ������ʪ�ȸ������� */
		}


		//���Ͳ�������
		printf( "���ڷ�������,���Ե�...\r\n" );
		strcpy( p, "GET /test" );

		atk_8266_send_cmd( p, "OK", 200 );


		t++;
		//��ȡ���ص�����
		delay_ms( 1000 );			
		rlen			= USART2_RX_STA & 0X7FFF;  
		printf( "�յ�%d�ֽ�,��������\r\n", rlen );
		printf( "---%s\r\n", USART2_RX_BUF );
			
	}


}


