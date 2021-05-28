/*!
    \file  gd32f3x0_it.c
    \brief interrupt service routines
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-06, V1.0.0, firmware for GD32F3x0
*/

#include "gd32f10x_it.h"
#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include "FLASH.h"
#include "gd32f10x_dma.h"
#include "systick.h"

#define rxbuffersize   0x200

extern volatile uint8_t boot_to_reflash;
uint32_t start_addr;
uint8_t check_sum;
uint8_t read_mem_num;
uint8_t read_mem_data[32];
uint16_t num_pages;  //flash需要擦除的页数
uint16_t page_num;  //指定哪一页
uint8_t command_data[11] = {0x00, 0x01, 0x02, 0x11, 0x21, 0x31, 0x43, 0x63, 0x73, 0x82, 0x92};
static uint8_t USART_RX[12];
extern uint32_t num_of_data_write;

extern uint8_t rxbuffer[rxbuffersize];


void usart_rx_clear(void)
{
    uint8_t i;
    for(i = 0; i < 12; i++){
        USART_RX[i] = 0;
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

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1){
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

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	delay_decrement();
}

/*!
    \brief      this function handles DMA interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA0_Channel4_IRQHandler(void)
{
	uint8_t i=0;
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
						else
							i=1;
							return;
        }
    }
}

/*!
    \brief      this function handles USART interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART0_IRQHandler(void)
{
	if(boot_to_reflash == 0)
	{
		if(RESET != usart_flag_get(USART0, USART_FLAG_RBNE))                                          //等待串口命令
    {
        uint8_t af=0;

        USART_RX[af++] = usart_data_receive(USART0);
				if((USART_RX[af-1] == 'Y') || (USART_RX[af-1] == 'y'))
					boot_to_reflash = 1;
		}
	}
	else
	{
    if(RESET != usart_flag_get(USART0, USART_FLAG_RBNE))                                          //等待串口命令
    {
        static uint8_t i;
        uint8_t j;

        USART_RX[i++] = usart_data_receive(USART0);

        if(0x7F == USART_RX[0]){
            i = 0;
            usart_data_transmit(USART0, 0x79);
            while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
        }

        /* read memory */
        if((0x11 == USART_RX[0]) && (0xEE == USART_RX[1])){
            if(2 == i){
                usart_data_transmit(USART0, 0x79);
                while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
            }
            
            if(7 == i){
                start_addr = ((uint32_t)USART_RX[2] << 24) | ((uint32_t)USART_RX[3] << 16) | (uint32_t)(USART_RX[4] << 8) | (uint32_t)USART_RX[5];
                check_sum = USART_RX[6];
                if(SUCCESS == gd_check_sum(&USART_RX[2], 4, check_sum)){
                    usart_data_transmit(USART0, 0x79);
                    while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
                }
            }
            
            if(9 == i){
                i = 0;
                read_mem_num = USART_RX[7];
                check_sum = USART_RX[8];
                if(SUCCESS == gd_check_sum(&USART_RX[7], 1, check_sum)){
                    usart_data_transmit(USART0, 0x79);
                    while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
                }
                
                read_memory(start_addr, read_mem_data, read_mem_num + 1);
                for(j = 0; j < read_mem_num + 1; j++){
                    usart_data_transmit(USART0, read_mem_data[j]);
                    while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
                }
                usart_rx_clear();
            }
        }
        
        /* Get the GD product ID 0x40022100*/
        if((0x06 == USART_RX[0]) && (0xF9 == USART_RX[1])){
            usart_data_transmit(USART0, 0x79);
            
            while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
            usart_data_transmit(USART0, 0x04);      //获取的数据字节数
            while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
            
            read_fmc_pid(read_mem_data);            //读FMC_PID寄存器，并放入数组
            for(j = 0; j < 4; j++){
                usart_data_transmit(USART0, read_mem_data[j]);
                while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
            }
            usart_data_transmit(USART0, 0x79);
            while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
            usart_rx_clear();
            i = 0;
        }

        /* get the version of the bootloader and the supported commands */
        if((0x00 == USART_RX[0]) && (0xFF == USART_RX[1])){
            usart_data_transmit(USART0, 0x79);      //ACK
            while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
            
            usart_data_transmit(USART0, 0x0B);      //数据长度（boot版本+支持的命令）
            while(RESET == usart_flag_get(USART0, USART_FLAG_TC));

            usart_data_transmit(USART0, 0x22);      //boot版本
            while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
            
            /* 发送支持的命令 */
            for(j = 0; j < 11; j++){
                usart_data_transmit(USART0, command_data[j]);
                while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
            }
            usart_data_transmit(USART0, 0x79);
            while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
            usart_rx_clear();
            i = 0;
        }
        
        
        //擦除Flash，上位机会计算bin文件的大小，将需要擦除的页数发送到下位机
        if((0x43 == USART_RX[0]) && (0xBC == USART_RX[1])){
            if(2 == i){
                usart_data_transmit(USART0, 0x79);
                while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
            }
            
            if(5 == i){
                num_pages  = (uint16_t)(USART_RX[2] + 1);
                page_num = (uint16_t)USART_RX[3];
                check_sum = USART_RX[4];
                
                if(SUCCESS == gd_check_sum(&USART_RX[2], 2, check_sum)){
                    erase_page(num_pages, page_num);
                    usart_data_transmit(USART0, 0x79);
                    while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
                }else{
                    usart_data_transmit(USART0, 0xFF);
                    while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
                }
                
                usart_rx_clear();
                i = 0;
            }
        }
        
        //Extended Erase Memory command
        if((0x44 == USART_RX[0]) && (0xBB == USART_RX[1])){
            if(2 == i){
                usart_data_transmit(USART0, 0x79);
                while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
            }
            
            if(7 == i){
                num_pages = (USART_RX[2] << 8) | (USART_RX[3]) + 1;
                page_num = (USART_RX[4] << 8) | (USART_RX[5]);
                check_sum = USART_RX[6];
                
                if(SUCCESS == gd_check_sum(&USART_RX[2], 4, check_sum)){
                    erase_page(num_pages, page_num);
                    usart_data_transmit(USART0, 0x79);
                    while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
                }else{
                    usart_data_transmit(USART0, 0xFF);
                    while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
                }
                usart_rx_clear();
                i = 0;
            }
        }
        
        //Write Memory命令
        if((0x31 == USART_RX[0]) && 0xCE == USART_RX[1]){
            if( 2 == i){
                usart_data_transmit(USART0, 0x79);
                while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
            }
            
            //写入内存的地址
            if(7 == i){
                start_addr = (USART_RX[2] << 24) | (USART_RX[3] << 16) | (USART_RX[4] << 8) | USART_RX[5];
                check_sum = USART_RX[6];
                if(SUCCESS == gd_check_sum(&USART_RX[2], 4, check_sum)){
                    usart_data_transmit(USART0, 0x79);
                    while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
                }
            }
            
            if(8 == i){
                i = 0;
                num_of_data_write = (uint32_t)USART_RX[7] + 1;
                
                dma_transfer_number_config(DMA0, DMA_CH4, num_of_data_write + 1);
                
                usart_dma_receive_config(USART0, USART_DENR_ENABLE);
                usart_interrupt_disable(USART0, USART_INT_RBNE);
                dma_channel_enable(DMA0, DMA_CH4);
                
                usart_rx_clear();
            }
        }
        
        if((0x21 == USART_RX[0]) && 0xDE == USART_RX[1]){
            if(2 == i){
                while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
                usart_data_transmit(USART0, 0x79);
            }
            if(7 == i){
                i = 0;
                check_sum = USART_RX[6];
                if(SUCCESS == gd_check_sum(&USART_RX[2], 4, check_sum)){
                    usart_data_transmit(USART0, 0x79);
                    while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
                }
                usart_rx_clear();
                NVIC_SystemReset();
            }
        }
    }
	}
}
