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
#ifndef	__GLOBAL_H__
#define	__GLOBAL_H__

/******************************include*****************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gd32f10x.h"
#include "gd32f10x_adc.h"
#include "gd32f10x_gpio.h"
#include "gd32f10x_usart.h"
#include "gd32f10x_dma.h"
#include "gd32f10x_it.h"
#include "gd32f10x_i2c.h"
#include "systick.h"
#include "gd32f10x_fmc.h"
#include "gd32f10x_timer.h"
#include "FLASH.h"
#include "command.h"
/******************************include*****************************/
#define	YEAR						2021
#define MONTH						3
#define	DATE						4
//V1.0.0
#define VERSION_H				0x1
#define	VERSION_M	      0
#define VERSION_L				0
/******************************define******************************/
#define DEBUGON	1
#define BOOTLOADER 1
#define I2CTESTEN	1
/* OEM */
#define	MCUTYPE										"GD32F103RBT6"
#define DSPTYPE										"TMS320C6678"
#define	FPGATYPE									"XC7VX690T"
#define MCUSOFTWAREVERSION				"V00B00F00"
#define	MCUSOFTWAREVERSIONBEFORE	"V00B00F00"
#define MCUHARDWAREVERSION				"3UVPX_C6678_V7_IO_V1.0"
#define	BOARDNUMBER								"2020080004"
/******************************define******************************/

/******************************Extern******************************/
extern uint8_t debugUsartRxBuffer[256];				//usart0 接收buff
extern uint8_t debugUsartTxBuffer[256];				//usart0 发送buff
extern uint8_t debugUsartRxCnt;								//usart0 接收buff计数	,	<=256
extern uint8_t debugUsartTxCnt;								//usart0 发送buff计数	,	<=256
extern uint16_t debugUsartRxSize;							//usart0 计划接收数据总长度	,	<=256
extern uint8_t debugUsartTxSize;							//usart0 计划发送数据总长度	,	<=256

extern uint8_t fpgaUsartRxBuffer[256];					//usart2 接收buff
extern uint8_t fpgaUsartTxBuffer[256];					//usart2 发送buff
extern uint8_t fpgaUsartRxCnt;									//usart2 接收buff计数	,	<=256
extern uint8_t fpgaUsartTxCnt;									//usart2 发送buff计数	,	<=256
extern uint8_t fpgaUsartRxSize;									//usart2 计划接收数据总长度	,	<=256
extern uint8_t fpgaUsartTxSize;									//usart2 计划发送数据总长度	,	<=256

extern uint8_t dsp1UsartRxBuffer[256];				//usart1 接收buff
extern uint8_t dsp1UsartTxBuffer[256];				//usart1 发送buff
extern uint8_t dsp1UsartRxCnt;								//usart1 接收buff计数	,	<=256
extern uint8_t dsp1UsartTxCnt;								//usart1 发送buff计数	,	<=256
extern uint8_t dsp1UsartRxSize;								//usart1 计划接收数据总长度	,	<=256
extern uint8_t dsp1UsartTxSize;								//usart1 计划发送数据总长度	,	<=256

extern	uint8_t	sensor_data_ipmb_read[48];		//数据值发送至fpga
extern	uint16_t	current_temperature[3];			//系统温度
extern	uint16_t current_voltage[8];					//系统电压
extern	uint8_t board_slot;										//槽位号

/* 待处理串口数据缓冲区 */
extern uint8_t rx0_date_buf[600];
/* 待处理数据个数：大于1为有数据待处理，0为没有数据待处理*/
extern uint32_t USART_RX_NUM;
/* DMA接收缓冲区 */
extern uint8_t dma_buffer[600];
/******************************Extern******************************/

/******************************Param*******************************/
/******************************Param*******************************/

/******************************Func********************************/
/***************************************************
* 函数：		dohitech_mcu_rcu_configuration
* 功能：		rcu使能
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght(DylanShang)		创建
**************************************************/
extern void dohitech_mcu_rcu_configuration(void);

/***************************************************
* 函数：	dohitech_mcu_led_toggle
* 功能：	led 闪烁
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght(DylanShang)		创建
**************************************************/
void dohitech_mcu_led_toggle(void);	

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
void dohitech_mcu_dsp_reset(void);

