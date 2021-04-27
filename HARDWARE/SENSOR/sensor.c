#include "sensor.h"
#include "usart.h"
#include "math.h"
#include "adc.h"
#include "dma.h"

UPLOAD_STATUS SENSOR_DATA;
u8 SendBuff[119];
//CPUֱ�ӿ��Ʒ��ʹ��������ݺ���
void SEND_SENSOR_DATA(void)
{
	//����DMA���Ʒ�������
	int i;
	static int j = 0;
	SendBuff[0] = 0xcd;
	SendBuff[1] = 0xeb;
	SendBuff[2] = 0xd7;
	SendBuff[3] = sizeof(SENSOR_DATA)/4*5;
  for(i = 0; i < SENSOR_DLEN; i++)
  {
		SendBuff[4+i*5] = *(((u8 *)(&SENSOR_DATA))+i*4+0); //
		SendBuff[5+i*5] = *(((u8 *)(&SENSOR_DATA))+i*4+1); //
		SendBuff[6+i*5] = *(((u8 *)(&SENSOR_DATA))+i*4+2); //
		SendBuff[7+i*5] = *(((u8 *)(&SENSOR_DATA))+i*4+3); //
		SendBuff[8+i*5] =  0x20;
  }
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //ʹ�ܴ���1��DMA����
	MYDMA_Enable(DMA1_Channel4,119);//����һ�δ�������
	
	delta[j] = SENSOR_DATA.encoder_delta_car;
	j = j < 99? (j+1) : 0;
	SENSOR_DATA.time_stamp++;//ʱ�����1
	Milestones();
  SENSOR_DATA.encoder_delta_car = 0; //�������������м�λ����λ���ղ�������ֵ
	
}

float alpha,beta,vf,vv,theta,vx,vy,delta_x,delta_y,delta_th,w;//���ӽǶȣ���������ٶȣ����������ٶȣ����ӵĺ����ٶȣ����ӵ������ٶ�,�ۼ�x���,�ۼ�y���,�ۼƳ���Ƕ�,������ٶ�
float PI=3.1415926, delta_t=0.02, l=1.02;

void Milestones(void)  //��̼Ƽ���
{
	double v_steering = PI*0.183/(84.2/((SENSOR_DATA.encoder_delta_car/(20*0.001))/2000));  //figure out the speed by encoder
	beta = SENSOR_DATA.theta;//the steering angle
	if (0 <= beta < 90)
	{
		vf= v_steering * cos(beta* PI / 180.0f);
		vv= -v_steering * sin(beta* PI / 180.0f);
		theta = alpha - beta ;//degree //
		vx= v_steering * cos(theta* PI / 180.0f);
		vy= v_steering * sin(theta* PI / 180.0f);
	}
	else if (90 <= beta < 135) 
	{
		vf=  v_steering * cos(beta* PI / 180.0f);
		vv= -v_steering * sin(beta* PI / 180.0f);
		theta = alpha - beta ;//degree
		vx= v_steering * cos(theta* PI / 180.0f);
		vy= v_steering * sin(theta* PI / 180.0f);
	}
	else if (-135 <= beta < -90) 
	{
		vf=  v_steering * cos(beta* PI / 180.0f);
		vv= -v_steering * sin(beta* PI / 180.0f);
		theta = alpha -beta;//degree
		vx= v_steering * cos(theta* PI / 180.0f);
		vy= v_steering * sin(theta* PI / 180.0f);
	}
	else if (-90 <= beta < 0) 
	{
		vf=  v_steering * cos(beta* PI / 180.0f);
		vv= -v_steering * sin(beta* PI / 180.0f);
		theta = alpha - beta ;//degree
		vx= v_steering * cos(theta* PI / 180.0f);
		vy= v_steering * sin(theta* PI / 180.0f);
	}
	
	SENSOR_DATA.omga_l = vf;
	SENSOR_DATA.omga_r = vv;
	w = vv / l;// the unit is rad/s
	SENSOR_DATA.distance4 = w;
  alpha += w * delta_t * 180.0f / PI;//degree
	
 if (v_steering != 0.0)
	{ // CarPos2D.theta is the initial and last angle.While car_status.theta is the current angle
		delta_x = vx * delta_t;
		delta_y = vy * delta_t;
		delta_th= w *delta_t;//the angle's unit is rad 
	}
	else
	{
		delta_x =0.0;//ͨ�����ַ�ʽ�������
		delta_y = 0.0;//�����˶�ģ��������
		delta_th = 0.0;
	}
	SENSOR_DATA.distance1 +=delta_x;
	SENSOR_DATA.distance2 +=delta_y;
	SENSOR_DATA.distance3 +=delta_th * 180.0f /PI;//the car during dtʱ����ת���ĽǶ�

	if (SENSOR_DATA.distance3 > 360)							//limit the angle to (0-360)
		 SENSOR_DATA.distance3 -= 360;
	if (SENSOR_DATA.distance3 < 0)
		 SENSOR_DATA.distance3 += 360;

}

