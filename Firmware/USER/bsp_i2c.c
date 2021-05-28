/**********************************************
 * FileName:	Bsp_i2c.c
 *
 * FileBrief: 	ipmb-i2c驱动
 *
 * Created on: 
 *
 * Author:   
 *
 * Remark:
 ******************************************/
#include "global.h"

uint16_t bspUsartSendIpmbCmd;
uint8_t i2c_buffer_transmitter[64];
uint8_t i2c_buffer_receiver[256];
uint16_t I2C_nBytes;
uint16_t I2C_nBytes_rx;
uint8_t addr_i2c_send_to = 0x03;
extern uint8_t sensor_thresholds[4];

extern void dohitech_mcu_i2c_reset(void);
uint32_t dtime=0;
#define DELAY_US(n) { \
    delay_1us(1); \
    dtime++; \
    if(dtime >n) { \
        dohitech_mcu_i2c_reset(); \
        return; } }


/***************************************************
* 函数：	mcu_i2c_rcu_config
* 功能：	使能i2c功能时钟
* 参数：                           in/out          说明
* 		periph:	RCU_I2Cx (x=0,1,2): I2C clock
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void mcu_i2c_rcu_config(rcu_periph_enum periph)
{
	rcu_periph_clock_enable(periph);
}

/***************************************************
* 函数：	mcu_i2c_gpio_config
* 功能：	配置gpio功能引脚
* 参数：                           in/out          说明
* 		gpio_periph_scl：	scl引脚组
* 		gpio_periph_sda：	sda引脚组
* 		pin_scl：	scl引脚
* 		pin_sda：	sda引脚
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void mcu_i2c_gpio_config(uint32_t gpio_periph_scl, uint32_t gpio_periph_sda, uint32_t pin_scl, uint32_t pin_sda)
{
    /* I2C0 and I2C1 GPIO ports */
    /* connect PB6 to I2C0_SCL */
    gpio_init(gpio_periph_scl, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, pin_scl);
    /* connect PB7 to I2C0_SDA */
    gpio_init(gpio_periph_sda, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, pin_sda);
}

/***************************************************
* 函数：	mcu_i2c_function_config
* 功能：	i2c 功能配置
*			i2c_periph：I2Cx(x=0,1,2)
*			clkspeed：i2c clock speed 
*			dutycyc：duty cycle in fast mode
*      I2C_DTCY_2:    T_low/T_high=2 
*      I2C_DTCY_16_9: T_low/T_high=16/9
*			address: i2c address
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void mcu_i2c_function_config(uint32_t i2c_periph,uint32_t clkspeed,uint32_t dutycyc, uint8_t address)
{
	    /* I2C clock configure */
    i2c_clock_config(i2c_periph, clkspeed, dutycyc);
    /* I2C address configure */
    i2c_mode_addr_config(i2c_periph, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, address);
    /* enable I2C0 */
    i2c_enable(i2c_periph);
    /* enable acknowledge */
    i2c_ack_config(i2c_periph, I2C_ACK_ENABLE);
}

/***************************************************
* 函数：	dohitech_mcu_i2c_ipmb_rcu_enable
* 功能：	enable ipmb i2c clock
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_i2c_ipmb_rcu_enable(void)
{
    /* enable I2C clock */
		rcu_periph_clock_enable(RCU_I2C0);
		rcu_periph_clock_enable(RCU_I2C1);
}
#if 0
/***************************************************
* 函数：	dohitech_mcu_ipmb_b_gpio_config
* 功能：	cofigure the ipmb b GPIO ports
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_ipmb_gpio_config(void)
{
    /* I2C0 and I2C1 GPIO ports */
    /* connect PB6 to I2C0_SCL */
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_8);
    /* connect PB7 to I2C0_SDA */
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_9);


    /* configure I2C0 GPIO */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_8);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	
//		gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_2);
//    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
		gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_3);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	
		GPIO_BOP(GPIOE) = GPIO_PIN_3;
//		GPIO_BOP(GPIOE) = GPIO_PIN_2;
}

/***************************************************
* 函数：	dohitech_mcu_ipmb_b_i2c_config
* 功能：	cofigure the ipmb b (i2c0) interfaces
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_ipmb_b_i2c_config(void)
{
    /* I2C clock configure */
    i2c_clock_config(I2C0, 400000, I2C_DTCY_2);
    /* I2C address configure */
	  i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, IPMBBADDRESS);
    /* enable I2C0 */
    i2c_enable(I2C0);
    /* enable acknowledge */
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
}

/***************************************************
* 函数：	dohitech_mcu_ipmb_a_gpio_config
* 功能：	cofigure the ipmb a GPIO ports
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_ipmb_a_gpio_config(void)
{
    /* I2C0 and I2C1 GPIO ports */
    /* connect PB6 to I2C0_SCL */
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_10);
    /* connect PB7 to I2C0_SDA */
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_11);

    /* configure I2C1 GPIO */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_11);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	
//	gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_0);
//	gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
	gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_1);
	gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
	GPIO_BOP(GPIOE) = GPIO_PIN_1;
//	GPIO_BOP(GPIOE) = GPIO_PIN_0;
}

/***************************************************
* 函数：	dohitech_mcu_ipmb_a_i2c_config
* 功能：	cofigure the ipmb a (i2c0) interfaces
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_ipmb_a_i2c_config(void)
{
    /* I2C clock configure */
    i2c_clock_config(I2C1, 400000, I2C_DTCY_2);
    /* I2C address configure */
	  i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, IPMBAADDRESS);
    /* enable I2C0 */
    i2c_enable(I2C1);
    /* enable acknowledge */
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);
}

