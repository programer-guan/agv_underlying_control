#include "hmi.h"
#include "sys.h"
#include "sensor.h"
#include "lcd1602.h"
#include "motor.h"

HMI_MAIN HMI_M;
int count;

/**************************************/
/*          HMIϵͳ��ʼ������         */
/**************************************/
//���ܣ���ʼ���˵��б������ָ��
void HMI_INIT(void)
{
  static HMI_MENU M_MAIN;                         //�˵�����
	static HMI_MENU M_MENU[8];                      //��ҳ��˵�ָ�붨��
	static HMI_PARA para_status[1],para_power[1],para_theta[1],para_encoder[4],para_omga[2],para_distance[4],para_imu[9],para_control[5];

	HMI_M.PUTC = &LCD1602_Show_char;
	HMI_M.CLRAR = &LCD1602_clear;

//�������	
	//С��״̬
	para_status[0].Name = "STATUS";
	para_status[0].ValType = HMI_VAL_SINT;
	para_status[0].Val.PSI = &(SENSOR_DATA.status);
	para_status[0].WRS = HMI_WRS_RO;
	
  //��Դ����
	para_power[0].Name = "POWER";
	para_power[0].ValType = HMI_VAL_FLOAT;
	para_power[0].Val.PF = &(SENSOR_DATA.power);
	para_power[0].WRS = HMI_WRS_RO;
	
	//��λ��
	para_theta[0].Name = "THETA";
	para_theta[0].ValType = HMI_VAL_FLOAT;
	para_theta[0].Val.PF = &(SENSOR_DATA.theta);
	para_theta[0].WRS = HMI_WRS_RO;
	
	//�������鿴��ʼ��
  para_encoder[0].Name = "PPR";					                    //��ʾ�ı�����
  para_encoder[0].ValType = HMI_VAL_SINT;                   //��������
  para_encoder[0].Val.PSI = &(SENSOR_DATA.encoder_ppr);     //�����ĵ�ַ
  para_encoder[0].WRS = HMI_WRS_RO;                         //�Ƿ��ͨ���ⲿ����������ֵ

	para_encoder[1].Name = "DELTA_R";							
  para_encoder[1].ValType = HMI_VAL_SINT;             
  para_encoder[1].Val.PSI = &(SENSOR_DATA.encoder_delta_r);          
  para_encoder[1].WRS = HMI_WRS_RO;    
	
	
	para_encoder[2].Name = "DELTA_L";							
  para_encoder[2].ValType = HMI_VAL_SINT;             
  para_encoder[2].Val.PSI = &(SENSOR_DATA.encoder_delta_l);          
  para_encoder[2].WRS = HMI_WRS_RO;  

	para_encoder[3].Name = "D_CAR";							
  para_encoder[3].ValType = HMI_VAL_SINT;             
  para_encoder[3].Val.PSI = &(SENSOR_DATA.encoder_delta_car);          
  para_encoder[3].WRS = HMI_WRS_RO;                

  //����ת��
	para_omga[0].Name = "OMGA_R";
	para_omga[0].ValType = HMI_VAL_FLOAT;
	para_omga[0].Val.PF = &(SENSOR_DATA.omga_r);
	para_omga[0].WRS = HMI_WRS_RO;

	para_omga[1].Name = "OMGA_L";
	para_omga[1].ValType = HMI_VAL_FLOAT;
	para_omga[1].Val.PF = &(SENSOR_DATA.omga_l);
	para_omga[1].WRS = HMI_WRS_RO;

  //��������
	para_distance[0].Name = "DIS0";
	para_distance[0].ValType = HMI_VAL_FLOAT;
	para_distance[0].Val.PF = &(SENSOR_DATA.distance1);
	para_distance[0].WRS = HMI_WRS_RO;
	
	para_distance[1].Name = "DIS1";
	para_distance[1].ValType = HMI_VAL_FLOAT;
	para_distance[1].Val.PF = &(SENSOR_DATA.distance2);
	para_distance[1].WRS = HMI_WRS_RO;
	
	para_distance[2].Name = "DIS2";
	para_distance[2].ValType = HMI_VAL_FLOAT;
	para_distance[2].Val.PF = &(SENSOR_DATA.distance3);
	para_distance[2].WRS = HMI_WRS_RO;

	para_distance[3].Name = "DIS3";
	para_distance[3].ValType = HMI_VAL_FLOAT;
	para_distance[3].Val.PF = &(SENSOR_DATA.distance4);
	para_distance[3].WRS = HMI_WRS_RO;

	
	//��������
	para_imu[0].Name = "IMU0";
	para_imu[0].ValType = HMI_VAL_FLOAT;
	para_imu[0].Val.PF = &(SENSOR_DATA.IMU[0]);
	para_imu[0].WRS = HMI_WRS_RO;

	para_imu[1].Name = "IMU1";
	para_imu[1].ValType = HMI_VAL_FLOAT;
	para_imu[1].Val.PF = &(SENSOR_DATA.IMU[1]);
	para_imu[1].WRS = HMI_WRS_RO;
	
	para_imu[2].Name = "IMU2";
	para_imu[2].ValType = HMI_VAL_FLOAT;
	para_imu[2].Val.PF = &(SENSOR_DATA.IMU[2]);
	para_imu[2].WRS = HMI_WRS_RO;

	para_imu[3].Name = "IMU3";
	para_imu[3].ValType = HMI_VAL_FLOAT;
	para_imu[3].Val.PF = &(SENSOR_DATA.IMU[3]);
	para_imu[3].WRS = HMI_WRS_RO;

	para_imu[4].Name = "IMU4";
	para_imu[4].ValType = HMI_VAL_FLOAT;
	para_imu[4].Val.PF = &(SENSOR_DATA.IMU[4]);
	para_imu[4].WRS = HMI_WRS_RO;

	para_imu[5].Name = "IMU5";
	para_imu[5].ValType = HMI_VAL_FLOAT;
	para_imu[5].Val.PF = &(SENSOR_DATA.IMU[5]);
	para_imu[5].WRS = HMI_WRS_RO;
	
	para_imu[6].Name = "IMU6";
	para_imu[6].ValType = HMI_VAL_FLOAT;
	para_imu[6].Val.PF = &(SENSOR_DATA.IMU[6]);
	para_imu[6].WRS = HMI_WRS_RO;

	para_imu[7].Name = "IMU7";
	para_imu[7].ValType = HMI_VAL_FLOAT;
	para_imu[7].Val.PF = &(SENSOR_DATA.IMU[7]);
	para_imu[7].WRS = HMI_WRS_RO;

	para_imu[8].Name = "IMU8";
	para_imu[8].ValType = HMI_VAL_FLOAT;
	para_imu[8].Val.PF = &(SENSOR_DATA.IMU[8]);
	para_imu[8].WRS = HMI_WRS_RO;	
	
	//�˶�����
	para_control[0].Name = "FOWARD";					                    //��ʾ�ı�����
	para_control[0].ValType = HMI_VAL_SINT;
	para_control[0].Val.PSI = &(count);
	para_control[0].WRS = HMI_WRS_RO;	
		
	para_control[1].Name = "BACK";							  
	para_control[1].ValType = HMI_VAL_SINT;
	para_control[1].Val.PSI = &(count);
	para_control[1].WRS = HMI_WRS_RO;	
		
	para_control[2].Name = "STOP";							
	para_control[2].ValType = HMI_VAL_SINT;
	para_control[2].Val.PSI = &(count);
	para_control[2].WRS = HMI_WRS_RO;	
		
	para_control[3].Name = "LEFT";							
 	para_control[3].ValType = HMI_VAL_SINT;
	para_control[3].Val.PSI = &(count);
	para_control[3].WRS = HMI_WRS_RO;	
		
	para_control[4].Name = "RIGHT";							
	para_control[4].ValType = HMI_VAL_SINT;
	para_control[4].Val.PSI = &(count);
	para_control[4].WRS = HMI_WRS_RO;	
	
//�����ҳ
	//��ҳ�����
	M_MENU[0].Name = "STATUS";
	M_MENU[0].ChildCount = 1;
	M_MENU[0].ChildPoint = 0;
	M_MENU[0].Type = HMI_TYPE_NUM;
	M_MENU[0].ParentMenus = &M_MAIN;
	M_MENU[0].ChildrenMenus = 0;
	M_MENU[0].ChildPara = para_status;
	M_MENU[0].KEY_CALLBACK = &HMI_KEY_CALLBACK;

	M_MENU[1].Name = "POWER";
	M_MENU[1].ChildCount = 1;
	M_MENU[1].ChildPoint = 0;
	M_MENU[1].Type = HMI_TYPE_NUM;
	M_MENU[1].ParentMenus = &M_MAIN;
	M_MENU[1].ChildrenMenus = 0;
	M_MENU[1].ChildPara = para_power;
	M_MENU[1].KEY_CALLBACK = &HMI_KEY_CALLBACK;
	
	M_MENU[2].Name = "THETA";
	M_MENU[2].ChildCount = 1;
	M_MENU[2].ChildPoint = 0;
	M_MENU[2].Type = HMI_TYPE_NUM;
	M_MENU[2].ParentMenus = &M_MAIN;
	M_MENU[2].ChildrenMenus = 0;
	M_MENU[2].ChildPara = para_theta;
	M_MENU[2].KEY_CALLBACK = &HMI_KEY_CALLBACK;
	
	//������
	M_MENU[3].Name = "ENCODER";                             //�Ӽ��˵���
	M_MENU[3].ChildCount = 4;                               //��������
	M_MENU[3].ChildPoint = 0;                               //���ݳ�ʼ��ָ��
	M_MENU[3].Type = HMI_TYPE_NUM;                          //��������
	M_MENU[3].ParentMenus = &M_MAIN;                        //�����˵�
	M_MENU[3].ChildrenMenus = 0;                            //�����Ӳ˵�
	M_MENU[3].ChildPara = para_encoder;                    	//���������ݱ���
	M_MENU[3].KEY_CALLBACK = &HMI_KEY_CALLBACK;             //�˵��ڰ����ص�����

	M_MENU[4].Name = "OMGA";
	M_MENU[4].ChildCount = 2;
	M_MENU[4].ChildPoint = 0;
	M_MENU[4].Type = HMI_TYPE_NUM;
	M_MENU[4].ParentMenus = &M_MAIN;
	M_MENU[4].ChildrenMenus = 0;
	M_MENU[4].ChildPara = para_omga;
	M_MENU[4].KEY_CALLBACK = &HMI_KEY_CALLBACK;

	M_MENU[5].Name = "DISTANCE";
	M_MENU[5].ChildCount = 4;
	M_MENU[5].ChildPoint = 0;
	M_MENU[5].Type = HMI_TYPE_NUM;
	M_MENU[5].ParentMenus = &M_MAIN;
	M_MENU[5].ChildrenMenus = 0;
	M_MENU[5].ChildPara = para_distance;
	M_MENU[5].KEY_CALLBACK = &HMI_KEY_CALLBACK;

  M_MENU[6].Name = "IMU";
	M_MENU[6].ChildCount = 9;
	M_MENU[6].ChildPoint = 0;
	M_MENU[6].Type = HMI_TYPE_NUM;
	M_MENU[6].ParentMenus = &M_MAIN;
	M_MENU[6].ChildrenMenus = 0;
	M_MENU[6].ChildPara = para_imu;
	M_MENU[6].KEY_CALLBACK = &HMI_KEY_CALLBACK;
	
	//������
	M_MENU[7].Name = "CONTROL";
	M_MENU[7].ChildCount = 5;
	M_MENU[7].ChildPoint = 0;
	M_MENU[7].Type = HMI_TYPE_NUM;
	M_MENU[7].ParentMenus = &M_MAIN;
	M_MENU[7].ChildrenMenus = 0;
	M_MENU[7].ChildPara = para_control;
	M_MENU[7].KEY_CALLBACK = &HMI_CONTROL_CALLBACK;

	
 //���˵�
  M_MAIN.Type = HMI_TYPE_MENU;
  M_MAIN.KEY_CALLBACK = &HMI_KEY_CALLBACK;
  M_MAIN.Name = "MENU";
  M_MAIN.ChildCount = 8;
  M_MAIN.ChildPoint = 0;
  M_MAIN.ChildrenMenus = M_MENU;
	
	HMI_M.Menu = &M_MAIN;
}

