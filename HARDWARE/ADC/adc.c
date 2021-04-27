 #include "adc.h"
 #include "dma.h"
 #include "delay.h"
 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//ADC ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
__IO uint16_t ADC_ConvertedValue[50][2];// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
float ADC_ConvertedValueLocal[2]; // �ֲ����������ڱ���ת�������ĵ�ѹֵ
		   
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	//ʹ��ADC1ʱ��
	
  ADC_DeInit(ADC1);  //��λADC1 ��ADC1�ӿڸ�λ����RCC_APB2RSTR�Ĵ�����

	//��ʼ��ADCx���� 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//ģ��ת��������ɨ��ģʽ(��ͨ��)
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת������������ת��ģʽ
//ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ��루���Ҷ�����ʲô���𣿣���
	ADC_InitStructure.ADC_NbrOfChannel = 2;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���  
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //����ADC��Ƶ����6��72M/6=12,ADC����ܳ���14M 

	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5 );
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5 );

  
  ADC_DMACmd(ADC1, ENABLE);//����ADC��DMA֧�֡�ʹ��ADC DMA ����
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼   У׼AD��ʱ��ᴥ��DMA����ͨ����λ,���У׼ADǰ��Ҫ����DMA
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����

	ADC_StartCalibration(ADC1);	 //����ADУ׼   
	while(ADC_GetCalibrationStatus(ADC1)); //�ȴ�У׼����
	 
////��ʼ��DMA1���� 	
	MYDMA_Config_Word(DMA1_Channel1,(u32)&ADC1->DR,(u32)ADC_ConvertedValue,100);
///////////////////////////////////////////////////////////////////////////////////////////////////	

  DMA_Cmd(DMA1_Channel1, ENABLE);  //����DMAͨ��
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	// ����û�в����ⲿ����������ʹ���������ADCת��	
}

int N=50;
u16 Filter(u8 ch)  //˫ͨ����ֵ�˲�������
{
	int count;
	for(count=1;count<N;count++)   //ȡƽ��ֵ
   {  
      ADC_ConvertedValueLocal[ch]+=ADC_ConvertedValue[count][ch];    
   }
   ADC_ConvertedValueLocal[ch]=ADC_ConvertedValueLocal[ch]/N;
	 return ADC_ConvertedValueLocal[ch];	//�������һ��ADC1ת�����
}
/*
void bubbleSort(uint8_t column) 				 //ð�ݷ������㷨���Ӵ�С����
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