/***************************************************
* 函数：	dohitech_mcu_get_ipmb_ready_status
* 功能：	获取ipmb 总线ready 状态
* 参数：                           in/out          说明
* 				ipmb_type： 0：IPMB A ready状态，	1：IPMB B ready状态
*返回值              说明
*					0: not ready,	else : ready
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
FlagStatus dohitech_mcu_get_ipmb_ready_status(uint8_t ipmb_type)
{
	if(0 == ipmb_type)
	{
		gpio_mode_set(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_0);
		return gpio_input_bit_get(GPIOE, GPIO_PIN_0);
	}
	else if(1 == ipmb_type)
	{
		gpio_mode_set(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_2);
		return gpio_input_bit_get(GPIOE, GPIO_PIN_2);
	}
	else
	{
		printf(" input ipmb type invalid\n");
		return 0xff;
	}
}

/***************************************************
* 函数：	dohitech_mcu_get_ipmb_enable_status
* 功能：	获取ipmb 总线使能状态
* 参数：                           in/out          说明
* 				ipmb_type： 0：IPMB A ready状态，	1：IPMB B ready状态
*返回值              说明
*					0: not ready,	else : ready
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
FlagStatus dohitech_mcu_get_ipmb_enable_status(uint8_t ipmb_type)
{
	if(0 == ipmb_type)
	{
		gpio_mode_set(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_1);
		return gpio_input_bit_get(GPIOE, GPIO_PIN_1);
	}
	else if(1 == ipmb_type)
	{
		gpio_mode_set(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_3);
		return gpio_input_bit_get(GPIOE, GPIO_PIN_3);
	}
	else
	{
		printf(" input ipmb type invalid\n");
		return 0xff;
	}
}

/***************************************************
* 函数：	dohitech_mcu_ipmb_b_transfer
* 功能：	ipmb b 数据发送函数
* 参数：                           in/out          说明
* 				tx_buffer:	发送数据buffer
*					tx_length：	发送数据长度
*返回值              说明
*					0: 正常 else：异常
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
uint8_t dohitech_mcu_ipmb_b_transfer(uint8_t* tx_buffer, uint16_t tx_length)
{
	uint16_t i=0;
	uint16_t tx_timer=0;
	/* wait until I2C bus is idle */
	while(i2c_flag_get(I2C0, I2C_I2CBSY))
	{
		if(tx_timer > 65535)
		{
			return ERR4;
		}
		tx_timer++;
	}
	tx_timer=0;
	/* send a start condition to I2C bus */
	i2c_start_on_bus(I2C0);
	/* wait until SBSEND bit is set */
	while(!i2c_flag_get(I2C0, I2C_SBSEND))
	{
		if(tx_timer > 65535)
		{
			i2c_disable(I2C0);
			delay_us(10000);
			i2c_enable(I2C0);
			return ERR5;
		}
		tx_timer++;
	}
	tx_timer=0;
	i2c_master_addressing(I2C0, 0x30+0x01-1, I2C_TRANSMITTER);
	/* wait until ADDSEND bit is set*/
	while(!i2c_flag_get(I2C0, I2C_ADDSEND))
	{
		if(tx_timer > 65535)
		{
			i2c_disable(I2C0);
			delay_us(10000);
			i2c_enable(I2C0);
			return ERR6;
		}
		tx_timer++;
	}
	tx_timer=0;	
	
	i2c_flag_clear(I2C0, I2C_STAT0_ADDSEND);
		
	i=0;
	while(tx_length > 0)
	{
		i2c_transmit_data(I2C0, tx_buffer[i]);
		while(!i2c_flag_get(I2C0, I2C_TBE))
		{
			if(tx_timer > 65535)
			{
				i2c_disable(I2C0);
				delay_us(10000);
				i2c_enable(I2C0);
				return ERR3;
			}
			tx_timer++;
		};
		tx_timer = 0;
		
		tx_length --;
		i++;
	}
	/* send a stop condition to I2C bus*/
	i2c_stop_on_bus(I2C0);
	while(I2C_CTL0(I2C0)&0x0200)
	{	
		if(tx_timer > 65535)
		{
			i2c_disable(I2C0);
			delay_us(10000);
			i2c_enable(I2C0);
			return ERR5;
		}
		tx_timer++;
	}
	tx_timer=0;
	
	i2c_enable(I2C0);
	
	return IPMBOK;
}