//HMI�����¼��ص�����
void HMI_KEY_EVENT(unsigned char key_code)
{
	u8 key = 0;
	HMI_MENU *pm = HMI_M.Menu;
	if(key_code & 0x01) key = HMI_KEY_BACK;
	else if(key_code & 0x02) key = HMI_KEY_OK;
	else if(key_code & 0x04) key = HMI_KEY_LEFT;
	else if(key_code & 0x08) key = HMI_KEY_RIGHT;
	else if(key_code & 0x10) key = HMI_KEY_UP;
	else if(key_code & 0x20) key = HMI_KEY_DOWN;
	
	pm->KEY_CALLBACK(key);
}

/**************************************************************************/
/*        			    Ĭ�ϰ�������            */
/**************************************************************************/
//Ĭ�ϲ˵�����ֵ�����ص�����
void HMI_KEY_CALLBACK(unsigned char key_code)
{
	if(key_code == HMI_KEY_BACK){						//����BACK��
		if(HMI_M.Menu->ParentMenus != 0){		  //���ظ��˵�
			HMI_M.Menu = HMI_M.Menu->ParentMenus;
		}
	}
	if(key_code == HMI_KEY_OK){					  	//	����OK��
	    switch(HMI_M.Menu->Type){
        case HMI_TYPE_MENU:                                //�˵���������Ӳ˵�
            if(HMI_M.Menu->ChildrenMenus+HMI_M.Menu->ChildPoint != 0){
                HMI_M.Menu = HMI_M.Menu->ChildrenMenus+HMI_M.Menu->ChildPoint;    //ָ����һ��˵�
            }
            break;
        case HMI_TYPE_NUM:                          //�ַ��ͱ�����������Ϊ�ɶ����޸ı���
            if(HMI_M.Menu->ChildPara[HMI_M.Menu->ChildPoint].WRS == HMI_WRS_RW){         //����Ϊ�ɸ���

            }
            break;
        case HMI_TYPE_WAVE:                           //�����
            break;
        default:
            break;
	    }
	}

	if(key_code == HMI_KEY_DOWN){						//����DOWN��
        switch(HMI_M.Menu->Type){
        case HMI_TYPE_MENU:                                  //�˵�ָ�������ƶ�
            if(HMI_M.Menu->ChildPoint < HMI_M.Menu->ChildCount-1){
                HMI_M.Menu->ChildPoint += 1;            //ָ����һ�в˵�
            }
            else{
                HMI_M.Menu->ChildPoint = 0;
            }
            break;
        case HMI_TYPE_NUM:                              //ָ����һ����
            if(HMI_M.Menu->ChildPoint < HMI_M.Menu->ChildCount-1){
                HMI_M.Menu->ChildPoint += 1;            //ָ����һ�в˵�
            }
            else{
                HMI_M.Menu->ChildPoint = 0;
            }
            break;
        case HMI_TYPE_WAVE:
            break;
        default:
            break;
	    }
	}
	if(key_code == HMI_KEY_UP){						//����UP��
	    switch(HMI_M.Menu->Type){
        case HMI_TYPE_MENU:                                //�˵�ָ�������ƶ�
            if(HMI_M.Menu-> ChildPoint != 0){
                HMI_M.Menu-> ChildPoint -= 1;             	//ָ����һ�в˵�
            }
            else{   //ѭ���б�
                HMI_M.Menu -> ChildPoint = HMI_M.Menu->ChildCount - 1;
            }
            break;
        case HMI_TYPE_NUM:                                 //ָ����һ����
            if(HMI_M.Menu->ChildPoint != 0){
                HMI_M.Menu->ChildPoint -= 1;             	//ָ����һ����
            }
            else{                                           //ѭ���б�
                HMI_M.Menu -> ChildPoint = HMI_M.Menu->ChildCount - 1;
            }
            break;
        case HMI_TYPE_WAVE:
            break;
        default:
            break;
	    }
	}
	else if(key_code == HMI_KEY_RIGHT){			//����RIGHT��
        switch(HMI_M.Menu->Type){
        case HMI_TYPE_MENU:                                   //�˵���������Ӳ˵�
            if(HMI_M.Menu->ChildrenMenus+HMI_M.Menu->ChildPoint != 0){
                HMI_M.Menu = HMI_M.Menu->ChildrenMenus + HMI_M.Menu->ChildPoint;     //ָ����һ��˵�
            }
            break;
        case HMI_TYPE_NUM:                        //�ַ��ͱ�����������Ϊ�ɶ����޸ı���
            if(HMI_M.Menu->ChildPara[HMI_M.Menu->ChildPoint].WRS == HMI_WRS_RW){         //����Ϊ�ɸ���
							
            }
            break;
        case HMI_TYPE_WAVE:                          //�����
            break;
        default:
            break;
	    }
	}
	else if(key_code == HMI_KEY_LEFT){			//����LEFT��
        if(HMI_M.Menu->ParentMenus != 0){	//���ظ��˵�
			HMI_M.Menu = HMI_M.Menu->ParentMenus;
		}
	}
}

