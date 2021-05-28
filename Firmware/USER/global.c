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
uint8_t debugUsartRxBuffer[256];			//usart0 ����buff
uint8_t debugUsartTxBuffer[256];			//usart0 ����buff
uint8_t debugUsartRxCnt;							//usart0 ����buff����	,	<=256
uint8_t debugUsartTxCnt;							//usart0 ����buff����	,	<=256
uint16_t debugUsartRxSize;							//usart0 �ƻ����������ܳ���	,	<=256
uint8_t debugUsartTxSize;							//usart0 �ƻ����������ܳ���	,	<=256

uint8_t fpgaUsartRxBuffer[256];					//usart2 ����buff
uint8_t fpgaUsartTxBuffer[256];					//usart2 ����buff
uint8_t fpgaUsartRxCnt;									//usart2 ����buff����	,	<=256
uint8_t fpgaUsartTxCnt;									//usart2 ����buff����	,	<=256
uint8_t fpgaUsartRxSize;								//usart2 �ƻ����������ܳ���	,	<=256
uint8_t fpgaUsartTxSize;								//usart2 �ƻ����������ܳ���	,	<=256

uint8_t dsp1UsartRxBuffer[256];				//usart1 ����buff
uint8_t dsp1UsartTxBuffer[256];				//usart1 ����buff
uint8_t dsp1UsartRxCnt;								//usart1 ����buff����	,	<=256
uint8_t dsp1UsartTxCnt;								//usart1 ����buff����	,	<=256
uint8_t dsp1UsartRxSize;								//usart1 �ƻ����������ܳ���	,	<=256
uint8_t dsp1UsartTxSize;								//usart1 �ƻ����������ܳ���	,	<=256

uint8_t board_slot;
/******************************Param*******************************/

/******************************Func********************************/
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
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while (RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}

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
void dohitech_mcu_rcu_configuration(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_GPIOD);
	rcu_periph_clock_enable(RCU_AF);
}

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
void dohitech_board_oem(void)
{
	printf("\tMCU�ͺţ�%s\n", MCUTYPE);
	printf("\tDSP�ͺţ�%s\n", DSPTYPE);
	printf("\tFPGA�ͺţ�%s\n", FPGATYPE);
	printf("\tMCU ����汾��%s\n", MCUSOFTWAREVERSION);
	printf("\tMCU ��������汾��%s\n", MCUSOFTWAREVERSIONBEFORE);
	printf("\tMCU Ӳ���汾��%s\n", MCUHARDWAREVERSION);
	printf("\t�忨��ţ�%s\n", BOARDNUMBER);
	board_slot = dohitech_mcu_vpx_read_slot();
	printf("\t�忨��λ��: 0x%x\n", board_slot);
}
/******************************Func********************************/
