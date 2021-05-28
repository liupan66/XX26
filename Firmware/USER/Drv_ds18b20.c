/**********************************************
 * FileName:	drv_ds18b20.c
 *
 * FileBrief: 	ds18b20����
 *
 * Created on: 
 *
 * Author:   shanght
 *
 * Remark:
 ******************************************/

#include "global.h"

uint8_t	sensor_data_ipmb_read[48] = {0};

uint16_t	current_temperature[3] = {0};


#define HIGH	1
#define LOW		0
#define GPIO_TEMP1	1
#define GPIO_TEMP2	1
#define GPIO_TEMP3	1

#if GPIO_TEMP1
//#define DS18B20_DATA_OUT(a) if( HIGH == a)\
//		gpio_bit_set(GPIOC, GPIO_PIN_7);\
//		else \
//			gpio_bit_reset(GPIOC, GPIO_PIN_7);
			
#define DS18B20_DATA_IN() gpio_input_bit_get(GPIOC, GPIO_PIN_7)
#endif

#if GPIO_TEMP2
//#define DS18B20_DATA_OUT(a) if(a)\
//		gpio_bit_set(GPIOC,GPIO_PIN_1);\
//		else \
//			gpio_bit_reset(GPIOC,GPIO_PIN_1);
			
#define DS18B20_1_DATA_IN() gpio_input_bit_get(GPIOC, GPIO_PIN_6)
#endif

#if GPIO_TEMP2
//#define DS18B20_DATA_OUT(a) if(a)\
//		gpio_bit_set(GPIOC,GPIO_PIN_1);\
//		else \
//			gpio_bit_reset(GPIOC,GPIO_PIN_1);
			
#define DS18B20_2_DATA_IN() gpio_input_bit_get(GPIOB, GPIO_PIN_15)
#endif

/**************************************GPIO_TEMP1*********************************************/
//ds18b20 �豸���к�;������Ҫ����ʵ�ʵ��豸�޸����к�
uint8_t serial_1[8]={0x28,0x2d,0x9a,0xdd,0x02,0x00,0x00,0x3d};
uint8_t serial_2[8]={0x28,0x3b,0x2b,0xbc,0x02,0x00,0x00,0x4f};
uint8_t serial_3[8]={0x28,0x00,0x49,0x1b,0x03,0x00,0x00,0x4c};