/***************************************************
* 函数：	dohitech_mcu_ipmb_b_receiver
* 功能：	ipmb  b 数据接收函数
* 参数：                           in/out          说明
* 				rx_buffer:	接收数据buffer
*					rx_length：	接收数据长度
*返回值              说明
*					0: 正常 else：异常
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
uint8_t dohitech_mcu_ipmb_b_receiver(uint8_t* rx_buffer, uint16_t rx_length)
{
	uint16_t i;
	uint16_t tx_timer=0;
	
	/* wait until I2C bus is idle */
	while(i2c_flag_get(I2C0, I2C_I2CBSY))
	{
		if(tx_timer > 65535)
		{
			return ERR4;
		}
		tx_timer++;
	}
	tx_timer=0;
	/* send a start condition to I2C bus */
	i2c_start_on_bus(I2C0);
	/* wait until SBSEND bit is set */
	while(!i2c_flag_get(I2C0, I2C_SBSEND))
	{
		if(tx_timer > 65535)
		{
			i2c_disable(I2C0);
			delay_us(10000);
			i2c_enable(I2C0);
			return ERR5;
		}
		tx_timer++;
	}
	tx_timer=0;
		
	i2c_master_addressing(I2C0, 0x30+0x01-1, I2C_RECEIVER);
	/* wait until ADDSEND bit is set*/
	while(!i2c_flag_get(I2C0, I2C_ADDSEND))
	{
		if(tx_timer > 65535)
		{
			i2c_disable(I2C0);
			delay_us(10000);
			i2c_enable(I2C0);
			return ERR6;
		}
		tx_timer++;
	}
	tx_timer=0;	
		
	i2c_flag_clear(I2C0, I2C_STAT0_ADDSEND);
	i=0;
	while(rx_length>1)
	{
		 /* wait until the RBNE bit is set */
		while(!i2c_flag_get(I2C0, I2C_RBNE))
		{
			if(tx_timer > 65535)
			{
				i2c_disable(I2C0);
				delay_us(10000);
				i2c_enable(I2C0);
				return ERR8;
			}
			tx_timer++;
		};
		tx_timer = 0;
		
		/* read a data byte from I2C_DATA*/
		rx_buffer[i] = i2c_receive_data(I2C0);
		i++;
		rx_length--;
	}
	 /* send a NACK for the last data byte */
	i2c_ack_config(I2C0, I2C_ACK_DISABLE);
	/* send a stop condition to I2C bus*/
	i2c_stop_on_bus(I2C0);
	while(I2C_CTL0(I2C0)&0x0200)
	{	
		if(tx_timer > 65535)
		{
			i2c_disable(I2C0);
			delay_us(10000);
			i2c_enable(I2C0);
			return ERR5;
		}
		tx_timer++;
	}
	tx_timer=0;
	
	rx_buffer[i] = i2c_receive_data(I2C0);
	i2c_ack_config(I2C0, I2C_ACK_ENABLE);
	
	return IPMBOK;
}

/***************************************************
* 函数：	dohitech_mcu_ipmb_a_transfer
* 功能：	ipmb  a 数据发送函数
* 参数：                           in/out          说明
* 				tx_buffer:	发送数据buffer
*					tx_length：	发送数据长度
*返回值              说明
*					0: 正常 else：异常
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
uint8_t dohitech_mcu_ipmb_a_transfer(uint8_t* tx_buffer, uint16_t tx_length)
{
	uint16_t i=0;
	uint16_t tx_timer=0;
	/* wait until I2C bus is idle */
	while(i2c_flag_get(I2C1, I2C_I2CBSY))
	{
		if(tx_timer > 65535)
		{
			return ERR4;
		}
		tx_timer++;
	}
	tx_timer=0;
	/* send a start condition to I2C bus */
	i2c_start_on_bus(I2C1);
	/* wait until SBSEND bit is set */
	while(!i2c_flag_get(I2C1, I2C_SBSEND))
	{
		if(tx_timer > 65535)
		{
			i2c_disable(I2C1);
			delay_us(10000);
			i2c_enable(I2C1);
			return ERR5;
		}
		tx_timer++;
	}
	tx_timer=0;
	i2c_master_addressing(I2C1, 0x30+0x01-1, I2C_TRANSMITTER);
	/* wait until ADDSEND bit is set*/
	while(!i2c_flag_get(I2C1, I2C_ADDSEND))
	{
		if(tx_timer > 65535)
		{
			i2c_disable(I2C1);
			delay_us(10000);
			i2c_enable(I2C1);
			return ERR6;
		}
		tx_timer++;
	}
	tx_timer=0;	
	
	i2c_flag_clear(I2C1, I2C_STAT0_ADDSEND);
		
	i=0;
	while(tx_length > 0)
	{
		i2c_transmit_data(I2C1, tx_buffer[i]);
		while(!i2c_flag_get(I2C1, I2C_TBE))
		{
			if(tx_timer > 65535)
			{
				i2c_disable(I2C1);
				delay_us(10000);
				i2c_enable(I2C1);
				return ERR3;
			}
			tx_timer++;
		};
		tx_timer = 0;
		
		tx_length --;
		i++;
	}
	/* send a stop condition to I2C bus*/
	i2c_stop_on_bus(I2C1);
	while(I2C_CTL0(I2C1)&0x0200)
	{	
		if(tx_timer > 65535)
		{
			i2c_disable(I2C1);
			delay_us(10000);
			i2c_enable(I2C1);
			return ERR5;
		}
		tx_timer++;
	}
	tx_timer=0;
	
	i2c_enable(I2C1);
	
	return IPMBOK;
}

