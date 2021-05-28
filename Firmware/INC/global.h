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
extern uint8_t debugUsartRxBuffer[256];				//usart0 ����buff
extern uint8_t debugUsartTxBuffer[256];				//usart0 ����buff
extern uint8_t debugUsartRxCnt;								//usart0 ����buff����	,	<=256
extern uint8_t debugUsartTxCnt;								//usart0 ����buff����	,	<=256
extern uint16_t debugUsartRxSize;							//usart0 �ƻ����������ܳ���	,	<=256
extern uint8_t debugUsartTxSize;							//usart0 �ƻ����������ܳ���	,	<=256

extern uint8_t fpgaUsartRxBuffer[256];					//usart2 ����buff
extern uint8_t fpgaUsartTxBuffer[256];					//usart2 ����buff
extern uint8_t fpgaUsartRxCnt;									//usart2 ����buff����	,	<=256
extern uint8_t fpgaUsartTxCnt;									//usart2 ����buff����	,	<=256
extern uint8_t fpgaUsartRxSize;									//usart2 �ƻ����������ܳ���	,	<=256
extern uint8_t fpgaUsartTxSize;									//usart2 �ƻ����������ܳ���	,	<=256

extern uint8_t dsp1UsartRxBuffer[256];				//usart1 ����buff
extern uint8_t dsp1UsartTxBuffer[256];				//usart1 ����buff
extern uint8_t dsp1UsartRxCnt;								//usart1 ����buff����	,	<=256
extern uint8_t dsp1UsartTxCnt;								//usart1 ����buff����	,	<=256
extern uint8_t dsp1UsartRxSize;								//usart1 �ƻ����������ܳ���	,	<=256
extern uint8_t dsp1UsartTxSize;								//usart1 �ƻ����������ܳ���	,	<=256

extern	uint8_t	sensor_data_ipmb_read[48];		//����ֵ������fpga
extern	uint16_t	current_temperature[3];			//ϵͳ�¶�
extern	uint16_t current_voltage[8];					//ϵͳ��ѹ
extern	uint8_t board_slot;										//��λ��

/* �����������ݻ����� */
extern uint8_t rx0_date_buf[600];
/* ���������ݸ���������1Ϊ�����ݴ�����0Ϊû�����ݴ�����*/
extern uint32_t USART_RX_NUM;
/* DMA���ջ����� */
extern uint8_t dma_buffer[600];
/******************************Extern******************************/

/******************************Param*******************************/
/******************************Param*******************************/

/******************************Func********************************/
/***************************************************
* ������		dohitech_mcu_rcu_configuration
* ���ܣ�		rcuʹ��
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght(DylanShang)		����
**************************************************/
extern void dohitech_mcu_rcu_configuration(void);

/***************************************************
* ������	dohitech_mcu_led_toggle
* ���ܣ�	led ��˸
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght(DylanShang)		����
**************************************************/
void dohitech_mcu_led_toggle(void);	

/***************************************************
* ������	dohitech_mcu_dsp_reset
* ���ܣ�	RESET HX DSP
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght(DylanShang)		����
**************************************************/
void dohitech_mcu_dsp_reset(void);