/***************************************************
* ������	temp1_DS18B20_GPIO_Config
* ���ܣ�	ds18b20 gpio config
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static void temp1_DS18B20_GPIO_Config(void)
{
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
}

/***************************************************
* ������	temp1_DS18B20_Mode_IPU
* ���ܣ�	ds18b20 gpio input
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static void temp1_DS18B20_Mode_IPU(void) 
{
	gpio_bit_reset(GPIOC, GPIO_PIN_7);

	gpio_init(GPIOC, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
}

/***************************************************
* ������	temp1_DS18B20_Mode_OUT_PP
* ���ܣ�	ds18b20 gpio output
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static void temp1_DS18B20_Mode_OUT_PP(void) 
{
	gpio_bit_reset(GPIOC, GPIO_PIN_7);
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
	
}

/***************************************************
* ������	temp1_DS18B20_Rst
* ���ܣ�	ds18b20 reset
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static void temp1_DS18B20_Rst(void)
{
	temp1_DS18B20_Mode_OUT_PP();
	gpio_bit_reset(GPIOC, GPIO_PIN_7);
	delay_1us(750); 
	gpio_bit_set(GPIOC, GPIO_PIN_7);
	delay_1us(15); 
}

/***************************************************
* ������	temp1_DS18B20_Presence
* ���ܣ�	ds18b20 check ds18b20 pulse ���ӻ����͵�����
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static uint8_t temp1_DS18B20_Presence(void)
{
	uint8_t pluse_time = 0;
	temp1_DS18B20_Mode_IPU();	
	delay_1us(1);
	while(DS18B20_DATA_IN() && pluse_time<200)
	{
		pluse_time++;
		delay_1us(1);
		delay_1us(1);
	}
	
	if(pluse_time >= 200)
		return 1;
	else
		pluse_time = 0;
	
	while(!DS18B20_DATA_IN() && pluse_time<240)
	{
		pluse_time++;
		delay_1us(1);
		delay_1us(1);
	}
	
	if(pluse_time >= 240)
		return 1;
	
	return 0;
}

/***************************************************
* ������	temp1_DS18B20_Read_Bit
* ���ܣ�	read one bits from ds18b20 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static uint8_t temp1_DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	temp1_DS18B20_Mode_OUT_PP();
	gpio_bit_reset(GPIOC, GPIO_PIN_7);
	delay_1us(6);
	gpio_bit_set(GPIOC, GPIO_PIN_7); //sht
	delay_1us(9);
	temp1_DS18B20_Mode_IPU();
	
	gpio_bit_set(GPIOC, GPIO_PIN_7);
	delay_1us(1);
	
	if(DS18B20_DATA_IN())
		dat = 1;
	else
		dat = 0;
	delay_1us(60);

	return dat;
}

/***************************************************
* ������	temp1_DS18B20_Write_Zero
* ���ܣ�	wite 0 to ds18b20 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void temp1_DS18B20_Write_Zero(void)
{
	temp1_DS18B20_Mode_OUT_PP();
	gpio_bit_reset(GPIOC, GPIO_PIN_7);
	delay_1us(60);
	gpio_bit_set(GPIOC, GPIO_PIN_7);
	delay_1us(10);

}

/***************************************************
* ������	temp1_DS18B20_Write_One
* ���ܣ�	write 1 to ds18b20
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void temp1_DS18B20_Write_One(void)
{
	temp1_DS18B20_Mode_OUT_PP();
	gpio_bit_reset(GPIOC, GPIO_PIN_7);
	delay_1us(6);
	
	gpio_bit_set(GPIOC, GPIO_PIN_7);
	delay_1us(64);
}

/***************************************************
* ������	temp1_DS18B20_Read_Byte
* ���ܣ�	read one byte from ds18b20 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
uint8_t temp1_DS18B20_Read_Byte(void)
{
	uint8_t dat, dat_i, for_i;
	dat=0;
	for(for_i=0; for_i<8; for_i++)
	{
		dat_i = temp1_DS18B20_Read_Bit();
		dat = (dat>>1) | (dat_i<<7);
	}
	return dat;
}

/***************************************************
* ������	temp1_DS18B20_Write_Byte
* ���ܣ�	write one byte to ds18b20 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void temp1_DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t for_i,testb;
	
	temp1_DS18B20_Mode_OUT_PP();
	delay_1us(1);
	for(for_i=0; for_i<8; for_i++)
	{
		testb = dat&0x01;
		dat = dat>>1;
		if(testb)		
		{
			temp1_DS18B20_Write_One();
		}
		else
		{
			temp1_DS18B20_Write_Zero();
		}
		delay_1us(10);
	}
}

/***************************************************
* ������	temp1_DS18B20_start
* ���ܣ�	let ds18b20 start to convers 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void temp1_DS18B20_start(void)
{
	temp1_DS18B20_Rst();
	temp1_DS18B20_Presence();
	temp1_DS18B20_Write_Byte(0xCC); 
	temp1_DS18B20_Write_Byte(0x44); 
}

/***************************************************
* ������	dohitech_mcu_temp1_ds18b20_Init
* ���ܣ�	init ds18b20 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
uint8_t dohitech_mcu_temp1_ds18b20_Init(void)
{
	temp1_DS18B20_GPIO_Config();
	temp1_DS18B20_Rst();
	return temp1_DS18B20_Presence();
}

/***************************************************
* ������	DS18B20_Match_Serial
* ���ܣ�	pi pei xu lie hao
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void DS18B20_Match_Serial(uint8_t a)
{
	uint8_t for_i;
	temp1_DS18B20_Rst();
	temp1_DS18B20_Presence();
	temp1_DS18B20_Write_Byte(0X55); 
	
	if(a==1)
	{
		for(for_i=0; for_i<8; for_i++)
			temp1_DS18B20_Write_Byte(serial_1[for_i]);
	}
	else if(a==2)
	{
		for(for_i=0; for_i<8; for_i++)
			temp1_DS18B20_Write_Byte(serial_2[for_i]);
	}
	else if(a==3)
	{
		for(for_i=0; for_i<8; for_i++)
			temp1_DS18B20_Write_Byte(serial_3[for_i]);
	}
}

/***************************************************
* ������	DohitechMcuTemp1Ds18b20GetTemp
* ���ܣ�  get temperature
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*					��ȡ�¶�ֵ
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
float DohitechMcuTemp1Ds18b20GetTemp(void)
{
	uint8_t temp;
	uint8_t TL, TH;
	short tem;
	float tempN=0;
	temp1_DS18B20_start();
	temp1_DS18B20_Rst();
	if(temp1_DS18B20_Presence())
	{
			return 0.0;
	}
	temp1_DS18B20_Write_Byte(0xcc);
	temp1_DS18B20_Write_Byte(0xbe);
	TL = temp1_DS18B20_Read_Byte();
	TH = temp1_DS18B20_Read_Byte();
	if(TH>7)
	{
		TH=~TH;
		TL=~TL;
		temp =0; 
	}
	else
		temp= 1;
	tem=TH;	//��8λ
	tem<<=8;
	tem+=TL;
	//sensor_data_ipmb_read[16~17]
	sensor_data_ipmb_read[16] = TH;
	sensor_data_ipmb_read[17] = TL;
	current_temperature[0] = tem;
	tempN=(float)tem*0.0625;
	if(temp)
		return tempN;
	else
		return -tempN;
	
}

float ds18b20_test(void)
{
	float temp=0;
	if(0 == dohitech_mcu_temp1_ds18b20_Init())
	{	
		temp = DohitechMcuTemp1Ds18b20GetTemp();
//		temp *= 1000;
	//	printf("1�¶ȣ�%f��\n",temp);
	}
		return temp;
}

/***************************************************
* ������	dohitech_mcu_temp1_ds18b20_read_serial
* ���ܣ�	��ȡ���������к�
* ������                           in/out          ˵��
* 				��ȡ����buff
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void dohitech_mcu_temp1_ds18b20_read_serial(uint8_t * serial)
{
	uint8_t for_i;
	temp1_DS18B20_Rst();
	temp1_DS18B20_Presence();
	temp1_DS18B20_Write_Byte(0x33);
	for(for_i=0; for_i<8; for_i++)
	{
		serial[for_i] = temp1_DS18B20_Read_Byte();
	}
}
/**************************************GPIO_TEMP1*********************************************/