/***************************************************
* 函数：	dohitech_mcu_ipmb_a_receiver
* 功能：	ipmb a 数据接收函数
* 参数：                           in/out          说明
* 				rx_buffer:	接收数据buffer
*					rx_length：	接收数据长度
*返回值              说明
*					0: 正常 else：异常
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
uint8_t dohitech_mcu_ipmb_a_receiver(uint8_t* rx_buffer, uint16_t rx_length)
{
	uint16_t i;
	uint16_t tx_timer=0;
	
	/* wait until I2C bus is idle */
	while(i2c_flag_get(I2C1, I2C_I2CBSY))
	{
		if(tx_timer > 65535)
		{
			return ERR4;
		}
		tx_timer++;
	}
	tx_timer=0;
	/* send a start condition to I2C bus */
	i2c_start_on_bus(I2C1);
	/* wait until SBSEND bit is set */
	while(!i2c_flag_get(I2C1, I2C_SBSEND))
	{
		if(tx_timer > 65535)
		{
			i2c_disable(I2C1);
			delay_us(10000);
			i2c_enable(I2C1);
			return ERR5;
		}
		tx_timer++;
	}
	tx_timer=0;
		
	i2c_master_addressing(I2C1, 0x30+0x01-1, I2C_RECEIVER);
	/* wait until ADDSEND bit is set*/
	while(!i2c_flag_get(I2C1, I2C_ADDSEND))
	{
		if(tx_timer > 65535)
		{
			i2c_disable(I2C1);
			delay_us(10000);
			i2c_enable(I2C1);
			return ERR6;
		}
		tx_timer++;
	}
	tx_timer=0;	
		
	i2c_flag_clear(I2C1, I2C_STAT0_ADDSEND);
	i=0;
	while(rx_length>1)
	{
		 /* wait until the RBNE bit is set */
		while(!i2c_flag_get(I2C1, I2C_RBNE))
		{
			if(tx_timer > 65535)
			{
				i2c_disable(I2C1);
				delay_us(10000);
				i2c_enable(I2C1);
				return ERR8;
			}
			tx_timer++;
		};
		tx_timer = 0;
		
		/* read a data byte from I2C_DATA*/
		rx_buffer[i] = i2c_receive_data(I2C1);
		i++;
		rx_length--;
	}
	 /* send a NACK for the last data byte */
	i2c_ack_config(I2C1, I2C_ACK_DISABLE);
	/* send a stop condition to I2C bus*/
	i2c_stop_on_bus(I2C1);
	while(I2C_CTL0(I2C1)&0x0200)
	{	
		if(tx_timer > 65535)
		{
			i2c_disable(I2C1);
			delay_us(10000);
			i2c_enable(I2C1);
			return ERR5;
		}
		tx_timer++;
	}
	tx_timer=0;
	
	rx_buffer[i] = i2c_receive_data(I2C1);
	i2c_ack_config(I2C1, I2C_ACK_ENABLE);
	
	return IPMBOK;
}

