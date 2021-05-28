/**********************************************
 * FileName:
 *
 * FileBrief: 
 *
 * Created on: 
 *
 * Author:   shanght(DylanShang)
 *
 * Remark:
 ******************************************/
#include "global.h"
/******************************Extern******************************/

/******************************Param*******************************/
/******************************Param*******************************/

/******************************Func********************************/
static	void delay_us(uint32_t time)
{
	int i=0;
	while(time--)
	{
		for(i=0; i<28;i++)
		{
			__NOP;
		}
	}
}

/***************************************************
* 函数：	dohitech_mcu_dsp_reset
* 功能：	RESET HX DSP
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght(DylanShang)		创建
**************************************************/
void dohitech_mcu_dsp_reset(void)
{
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_0);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_9);
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_13);
	#if 1
	GPIO_BC(GPIOA) = GPIO_PIN_0;
	GPIO_BC(GPIOB) = GPIO_PIN_9;
	GPIO_BC(GPIOC) = GPIO_PIN_13;
	delay_us(5000);	
	GPIO_BOP(GPIOA) = GPIO_PIN_0;
	delay_us(5000);
	GPIO_BOP(GPIOC) = GPIO_PIN_13;
	delay_us(5000);
	GPIO_BOP(GPIOB) = GPIO_PIN_9;
	
	#else
	GPIO_BOP(GPIOA) = GPIO_PIN_0;
	GPIO_BC(GPIOC) = GPIO_PIN_13;
	GPIO_BC(GPIOB) = GPIO_PIN_9;
	delay_1ms(5000);
	GPIO_BOP(GPIOB) = GPIO_PIN_9;
	delay_1ms(1000);
	GPIO_BOP(GPIOC) = GPIO_PIN_13;

	#endif
}

/******************************Func********************************/