/***************************************************
* ������dohitech_mcu_debug_usart_init
* ���ܣ�mcu �����ô��ڳ�ʼ��
* ������                           in/out          ˵��
* 			baudrate		���ڲ�����
*����ֵ              ˵��
*					���س�ʼ��״̬
* ˵����		
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
ErrStatus dohitech_mcu_debug_usart_init(uint32_t baudrate);

/***************************************************
* ������dohitech_mcu_dsp1_usart_init
* ���ܣ�mcu - dsp1���ڳ�ʼ��
* ������                           in/out          ˵��
* 			baudrate		���ڲ�����
*����ֵ              ˵��
*					���س�ʼ��״̬
* ˵����		
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
ErrStatus dohitech_mcu_dsp1_usart_init(uint32_t baudrate);


/***************************************************
* ������	ds18b20_test
* ���ܣ�  get temperature
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*					��ȡ�¶�ֵ
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
float ds18b20_test(void);

/***************************************************
* ������	ds18b20_2_test
* ���ܣ�  get temperature
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*					��ȡ�¶�ֵ
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void ds18b20_2_test(void);

/***************************************************
* ������	ds18b20_3_test
* ���ܣ�  get temperature
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*					��ȡ�¶�ֵ
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void ds18b20_3_test(void);

/***************************************************
* ������dohitech_mcu_fpga_usart_init
* ���ܣ�mcu - fpga���ڳ�ʼ��
* ������                           in/out          ˵��
* 			baudrate		���ڲ�����
*����ֵ              ˵��
*					���س�ʼ��״̬
* ˵����		
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
ErrStatus dohitech_mcu_fpga_usart_init(uint32_t baudrate);

/***************************************************
* ������dohitech_mcu_fpga_usart_transfer
* ���ܣ�mcu - fpga�������ݷ��ͺ���
* ������                           in/out          ˵��
* 			txBuff		�������ݵ�ַ
*				txLen			���������ܳ���(<=256)
*����ֵ              ˵��
*					���ط��ͽ��
* ˵����			
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
ErrStatus dohitech_mcu_fpga_usart_selfTest(void);

/***************************************************
* ������	dohitech_mcu_adc_init
* ���ܣ�	init adc function
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void dohitech_mcu_adc_init(void);

/***************************************************
* ������	dohitech_mcu_adc_value_get
* ���ܣ�	��ȡadc ����ֵ
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void dohitech_mcu_adc_value_get(void);

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
uint8_t dohitech_mcu_vpx_read_slot(void);

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
void dohitech_mcu_reset_vpx(void);

/***************************************************
* ������	dohitech_mcu_v7_mode_set
* ���ܣ�	����v7����ģʽ
* ������                           in/out          ˵��
* 					v7����ģʽ
*����ֵ              ˵��
*						���ý��
* ˵���� 
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
ErrStatus dohitech_mcu_v7_mode_set(uint8_t v7_mode);

/***************************************************
* ������	DohitechMcuResetV7
* ���ܣ�	��λv7
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void DohitechMcuResetV7(void);

/***************************************************
* ������	dohitech_mcu_v7_gpio
* ���ܣ�	����v7 gpio0~7 Ϊ����ߵ�ƽ
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*					
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void dohitech_mcu_v7_gpio(void);

/***************************************************
* ������	dohitech_mcu_bsp_i2c_init
* ���ܣ�	i2c��ʼ��
* ������                           in/out          ˵��
* 				
*����ֵ              ˵��
*					
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void dohitech_mcu_bsp_i2c_init(void);

/***************************************************
* ������	dohitech_mcu_i2c_test
* ���ܣ�	i2c test
* ������                           in/out          ˵��
* 				
*����ֵ              ˵��
*					
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
void dohitech_mcu_i2c_test(void);
	
/***************************************************
* ������		dohitech_board_oem
* ���ܣ�		�忨��Ϣ
* ������                           in/out          ˵��
* 
*����ֵ              ˵��
*
* ˵����
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght(DylanShang)		����
**************************************************/
void dohitech_board_oem(void);

/***************************************************
* ������	fputc
* ���ܣ�retarget the C library printf function to the USART
* ������                           in/out          ˵��
* 			
*����ֵ              ˵��
*
* ˵����		
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
int fputc(int ch, FILE *f);
/***************************************************
* ������	DohitechMcuAdcState
* ���ܣ���ȡ��ѹ״ֵ̬
* ������                           in/out          ˵��
* 			
*����ֵ              ˵��
*
* ˵����		
* ʱ��         �汾          ����            ˵��
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     ����
**************************************************/
uint8_t DohitechMcuAdcState(void);


void change_to_boot(void);
/******************************Func********************************/

void DMA_config(void);
uint8_t process_command(uint8_t *command);
void SelfonlineCheck(void);
ErrStatus dohitech_mcu_bsp_timer_init(void);
#endif	//__GLOBAL_H__