/***************************************************
* 函数：	dohitech_mcu_dsp_usart_cmd_analysis
* 功能：	dsp 串口命令解析
* 参数：                           in/out          说明
* 
*返回值              说明
*					0：正常  1：命令错误
* 说明：	在接收到dsp 命令后调用
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
uint8_t dohitech_mcu_dsp_usart_cmd_analysis(void)
{
	switch(bspUsartSendIpmbCmd)
	{
		//设置temp/volt
		case 1:
		case 2:
			i2c_buffer_transmitter[0] = RSSA;
			i2c_buffer_transmitter[1] = 0x04<<2 | RQLUN;
			i2c_buffer_transmitter[2] = ipmb_checkSumRight(0, i2c_buffer_transmitter, 2);
			i2c_buffer_transmitter[3] = RQSA;
			i2c_buffer_transmitter[4] = RQSEQ<<2 | RSLUN;
			i2c_buffer_transmitter[5] = 0x26;
//			i2c_buffer_transmitter[6] = usart2TempVoltNum;
			i2c_buffer_transmitter[7] = 0x01;		//设置最高临界值
			i2c_buffer_transmitter[8] = 0x00;
			i2c_buffer_transmitter[9] = 0x00;
			i2c_buffer_transmitter[10] = 0x00;
			i2c_buffer_transmitter[11] = 0x00;
//			i2c_buffer_transmitter[12] = usartRemoteRecvData[0];
			i2c_buffer_transmitter[13] = 0x00;
			i2c_buffer_transmitter[14] = ipmb_checkSumRight(0, &i2c_buffer_transmitter[3], 11);	
			I2C_nBytes = 15;
			I2C_nBytes_rx = 8;
			break;
		//check all data
		case 3:
			i2c_buffer_transmitter[0] = RSSA;
			i2c_buffer_transmitter[1] = 0x04<<2 | RQLUN;
			i2c_buffer_transmitter[2] = ipmb_checkSumRight(0, i2c_buffer_transmitter, 2);
			i2c_buffer_transmitter[3] = RQSA;
			i2c_buffer_transmitter[4] = RQSEQ<<2 | RSLUN;
			i2c_buffer_transmitter[5] = 0x2D;
			i2c_buffer_transmitter[6] = 0xff;
			i2c_buffer_transmitter[7] = ipmb_checkSumRight(0, &i2c_buffer_transmitter[3], 4);
			I2C_nBytes = 8;	
			I2C_nBytes_rx = 152;
			break;
		//power down
		case 4:
			i2c_buffer_transmitter[0] = RSSA;
			i2c_buffer_transmitter[1] = 0x00<<2 | RQLUN;
			i2c_buffer_transmitter[2] = ipmb_checkSumRight(0, i2c_buffer_transmitter, 2);
			i2c_buffer_transmitter[3] = RQSA;
			i2c_buffer_transmitter[4] = RQSEQ<<2 | RSLUN;
			i2c_buffer_transmitter[5] = 0x02;
			i2c_buffer_transmitter[6] = 0x02;
			i2c_buffer_transmitter[7] = ipmb_checkSumRight(0, &i2c_buffer_transmitter[3], 4);	
			I2C_nBytes = 8;
			I2C_nBytes_rx = 8;
			break;
		//power on
		case 5:
			i2c_buffer_transmitter[0] = RSSA;
			i2c_buffer_transmitter[1] = 0x00<<2 | RQLUN;
			i2c_buffer_transmitter[2] = ipmb_checkSumRight(0, i2c_buffer_transmitter, 2);
			i2c_buffer_transmitter[3] = RQSA;
			i2c_buffer_transmitter[4] = RQSEQ<<2 | RSLUN;
			i2c_buffer_transmitter[5] = 0x02;
			i2c_buffer_transmitter[6] = 0x01;
			i2c_buffer_transmitter[7] = ipmb_checkSumRight(0, &i2c_buffer_transmitter[3], 4);	
			I2C_nBytes = 8;
			I2C_nBytes_rx = 8;
			break;
		default:
			printf("i2c data error which i2c use to send from usart \n");
			return 1;
	}
	return 0;
}

/***************************************************
* 函数：	dohitech_mcu_ipmb_param_init
* 功能：	ipmb 参数初始化
* 参数：                           in/out          说明
* 				a: ipmb 参数初始化数据
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_ipmb_param_init(ipmb_param_str a)
{
	ipmb_param.netFn = a.netFn;
	ipmb_param.rqLun = a.rqLun;
	ipmb_param.rqSa = a.rqSa;
	ipmb_param.rqSeq = a.rqSeq;
	ipmb_param.rsLun = a.rsLun;
	ipmb_param.rssa = a.rssa;
	
	I2C_nBytes = 0;
	I2C_nBytes_rx = 0;
	bspUsartSendIpmbCmd = 0;

	memset(i2c_buffer_transmitter, 0 , 64);
}
/***************************************************
* 函数：	dohitech_mcu_ipmb_operation
* 功能：	ipmb 功能函数
* 参数：                           in/out          说明
* 				ipmb_type： 0：IPMB A， 1： IPMB B
*返回值              说明
*					0: 正常 else：异常
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
uint8_t dohitech_mcu_ipmb_operation(uint8_t ipmb_type)
{
	dohitech_mcu_dsp_usart_cmd_analysis();
	
	if(0 == ipmb_type)
	{
		dohitech_mcu_ipmb_a_transfer(i2c_buffer_transmitter, I2C_nBytes);
		delay_us(1000);
		dohitech_mcu_ipmb_a_receiver(i2c_buffer_receiver, I2C_nBytes_rx);
	}
	else if(1 == ipmb_type)
	{
		dohitech_mcu_ipmb_b_transfer(i2c_buffer_transmitter, I2C_nBytes);
		delay_us(1000);
		dohitech_mcu_ipmb_b_receiver(i2c_buffer_receiver, I2C_nBytes_rx);
	}
	else
		printf("input ipmb type is fault!\n");
	
	return 0;
}

#endif
/***************************************************
* 函数：	dohitech_mcu_bsp_i2c_nvic_config
* 功能：	i2c 中断优先级配置
* 参数：                           in/out          说明
* 				
*返回值              说明
*					
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_bsp_i2c_nvic_config(void)
{
		nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(I2C0_EV_IRQn, 0, 3);
    nvic_irq_enable(I2C1_EV_IRQn, 0, 4);
    nvic_irq_enable(I2C0_ER_IRQn, 0, 2);
    nvic_irq_enable(I2C1_ER_IRQn, 0, 1);
}

/***************************************************
* 函数：	dohitech_mcu_bsp_i2c_irq_enable
* 功能：	i2c 中断使能
* 参数：                           in/out          说明
* 				
*返回值              说明
*					
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_bsp_i2c_irq_enable(void)
{
    /* enable the I2C0 interrupt */
    i2c_interrupt_enable(I2C0, I2C_INT_ERR);
    i2c_interrupt_enable(I2C0, I2C_INT_EV);
    i2c_interrupt_enable(I2C0, I2C_INT_BUF);

    /* enable the I2C1 interrupt */
    i2c_interrupt_enable(I2C1, I2C_INT_ERR);
    i2c_interrupt_enable(I2C1, I2C_INT_EV);
    i2c_interrupt_enable(I2C1, I2C_INT_BUF);
}

