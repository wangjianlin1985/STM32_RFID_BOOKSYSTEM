#include "common.h"
#include "stdlib.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-ESP8266 WIFIģ�� WIFI STA��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2015/4/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//ATK-ESP8266 WIFI STA����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������
u8 netpro=0;	//����ģʽ
u8 atk_8266_wifista_test(void)
{
	//u8 netpro=0;	//����ģʽ
//	u8 key;
	u8 timex=0; 
	u8 ipbuf[16]; 	//IP����
	u8 *p;
	u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//����״̬
	p=mymalloc(32);							//����32�ֽ��ڴ�
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//����WIFI STAģʽ
	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP�������ر�(��APģʽ��Ч) 
	delay_ms(1000);         //��ʱ3S�ȴ������ɹ�
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//����Ŀ��·����,���һ��IP
PRESTA:
	//netpro|=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[0]);	//ѡ������ģʽ
	if(netpro&0X02)   //UDP
	{
				//LCD_Clear(WHITE);
			//	POINT_COLOR=RED;
			//	Show_Str_Mid(0,30,"ATK-ESP WIFI-STA ����",16,240); 
			//	Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
			//	if(atk_8266_ip_set("WIFI-STA Զ��UDP IP����",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
				sprintf((char*)p,"AT+CIPSTART=\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��UDP������
				delay_ms(200);
				atk_8266_send_cmd("AT+CIPMUX=0","OK",20);  //������ģʽ
				delay_ms(200);
			//	LCD_Clear(WHITE);
				while(atk_8266_send_cmd(p,"OK",500));
	}
	else     //TCP
	{
		if(netpro&0X01)     //TCP Client    ͸��ģʽ����
		{
			//LCD_Clear(WHITE);
			//POINT_COLOR=RED;
		//	Show_Str_Mid(0,30,"ATK-ESP WIFI-STA ����",16,240); 
		//	Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
		//	if(atk_8266_ip_set("WIFI-STA Զ��IP����",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
			atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0�������ӣ�1��������
			sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��TCP������
			while(atk_8266_send_cmd(p,"OK",200))
			{
					//LCD_Clear(WHITE);
					//POINT_COLOR=RED;
					//Show_Str_Mid(0,40,"WK_UP:������ѡ",16,240);
					//Show_Str(30,80,200,12,"ATK-ESP ����TCPʧ��",12,0); //����ʧ��	 
					//key=KEY_Scan(0);
					//if(key==WKUP_PRES)goto PRESTA;
			}	
			atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //����ģʽΪ��͸��			
		}
		else					//TCP Server
		{
				//LCD_Clear(WHITE);
				//POINT_COLOR=RED;
				//Show_Str_Mid(0,30,"ATK-ESP WIFI-STA ����",16,240); 
				//Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
				atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
				sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);    //����Serverģʽ(0���رգ�1����)���˿ں�Ϊportnum
				atk_8266_send_cmd(p,"OK",50);    
		}
	}
			//LCD_Clear(WHITE);
			//POINT_COLOR=RED;
		//	Show_Str_Mid(0,30,"ATK-ESP WIFI-STA ����",16,240);
		//	Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);			
		//	LCD_Fill(30,50,239,50+12,WHITE);			//���֮ǰ����ʾ
		//	Show_Str(30,50,200,16,"WK_UP:�˳�����  KEY0:��������",12,0);
			//LCD_Fill(30,80,239,80+12,WHITE);
			atk_8266_get_wanip(ipbuf);//������ģʽ,��ȡWAN IP
			sprintf((char*)p,"IP��ַ:%s �˿�:%s",ipbuf,(u8*)portnum);
			//Show_Str(30,65,200,12,p,12,0);				//��ʾIP��ַ�Ͷ˿�	
			//Show_Str(30,80,200,12,"״̬:",12,0); 		//����״̬
			//Show_Str(120,80,200,12,"ģʽ:",12,0); 		//����״̬
		//	Show_Str(30,100,200,12,"��������:",12,0); 	//��������
		//	Show_Str(30,115,200,12,"��������:",12,0);	//��������
		//	atk_8266_wificonf_show(30,180,"������·�������߲���Ϊ:",(u8*)wifista_ssid,(u8*)wifista_encryption,(u8*)wifista_password);
		//	POINT_COLOR=BLUE;
		//	Show_Str(120+30,80,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,0); 		//����״̬
			USART2_RX_STA=0;
			while(1)
			{
			//	key=KEY_Scan(0);
			//	if(key==WKUP_PRES)			//WK_UP �˳�����		 
			//	{ 
			//		res=0;					
			//		atk_8266_quit_trans();	//�˳�͸��
			//		atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //�ر�͸��ģʽ
			//		break;												 
			//	}
			//	else if(key==KEY0_PRES)	//KEY0 �������� 
			/*		{
			
					if((netpro==3)||(netpro==2))   //UDP
					{
						sprintf((char*)p,"ATK-8266%s����%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
						Show_Str(30+54,100,200,12,p,12,0);
						atk_8266_send_cmd("AT+CIPSEND=25","OK",200);  //����ָ�����ȵ�����
						delay_ms(200);
						atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
						timex=100;
					}
					else if((netpro==1))   //TCP Client
					{
						atk_8266_quit_trans();
						atk_8266_send_cmd("AT+CIPSEND","OK",20);         //��ʼ͸��           
						sprintf((char*)p,"ATK-8266%s����%d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
						Show_Str(30+54,100,200,12,p,12,0);
						u2_printf("%s",p);
						timex=100;
					}
					else    //TCP Server
					{
						sprintf((char*)p,"ATK-8266%s����%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
						Show_Str(30+54,100,200,12,p,12,0);
						atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //����ָ�����ȵ�����
						delay_ms(200);
						atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
						timex=100;
					}
					
				}else;
			*/
				if(timex)timex--;
				//if(timex==1)LCD_Fill(30+54,100,239,112,WHITE);
				t++;
				delay_ms(10);
				if(USART2_RX_STA&0X8000)		//���յ�һ��������
				{ 
					rlen=USART2_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
					USART2_RX_BUF[rlen]=0;		//��ӽ����� 
					printf("%s",USART2_RX_BUF);	//���͵�����   
					sprintf((char*)p,"�յ�%d�ֽ�,��������",rlen);//���յ����ֽ��� 
				//	LCD_Fill(30+54,115,239,130,WHITE);
				//	POINT_COLOR=BRED;
				//	Show_Str(30+54,115,156,12,p,12,0); 			//��ʾ���յ������ݳ���
				//	POINT_COLOR=BLUE;
				//	LCD_Fill(30,130,239,319,WHITE);
				//	Show_Str(30,130,180,190,USART2_RX_BUF,12,0);//��ʾ���յ�������  
					USART2_RX_STA=0;
					if(constate!='+')t=1000;		//״̬Ϊ��δ����,������������״̬
					else t=0;                   //״̬Ϊ�Ѿ�������,10����ټ��
				}  
				if(t==1000)//����10����û���յ��κ�����,��������ǲ��ǻ�����.
				{
//			//		LCD_Fill(30+54,125,239,130,WHITE);
//					LCD_Fill(60,80,120,92,WHITE);
					constate=atk_8266_consta_check();//�õ�����״̬
//					if(constate=='+')Show_Str(30+30,80,200,12,"���ӳɹ�",12,0);  //����״̬
//					else Show_Str(30+30,80,200,12,"����ʧ��",12,0); 	 
					t=0;
				}
			//	if((t%20)==0)LED0=!LED0;
			//	atk_8266_at_response(1);
			}
	myfree(p);		//�ͷ��ڴ� 
	return res;		
} 




























