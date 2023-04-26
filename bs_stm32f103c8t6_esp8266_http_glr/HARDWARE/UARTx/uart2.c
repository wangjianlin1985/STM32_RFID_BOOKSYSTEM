#include "uart2.h"

//���ڷ��ͻ����� 	
u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//���ͻ���,���USART2_MAX_SEND_LEN�ֽ� 
//static u8 USART2_TX_BUF[1024];
u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; //���ջ���,���USART2_MAX_RECV_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
u16 USART2_RX_STA;         		//����״̬���	



//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���

void USART2_IRQHandler(void)
{
	u8 res;	      
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		res =USART_ReceiveData(USART2);		 
		if((USART2_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
		{ 
			if(USART2_RX_STA<USART2_MAX_RECV_LEN)	//�����Խ�������
			{
				TIM_SetCounter(TIM4,0);//���������          				//���������
				if(USART2_RX_STA==0) 				//ʹ�ܶ�ʱ��4���ж� 
				{
					TIM_Cmd(TIM4,ENABLE);//ʹ�ܶ�ʱ��4
				}
				USART2_RX_BUF[USART2_RX_STA++]=res;	//��¼���յ���ֵ	 
			}else 
			{
				USART2_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
			} 
		}
	}  				 											 
}  




//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void USART2_INIT(u32 bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); //����2ʱ��ʹ��

 	USART_DeInit(USART2);  //��λ����2
		 //USART2_TX   PA2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA
   
    //USART2_RX	  PA3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3
	
	USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
  

	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
	
	//ʹ�ܽ����ж�
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�   
	
	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
	TIM4_Int_Init(1000-1,7200-1);		//10ms�ж�
	USART2_RX_STA=0;		//����
	TIM_Cmd(TIM4,DISABLE);			//�رն�ʱ��4

}



//ͨ�ö�ʱ��4�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ42M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM4_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//TIM4ʱ��ʹ��    
	
	//��ʱ��TIM7��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�
	
	TIM_Cmd(TIM4,ENABLE);//������ʱ��4
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}
//��ʱ��4�жϷ������		    
void TIM4_IRQHandler(void)
{ 	
	//printf("��ʱ��4�ж�\r\n");
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		USART2_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIM4�����жϱ�־    
		TIM_Cmd(TIM4, DISABLE);  //�ر�TIM4
	}	    
}


void u2_printf(char* fmt,...)  
{     
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
		while((USART2->SR&0X40)==0);			//ѭ������,ֱ���������   
		USART2->DR=USART2_TX_BUF[j];  
	} 
}


void uart2_test(void)
{
	u32 len,t;
	static u32 times=0;
	if(USART2_RX_STA&0x8000)
	{					   
		len=USART2_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		u2_printf("\r\n usart2:�����͵���ϢΪ:\r\n");
		for(t=0;t<len;t++)
        {
            USART_SendData(USART2, USART2_RX_BUF[t]);//�򴮿�2��������
            while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
        }
		u2_printf("\r\n\r\n");//���뻻��
		USART2_RX_STA=0;
	}else
	{
		times++;
		if(times%200==0)u2_printf("usart2:����������,�Իس�������\r\n");  
	}
}