/***************************************************
* 函数：	dohitech_mcu_bsp_i2c_init
* 功能：	i2c初始化
* 参数：                           in/out          说明
* 				
*返回值              说明
*					
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_bsp_i2c_init(void)
{
	dohitech_mcu_i2c_ipmb_rcu_enable();
	mcu_i2c_rcu_config(RCU_I2C0);
	mcu_i2c_rcu_config(RCU_I2C1);
	mcu_i2c_gpio_config(GPIOB, GPIOB, GPIO_PIN_6, GPIO_PIN_7);
	mcu_i2c_gpio_config(GPIOB, GPIOB, GPIO_PIN_10, GPIO_PIN_11);
	mcu_i2c_function_config(I2C0, 100000, I2C_DTCY_2, 0x20);
	mcu_i2c_function_config(I2C1, 100000, I2C_DTCY_2, 0x20);
	dohitech_mcu_bsp_i2c_nvic_config();
}

/***************************************************
* 函数：	dohitech_mcu_i2c_test
* 功能：	i2c send test
* 参数：                           in/out          说明
* 				
*返回值              说明
*					
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
#define I2CTYPE	I2C1
void dohitech_mcu_i2c_test(void)
{
	uint8_t i2c_buffer_transmitter[16] = {0}, i=0;
	uint8_t i2c_buffer_receiver[16] = {0};
	
	for(i=0; i<16; i++){
        i2c_buffer_transmitter[i] = i + 0x80;
    }
    dtime=0;
    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2CTYPE, I2C_FLAG_I2CBSY))
        DELAY_US(2999999)
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2CTYPE);
    dtime=0;
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2CTYPE, I2C_FLAG_SBSEND))
        DELAY_US(2999999)
    /* send slave address to I2C bus */
    i2c_master_addressing(I2CTYPE, 0x32, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set */
    dtime=0;
    while(!i2c_flag_get(I2CTYPE, I2C_FLAG_ADDSEND))
        DELAY_US(2999999)
    /* clear ADDSEND bit */
    i2c_flag_clear(I2CTYPE, I2C_FLAG_ADDSEND);
    
    for(i=0; i<16; i++){
        /* send a data byte */
        i2c_data_transmit(I2CTYPE, i2c_buffer_transmitter[i]);
        /* wait until the transmission data register is empty */
        dtime=0;
        while(!i2c_flag_get(I2CTYPE, I2C_FLAG_TBE))
            DELAY_US(2999999)
    }

    i2c_stop_on_bus(I2CTYPE);
    dtime=0;
    while(I2C_CTL0(I2CTYPE)&0x0200)
        DELAY_US(2999999)
		
		/* wait until I2C bus is idle */
    dtime=0;
    while(i2c_flag_get(I2CTYPE, I2C_FLAG_I2CBSY))
        DELAY_US(2999999)
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2CTYPE);
    /* wait until SBSEND bit is set */
    dtime=0;
    while(!i2c_flag_get(I2CTYPE, I2C_FLAG_SBSEND))
        DELAY_US(2999999)
    /* send slave address to I2C bus */
    i2c_master_addressing(I2CTYPE, 0x32, I2C_RECEIVER);
    /* wait until ADDSEND bit is set */
    dtime=0;
    while(!i2c_flag_get(I2CTYPE, I2C_FLAG_ADDSEND))
        DELAY_US(2999999)
    /* clear ADDSEND bit */
    i2c_flag_clear(I2CTYPE, I2C_FLAG_ADDSEND);
		
		for(i=0; i<16; i++){
        /* wait until the transmission data register is empty */
            dtime=0;
        while(!i2c_flag_get(I2CTYPE, I2C_FLAG_RBNE))
            DELAY_US(2999999)
			        /* send a data byte */
        i2c_buffer_receiver[i] = i2c_data_receive(I2CTYPE);
    }
		
		if(0 == memcmp(i2c_buffer_receiver, i2c_buffer_transmitter, 16))
			printf("\tI2C 接口收发测试成功！\n");
		else
			printf("\tI2C 接口收发测试异常！\n");
}

void dohitech_mcu_i2c_reset(void)
{
    i2c_disable(I2CTYPE);
    i2c_enable(I2CTYPE);
    i2c_ack_config(I2CTYPE, I2C_ACK_ENABLE);
}
    
#if 0
/* convert raw register value to sign-extended integer in 16-bit range */
int ltc2991_voltage_to_int(int raw) {
	/* NOT check MSB[7] -- DV */
	if (raw & BIT(14))
		return -(0x4000 - (raw & 0x3FFF)) << 2;
	else
		return (raw & 0x3FFF) << 2; /* (#>>2)*2.5/(2^13) */
}

