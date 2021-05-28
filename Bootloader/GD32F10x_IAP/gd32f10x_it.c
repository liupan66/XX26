/*!
    \file  gd32f3x0_it.c
    \brief interrupt service routines
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-06, V1.0.0, firmware for GD32F3x0
*/

#include "gd32f10x_it.h"
#include "gd32f10x_dma.h"
#include "gd32f10x_fmc.h"
#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include "FLASH.h"
#include "command.h"
#include "string.h"
extern volatile uint8_t boot_to_reflash;
uint8_t	flag=0;
#define rxbuffersize   0x200
uint16_t u2cnt = 0;
uint32_t start_addr;
uint8_t check_sum;
uint8_t read_mem_num;
uint8_t read_mem_data[32];
uint16_t num_pages;  //flash需要擦除的页数
uint16_t page_num;  //指定哪一页
extern uint8_t Packet_header[4];
uint8_t USART_RX[544]={0};
extern uint32_t num_of_data_write;

extern uint8_t rxbuffer[rxbuffersize];

extern	void delay_us(uint32_t delay);
extern void dohitech_mcu_dsp_reset(void);

void usart_rx_clear(void)
{
//    uint16_t i;
//    for(i = 0; i <544; i++){
//        USART_RX[i] = 0;
//    }
	memset(USART_RX,0,544);
	u2cnt=0;
}

static void delay_us(uint32_t delay)
{
	uint8_t i;
	while(delay--)
	{
		for(i=0; i<27; i++)
			__NOP;
	}
}

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

__ASM void wait(void)
{
	BX lr
}
/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
	if(CoreDebug->DHCSR&1)
	{
		__breakpoint(0);
	}
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1){
//			__asm voalite("BKPT #1");
//			wait();
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles DMA interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA0_Channel4_IRQHandler(void)
{
    /* Wait until USARTy RX DMA1 Channel Transfer Complete */
    if(RESET != dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF))            //DMA发送完成
    {
        dma_flag_clear(DMA0, DMA_CH4, DMA_FLAG_FTF);                  //清除DMA中断标志
        dma_channel_disable(DMA0, DMA_CH4);
        rxbuffer[0] = num_of_data_write - 1;

        if(SUCCESS == gd_check_sum(rxbuffer, num_of_data_write + 1, rxbuffer[num_of_data_write + 1])){
            if(FLASH_Program(start_addr, num_of_data_write, (uint32_t *)&rxbuffer[1]) != FAILED){
							
                while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
                usart_data_transmit(USART0, 0x79);
                
                //一包数据写入Flash完成，关闭串口DMA，打开串口中断等待下一包数据
                usart_dma_receive_config(USART0, USART_DENR_DISABLE);
                usart_interrupt_enable(USART0, USART_INT_RBNE);
            }
        }
    }
}

/*!
    \brief      this function handles DMA interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA0_Channel2_IRQHandler(void)
{
    /* Wait until USARTy RX DMA1 Channel Transfer Complete */
    if(RESET != dma_flag_get(DMA0, DMA_CH2, DMA_FLAG_FTF))            //DMA发送完成
    {
        dma_flag_clear(DMA0, DMA_CH2, DMA_FLAG_FTF);                  //清除DMA中断标志
        dma_channel_disable(DMA0, DMA_CH2);
        rxbuffer[0] = num_of_data_write - 1;

        if(SUCCESS == gd_check_sum(rxbuffer, num_of_data_write + 1, rxbuffer[num_of_data_write + 1])){			
            if(FLASH_Program(start_addr, num_of_data_write, (uint32_t *)&rxbuffer[1]) != FAILED){				
                usart_data_transmit(USART2, 0x79);
								while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
                //一包数据写入Flash完成，关闭串口DMA，打开串口中断等待下一包数据
                usart_dma_receive_config(USART2, USART_DENR_DISABLE);
                usart_interrupt_enable(USART2, USART_INT_RBNE); 
            }
						else
						{
							usart_data_transmit(USART2, 0xff);
							while(RESET == usart_flag_get(USART2, USART_FLAG_TC));
						}
        }
				else{
					usart_data_transmit(USART2, 0xff);
					while(RESET == usart_flag_get(USART2, USART_FLAG_TC));
				}
    }
}

/*!
    \brief      this function handles USART interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/

void USART2_IRQHandler(void)
{
	
	if(RESET != usart_flag_get(USART2, USART_FLAG_IDLEF)) //空闲中断
	{
		usart_interrupt_flag_clear(USART2,USART_FLAG_IDLEF);	/* 清除空闲中断标志位 */
		usart_data_receive(USART2);								/* 清除接收完成标志位 */	
		dma_channel_disable(DMA0, DMA_CH2);						/* 关闭DMA传输 */
		USART_RX_NUM = sizeof(dma_buffer) - dma_transfer_number_get(DMA0,DMA_CH2);
		memcpy(rx0_date_buf,dma_buffer,USART_RX_NUM); /* 转存数据到待处理数据缓冲区*/
		rx0_date_buf[USART_RX_NUM] = '\0';	/* 添加字符串结束符 */
		/* 重新设置DMA传输 */
		dma_memory_address_config(DMA0,DMA_CH2,(uint32_t)dma_buffer);
		dma_transfer_number_config(DMA0,DMA_CH2,sizeof(dma_buffer));
		dma_channel_enable(DMA0, DMA_CH2);		/* 开启DMA传输 */
	}		
}