/*ֱ�ӿ��ƻص�����*/
///BACK			OK
///LEFT RIGHT UP DOWN 
///
void HMI_CONTROL_CALLBACK(unsigned char key_code)
{
	if(key_code == HMI_KEY_BACK){						//����BACK��
		count = 0;
		if(HMI_M.Menu->ParentMenus != 0){		  //���ظ��˵�
			HMI_M.Menu = HMI_M.Menu->ParentMenus;
		}
	}
	if(key_code == HMI_KEY_OK){					  	//����OK��
			count =	1;
	    switch(HMI_M.Menu->Type){
        case HMI_TYPE_MENU:                          //�˵���������Ӳ˵�
            if(HMI_M.Menu->ChildrenMenus+HMI_M.Menu->ChildPoint != 0){
                HMI_M.Menu = HMI_M.Menu->ChildrenMenus+HMI_M.Menu->ChildPoint;    //ָ����һ��˵�
            }
            break;
        case HMI_TYPE_NUM:                          //�ַ��ͱ�����������Ϊ�ɶ����޸ı���
            if(HMI_M.Menu->ChildPara[HMI_M.Menu->ChildPoint].WRS == HMI_WRS_RW){  //����Ϊ�ɸ���
            }
            break;
        case HMI_TYPE_WAVE:                           //�����
            break;
        default:
            break;
	    }
	}

	if(key_code == HMI_KEY_DOWN){		//����DOWN��			���ٺ���
		MOTOR_STRAIGHT(-25);
		MOTOR_DIRECT(1,0);
	}
	else if(key_code == HMI_KEY_UP){//����UP��				����ǰ��
	   MOTOR_STRAIGHT(25);
		 MOTOR_DIRECT(1,0);
	}
	else if(key_code == HMI_KEY_RIGHT){//����RIGHT��	 ��ת��  -
		MOTOR_STRAIGHT(0);
		MOTOR_DIRECT(1,15);
	}
	else if(key_code == HMI_KEY_LEFT){//����LEFT��		��ת��  +
		MOTOR_STRAIGHT(0);
		MOTOR_DIRECT(1,-15);
	}
	else {
		MOTOR_STRAIGHT(0);
		MOTOR_DIRECT(1,0);
	}
}
/**************************************/
/*            ��Ļˢ�º���            */
/**************************************/
//���ܣ�һ�����ڵ��ã�����HMI�豸����ˢ����ʾ
void HMI_REFRESH(void)
{
	unsigned char i;
	unsigned char lh;
	HMI_MENU* pm = HMI_M.Menu;

	HMI_M.CLRAR();

	HMI_M.x = 0;
	HMI_M.y = 0;
    lh = (pm->ChildCount < SCREEN_HEIGHT-1)?(pm->ChildCount):(SCREEN_HEIGHT-1);
	if(pm->ChildPoint < SCREEN_HEIGHT-1){         //ָ��ָ�����ݿ��ڽ�������ʾ
        HMI_PRINT_STR(pm->Name);                //��ʾ�˵���
        HMI_M.y = 1;
        if(pm->ChildCount != 0){						//��ʾ���ݱ�ǩ
            switch(pm->Type){
            case HMI_TYPE_MENU:
                for(i = 0; i < lh; i++){
                    HMI_M.x = 0;
                    HMI_PRINT_STR((pm->ChildrenMenus+i)->Name);
                    if(i == pm->ChildPoint){
                        HMI_M.x = SCREEN_WIDTH-2;
                        HMI_PRINT_STR("<=");
                    }
                    HMI_M.y += 1;
                }
                break;
            case HMI_TYPE_NUM:              //��ʾ���ݱ���
                for(i = 0; i < lh; i++){
                    HMI_M.x = 0;
                    HMI_PRINT_PARA(pm->ChildPara+i);
                    if(i == pm->ChildPoint){
                        HMI_M.x = SCREEN_WIDTH-2;
                        HMI_PRINT_STR("<=");
                    }
                    HMI_M.y += 1;
                }
                break;
            case HMI_TYPE_WAVE:
                break;
            }
        }
    }
    else{                                         //ָ�볬������
        if(pm->ChildCount != 0){
            switch(pm->Type){
            case HMI_TYPE_MENU:
                for(i = pm->ChildPoint - SCREEN_HEIGHT+1; i <= pm->ChildPoint; i++){
                    HMI_M.x = 0;
                    HMI_PRINT_STR((pm->ChildrenMenus+i)->Name);
                    if(i == pm->ChildPoint){
                        HMI_M.x = SCREEN_WIDTH-2;
                        HMI_PRINT_STR("<=");
                    }
                    HMI_M.y += 1;
                }
                break;
            case HMI_TYPE_NUM:                     //��ʾ���ݱ���
                for(i = pm->ChildPoint-SCREEN_HEIGHT+1; i <= pm->ChildPoint; i++){
                    HMI_M.x = 0;
                    HMI_PRINT_PARA(pm->ChildPara+i);
                    if(i == pm->ChildPoint){
                        HMI_M.x = SCREEN_WIDTH-2;
                        HMI_PRINT_STR("<=");
                    }
                    HMI_M.y += 1;
                }
                break;
            case HMI_TYPE_WAVE:
                break;
            }
        }
    }
}