/***************************************************
* 函数：	dohitech_mcu_bsp_i2c_ipmb_ops
* 功能：	ipmb发送接收数据
* 参数：                           in/out          说明
* 				
*返回值              说明
*					
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
#define I2CTYPE	I2C0 //I2C1 ok
int dohitech_mcu_bsp_i2c_ipmb_ops(void)
{
	uint32_t i=0, k=0;
	int j=2, ltc29xx_val=0;
	//发送查询命令
	// config mode
	switch(usart2I2c2SendData)
	{
		//设置temp/volt
		case 1:
			i2c_buffer_transmitter[0] = RSSA;
			i2c_buffer_transmitter[1] = 0x04<<2 | RQLUN;
			i2c_buffer_transmitter[2] = ipmb_checkSumRight(0, i2c_buffer_transmitter, 2);
			i2c_buffer_transmitter[3] = RQSA;
			i2c_buffer_transmitter[4] = RQSEQ<<2 | RSLUN;
			i2c_buffer_transmitter[5] = 0x26;
			i2c_buffer_transmitter[6] = 01;
			i2c_buffer_transmitter[7] = sensor_thresholds[0];		//设置最高临界值
			i2c_buffer_transmitter[8] = sensor_thresholds[1];
			i2c_buffer_transmitter[9] = sensor_thresholds[2];
			i2c_buffer_transmitter[10] = sensor_thresholds[3];
			i2c_buffer_transmitter[11] = 0x00;
			i2c_buffer_transmitter[12] = usartRemoteRecvData[0];
			i2c_buffer_transmitter[13] = 0x00;
			i2c_buffer_transmitter[14] = ipmb_checkSumRight(0, &i2c_buffer_transmitter[3], 11);	
			I2C_nBytes = 15;
			break;
		case 2:
			i2c_buffer_transmitter[0] = RSSA;
			i2c_buffer_transmitter[1] = 0x04<<2 | RQLUN;
			i2c_buffer_transmitter[2] = ipmb_checkSumRight(0, i2c_buffer_transmitter, 2);
			i2c_buffer_transmitter[3] = RQSA;
			i2c_buffer_transmitter[4] = RQSEQ<<2 | RSLUN;
			i2c_buffer_transmitter[5] = 0x26;
			i2c_buffer_transmitter[6] = 00;
			i2c_buffer_transmitter[7] = sensor_thresholds[0];		//设置最高临界值
			i2c_buffer_transmitter[8] = sensor_thresholds[1];
			i2c_buffer_transmitter[9] = sensor_thresholds[2];
			i2c_buffer_transmitter[10] = sensor_thresholds[3];
			i2c_buffer_transmitter[11] = 0x00;
			i2c_buffer_transmitter[12] = usartRemoteRecvData[0];
			i2c_buffer_transmitter[13] = 0x00;
			i2c_buffer_transmitter[14] = ipmb_checkSumRight(0, &i2c_buffer_transmitter[3], 11);	
			I2C_nBytes = 15;
			break;
		//check all data
		case 3:
			i2c_buffer_transmitter[0] = RSSA;
			i2c_buffer_transmitter[1] = 0x04<<2 | RQLUN;
			i2c_buffer_transmitter[2] = ipmb_checkSumRight(0, i2c_buffer_transmitter, 2);
			i2c_buffer_transmitter[3] = RQSA;
			i2c_buffer_transmitter[4] = RQSEQ<<2 | RSLUN;
			i2c_buffer_transmitter[5] = 0x2D;
			i2c_buffer_transmitter[6] = 0xff;
			i2c_buffer_transmitter[7] = ipmb_checkSumRight(0, &i2c_buffer_transmitter[3], 4);
			I2C_nBytes = 8;	
			break;
		//power down
		case 4:
			i2c_buffer_transmitter[0] = RSSA;
			i2c_buffer_transmitter[1] = 0x00<<2 | RQLUN;
			i2c_buffer_transmitter[2] = ipmb_checkSumRight(0, i2c_buffer_transmitter, 2);
			i2c_buffer_transmitter[3] = RQSA;
			i2c_buffer_transmitter[4] = RQSEQ<<2 | RSLUN;
			i2c_buffer_transmitter[5] = 0x02;
			i2c_buffer_transmitter[6] = 0x02;
			i2c_buffer_transmitter[7] = ipmb_checkSumRight(0, &i2c_buffer_transmitter[3], 4);	
			I2C_nBytes = 8;
			break;
		//power on
		case 5:
			i2c_buffer_transmitter[0] = RSSA;
			i2c_buffer_transmitter[1] = 0x00<<2 | RQLUN;
			i2c_buffer_transmitter[2] = ipmb_checkSumRight(0, i2c_buffer_transmitter, 2);
			i2c_buffer_transmitter[3] = RQSA;
			i2c_buffer_transmitter[4] = RQSEQ<<2 | RSLUN;
			i2c_buffer_transmitter[5] = 0x02;
			i2c_buffer_transmitter[6] = 0x01;
			i2c_buffer_transmitter[7] = ipmb_checkSumRight(0, &i2c_buffer_transmitter[3], 4);	
			I2C_nBytes = 8;
			break;
		default:
			printf("i2c data error which i2c use to send from usart \n");
			return 1;
	}
		k=0;
	/* wait until I2C bus is idle */
    while(i2c_flag_get(I2CTYPE, I2C_I2CBSY))
		{
			k++;
			if(k>0x7ffffffe)
			{
				k=0;
				i2c_disable(I2CTYPE);
				dohitech_mcu_bsp_i2c_init();
				return -1;
			}
		}
//		mcu_bsp_i2c_16_clock();
	
	/* send a start condition to I2C bus */
    i2c_start_on_bus(I2CTYPE);
	/* wait until SBSEND bit is set */
		k=0;
    while(!i2c_flag_get(I2CTYPE, I2C_SBSEND))
		{
			k++;
			if(k>0x7ffffffe)
			{
				k=0;
				i2c_disable(I2CTYPE);
				dohitech_mcu_bsp_i2c_init();
				return -1;
			}
		}
	
		i2c_master_addressing(I2CTYPE, addr_i2c_send_to, I2C_TRANSMITTER);
	/* wait until ADDSEND bit is set*/
		k=0;
    while(!i2c_flag_get(I2CTYPE, I2C_ADDSEND))
		{
			k++;
			if(k>0x7ffffffe)
			{
				k=0;
				i2c_disable(I2CTYPE);
				dohitech_mcu_bsp_i2c_init();
				return -1;
			}
		}
    i2c_flag_clear(I2CTYPE, I2C_STAT0_ADDSEND);

	i=0;
//	delay_us(1000000);
	while(I2C_nBytes > 0)
	{
		k=0;
		i2c_transmit_data(I2CTYPE, i2c_buffer_transmitter[i]);
		while(!i2c_flag_get(I2CTYPE, I2C_TBE))
		{
			k++;
			if(k>0x7ffffffe)
			{
				k=0;
				i2c_disable(I2CTYPE);
				dohitech_mcu_bsp_i2c_init();
				return -1;
			}
		}
		I2C_nBytes --;
		i++;
	}
	k=0;
	/* send a stop condition to I2C bus*/
	i2c_stop_on_bus(I2CTYPE);
	while(I2C_CTL0(I2CTYPE)&0x0200)
	{
		k++;
		if(k>0x7ffffffe)
		{
			k=0;
			i2c_disable(I2CTYPE);
			dohitech_mcu_bsp_i2c_init();
			return -1;
		}
	}
	i2c_enable(I2CTYPE);
	switch(usart2I2c2SendData)
	{
		//设置temp/volt
		case 1:
		case 2:
			I2C_nBytes = 15;
		break;
		case 3:
		case 4:
		case 5:
			I2C_nBytes = 8;
		break;
	}
