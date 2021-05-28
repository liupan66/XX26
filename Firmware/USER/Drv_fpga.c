/**********************************************
 * FileName:	drv_vpx.c
 *
 * FileBrief: 	vpx�������
 *
 * Created on: 
 *
 * Author:   	shanght
 *
 * Remark:
 ******************************************/
#include "global.h"

/***************************************************
* ������	dohitech_mcu_v7_gpio
* ���ܣ�	����v7 gpio0~7 Ϊ����ߵ�ƽ
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*					
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void dohitech_mcu_v7_gpio(void)
{
	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 );
	
	gpio_init(GPIOC, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_5);
	gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_7 | GPIO_PIN_4 | GPIO_PIN_15);
	
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_0);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
//	gpio_bit_set(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4);
//	gpio_bit_set(GPIOC, GPIO_PIN_4 | GPIO_PIN_5);
//	gpio_bit_set(GPIOA, GPIO_PIN_7 | GPIO_PIN_4 | GPIO_PIN_15);
}

/***************************************************
* ������	dohitech_mcu_reset_v7
* ���ܣ�	��λv7
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void DohitechMcuResetV7(void)
{
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
	
//	gpio_bit_set(GPIOB, GPIO_PIN_5);
///	delay_1us(10000);
//	gpio_bit_reset(GPIOB, GPIO_PIN_5);
	

	gpio_bit_reset(GPIOB, GPIO_PIN_5);
	delay_1us(10000);
	gpio_bit_set(GPIOB, GPIO_PIN_5);
}

/***************************************************
* ������	dohitech_mcu_v7_mode_set
* ���ܣ�	����v7����ģʽ
* ������                           in/out          ˵��
* 					v7����ģʽ
*����ֵ              ˵��
*						���ý��
* ˵���� 
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
ErrStatus dohitech_mcu_v7_mode_set(uint8_t v7_mode)
{
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
	if(v7_mode & 0x1)
		gpio_bit_set(GPIOB, GPIO_PIN_14);
	else
		gpio_bit_reset(GPIOB, GPIO_PIN_14);
	if((v7_mode >> 1) & 0x1)
		gpio_bit_set(GPIOB, GPIO_PIN_13);
	else
		gpio_bit_reset(GPIOB, GPIO_PIN_13);
	if((v7_mode>>2) & 0x1)
		gpio_bit_set(GPIOB, GPIO_PIN_12);
	else
		gpio_bit_reset(GPIOB, GPIO_PIN_12);
	
	return SUCCESS;
}
