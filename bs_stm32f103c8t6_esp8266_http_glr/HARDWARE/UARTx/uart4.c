#include "uart4.h"


u8  USART4_RX_BUF[USART4_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
u16 USART4_RX_STA;         		//接收状态标记	

void uart4_Init(u32 baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能USART4，GPIOA时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOA.2

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOA.4  

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = baudrate;//串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(UART4, &USART_InitStructure); //初始化串口4
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启串口接受中断
    USART_Cmd(UART4, ENABLE);                    //使能串口4
}

void UART4_IRQHandler(void)                	//串口4中断服务程序
{
	u8 Res;

    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res =USART_ReceiveData(UART4);	//读取接收到的数据

        if((USART4_RX_STA&0x8000)==0)//接收未完成
        {
            if(USART4_RX_STA&0x4000)//接收到了0x0d
            {
                if(Res!=0x0a)USART4_RX_STA=0;//接收错误,重新开始
                else USART4_RX_STA|=0x8000;	//接收完成了 
            }
            else //还没收到0X0D
            {	
                if(Res==0x0d)USART4_RX_STA|=0x4000;
                else
                {
                    USART4_RX_BUF[USART4_RX_STA&0X3FFF]=Res ;
                    USART4_RX_STA++;
                    if(USART4_RX_STA>(USART4_REC_LEN-1))USART4_RX_STA=0;//接收数据错误,重新开始接收	  
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
	i=strlen((const char*)USART4_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
		while((UART4->SR&0X40)==0);			//循环发送,直到发送完毕   
		UART4->DR=USART4_TX_BUF[j];  
	} 
}

void uart4_test(void)
{
	u32 len,t;
	static u32 times=0;
	if(USART4_RX_STA&0x8000)
	{					   
		len=USART4_RX_STA&0x3fff;//得到此次接收到的数据长度
		u4_printf("\r\n usart4:您发送的消息为:\r\n");
		for(t=0;t<len;t++)
        {
            USART_SendData(UART4, USART4_RX_BUF[t]);//向串口2发送数据
            while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
        }
		u4_printf("\r\n\r\n");//插入换行
		USART4_RX_STA=0;
	}else
	{
		times++;
		if(times%200==0)u4_printf("usart4:请输入数据,以回车键结束\r\n");  
	}
}