void send_frame_10byte(int *d)
{
	int i;
	u8 send_d[43];
	send_d[ 0] = 0xff;
	send_d[ 1] = 0xa5;
	send_d[ 2] = 0x5a;
	for(i = 0; i < 10; i++)
	{
		send_d[3+i*4] = *((u8 *)d+i*4+3);
		send_d[4+i*4] = *((u8 *)d+i*4+2);
		send_d[5+i*4] = *((u8 *)d+i*4+1);
		send_d[6+i*4] = *((u8 *)d+i*4+0);
	}
	USART_Send_Data(USART2,send_d,43);
}
//��ȡ���������ݺ���
void GET_SENSOR_DATA(void)
{
#ifdef DEBUG_MODE1
	SENSOR_DATA.status = 1;
	SENSOR_DATA.power = 12.6;
	SENSOR_DATA.theta = 0;
	SENSOR_DATA.encoder_ppr = 50;
	SENSOR_DATA.encoder_delta_r = 25;
	SENSOR_DATA.encoder_delta_l = 25;
	SENSOR_DATA.encoder_delta_car =280;
	SENSOR_DATA.omga_r = 76.8;
	SENSOR_DATA.omga_l = 76.8;
	
	SENSOR_DATA.distance1 = 1.36;
	SENSOR_DATA.distance2 = 1.37;
	SENSOR_DATA.distance3 = 1.38;
	SENSOR_DATA.distance4 = 1.39;
	SENSOR_DATA.IMU[0] = 5.12;
	SENSOR_DATA.IMU[1] = 6.12;
	SENSOR_DATA.IMU[2] = 7.12;
	SENSOR_DATA.IMU[3] = 8.12;
	SENSOR_DATA.IMU[4] = 9.12;
	SENSOR_DATA.IMU[5] = 10.12;
	SENSOR_DATA.IMU[6] = 11.12;
	SENSOR_DATA.IMU[7] = 12.12;
	SENSOR_DATA.IMU[8] = 13.12;
	SENSOR_DATA.time_stamp = 26666;
#endif
	ADC_Cmd(ADC1, ENABLE);			 		//ʹ��ָ����ADC1
	MYDMA_Enable(DMA1_Channel1,100); //����һ��ADC˫ͨ������
	Get_mpu9250();

}

void Get_power(void)
{
	u16 power_adc;
	power_adc = Filter(1);//��ȡ��Դ��ѹֵ//��ȡ��ѹֵ
	SENSOR_DATA.power = (float)power_adc*(3.3/4096); //2^12=4096����3.3V��Ϊ4096��
}


void Get_mpu9250(void)
{
//	float pitch,roll,yaw; 	        //ŷ����
//	short aacx,aacy,aacz;	          //���ٶȴ�����ԭʼ����
//	short gyrox,gyroy,gyroz;        //������ԭʼ���� 
	u8 i;
	u8 INS[USART_REC_LEN];
	u8 frame_length;
	//�����ж��¼�����
	if(USART3_RX_STA == 2)			//�Ѿ�������һ֡����֡
	{
		for(i = 0; i < USART_REC_LEN; i++)//ָ��֡���
		{
			INS[i] = USART3_RX_BUF[i];
		}
		USART3_RX_STA = 0;
		frame_length = INS[0];
		switch(frame_length)
		{
			case 0x09:
				SENSOR_DATA.IMU[0] = INS[1];	//pitch
				SENSOR_DATA.IMU[1] = INS[2];	//roll;
				SENSOR_DATA.IMU[2] = INS[3];	//yaw;
				SENSOR_DATA.IMU[3] = INS[4];	//(float)aacx;
				SENSOR_DATA.IMU[4] = INS[5];	//(float)aacy;
				SENSOR_DATA.IMU[5] = INS[6];	//(float)aacz;
				SENSOR_DATA.IMU[6] = INS[7];	//(float)gyrox;
				SENSOR_DATA.IMU[7] = INS[8];	//(float)gyroy;
				SENSOR_DATA.IMU[8] = INS[9];	//(float)gyroz;  
			
				USART_Send_Data(USART2,INS,10);
			break;
			default:break;
		}
	}
}
