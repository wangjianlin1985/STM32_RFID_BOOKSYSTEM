#include "uart5.h"


u8  USART5_RX_BUF[USART5_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
u16 USART5_RX_STA;         		//����״̬���	

void uart5_Init(u32 baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOD, ENABLE);	//ʹ��USART5��GPIOAʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PD2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
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

    USART_Init(UART5, &USART_InitStructure); //��ʼ������5
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ���5
}

void UART5_IRQHandler(void)                	//����5�жϷ������
{
	u8 Res;

    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res =USART_ReceiveData(UART5);	//��ȡ���յ�������

        if((USART5_RX_STA&0x8000)==0)//����δ���
        {
            if(USART5_RX_STA&0x4000)//���յ���0x0d
            {
                if(Res!=0x0a)USART5_RX_STA=0;//���մ���,���¿�ʼ
                else USART5_RX_STA|=0x8000;	//��������� 
            }
            else //��û�յ�0X0D
            {	
                if(Res==0x0d)USART5_RX_STA|=0x4000;
                else
                {
                    USART5_RX_BUF[USART5_RX_STA&0X3FFF]=Res ;
                    USART5_RX_STA++;
                    if(USART5_RX_STA>(USART5_REC_LEN-1))USART5_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
                }		 
            }
        }   		 
    } 

} 
static u8 USART5_TX_BUF[200];

void u5_printf(char* fmt,...)  
{     
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART5_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART5_TX_BUF);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
		while((UART5->SR&0X40)==0);			//ѭ������,ֱ���������   
		UART5->DR=USART5_TX_BUF[j];  
	} 
}
void uart5_test(void)
{
	u32 len,t;
	static u32 times=0;
	if(USART5_RX_STA&0x8000)
	{					   
		len=USART5_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		u5_printf("\r\n usart5:�����͵���ϢΪ:\r\n");
		for(t=0;t<len;t++)
        {
            USART_SendData(UART5, USART5_RX_BUF[t]);//�򴮿�2��������
            while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
        }
		u5_printf("\r\n\r\n");//���뻻��
		USART5_RX_STA=0;
	}else
	{
		times++;
		if(times%200==0)u5_printf("usart5:����������,�Իس�������\r\n");  
	}
}