/*!
    \file  main.c
    \brief The main function file
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-06, V1.0.0, firmware for GD32F3x0
*/

#include "gd32f10x.h"
#include <stdlib.h>
#include <stdio.h>
#include "gd32f10x_it.h"
#include "gd32f10x_dma.h"
#include "gd32f10x_fmc.h"
#include "gd32f10x_crc.h"
#include "main.h"
#include "FLASH.h"
#include "command.h"
void USART_config(void);
void DMA_config(void);
void NVIC_config(void);
void gpio_config(void);
void USART0_config(void);
int fputc(int ch, FILE *f);

typedef  void (*pFunction)(void);
#define rxbuffersize   0x200
#define ApplicationAddress    0x08002800
extern  uint8_t flag;
pFunction Jump_To_Application;
uint32_t JumpAddress = 0;
uint8_t rxbuffer[rxbuffersize];

uint32_t num_of_data_write = rxbuffersize;


volatile uint8_t boot_to_reflash;

/* DMA接收缓冲区 */
uint8_t dma_buffer[600];
/* 待处理串口数据缓冲区 */
uint8_t rx0_date_buf[600];
/* 待处理数据个数：大于1为有数据待处理，0为没有数据待处理*/
uint32_t USART_RX_NUM = 0;
union Online_update_order_feedback_union Online_update_order_feedback_union_entity_1;
	
void delay_us(uint32_t delay)
{
	uint8_t i;
	while(delay--)
	{
		for(i=0; i<27; i++)
			__NOP;
	}
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

	GPIO_BC(GPIOA) = GPIO_PIN_0;
	GPIO_BC(GPIOB) = GPIO_PIN_9;
	GPIO_BC(GPIOC) = GPIO_PIN_13;
	delay_us(5000);	
	GPIO_BOP(GPIOA) = GPIO_PIN_0;
	delay_us(5000);
	GPIO_BOP(GPIOC) = GPIO_PIN_13;
	delay_us(5000);
	GPIO_BOP(GPIOB) = GPIO_PIN_9;
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
	uint8_t str1[8]={0};
	uint8_t i=0;
	uint32_t va0;
	boot_to_reflash = 0x79;	
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	DMA_config();
	USART_config();
	NVIC_config();
	USART0_config();
	//准备反馈数据
	Online_update_order_feedback_union_entity_1.Online_update_order_feedback.Packet_header=0x7E7EAA55;
	Online_update_order_feedback_union_entity_1.Online_update_order_feedback.Data_length_1=0x0020;							//Data_length_1
	Online_update_order_feedback_union_entity_1.Online_update_order_feedback.Data_length_2=0x0020;							//Data_length_2
	Online_update_order_feedback_union_entity_1.Online_update_order_feedback.Information_label=0x0F01;						//Information_label
	Online_update_order_feedback_union_entity_1.Online_update_order_feedback.Frame_number=0;								//Frame_number
	Online_update_order_feedback_union_entity_1.Online_update_order_feedback.Online_programming_source=0x1111;				//Online_programming_source
	Online_update_order_feedback_union_entity_1.Online_update_order_feedback.Backup_1=0;									//Backup_1
	Online_update_order_feedback_union_entity_1.Online_update_order_feedback.Software_configuration_item_name_1=0x00EE;		//Software_configuration_item_name_1
	Online_update_order_feedback_union_entity_1.Online_update_order_feedback.Software_configuration_item_name_2=0x00EE;		//Software_configuration_item_name_2
	Online_update_order_feedback_union_entity_1.Online_update_order_feedback.Order_status=0x5555;							//Order_status
	Online_update_order_feedback_union_entity_1.Online_update_order_feedback.Response_status=0xAAAA;							//Response_status
	Online_update_order_feedback_union_entity_1.Online_update_order_feedback.Backup_2=0;										//Backup_2
	for(i=0;i<8;i++)
	{
		Online_update_order_feedback_union_entity_1.Online_update_order_feedback.Reserve[i]=str1[i];							//Reserve[8];
	}
	Online_update_order_feedback_union_entity_1.Online_update_order_feedback.Checksum=calc_checksum(&Online_update_order_feedback_union_entity_1.str[4],30);	//Checksum
	Online_update_order_feedback_union_entity_1.Online_update_order_feedback.Packet_tail=0x0D0DFE5A; 							//Packet_tail		
	/*1、芯片第一次使用，未烧录App时，对该标志进行初始化*/
	if((*(__IO uint32_t*)ApplicationATarget != 0) && (*(__IO uint32_t*)ApplicationATarget != 1))
	{	
		va0=0;		
		erase_page(1,4);
		if(FLASH_Program(ApplicationATarget, 4, &va0) != PASSED){
			if(FLASH_Program(ApplicationATarget, 4, &va0) != PASSED){
				printf("\t程序更新标志初始化失败\n");
			}
		}
	}

//	dohitech_mcu_v7_mode_set(0x06);

	/*2、判断标志，根据标志判断进入哪个分支Boot/App*/
	if(*(__IO uint32_t*)ApplicationATarget == 0)
	{
			/*3、App地区有程序，则进入APP分支运行*/
			if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)        //ApplicationAddress为新程序的起始地址，检查栈顶地址是否合法，即栈顶地址是否为0x2000xxxx（内置SRAM）
			{
					usart_disable(USART0);
					usart_disable(USART2);
					dma_channel_disable(DMA0, DMA_CH2);
					JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);               //用户代码区第二个字存储为新程序起始地址（新程序复位向量指针）
					Jump_To_Application = (pFunction) JumpAddress;                          
					__set_MSP(*(__IO uint32_t*) ApplicationAddress);                        //初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
					Jump_To_Application();                                                  // 设置PC指针为新程序复位中断函数的地址
			}
			/*4、App地区无程序，则将标志位置位，进入BOOT分支，等待程序更新*/
			else
			{
				dohitech_mcu_dsp_reset();
				delay_us(500000);	
				usart_data_transmit(USART2, 0xFF);
				boot_to_reflash = 0xff;
				va0 = 1;
				erase_page(1,4);
				if(FLASH_Program(ApplicationATarget, 4, &va0) != PASSED){
					if(FLASH_Program(ApplicationATarget, 4, &va0) != PASSED){
						printf("\t写标志位失败!\n");
					}
				}
//				for(i=0;i<40;i++)//发送回馈指令数据
//				{
//					usart_data_transmit(USART2,Online_update_order_feedback_union_entity_1.str[i]);
//					while(RESET == usart_flag_get(USART2, USART_FLAG_TC));
//				}
			}
	}
	if(*(__IO uint32_t*)(ApplicationATarget+4) == 1)
	{
		for(i=0;i<40;i++)//发送回馈指令数据
		{
			usart_data_transmit(USART2,Online_update_order_feedback_union_entity_1.str[i]);
			while(RESET == usart_flag_get(USART2, USART_FLAG_TC));
		}
	}
	/*进入BOOT分支*/
	else
	{
		dohitech_mcu_dsp_reset();
		delay_us(500000);	
		usart_data_transmit(USART2, 0xFF);
		printf("\t等待程序更新......\n");		
	}

  while (1)
	{
		delay_us(500);
		if(USART_RX_NUM > 0)
		{
			process_command(rx0_date_buf);
			USART_RX_NUM = 0;
		}
	}
}

