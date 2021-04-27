 #include "adc.h"
 #include "dma.h"
 #include "delay.h"
 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//ADC 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
__IO uint16_t ADC_ConvertedValue[50][2];// ADC1转换的电压值通过MDA方式传到SRAM
float ADC_ConvertedValueLocal[2]; // 局部变量，用于保存转换计算后的电压值
		   
//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	//使能ADC1时钟
	
  ADC_DeInit(ADC1);  //复位ADC1 （ADC1接口复位——RCC_APB2RSTR寄存器）

	//初始化ADCx参数 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//模数转换工作在扫描模式(多通道)
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在连续转换模式
//ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐（左右对齐有什么区别？？）
	ADC_InitStructure.ADC_NbrOfChannel = 2;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器  
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //设置ADC分频因子6，72M/6=12,ADC最大不能超过14M 

	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5 );
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5 );

  
  ADC_DMACmd(ADC1, ENABLE);//开启ADC的DMA支持、使能ADC DMA 请求
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准   校准AD的时候会触发DMA导致通道错位,因此校准AD前不要启用DMA
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束

	ADC_StartCalibration(ADC1);	 //开启AD校准   
	while(ADC_GetCalibrationStatus(ADC1)); //等待校准结束
	 
////初始化DMA1参数 	
	MYDMA_Config_Word(DMA1_Channel1,(u32)&ADC1->DR,(u32)ADC_ConvertedValue,100);
///////////////////////////////////////////////////////////////////////////////////////////////////	

  DMA_Cmd(DMA1_Channel1, ENABLE);  //启动DMA通道
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	// 由于没有采用外部触发，所以使用软件触发ADC转换	
}

int N=50;
u16 Filter(u8 ch)  //双通道均值滤波处理函数
{
	int count;
	for(count=1;count<N;count++)   //取平均值
   {  
      ADC_ConvertedValueLocal[ch]+=ADC_ConvertedValue[count][ch];    
   }
   ADC_ConvertedValueLocal[ch]=ADC_ConvertedValueLocal[ch]/N;
	 return ADC_ConvertedValueLocal[ch];	//返回最近一次ADC1转换结果
}
/*
void bubbleSort(uint8_t column) 				 //冒泡法排序算法，从大到小排序
{  
		int i,j;
    int flag = 1;  
    for(i=0;i<N&&flag;i++) 
    {  
        for(j=0;j<N-1-i;j++) 
        {  
            flag = 0;  
            if(ADC_ConvertedValue[j][column]>ADC_ConvertedValue[j+1][column])
            {
                uint16_t temp = ADC_ConvertedValue[j][column];  
                ADC_ConvertedValue[j][column] = ADC_ConvertedValue[j+1][column];  
                ADC_ConvertedValue[j+1][column] = temp;  
                flag = 1;  
            } 
         }   
      } 
}
*/
