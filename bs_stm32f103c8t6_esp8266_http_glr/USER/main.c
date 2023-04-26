#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "uart2.h"
#include "uart3.h"
#include "ds18b20.h"
#include "wifi.h"
#include "gpio.h"
#include "stdio.h"
#include "glr_adc.h"
#include "rc522.h"
extern __IO u16 data_value[6];
u32		AD1_value, hy, trq, yw, AD5_value;
//�¶ȱ���ֵ
 double	temperatureLimit;
 int	flameLimit;
 int	gasLimit;
 int	smokeLimit;
u8		ds18b20_state;
int		i;





/**
 *   ����˵����
 *   1--SDA  <----->PA4 SPIͨѶ��ʱ������͵�ƽ
 *   2--SCK  <----->PA5
 *   3--MOSI <----->PA7
 *   4--MISO <----->PA6
 *   5--����
 *   6--GND <----->GND
 *   7--RST <----->PB0
 *   8--VCC <----->VCC
 **/


/* a9 a10 ��Ƭ���������  a2 a3 wifiʹ�� PB1 - ������  PB8-������ PB9-���� PC15-���� */

void init222( void )
{
	DELAY_INIT(); //��ʼ��ʱ�� �ӳٺ���ʹ��
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 ); /* ����NVIC�жϷ��� */
	USART1_INIT( 115200 );//����1��ʼ��
	USART2_INIT( 115200 );//����2��ʼ��
	
//	ADC_INIT();
	LED1_INIT(); /* ��ʼ���ƺͷ��� */
//	LED2_INIT();
//	FAN_INIT();
	//BUZZER_INIT();
//	/* �������� */
//	GPIO_SetBits( GPIOB, GPIO_Pin_8 );

	delay_ms(10000 );
	ESP_INIT();
//	ds18b20_state = DS18B20_Init(); //��ʼ���¶ȴ�����
//	GPIO_SetBits( GPIOB, GPIO_Pin_9 );
	printf("wifi��ʼ���ɹ�\n");
		RC522_Init();    
}


int main( void )
{
	float wd = 0;
	int num = 0;

	u8	p[100]		= "";
//	char	cRes[512]	= "";
   init222();
	printf("ϵͳ��ʼ���ɹ�!\n");
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);	
		while ( 1 )
		{
			
			RC522_Handel();
			delay_ms( 10 );
			num ++;
			if(num>100){
				num = 0;
				printf("���ͱ���� \r\n");
				//���ͱ����
				u2_printf( "GET http://192.168.1.4:9528/user/test\r\n", p );
			} 
			
				if ( USART2_RX_STA & 0X8000 )   /* ���յ��ڴ���Ӧ���� */
			{
				USART2_RX_BUF[USART2_RX_STA & 0X7FFF]	= 0;    /* ��ӽ����� */
				printf("\r\n---��ʼ-- ���յ���Ƭ�����أ�%s ---����--\r\n\n\n",USART2_RX_BUF);
 
				
				USART2_RX_STA = 0;
			}
		}

}


