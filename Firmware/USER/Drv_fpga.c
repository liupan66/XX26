/**********************************************
 * FileName:	drv_vpx.c
 *
 * FileBrief: 	vpx相关驱动
 *
 * Created on: 
 *
 * Author:   	shanght
 *
 * Remark:
 ******************************************/
#include "global.h"

/***************************************************
* 函数：	dohitech_mcu_v7_gpio
* 功能：	设置v7 gpio0~7 为输出高电平
* 参数：                           in/out          说明
* 
*返回值              说明
*					
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
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
* 函数：	dohitech_mcu_reset_v7
* 功能：	复位v7
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
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
* 函数：	dohitech_mcu_v7_mode_set
* 功能：	设置v7加载模式
* 参数：                           in/out          说明
* 					v7加载模式
*返回值              说明
*						设置结果
* 说明： 
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
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
