#include "uart4.h"


u8  USART4_RX_BUF[USART4_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
u16 USART4_RX_STA;         		//����״̬���	

void uart4_Init(u32 baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ��USART4��GPIOAʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA.2

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA.4  

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = baudrate;//���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(UART4, &USART_InitStructure); //��ʼ������4
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ���4
}

void UART4_IRQHandler(void)                	//����4�жϷ������
{
	u8 Res;

    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res =USART_ReceiveData(UART4);	//��ȡ���յ�������

        if((USART4_RX_STA&0x8000)==0)//����δ���
        {
            if(USART4_RX_STA&0x4000)//���յ���0x0d
            {
                if(Res!=0x0a)USART4_RX_STA=0;//���մ���,���¿�ʼ
                else USART4_RX_STA|=0x8000;	//��������� 
            }
            else //��û�յ�0X0D
            {	
                if(Res==0x0d)USART4_RX_STA|=0x4000;
                else
                {
                    USART4_RX_BUF[USART4_RX_STA&0X3FFF]=Res ;
                    USART4_RX_STA++;
                    if(USART4_RX_STA>(USART4_REC_LEN-1))USART4_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
                }		 
            }
        }   		 
    } 

} 
static u8 USART4_TX_BUF[200];

void u4_printf(char* fmt,...)  
{     
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART4_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART4_TX_BUF);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
		while((UART4->SR&0X40)==0);			//ѭ������,ֱ���������   
		UART4->DR=USART4_TX_BUF[j];  
	} 
}

void uart4_test(void)
{
	u32 len,t;
	static u32 times=0;
	if(USART4_RX_STA&0x8000)
	{					   
		len=USART4_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		u4_printf("\r\n usart4:�����͵���ϢΪ:\r\n");
		for(t=0;t<len;t++)
        {
            USART_SendData(UART4, USART4_RX_BUF[t]);//�򴮿�2��������
            while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
        }
		u4_printf("\r\n\r\n");//���뻻��
		USART4_RX_STA=0;
	}else
	{
		times++;
		if(times%200==0)u4_printf("usart4:����������,�Իس�������\r\n");  
	}
}