/**************************************GPIO_TEMP2*********************************************/
/***************************************************
* ������	temp2_DS18B20_GPIO_Config
* ���ܣ�	ds18b20 gpio config
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static void temp2_DS18B20_GPIO_Config(void)
{
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
}

/***************************************************
* ������	temp2_DS18B20_Mode_IPU
* ���ܣ�	ds18b20 gpio input
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static void temp2_DS18B20_Mode_IPU(void) 
{
	gpio_init(GPIOC, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_6);	
}

/***************************************************
* ������	temp2_DS18B20_Mode_OUT_PP
* ���ܣ�	ds18b20 gpio output
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static void temp2_DS18B20_Mode_OUT_PP(void) 
{
	gpio_bit_reset(GPIOC, GPIO_PIN_6);
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
}

/***************************************************
* ������	temp2_DS18B20_Rst
* ���ܣ�	ds18b20 reset
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static void temp2_DS18B20_Rst(void)
{
	temp2_DS18B20_Mode_OUT_PP();
	gpio_bit_reset(GPIOC, GPIO_PIN_6);
	delay_1us(750); 
	gpio_bit_set(GPIOC, GPIO_PIN_6);
	delay_1us(15); 
}

/***************************************************
* ������	temp2_DS18B20_Presence
* ���ܣ�	ds18b20 check ds18b20 pulse ���ӻ����͵�����
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static uint8_t temp2_DS18B20_Presence(void)
{
	uint8_t pluse_time = 0;
	temp2_DS18B20_Mode_IPU();	
	delay_1us(1);
	while(DS18B20_1_DATA_IN() && pluse_time<200)
	{
		pluse_time++;
		delay_1us(1);
		delay_1us(1);
	}
	
	if(pluse_time >= 200)
		return 1;
	else
		pluse_time = 0;
	
	while(!DS18B20_1_DATA_IN() && pluse_time<240)
	{
		pluse_time++;
		delay_1us(1);
		delay_1us(1);
	}
	
	if(pluse_time >= 240)
		return 1;
	
	return 0;
}

/***************************************************
* ������	temp2_DS18B20_Read_Bit
* ���ܣ�	read one bits from ds18b20 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static uint8_t temp2_DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	temp2_DS18B20_Mode_OUT_PP();
	gpio_bit_reset(GPIOC, GPIO_PIN_6);
	delay_1us(6);
	gpio_bit_set(GPIOC, GPIO_PIN_6); //sht
	delay_1us(9);
	temp2_DS18B20_Mode_IPU();
	
	gpio_bit_set(GPIOC, GPIO_PIN_6);
	delay_1us(1);
	
	if(DS18B20_1_DATA_IN())
		dat = 1;
	else
		dat = 0;
	delay_1us(60);

	return dat;
}

/***************************************************
* ������	temp2_DS18B20_Write_Zero
* ���ܣ�	wite 0 to ds18b20 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void temp2_DS18B20_Write_Zero(void)
{
	temp2_DS18B20_Mode_OUT_PP();
	gpio_bit_reset(GPIOC, GPIO_PIN_6);
	delay_1us(60);
	gpio_bit_set(GPIOC, GPIO_PIN_6);
	delay_1us(10);
}

/***************************************************
* ������	temp2_DS18B20_Write_One
* ���ܣ�	write 1 to ds18b20
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void temp2_DS18B20_Write_One(void)
{
	temp2_DS18B20_Mode_OUT_PP();
	gpio_bit_reset(GPIOC, GPIO_PIN_6);
	delay_1us(6);
	gpio_bit_set(GPIOC, GPIO_PIN_6);
	delay_1us(64);
}

/***************************************************
* ������	temp2_DS18B20_Read_Byte
* ���ܣ�	read one byte from ds18b20 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
uint8_t temp2_DS18B20_Read_Byte(void)
{
	uint8_t dat, dat_i, for_i;
	dat=0;
	for(for_i=0; for_i<8; for_i++)
	{
		dat_i = temp2_DS18B20_Read_Bit();
		dat = (dat>>1) | (dat_i<<7);
	}
	return dat;
}

/***************************************************
* ������	temp2_DS18B20_Write_Byte
* ���ܣ�	write one byte to ds18b20 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void temp2_DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t for_i,testb;
	temp2_DS18B20_Mode_OUT_PP();
	delay_1us(1);
	for(for_i=0; for_i<8; for_i++)
	{
		testb = dat&0x01;
		dat = dat>>1;
		if(testb)		
		{
			temp2_DS18B20_Write_One();
		}
		else
		{
			temp2_DS18B20_Write_Zero();
		}
		delay_1us(10);
	}
}

/***************************************************
* ������	temp2_DS18B20_start
* ���ܣ�	let ds18b20 start to convers 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void temp2_DS18B20_start(void)
{
	temp2_DS18B20_Rst();
	temp2_DS18B20_Presence();
	temp2_DS18B20_Write_Byte(0xCC); 
	temp2_DS18B20_Write_Byte(0x44); 
}

/***************************************************
* ������	dohitech_mcu_temp2_ds18b20_Init
* ���ܣ�	init ds18b20 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
uint8_t dohitech_mcu_temp2_ds18b20_Init(void)
{
	temp2_DS18B20_GPIO_Config();
	temp2_DS18B20_Rst();	
	return temp2_DS18B20_Presence();
}

/***************************************************
* ������	dohitech_mcu_temp2_ds18b20_get_temp
* ���ܣ�  get temperature
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*					��ȡ�¶�ֵ
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
float dohitech_mcu_temp2_ds18b20_get_temp(void)
{
	uint8_t temp;
	uint8_t TL, TH;
	short tem;
	float tempN=0;
	temp2_DS18B20_start();
	temp2_DS18B20_Rst();
	if(temp2_DS18B20_Presence())
	{
			return 0.0;
	}
	temp2_DS18B20_Write_Byte(0xcc);
	temp2_DS18B20_Write_Byte(0xbe);
	TL = temp2_DS18B20_Read_Byte();
	TH = temp2_DS18B20_Read_Byte();
	if(TH>7)
	{
		TH=~TH;
		TL=~TL;
		temp =0; 
	}
	else
		temp= 1;
	tem=TH;	//��8λ
	tem<<=8;
	tem+=TL;
	//sensor_data_ipmb_read[18~19]
	sensor_data_ipmb_read[18] = TH;
	sensor_data_ipmb_read[19] = TL;
	current_temperature[1] = tem;
	tempN=(float)tem*0.0625;
	if(temp)
		return tempN;
	else
		return -tempN;
	
}

/***************************************************
* ������	dohitech_mcu_temp2_ds18b20_read_serial
* ���ܣ�	��ȡ���������к�
* ������                           in/out          ˵��
* 				��ȡ����buff
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void dohitech_mcu_temp2_ds18b20_read_serial(uint8_t * serial)
{
	uint8_t for_i;
	temp2_DS18B20_Rst();
	temp2_DS18B20_Presence();
	temp2_DS18B20_Write_Byte(0x33);
	for(for_i=0; for_i<8; for_i++)
	{
		serial[for_i] = temp2_DS18B20_Read_Byte();
	}
}

void ds18b20_2_test(void)
{
	float temp=0;
	if(0 == dohitech_mcu_temp2_ds18b20_Init())
	{	
		temp = dohitech_mcu_temp2_ds18b20_get_temp();
		temp *= 1000;
		printf("2�¶ȣ�%f��\n",temp);
	}
		return;
}
/**************************************GPIO_TEMP2*********************************************/

