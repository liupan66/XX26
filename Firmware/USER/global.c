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
/******************************Extern******************************/

/******************************Param*******************************/
uint8_t debugUsartRxBuffer[256];			//usart0 接收buff
uint8_t debugUsartTxBuffer[256];			//usart0 发送buff
uint8_t debugUsartRxCnt;							//usart0 接收buff计数	,	<=256
uint8_t debugUsartTxCnt;							//usart0 发送buff计数	,	<=256
uint16_t debugUsartRxSize;							//usart0 计划接收数据总长度	,	<=256
uint8_t debugUsartTxSize;							//usart0 计划发送数据总长度	,	<=256

uint8_t fpgaUsartRxBuffer[256];					//usart2 接收buff
uint8_t fpgaUsartTxBuffer[256];					//usart2 发送buff
uint8_t fpgaUsartRxCnt;									//usart2 接收buff计数	,	<=256
uint8_t fpgaUsartTxCnt;									//usart2 发送buff计数	,	<=256
uint8_t fpgaUsartRxSize;								//usart2 计划接收数据总长度	,	<=256
uint8_t fpgaUsartTxSize;								//usart2 计划发送数据总长度	,	<=256

uint8_t dsp1UsartRxBuffer[256];				//usart1 接收buff
uint8_t dsp1UsartTxBuffer[256];				//usart1 发送buff
uint8_t dsp1UsartRxCnt;								//usart1 接收buff计数	,	<=256
uint8_t dsp1UsartTxCnt;								//usart1 发送buff计数	,	<=256
uint8_t dsp1UsartRxSize;								//usart1 计划接收数据总长度	,	<=256
uint8_t dsp1UsartTxSize;								//usart1 计划发送数据总长度	,	<=256

uint8_t board_slot;
/******************************Param*******************************/

/******************************Func********************************/
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
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while (RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}

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
void dohitech_mcu_rcu_configuration(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_GPIOD);
	rcu_periph_clock_enable(RCU_AF);
}

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
void dohitech_board_oem(void)
{
	printf("\tMCU型号：%s\n", MCUTYPE);
	printf("\tDSP型号：%s\n", DSPTYPE);
	printf("\tFPGA型号：%s\n", FPGATYPE);
	printf("\tMCU 软件版本：%s\n", MCUSOFTWAREVERSION);
	printf("\tMCU 软件基础版本：%s\n", MCUSOFTWAREVERSIONBEFORE);
	printf("\tMCU 硬件版本：%s\n", MCUHARDWAREVERSION);
	printf("\t板卡编号：%s\n", BOARDNUMBER);
	board_slot = dohitech_mcu_vpx_read_slot();
	printf("\t板卡槽位号: 0x%x\n", board_slot);
}
/******************************Func********************************/
