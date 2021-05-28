/**********************************************
 * FileName:		func_power_down.c
 *
 * FileBrief: 	板卡下电操作
 *
 * Created on: 
 *
 * Author:   shanght
 *
 * Remark:
 ******************************************/
#include "global.h"

/***************************************************
* 函数：	dohitech_mcu_power_down
* 功能：	control power down
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_power_down(void)
{
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	
	gpio_bit_set(GPIOC, GPIO_PIN_12);
}

/***************************************************
* 函数：	dohitech_mcu_power_up
* 功能：	control power up
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_power_up(void)
{
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	
	gpio_bit_reset(GPIOC, GPIO_PIN_12);
}

/***************************************************
* 函数：	dohitech_mcu_power_reset
* 功能：	control power reset
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_power_reset(void)
{
	dohitech_mcu_power_down();
	delay_1us(1000000);
	dohitech_mcu_power_up();
}