/**************************************GPIO_TEMP3*********************************************/
/***************************************************
* ������	temp2_DS18B20_GPIO_Config
* ���ܣ�	ds18b20 gpio config
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static void temp3_DS18B20_GPIO_Config(void)
{
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
}

/***************************************************
* ������	temp2_DS18B20_Mode_IPU
* ���ܣ�	ds18b20 gpio input
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static void temp3_DS18B20_Mode_IPU(void) 
{
	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_15);	
}

/***************************************************
* ������	temp3_DS18B20_Mode_OUT_PP
* ���ܣ�	ds18b20 gpio output
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static void temp3_DS18B20_Mode_OUT_PP(void) 
{
	gpio_bit_reset(GPIOB, GPIO_PIN_6);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
}

/***************************************************
* ������	temp3_DS18B20_Rst
* ���ܣ�	ds18b20 reset
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static void temp3_DS18B20_Rst(void)
{
	temp3_DS18B20_Mode_OUT_PP();
	gpio_bit_reset(GPIOB, GPIO_PIN_15);
	delay_1us(750); 
	gpio_bit_set(GPIOB, GPIO_PIN_15);
	delay_1us(15); 
}

/***************************************************
* ������	temp3_DS18B20_Presence
* ���ܣ�	ds18b20 check ds18b20 pulse ���ӻ����͵�����
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static uint8_t temp3_DS18B20_Presence(void)
{
	uint8_t pluse_time = 0;
	temp3_DS18B20_Mode_IPU();	
	delay_1us(1);
	while(DS18B20_2_DATA_IN() && pluse_time<200)
	{
		pluse_time++;
		delay_1us(1);
		delay_1us(1);
	}
	
	if(pluse_time >= 200)
		return 1;
	else
		pluse_time = 0;
	
	while(!DS18B20_2_DATA_IN() && pluse_time<240)
	{
		pluse_time++;
		delay_1us(1);
		delay_1us(1);
	}
	
	if(pluse_time >= 240)
		return 1;
	
	return 0;
}

/***************************************************
* ������	temp3_DS18B20_Read_Bit
* ���ܣ�	read one bits from ds18b20 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
static uint8_t temp3_DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	temp3_DS18B20_Mode_OUT_PP();
	gpio_bit_reset(GPIOB, GPIO_PIN_15);
	delay_1us(6);
	gpio_bit_set(GPIOB, GPIO_PIN_15); //sht
	delay_1us(9);
	temp3_DS18B20_Mode_IPU();
	
	gpio_bit_set(GPIOB, GPIO_PIN_15);
	delay_1us(1);
	
	if(DS18B20_2_DATA_IN())
		dat = 1;
	else
		dat = 0;
	delay_1us(60);

	return dat;
}

/***************************************************
* ������	temp3_DS18B20_Write_Zero
* ���ܣ�	wite 0 to ds18b20 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void temp3_DS18B20_Write_Zero(void)
{
	temp3_DS18B20_Mode_OUT_PP();
	gpio_bit_reset(GPIOB, GPIO_PIN_15);
	delay_1us(60);
	gpio_bit_set(GPIOB, GPIO_PIN_15);
	delay_1us(10);
}

/***************************************************
* ������	temp3_DS18B20_Write_One
* ���ܣ�	write 1 to ds18b20
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void temp3_DS18B20_Write_One(void)
{
	temp3_DS18B20_Mode_OUT_PP();
	gpio_bit_reset(GPIOB, GPIO_PIN_15);
	delay_1us(6);
	gpio_bit_set(GPIOB, GPIO_PIN_15);
	delay_1us(64);
}

/***************************************************
* ������	temp3_DS18B20_Read_Byte
* ���ܣ�	read one byte from ds18b20 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
uint8_t temp3_DS18B20_Read_Byte(void)
{
	uint8_t dat, dat_i, for_i;
	dat=0;
	for(for_i=0; for_i<8; for_i++)
	{
		dat_i = temp3_DS18B20_Read_Bit();
		dat = (dat>>1) | (dat_i<<7);
	}
	return dat;
}

/***************************************************
* ������	temp3_DS18B20_Write_Byte
* ���ܣ�	write one byte to ds18b20 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void temp3_DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t for_i,testb;
	
	temp3_DS18B20_Mode_OUT_PP();
	delay_1us(1);
	for(for_i=0; for_i<8; for_i++)
	{
		testb = dat&0x01;
		dat = dat>>1;
		if(testb)		
		{
			temp3_DS18B20_Write_One();
		}
		else
		{
			temp3_DS18B20_Write_Zero();
		}
		delay_1us(10);
	}
}

/***************************************************
* ������	temp3_DS18B20_start
* ���ܣ�	let ds18b20 start to convers 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void temp3_DS18B20_start(void)
{
	temp3_DS18B20_Rst();
	temp3_DS18B20_Presence();
	temp3_DS18B20_Write_Byte(0xCC); 
	temp3_DS18B20_Write_Byte(0x44); 
}

/***************************************************
* ������	dohitech_mcu_temp3_ds18b20_Init
* ���ܣ�	init ds18b20 
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
uint8_t dohitech_mcu_temp3_ds18b20_Init(void)
{
	temp3_DS18B20_GPIO_Config();
	temp3_DS18B20_Rst();
	return temp3_DS18B20_Presence();
}

/***************************************************
* ������	dohitech_mcu_temp3_ds18b20_get_temp
* ���ܣ�  get temperature
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*					��ȡ�¶�ֵ
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
float dohitech_mcu_temp3_ds18b20_get_temp(void)
{
	uint8_t temp;
	uint8_t TL, TH;
	short tem;
	float tempN=0;
	temp3_DS18B20_start();
	temp3_DS18B20_Rst();
	if(temp3_DS18B20_Presence())
	{
			return 0.0;
	}
	temp3_DS18B20_Write_Byte(0xcc);
	temp3_DS18B20_Write_Byte(0xbe);
	TL = temp3_DS18B20_Read_Byte();
	TH = temp3_DS18B20_Read_Byte();
	if(TH>7)
	{
		TH=~TH;
		TL=~TL;
		temp =0; 
	}
	else
		temp= 1;
	tem=TH;	//��8λ
	tem<<=8;
	tem+=TL;
	//sensor_data_ipmb_read[18~19]
	sensor_data_ipmb_read[18] = TH;
	sensor_data_ipmb_read[19] = TL;
	current_temperature[1] = tem;
	tempN=(float)tem*0.0625;
	if(temp)
		return tempN;
	else
		return -tempN;
	
}

/***************************************************
* ������	dohitech_mcu_temp3_ds18b20_read_serial
* ���ܣ�	��ȡ���������к�
* ������                           in/out          ˵��
* 				��ȡ����buff
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void dohitech_mcu_temp3_ds18b20_read_serial(uint8_t * serial)
{
	uint8_t for_i;
	temp3_DS18B20_Rst();
	temp3_DS18B20_Presence();
	temp3_DS18B20_Write_Byte(0x33);
	for(for_i=0; for_i<8; for_i++)
	{
		serial[for_i] = temp3_DS18B20_Read_Byte();
	}
}

void ds18b20_3_test(void)
{
	float temp=0;
	if(0 == dohitech_mcu_temp3_ds18b20_Init())
	{	
		temp = dohitech_mcu_temp3_ds18b20_get_temp();
		temp *= 1000;
		printf("3�¶ȣ�%f��\n",temp);
	}
		return;
}
/**************************************GPIO_TEMP3*********************************************/

