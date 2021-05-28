/**********************************************
 * FileName:		
 *
 * FileBrief: 		voltage 采样
 *
 * Created on: 
 *
 * Author:   
 *
 * Remark:
 ******************************************/

#include "global.h"

extern int fputc(int ch, FILE *f);

uint16_t adc_value[18]={0};
uint16_t current_voltage[8] = {0};

/***************************************************
* 函数：	dohitech_mcu_adc_rcu_config
* 功能：	configure the different system clocks
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_adc_rcu_config(void)
{
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* enable DMA0 clock */
    rcu_periph_clock_enable(RCU_DMA0);
    /* config ADC clock */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8);
}

/***************************************************
* 函数：	dohitech_mcu_adc_gpio_config
* 功能：	configure the GPIO peripheral
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_adc_gpio_config(void)
{
	/* config the GPIO as analog mode */
  gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
	gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

/***************************************************
* 函数：	dohitech_mcu_dma_config
* 功能：	configure the DMA peripheral
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_dma_config(void)
{	
	    /* ADC_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;
    
    /* ADC DMA_channel configuration */
    dma_deinit(DMA0, DMA_CH0);
    
    /* initialize DMA single data mode */
    dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0));
    dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr  = (uint32_t)(&adc_value);
    dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;  
    dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number       = 8;
    dma_data_parameter.priority     = DMA_PRIORITY_HIGH;
    dma_init(DMA0, DMA_CH0, dma_data_parameter);
    dma_circulation_enable(DMA0, DMA_CH0);
  
    /* enable DMA channel */
    dma_channel_enable(DMA0, DMA_CH0);
		
}

/***************************************************
* 函数：	dohitech_mcu_adc_config
* 功能：	configure the ADC peripheral
* 参数：                           in/out          说明
* 
*返回值              说明
*
* 说明：
* 时间         版本          作者            说明
* -------------------------------------------------
* 2020/09/26   v1.0    shanght     创建
**************************************************/
void dohitech_mcu_adc_config(void)
{
	    /* reset ADC */
    adc_deinit(ADC0);
    /* ADC mode config */
    adc_mode_config(ADC_MODE_FREE);
    /* ADC contineous function enable */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
    /* ADC scan mode disable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 8);
 
    /* ADC regular channel config */
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_4, ADC_SAMPLETIME_55POINT5);
		adc_regular_channel_config(ADC0 ,1,ADC_CHANNEL_5,ADC_SAMPLETIME_55POINT5);
		adc_regular_channel_config(ADC0 ,2,ADC_CHANNEL_6,ADC_SAMPLETIME_55POINT5);
		adc_regular_channel_config(ADC0 ,3,ADC_CHANNEL_10,ADC_SAMPLETIME_55POINT5);
		adc_regular_channel_config(ADC0 ,4,ADC_CHANNEL_11,ADC_SAMPLETIME_55POINT5);
		adc_regular_channel_config(ADC0 ,5,ADC_CHANNEL_12,ADC_SAMPLETIME_55POINT5);
		adc_regular_channel_config(ADC0 ,6,ADC_CHANNEL_13,ADC_SAMPLETIME_55POINT5);
		adc_regular_channel_config(ADC0 ,6,ADC_CHANNEL_1,ADC_SAMPLETIME_55POINT5);
    /* ADC trigger config */
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
    
    /* enable ADC interface */
    adc_enable(ADC0);
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);

    /* ADC DMA function enable */
    adc_dma_mode_enable(ADC0);
		adc_special_function_config(ADC0 ,ADC_SCAN_MODE,ENABLE);
    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}
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
void dohitech_mcu_adc_init(void)
{
	/* system clocks configuration */
    dohitech_mcu_adc_rcu_config();
    /* GPIO configuration */
    dohitech_mcu_adc_gpio_config();
    /* DMA configuration */
    dohitech_mcu_dma_config();
    /* ADC configuration */
    dohitech_mcu_adc_config();
}

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
void dohitech_mcu_adc_value_get(void)
{
	int i=0;
	
	for(i=0; i<8; i++)
	{
		current_voltage[i] = adc_value[i];
		//sensor_data_ipmb_read[0~15]
		sensor_data_ipmb_read[i*2] = adc_value[i] >> 8;
		sensor_data_ipmb_read[i*2+1] = adc_value[i] & 0xff;
		switch(i)
		{
			case 0:
				printf("VCC 3.3/2V : %2f V\n",(float)adc_value[i]/4096*3.3);
				break;
			case 1:
				printf("VCC 5/2V : %2f V\n",(float)adc_value[i]/4096*3.3);
				break;
			case 2:
				printf("VCC 1V2 : %2f V\n",(float)adc_value[i]/4096*3.3);
				break;
			case 3:
				printf("VCC  1V: %2f V\n",(float)adc_value[i]/4096*3.3);
				break;
			case 4:
				printf("VCC DSPCVDD : %2f V\n",(float)adc_value[i]/4096*3.3);
				break;
			case 5:
				printf("VCC 1V8 : %2f V\n",(float)adc_value[i]/4096*3.3);
				break;
			case 6:
				printf("VCC 1V5 : %2f V\n",(float)adc_value[i]/4096*3.3);
				break;
			case 7:
				printf("VCC 1V0 : %2f V\n",(float)adc_value[i]/4096*3.3);
				break;
			default:
				break;
		}	
	}
}

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
float value;
uint8_t DohitechMcuAdcState(void)
{
	uint8_t res = 0;
	float valuel = 0,valueh = 0;
	 value = adc_value[1]/4096.0*3.3;
	 valuel = 5/2 - 5/2*0.3;
   valueh = 5/2 + 5/2*0.3;
	if(value >= valuel && value <= valueh)
	{
    	res = 1;
	}
  return res;
}