//	printf("i2c transfer such as: \n");
//	for(i=0; i< I2C_nBytes; i++)
//	printf("%d : %x\n",i, i2c_buffer_transmitter[i]);
	memset(i2c_buffer_transmitter, 0, I2C_nBytes);
	I2C_nBytes = 0;

	//接收反馈数据
	//此处延时是发送数据后等待对方反应后进行接收
	switch(usart2I2c2SendData)
	{
		case 1:
		case 2:
		case 4:
		case 5:
			I2C_nBytes = 8;
			break;
		case 3:
			I2C_nBytes = 152;
			break;
	}
//	delay_us(3000000);
	//重新使能i2c的中断，该中断在中断中发送完毕后关闭
	/* wait until I2C bus is idle */
		k=0;
		while(i2c_flag_get(I2CTYPE, I2C_I2CBSY))
		{
			k++;
			if(k>0x7ffffffe)
			{
				k=0;
				i2c_disable(I2CTYPE);
				dohitech_mcu_bsp_i2c_init();
				return -1;
			}
		}
		k=0;
	/* send a start condition to I2C bus */
		i2c_start_on_bus(I2CTYPE);
	/* wait until SBSEND bit is set */
		while(!i2c_flag_get(I2CTYPE, I2C_SBSEND))
		{
			k++;
			if(k>0x7ffffffe)
			{
				k=0;
				i2c_disable(I2CTYPE);
				dohitech_mcu_bsp_i2c_init();
				return -1;
			}
		}
		k=0;
		i2c_master_addressing(I2CTYPE, addr_i2c_send_to, I2C_RECEIVER);
	/* wait until ADDSEND bit is set*/
		while(!i2c_flag_get(I2CTYPE, I2C_ADDSEND))
		{
			k++;
			if(k>0x7ffffffe)
			{
				k=0;
				i2c_disable(I2CTYPE);
				dohitech_mcu_bsp_i2c_init();
				return -1;
			}
		}	
		i2c_flag_clear(I2CTYPE, I2C_STAT0_ADDSEND);
		i=0;

		while(I2C_nBytes>1)
		{
			k=0;
			while(!i2c_flag_get(I2CTYPE, I2C_RBNE))
			{
				k++;
				if(k>0x7ffffffe)
				{
					k=0;
					i2c_disable(I2CTYPE);
					dohitech_mcu_bsp_i2c_init();
					return -1;
				}
			}
				/* read a data byte from I2C_DATA*/
			i2c_buffer_receiver[i++] = i2c_receive_data(I2CTYPE);
			I2C_nBytes--;

		}
		i2c_ack_config(I2CTYPE, I2C_ACK_DISABLE);
		k=0;
		i2c_stop_on_bus(I2CTYPE);
		while(I2C_CTL0(I2CTYPE)&0x0200)
		{
			k++;
			if(k>0x7ffffffe)
			{
				k=0;
				i2c_deinit(I2CTYPE);
				dohitech_mcu_bsp_i2c_init();
				return -1;
			}
		}
		i2c_buffer_receiver[i] = i2c_receive_data(I2CTYPE);
		i2c_ack_config(I2CTYPE, I2C_ACK_ENABLE);
		I2C_nBytes--;
		/* send a stop condition to I2C bus*/
		i2c_ackpos_config(I2CTYPE, I2C_ACK_ENABLE);
			
		if(usart2I2c2SendData == 3)
		{
			switch(addr_i2c_send_to)
			{
				case 0x5:
					printf("--------资源管理板-------\n");
					for(i=0; i<11; i++)
					{
						sensor_data_ipmb_read[22+i*2] = i2c_buffer_receiver[7+(4*(i*2))];
						sensor_data_ipmb_read[23+i*2] = i2c_buffer_receiver[11+(4*(i*2))];
//						if(i<8)
//							printf("voltage : %f V\n", ((float)((sensor_data_ipmb_read[22+i*2] << 8) | sensor_data_ipmb_read[23+i*2])/4096)*3.3);
//						else if((i==8) || (i==9))
//						{
//							printf("temperature : %2f ℃\n", ((sensor_data_ipmb_read[22+i*2] << 8) | sensor_data_ipmb_read[23+i*2])*0.0625);
//						}
//						else
//							printf("current : %2f A\n", (float)(((sensor_data_ipmb_read[22+i*2] << 8) | sensor_data_ipmb_read[23+i*2])>>4)/400);
					}
					break;
				case 0x3:
					printf("--------卫星导航板-------\n");
					for(i=0; i<15; i++)
					{
						sensor_data_ipmb_read[44+i*2] = i2c_buffer_receiver[7+(4*(i*2))];
						sensor_data_ipmb_read[45+i*2] = i2c_buffer_receiver[11+(4*(i*2))];
						
//						if(i<12)
//						{
//							ltc29xx_val = (sensor_data_ipmb_read[44+i*2] << 8) | sensor_data_ipmb_read[45+i*2];
//							ltc29xx_val = ltc2991_voltage_to_int(ltc29xx_val);
//							printf("voltage : %2f V\n", ltc29xx_val / 13107.2);
//						}
//						else if((i==12) || (i==13))
//						{
//							printf("temperature : %2f ℃\n", ((sensor_data_ipmb_read[44+i*2] << 8) | sensor_data_ipmb_read[45+i*2])*0.0625);
//						}
//						else if(i==14)
//						{
//							printf("current : %2f A\n", (float)(((sensor_data_ipmb_read[44+i*2] << 8) | sensor_data_ipmb_read[45+i*2]) >> 4)/400);
//						}
					}
					break;
				default:
					break;
			}
		}

	memset(i2c_buffer_receiver, 0, I2C_nBytes);
	I2C_nBytes = 0;
//	delay_us(1000000);
	//succeed!
	return 0;
}
#endif
