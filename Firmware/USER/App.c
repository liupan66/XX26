/**********************************************
 * FileName:	App.c
 *
 * FileBrief: 	主程序入口
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
extern uint8_t GlUart2RecvFlg;
extern uint8_t gMCU_V7_GPIO6_Flg;
/******************************Param*******************************/
/******************************Param*******************************/
unsigned short zhenID = 0;
/******************************Func********************************/
/***************************************************
* 函数：	dohitech_mcu_hardware_init
* 功能：	mcu hardware  init
* 参数：                           in/out          说明
* 			
*返回值              说明
*					
* 说明：		
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_hardware_init(void)
{
	ErrStatus ret;
	systick_config();
	dohitech_mcu_rcu_configuration();
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	debugUsartRxCnt =0;
	debugUsartRxSize = 1;
	ret = dohitech_mcu_debug_usart_init(115200U);
	if(ret == ERROR)
		printf("\tMCU 串口初始化失败！\n");
	printf("\tMCU 串口初始化成功！\n");

	dohitech_mcu_dsp_reset();		
	printf("\tDSP 复位完成\n");

	ret = dohitech_mcu_dsp1_usart_init(115200U);
	if(ret == ERROR)
		printf("\tDSP 串口初始化失败！\n");
	printf("\tDSP 串口初始化成功！\n");

	ret = dohitech_mcu_fpga_usart_init(9600U);
	if(ret == ERROR)
		printf("\tFPGA 串口初始化失败！\n");
	printf("\tFPGA 串口初始化成功！\n");

	dohitech_mcu_adc_init();

	ret=dohitech_mcu_v7_mode_set(0x06);
	if(ret == SUCCESS)
		printf("\tV7模式设置完毕！\n");
	DohitechMcuResetV7();
	//	printf("\tFPGA 复位完成\n");
		dohitech_mcu_v7_gpio();
	ret = dohitech_mcu_bsp_timer_init();
	if(ret == ERROR)
		printf("\tMCU 定时器初始化失败！\n");
	printf("\tMCU 定时器初始化成功！\n");
	/*板卡oem信息*/
	dohitech_board_oem();
	GPIO_BC(GPIOB) = GPIO_PIN_3;
}

/***************************************************
* 函数：	dohitech_mcu_task_1s
* 功能：	mcu first task
* 参数：                           in/out          说明
* 			
*返回值              说明
*					
* 说明：		
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_task_1s(void)
{
	dohitech_mcu_i2c_test();
}

/***************************************************
* 函数：	dohitech_mcu_task_2s
* 功能：	mcu second task
* 参数：                           in/out          说明
* 			
*返回值              说明
*					
* 说明：		
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_task_2s(void)
{
	ds18b20_test();
	ds18b20_2_test();
	ds18b20_3_test();
	dohitech_mcu_adc_value_get();
}

uint8_t RecvDspSolve(uint8_t *command)
{
	uint8_t  checksum = 0,i = 0,checksumnew = 0,res = 0;
	uint16_t cmd = 0;
	for(i = 4;i < 20;i++)
	{
	  checksum = command[i];
	}
	checksumnew = (command[18] << 8) | command[19];
//	if(checksum == checksumnew)
	{
		 zhenID = (command[10] << 8) |command[11];
	   cmd = (command[12] << 8) |command[13];
	   if(cmd == 0xaa)
		 {
		   res = 0x55;
		 }
		 else if(cmd == 0xaaaa)
		 {
		   res = 0xaa;
		 }
		 else
		 {
		   
		 }
	}
 return res;
}


void SelfonlineCheck(void)
{
	char temp = 0,val = 0;
	uint16_t VERSION = 0;
	VERSION = (VERSION_H << 12) | (VERSION_M << 4) | (VERSION_L&0x0f);
	temp = (char)ds18b20_test();
	val = DohitechMcuAdcState();
	Self_check_status=(temp<<8)|(val<<3);
	Version_Information[5]=(uint8_t)(VERSION & 0xff);
	Version_Information[4]=(uint8_t)(VERSION >> 8);
	Version_Information[3]=(uint8_t)(YEAR & 0xff);
	Version_Information[2]=(uint8_t)(YEAR >> 8);
	Version_Information[1]=(uint8_t)MONTH;
	Version_Information[0]=(uint8_t)DATE;
}

/***************************************************
* 函数：	main
* 功能：	main function
* 参数：                           in/out          说明
* 			
*返回值              说明
*					
* 说明：		
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
int main(void)
{
	#if BOOTLOADER
	__disable_irq();
	__disable_fiq();
	rcu_deinit();
	nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x2800);
	__enable_irq();
	__enable_fiq();
	SystemInit();
	#endif
	#if I2CTESTEN
	//bool i2ctar = FALSE;
	#endif
	uint8_t helthy_sample_cnt=0, task_1s_cnt=0,cmd = 0;
	dohitech_mcu_hardware_init();
	DMA_config();
	//dohitech_mcu_fpga_usart_selfTest();
	
	delay_1ms(500);
	usart_data_transmit(USART2, 0x79);
	
	while(1)
	{	
		#if 1
		if(USART_RX_NUM > 0)
		{
			cmd = RecvDspSolve(rx0_date_buf);
			if(cmd == 0x55)
			{
				gMCU_V7_GPIO6_Flg = 0;
				gpio_bit_set(GPIOB, GPIO_PIN_4);
				delay_1us(10000);
				gpio_bit_reset(GPIOB, GPIO_PIN_4);
				dohitech_mcu_dsp_reset();
				delay_1ms(1500);
				usart_data_transmit(USART2, 0xFF);
				delay_1ms(5000);
			}
			process_command(rx0_date_buf);
			USART_RX_NUM = 0;
		}
		//		if(helthy_sample_cnt > 2)
		//		{
		//			helthy_sample_cnt = 0;
		//			dohitech_mcu_task_2s();
		//		}
		#endif
		SelfonlineCheck();
		dohitech_mcu_led_toggle();
		helthy_sample_cnt++;		
		task_1s_cnt++;
	}
}
/******************************Func********************************/