void USART0_config(void)
{ 
				/* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);
	
    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_parity_config(USART0, USART_PM_EVEN);
    usart_word_length_set(USART0, USART_WL_9BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
}

void USART_config(void)
{ 
	rcu_periph_clock_enable(RCU_USART2);	
	rcu_periph_clock_enable(RCU_AF);
	gpio_pin_remap_config(GPIO_USART2_PARTIAL_REMAP, ENABLE);
	
    /* connect port to USARTx_Tx */
    gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    /* USART configure */
    usart_deinit(USART2);
    usart_baudrate_set(USART2, 115200U);//
	usart_stop_bit_set(USART2, USART_STB_1BIT);
	usart_parity_config(USART2, USART_PM_NONE);
	usart_word_length_set(USART2, USART_WL_8BIT);
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
	usart_interrupt_flag_clear(USART2,USART_FLAG_IDLEF);	
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
    usart_enable(USART2);
		
		 /* Enable USART2 DMA Rx request */
    usart_dma_receive_config(USART2, USART_DENR_ENABLE);
		/* enable USART TBE interrupt */  
    usart_interrupt_enable(USART2, USART_INT_IDLE);
}

void gpio_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
}

void DMA_config(void)
{
    dma_parameter_struct dma_init_struct;
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA0);
    
    /* deinitialize DMA channel2 */
    dma_deinit(DMA0, DMA_CH2);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)dma_buffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = sizeof(dma_buffer);//(uint32_t)num_of_data_write;
    
    dma_init_struct.periph_addr = 0x40004804;//0x40013804;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH2, &dma_init_struct);
    dma_circulation_disable(DMA0,DMA_CH2);
	dma_memory_to_memory_disable(DMA0,DMA_CH2);
	dma_channel_enable(DMA0, DMA_CH2);
   // dma_flag_clear(DMA0, DMA_CH2, DMA_FLAG_FTF);
   // dma_interrupt_enable(DMA0, DMA_CH2, DMA_INT_FTF);
}

void NVIC_config(void)
{
    /* Enable the USART2 Interrupt */
    nvic_irq_enable(USART2_IRQn, 0, 0);
    
    /* Enable the DMA Interrupt */
   // nvic_irq_enable(DMA0_Channel2_IRQn, 0, 0);
}

ErrStatus gd_check_sum(uint8_t *data, uint32_t data_num, uint8_t check_data)
{
    uint8_t check_sum = 0;
    uint8_t *p = data;
    uint16_t i;
    if(1 == data_num){
        check_sum = ~(*p);
    }else{
        for(i = 0; i < data_num; i++){
            check_sum ^= *p;
            p++;
        }
    }
    if(check_sum == check_data){
        return SUCCESS;
    }else{
        return ERROR;
    }
}

/***************************************************
* 函数：		fputc
* 功能：		Retargets the C library printf function to the USART.
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght(DylanShang)		创建
**************************************************/
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  usart_data_transmit(USART0, (uint8_t) ch);

  /* Loop until the end of transmission */
   while(RESET == usart_flag_get(USART0, USART_FLAG_TC));

  return ch;
}