/**************************************/
/*            ������ʾ����            */
/**************************************/
//����p:��Ҫ��ʾ�ı�������
void HMI_PRINT_PARA(HMI_PARA* p)
{
	HMI_PRINT_STR(p->Name);
	switch(p->ValType)
	{
		case HMI_VAL_SINT:
			HMI_PRINT_NUM(*(p->Val.PSI));
			break;
		case HMI_VAL_UINT:
			HMI_PRINT_NUM(*(p->Val.PUI));
			break;
		case HMI_VAL_FLOAT:
			HMI_PRINT_FLOAT(*(p->Val.PF));
			break;
		case HMI_VAL_SCHAR:
			HMI_PRINT_NUM(*(p->Val.PSC));
			break;
		case HMI_VAL_UCHAR:
			HMI_PRINT_NUM(*(p->Val.PUC));
			break;
	}
}

/**************************************/
/*            �ַ���ӡ����            */
/**************************************/
//����s:��ӡ�ַ�ָ��
void HMI_PRINT_STR(char* s)
{
	while(*s != '\0'){
		HMI_M.PUTC(HMI_M.x++,HMI_M.y,*(s++));
	}
	HMI_M.PUTC(HMI_M.x++,HMI_M.y,' ');
}

/**************************************/
/*            �����ӡ����            */
/**************************************/
//num:��ӡ����������
void HMI_PRINT_NUM(int num)
{
    int i;
    char dat[6];
    if(num < 0){                                //��������ʾ
				HMI_M.PUTC(HMI_M.x++,HMI_M.y,'-');
        num = -num;
    }
    //��ʼ����ʾ�ַ�����
    for(i = 0; i < 6; i++) dat[i] = 0;
    i = 0;	//��ʼ������

    while(num / 10){							//��λ
        dat[i] = num % 10 + '0';    			//���λ
        num /= 10; i++;
    }
    dat[i] = num % 10 + '0';    		    	//���λ
    for( ; i >= 0; i--){
        HMI_M.PUTC(HMI_M.x++,HMI_M.y,dat[i]);
    }
}

/**************************************/
/*            �����ӡ����            */
/**************************************/
//f:��ӡ����������
//deci:��Ҫ��ʾ��������С����λ��
void HMI_PRINT_FLOAT(float f)
{
    unsigned char i;
    unsigned char deci = 5;            //������ʾС������λ��
    if(f < 0){                         //��������ʾ
			HMI_M.PUTC(HMI_M.x++,HMI_M.y,'-');
			f = -f;
		}
		
		HMI_PRINT_NUM((int)f);             //��ʾ�����͵�����λ

    if(deci > 0){
        HMI_M.PUTC(HMI_M.x++,HMI_M.y,'.');
    }
    for(i = 0; i < deci; i++){         
        f *= 10;
        HMI_M.PUTC(HMI_M.x++, HMI_M.y,(unsigned int)f%10+48);
    }
}