/***************************************************
* 函数：dohitech_mcu_debug_usart_init
* 功能：mcu 调试用串口初始化
* 参数：                           in/out          说明
* 			baudrate		串口波特率
*返回值              说明
*					返回初始化状态
* 说明：		
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
ErrStatus dohitech_mcu_debug_usart_init(uint32_t baudrate);

/***************************************************
* 函数：dohitech_mcu_dsp1_usart_init
* 功能：mcu - dsp1串口初始化
* 参数：                           in/out          说明
* 			baudrate		串口波特率
*返回值              说明
*					返回初始化状态
* 说明：		
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
ErrStatus dohitech_mcu_dsp1_usart_init(uint32_t baudrate);


/***************************************************
* 函数：	ds18b20_test
* 功能：  get temperature
* 参数：                           in/out          说明
* 
*返回值              说明
*					读取温度值
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
float ds18b20_test(void);

/***************************************************
* 函数：	ds18b20_2_test
* 功能：  get temperature
* 参数：                           in/out          说明
* 
*返回值              说明
*					读取温度值
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void ds18b20_2_test(void);

/***************************************************
* 函数：	ds18b20_3_test
* 功能：  get temperature
* 参数：                           in/out          说明
* 
*返回值              说明
*					读取温度值
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void ds18b20_3_test(void);

/***************************************************
* 函数：dohitech_mcu_fpga_usart_init
* 功能：mcu - fpga串口初始化
* 参数：                           in/out          说明
* 			baudrate		串口波特率
*返回值              说明
*					返回初始化状态
* 说明：		
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
ErrStatus dohitech_mcu_fpga_usart_init(uint32_t baudrate);

/***************************************************
* 函数：dohitech_mcu_fpga_usart_transfer
* 功能：mcu - fpga串口数据发送函数
* 参数：                           in/out          说明
* 			txBuff		发送数据地址
*				txLen			发送数据总长度(<=256)
*返回值              说明
*					返回发送结果
* 说明：			
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
ErrStatus dohitech_mcu_fpga_usart_selfTest(void);

/***************************************************
* 函数：	dohitech_mcu_adc_init
* 功能：	init adc function
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_adc_init(void);

/***************************************************
* 函数：	dohitech_mcu_adc_value_get
* 功能：	获取adc 采样值
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_adc_value_get(void);

/***************************************************
* 函数：	dohitech_mcu_vpx_read_slot
* 功能：	读取vpx gpio 槽位号数据
* 参数：                           in/out          说明
* 
*返回值              说明
*					返回读取到的槽位号数据
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
uint8_t dohitech_mcu_vpx_read_slot(void);

/***************************************************
* 函数：	dohitech_mcu_reset_vpx
* 功能：	复位vpx
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_reset_vpx(void);

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
ErrStatus dohitech_mcu_v7_mode_set(uint8_t v7_mode);

/***************************************************
* 函数：	DohitechMcuResetV7
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
void DohitechMcuResetV7(void);

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
void dohitech_mcu_v7_gpio(void);

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
void dohitech_mcu_bsp_i2c_init(void);

/***************************************************
* 函数：	dohitech_mcu_i2c_test
* 功能：	i2c test
* 参数：                           in/out          说明
* 				
*返回值              说明
*					
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_i2c_test(void);
	
/***************************************************
* 函数：		dohitech_board_oem
* 功能：		板卡信息
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght(DylanShang)		创建
**************************************************/
void dohitech_board_oem(void);

/***************************************************
* 函数：	fputc
* 功能：retarget the C library printf function to the USART
* 参数：                           in/out          说明
* 			
*返回值              说明
*
* 说明：		
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
int fputc(int ch, FILE *f);
/***************************************************
* 函数：	DohitechMcuAdcState
* 功能：获取电压状态值
* 参数：                           in/out          说明
* 			
*返回值              说明
*
* 说明：		
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
uint8_t DohitechMcuAdcState(void);


void change_to_boot(void);
/******************************Func********************************/

void DMA_config(void);
uint8_t process_command(uint8_t *command);
void SelfonlineCheck(void);
ErrStatus dohitech_mcu_bsp_timer_init(void);
#endif	//__GLOBAL_H__
