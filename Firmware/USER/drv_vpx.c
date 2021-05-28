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
* ������	dohitech_mcu_vpx_read_slot
* ���ܣ�	��ȡvpx gpio ��λ������
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*					���ض�ȡ���Ĳ�λ������
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
uint8_t dohitech_mcu_vpx_read_slot(void)
{
	uint8_t slot=0;
	gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12);
	gpio_init(GPIOC, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_9 | GPIO_PIN_14 | GPIO_PIN_15);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_3);//led
	gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_2);//led
	
	slot |= gpio_input_bit_get(GPIOA, GPIO_PIN_11) << 0;
	slot |= gpio_input_bit_get(GPIOC, GPIO_PIN_15) << 1;
	slot |= gpio_input_bit_get(GPIOC, GPIO_PIN_14) << 2;
	slot |= gpio_input_bit_get(GPIOA, GPIO_PIN_8) << 3;
	slot |= gpio_input_bit_get(GPIOC, GPIO_PIN_9) << 4;
	slot |= gpio_input_bit_get(GPIOA, GPIO_PIN_12) << 5;
	
	switch(slot)
  {
	 case 0x3e:slot = 0x1;break;
	 case 0x3d:slot = 0x2;break;
	 case 0x1c:slot = 0x3;break;
	 case 0x3b:slot = 0x4;break;
	 case 0x1a:slot = 0x5;break;
	 case 0x19:slot = 0x6;break;
	 case 0x38:slot = 0x7;break;
	 case 0x37:slot = 0x8;break;
	 case 0x16:slot = 0x9;break;
	 case 0x15:slot = 0xa;break;
	 case 0x34:slot = 0xb;break;
	 case 0x13:slot = 0xc;break;
	 case 0x32:slot = 0xd;break;
	 case 0x31:slot = 0xe;break;
	 case 0x10:slot = 0xf;break;
	 default:	slot = 0x04;break;		 
	}
	
	return slot;
}

/***************************************************
* ������	dohitech_mcu_reset_vpx
* ���ܣ�	��λvpx
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void dohitech_mcu_reset_vpx(void)
{
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8 );
	
	gpio_bit_set(GPIOB, GPIO_PIN_8);
	delay_1us(10000);
	gpio_bit_reset(GPIOB, GPIO_PIN_8);
}

/***************************************************
* ������	dohitech_mcu_get_vpx_reset_status
* ���ܣ�	��ȡ vpx ��λ״̬
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵���� vpx��λ֮���ٸ�λhx dsp
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
FlagStatus dohitech_mcu_get_vpx_reset_status(void)
{
	return RESET;
}
