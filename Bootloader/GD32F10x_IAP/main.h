/*!
    \file  main.h
    \brief the header file of main 
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-06, V1.0.0, firmware for GD32F3x0
*/

#ifndef __MAIN_H
#define __MAIN_H

#include "gd32f10x.h"

#define ApplicationATarget    0x08002000

/* led spark function */
void delay_us(uint32_t delay);
void led_spark(void);
ErrStatus gd_check_sum(uint8_t *data, uint32_t data_num, uint8_t check_data);
extern volatile uint8_t boot_to_reflash;
/* 待处理串口数据缓冲区 */
extern uint8_t rx0_date_buf[600];
/* 待处理数据个数：大于1为有数据待处理，0为没有数据待处理*/
extern uint32_t USART_RX_NUM;
/* DMA接收缓冲区 */
extern uint8_t dma_buffer[600];
#endif


