/**********************************************
 * FileName:		func_power_down.c
 *
 * FileBrief: 	�忨�µ����
 *
 * Created on: 
 *
 * Author:   shanght
 *
 * Remark:
 ******************************************/
#include "global.h"

/***************************************************
* ������	dohitech_mcu_power_down
* ���ܣ�	control power down
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void dohitech_mcu_power_down(void)
{
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	
	gpio_bit_set(GPIOC, GPIO_PIN_12);
}

/***************************************************
* ������	dohitech_mcu_power_up
* ���ܣ�	control power up
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void dohitech_mcu_power_up(void)
{
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	
	gpio_bit_reset(GPIOC, GPIO_PIN_12);
}

/***************************************************
* ������	dohitech_mcu_power_reset
* ���ܣ�	control power reset
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void dohitech_mcu_power_reset(void)
{
	dohitech_mcu_power_down();
	delay_1us(1000000);
	dohitech_mcu_power_up();
}
