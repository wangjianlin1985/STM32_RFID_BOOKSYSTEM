/******************** 鑫盛电子工作室 ********************
 * 文件名  ：adc.c
 * 描述    ：DMA方式读取ADC值应用函数库         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接： PCA0 - ADC1 
 * 库版本  ：ST3.0.0  
 * 淘宝店：http://shop66177872.taobao.com
**********************************************************************************/

#include "glr_adc.h"

#define ADC1_DR_Address    ((u32)0x4001244C)

__IO u16 data_value[6];  

/*配置采样通道端口 使能GPIO时钟	  设置ADC采样PA0端口信号*/
 void ADC1_GPIO_Config(void)
{ GPIO_InitTypeDef GPIO_InitStructure;    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO设置为模拟输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
}


/*配置ADC1的工作模式为MDA模式  */
 void ADC1_Mode_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能MDA1时钟
	/* DMA channel1 configuration */
  DMA_DeInit(DMA1_Channel1);  //指定DMA通道
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;////ADC1地址---代表ADC1保存转换值的寄存器
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&data_value;	//设置DMA内存地址，ADC转换结果直接放入该地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设为设置为数据传输的来源
  DMA_InitStructure.DMA_BufferSize = 5;	//传输总数据---2通道需要传输2个数据
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址固定
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址自增---总体表示始终从外设ADC1地址处取值---依次保存到连续的两个内存变量中---
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //内存传输数据单元---半字16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式---2个数据依次循环接收从外设ADC1传输过来的ADC值---
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);  //使能DMA通道

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//使能ADC1时钟
     
  /* ADC1 configuration */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //使用独立模式，扫描模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE; //模数转换工作在扫描模式
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //无需外接触发器
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //使用数据右对齐
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel =5;  // ADC转换通道个数
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel11 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5); //通道1采样周期55.5个时钟周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5); 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 5, ADC_SampleTime_55Cycles5);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);	 //使能ADC的DMA
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE); //使能ADC1

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //开始转换
}

/*初始化ADC1 */
void ADC_INIT(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}




