#include "tsensor.h"
#include "delay.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//ADC ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/7 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
 
		   
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3	
void T_Adc_Init(void)  //ADCͨ����ʼ��
{
	ADC_InitTypeDef ADC_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	//ʹ��GPIOA,ADC1ͨ��ʱ��
  
    ADC_DeInit(ADC1);  //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ����λADC1��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //��Ƶ����6��ʱ��Ϊ72M/6=12MHz
 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

	ADC_TempSensorVrefintCmd(ENABLE); //�����ڲ��¶ȴ�����
	
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1

	ADC_ResetCalibration(ADC1);	//����ָ����ADC1�ĸ�λ�Ĵ�����ʹ�ܸ�λУ׼��
    while(ADC_GetResetCalibrationStatus(ADC1));	//��ȡADC1����У׼�Ĵ�����״̬,����״̬��ȴ����ȴ���λУ׼������

	ADC_StartCalibration(ADC1);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	//��ȡָ��ADC1��У׼����,����״̬��ȴ����ȴ�У׼������
	
}

u16 T_Get_Adc(u8 ch)   
	{
 
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );//ADC1,ADCͨ��ch,��һ��ת��,����ʱ��Ϊ239.5����	  			    
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
		
	}

	
//�õ�ADC�����ڲ��¶ȴ�������ֵ
//ȡTSENSOR_READ_TIMES��,Ȼ��ƽ��
u16 T_Get_Temp(void)
	{
	u16 temp_val=0;
	u8 t;
	for(t=0;t<TSENSOR_READ_TIMES;t++)
		{
		temp_val+=T_Get_Adc(ADC_Channel_16);  
		delay_ms(5);
		}
	return temp_val/TSENSOR_READ_TIMES;
	}

//��ȡͨ��ch��ת��ֵ
//ȡtimes��,Ȼ��ƽ��
u16 T_Get_Adc_Average(u8 ch,u8 times)
{	
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=T_Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
	
} 	   


//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��,��λ:��.)
double Get_Temprate(void)	//��ȡ�ڲ��¶ȴ������¶�ֵ  
{
	u32 adcx;
//	short result; //��C�������.̷��ǿ��P43��46
 	double temperate;
	adcx=T_Get_Adc_Average(ADC_Channel_16,20);	//��ȡͨ��16,20��ȡƽ��
	temperate=(float)adcx*(3.3/4096);		//��ѹֵ 
	temperate=(1.43-temperate)/0.0043+25;	//ת��Ϊ�¶�ֵ 	 
//	result=temperate*=100;					//����100��.
	return temperate;
}

