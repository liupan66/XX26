/**********************************************
 * FileName:	BSP_USART.C
 *
 * FileBrief: USART transmit and receive interrupt
 *
 * Created on: 
 *
 * Author:   SHANGHT
 *
 * Remark:
 ******************************************/
#include "global.h"

#define ARRAYNUM(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
#define TRANSMIT_SIZE            (ARRAYNUM(txbuffer0) - 1)


uint8_t usartRemoteRecvData[2]={0};			//串口配置远程板卡发送使用发送的数据
uint8_t usart2I2c2SendData=0;						//串口送i2c数据类型
uint8_t usart2TempVoltNum=0;						//串口发送的电压值
uint32_t debugUsartRcvDataLengthLastTime=0;	//slave map中使用，计数当前串口收发
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
ErrStatus dohitech_mcu_debug_usart_init(uint32_t baudrate)
{
		/* USART interrupt configuration */
		nvic_irq_enable(USART0_IRQn, 0, 0);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);
	
    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, baudrate);
    usart_parity_config(USART0, USART_PM_EVEN);
    usart_word_length_set(USART0, USART_WL_9BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
		
	usart_interrupt_enable(USART0, USART_INT_RBNE);
//	usart_interrupt_enable(USART0, USART_INT_FLAG_RBNE);		
	return SUCCESS;
}
#if 0
/***************************************************
* 函数：dohitech_mcu_debug_usart_transfer
* 功能：mcu 调试用串口数据发送函数
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
ErrStatus dohitech_mcu_debug_usart_transfer(uint8_t *txBuff, uint8_t txLen)
{
		uint32_t delayCnt=0;
	
		debugUsartTxCnt = 0;
		debugUsartTxSize = txLen;
		memcpy(debugUsartTxBuffer, txBuff, txLen);
	
		/* enable USART TBE interrupt */  
    usart_interrupt_enable(USART0, USART_INTEN_TBEIE);
	
    /* wait until USART send the transmitter_buffer */
    while(debugUsartTxCnt < debugUsartTxSize)
		{
			if(delayCnt > 100000)
			{
				usart_interrupt_disable(USART0, USART_INTEN_TBEIE);
				memset(debugUsartTxBuffer, 0, txLen);
				return  ERROR;
			} 
			delay_us(100);
			delayCnt++;
		}
		
    delayCnt=0;
    while(RESET == usart_flag_get(USART0, USART_FLAG_TC))
		{
			if(delayCnt > 100000)
			{
				usart_flag_clear(USART0, USART_FLAG_TC);
				memset(debugUsartTxBuffer, 0, txLen);
				return  ERROR;
			} 
			delay_us(100);
			delayCnt++;
		}
		
		memset(debugUsartTxBuffer, 0, txLen);
		debugUsartTxSize = 0;
		debugUsartTxCnt = 0;
		
		return SUCCESS;
}

/***************************************************
* 函数：dohitech_mcu_debug_usart_transfer
* 功能：mcu 调试用串口数据发送函数
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
ErrStatus dohitech_mcu_debug_usart_Receiver(uint8_t *rxBuff, uint8_t txLen)
{
		uint32_t delayCnt=0;
	
		debugUsartRxCnt = 0;
		debugUsartRxSize = txLen;
	
		usart_interrupt_enable(USART0, USART_INTEN_RBNEIE);
    
    /* wait until USART receive the receiver_buffer */
    while(debugUsartRxCnt < debugUsartRxSize)
		{
			if(delayCnt > 100000)
			{
				usart_interrupt_disable(USART0, USART_INTEN_RBNEIE);
				memset(debugUsartRxBuffer, 0, txLen);
				return  ERROR;
			} 
			delay_us(100);
			delayCnt++;
		}
		
		memcpy(rxBuff, debugUsartRxBuffer, txLen);
		memset(debugUsartRxBuffer, 0, txLen);
		debugUsartRxCnt = 0;
		debugUsartRxSize = 0;
		return SUCCESS;
}

/***************************************************
* 函数：dohitech_mcu_debug_usart_transfer
* 功能：mcu 调试用串口数据发送函数
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
ErrStatus dohitech_mcu_debug_usart_selfTest(void)
{
	uint8_t i=0;
	uint8_t data[256]={0};
	
	printf("debug usart test start： \n");
	printf("please input 8 Byte to test in 5 second and waiting...\n");
	delay_us(8000000);
	printf("Please send data...\n");
	while(i<1)
	{
		if( SUCCESS != dohitech_mcu_debug_usart_Receiver(data, 8))
		{
			printf("debug usart receive data time over!\n");
			return ERROR;
		}
		if( SUCCESS !=dohitech_mcu_debug_usart_transfer(data, 8))
		{
			printf("debug usart transfer data time over!\n");
			return ERROR;
		}
		printf("\n");
		i++;
	}
	printf("debug usart test over\n");
	return SUCCESS;
}

#endif

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
ErrStatus dohitech_mcu_fpga_usart_init(uint32_t baudrate)
{
    /* USART interrupt configuration */
	nvic_irq_enable(USART1_IRQn, 0, 3);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);
	
    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);

    /* USART configure */
    usart_deinit(USART1);
    usart_baudrate_set(USART1, baudrate);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_enable(USART1);
    
		
	return SUCCESS;
}

#if 0
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
ErrStatus dohitech_mcu_fpga_usart_transfer(uint8_t *txBuff, uint8_t txLen)
{
		uint32_t delayCnt=0;
	
		fpgaUsartTxCnt = 0;
		fpgaUsartTxSize = txLen;
		memcpy(fpgaUsartTxBuffer, txBuff, txLen);
	
		/* enable USART TBE interrupt */  
    usart_interrupt_enable(USART2, USART_INTEN_TBEIE);
	
    /* wait until USART send the transmitter_buffer */
    while(fpgaUsartTxCnt < fpgaUsartTxSize)
		{
			if(delayCnt > 100000)
			{
				usart_interrupt_disable(USART2, USART_INTEN_TBEIE);
				memset(fpgaUsartTxBuffer, 0, txLen);
				return  ERROR;
			} 
			delay_us(100);
			delayCnt++;
		}
		
    delayCnt=0;
    while(RESET == usart_flag_get(USART2, USART_FLAG_TC))
		{
			if(delayCnt > 100000)
			{
				usart_flag_clear(USART2, USART_FLAG_TC);
				memset(fpgaUsartTxBuffer, 0, txLen);
				return  ERROR;
			} 
			delay_us(100);
			delayCnt++;
		}
		
		memset(fpgaUsartTxBuffer, 0, txLen);
		fpgaUsartTxSize = 0;
		fpgaUsartTxCnt = 0;
		
		return SUCCESS;
}

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
ErrStatus dohitech_mcu_fpga_usart_Receiver(uint8_t *rxBuff, uint8_t txLen)
{
		uint32_t delayCnt=0;
	
		fpgaUsartRxCnt = 0;
		fpgaUsartRxSize = txLen;
	
		usart_interrupt_enable(USART2, USART_INTEN_RBNEIE);
    
    /* wait until USART receive the receiver_buffer */
    while(fpgaUsartRxCnt < fpgaUsartRxSize)
		{
			if(delayCnt > 100000)
			{
				usart_interrupt_disable(USART2, USART_INTEN_RBNEIE);
				memset(fpgaUsartRxBuffer, 0, txLen);
				return  ERROR;
			} 
			delay_us(100);
			delayCnt++;
		}
		
		memcpy(rxBuff, fpgaUsartRxBuffer, txLen);
		memset(fpgaUsartRxBuffer, 0, txLen);
		fpgaUsartRxCnt = 0;
		fpgaUsartRxSize = 0;
		return SUCCESS;
}
#endif
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
ErrStatus dohitech_mcu_fpga_usart_selfTest(void)
{
	uint8_t i=0,j=0;
	uint8_t data[10]={0};
	uint8_t  rxbuf[10] = {0};
	for(i=0; i<10; i++)
		data[i] = i+0xa0;
	i=0;

	printf("fpga usart test start\n");
	while(i<10)
	{
		/* transmit data */
		usart_data_transmit(USART1, data[j++]);
		while(RESET == usart_flag_get(USART1, USART_FLAG_TC));
		
		while(RESET != usart_flag_get(USART1, USART_FLAG_RBNE))
		{
        /* receive data */
        rxbuf[i++] = (usart_data_receive(USART1) & 0x7F);
		}
	}

	printf("fpga usart test over\n");
		/* enable USART TBE interrupt */  
    usart_interrupt_enable(USART1, USART_INT_RBNE);
	return SUCCESS;
}


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
ErrStatus dohitech_mcu_dsp1_usart_init(uint32_t baudrate)
{
		/* USART interrupt configuration */
////		nvic_irq_enable(USART1_IRQn, 0, 2);

//	/* enable USART clock */
//	rcu_periph_clock_enable(RCU_UART3);	

//	/* connect port to USARTx_Tx */
//	gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

//	/* connect port to USARTx_Rx */
//	gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

//	/* USART configure */
//	usart_deinit(UART3);
//	usart_baudrate_set(UART3, baudrate);
//	usart_stop_bit_set(UART3, USART_STB_1BIT);
//	usart_parity_config(UART3, USART_PM_NONE);
//	usart_word_length_set(UART3, USART_WL_8BIT);
//	usart_receive_config(UART3, USART_RECEIVE_ENABLE);
//	usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
//	usart_enable(UART3);

		/* USART interrupt configuration */
//		nvic_irq_enable(USART2_IRQn, 0, 1);
//		 rcu_periph_clock_enable(RCU_USART2);	
//		 rcu_periph_clock_enable(RCU_AF);
//		gpio_pin_remap_config(GPIO_USART2_PARTIAL_REMAP, ENABLE);
//	
//    /* connect port to USARTx_Tx */
//    gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

//    /* connect port to USARTx_Rx */
//    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

//    /* USART configure */
//    usart_deinit(USART2);
//    usart_baudrate_set(USART2, baudrate);
//		usart_stop_bit_set(USART2, USART_STB_1BIT);
//		usart_parity_config(USART2, USART_PM_NONE);
//		usart_word_length_set(USART2, USART_WL_8BIT);
//    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
//    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
//    usart_enable(USART2);
//		
//		/* enable USART TBE interrupt */  
//    usart_interrupt_enable(USART2, USART_INT_RBNE);
//		/* enable USART TBE interrupt */  
////    usart_interrupt_enable(USART0, USART_INTEN_TBEIE);
//		return SUCCESS;
	nvic_irq_enable(USART2_IRQn, 0, 1);
	rcu_periph_clock_enable(RCU_USART2);	
	rcu_periph_clock_enable(RCU_AF);
	gpio_pin_remap_config(GPIO_USART2_PARTIAL_REMAP, ENABLE);
	
    /* connect port to USARTx_Tx */
    gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    /* USART configure */
    usart_deinit(USART2);
    usart_baudrate_set(USART2, baudrate);//
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
	return SUCCESS;
}
#if 0
/***************************************************
* 函数：dohitech_mcu_dsp1_usart_transfer
* 功能：mcu - dsp1串口数据发送函数
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
ErrStatus dohitech_mcu_dsp1_usart_transfer(uint8_t *txBuff, uint8_t txLen)
{
		uint32_t delayCnt=0;
	
		dsp1UsartTxCnt = 0;
		dsp1UsartTxSize = txLen;
		memcpy(dsp1UsartTxBuffer, txBuff, txLen);
	
		/* enable USART TBE interrupt */  
    usart_interrupt_enable(USART1, USART_INTEN_TBEIE);
	
    /* wait until USART send the transmitter_buffer */
    while(dsp1UsartTxCnt < dsp1UsartTxSize)
		{
			if(delayCnt > 100000)
			{
				usart_interrupt_disable(USART1, USART_INTEN_TBEIE);
				memset(dsp1UsartTxBuffer, 0, txLen);
				return  ERROR;
			} 
			delay_us(100);
			delayCnt++;
		}
		
    delayCnt=0;
    while(RESET == usart_flag_get(USART1, USART_FLAG_TC))
		{
			if(delayCnt > 100000)
			{
				usart_flag_clear(USART1, USART_FLAG_TC);
				memset(dsp1UsartTxBuffer, 0, txLen);
				return  ERROR;
			} 
			delay_us(100);
			delayCnt++;
		}
		
		memset(dsp1UsartTxBuffer, 0, txLen);
		dsp1UsartTxSize = 80;
		dsp1UsartTxCnt = 0;
		
		return SUCCESS;
}

/***************************************************
* 函数：dohitech_mcu_dsp1_usart_transfer
* 功能：mcu - dsp1串口数据发送函数
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
ErrStatus dohitech_mcu_dsp1_usart_Receiver(uint8_t *rxBuff, uint8_t txLen)
{
		uint32_t delayCnt=0;
	
		dsp1UsartRxCnt = 0;
		dsp1UsartRxSize = txLen;
	
		usart_interrupt_enable(USART1, USART_INTEN_RBNEIE);
    
    /* wait until USART receive the receiver_buffer */
    while(dsp1UsartRxCnt < dsp1UsartRxSize)
		{
			if(delayCnt > 100000)
			{
				usart_interrupt_disable(USART1, USART_INTEN_RBNEIE);
				memset(dsp1UsartRxBuffer, 0, txLen);
				return  ERROR;
			} 
			delay_us(100);
			delayCnt++;
		}
		
		memcpy(rxBuff, dsp1UsartRxBuffer, txLen);
		memset(dsp1UsartRxBuffer, 0, txLen);
		dsp1UsartRxCnt = 0;
		dsp1UsartRxSize = 0;
		return SUCCESS;
}
#endif

/***************************************************
* 函数：dohitech_mcu_dsp1_usart_transfer
* 功能：mcu - dsp1串口数据发送函数
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

ErrStatus dohitech_mcu_dsp1_usart_selfTest(void)
{
	uint8_t rxxxxbuf[256] = {0};
	uint8_t iiiiiii=0;
	printf("dsp1 usart test start\n");
//	usart_interrupt_enable(USART1, USART_INTEN_RBNEIE);
	

//		usart_data_transmit(UART3, 0xa5);
//		while(RESET != usart_flag_get(UART3, USART_FLAG_TC));
//	while(1)
//	{
//	usart_data_transmit(USART2, 0xa5);
//		while(RESET != usart_flag_get(USART2, USART_FLAG_TC));
//	}
	while(iiiiiii<10)
	{	
		if(RESET != usart_flag_get(USART2, USART_FLAG_RBNE)){
        /* receive data */
        rxxxxbuf[iiiiiii] = (usart_data_receive(USART2) & 0xFF);
			
			if(RESET != usart_flag_get(USART2, USART_FLAG_TBE))
				usart_data_transmit(USART2, rxxxxbuf[iiiiiii++]);
    } 	
	}
	printf("dsp1 usart test over\n");
	return SUCCESS;
}


#if 0
/***************************************************
 * 函数：				dohitech_mcu_dsp1_usart_Temp_config
 * 功能：				温度阈值配置函数
 * 参数：                           in/out          说明
 *返回值              说明
 *
 * 说明：
 * 时间         版本          作者            说明
 * -------------------------------------------------
 * 2020/01/13   v1.0    shanght     创建
 **************************************************/
void dohitech_mcu_dsp1_usart_Temp_config(void)
{
	uint8_t i;
	int ascii2int[2]={0};
	usart2TempVoltNum = dsp1UsartRxBuffer[3];
	for(i=0; i<4; i++)
	{
		dsp1UsartTxBuffer[i] = dsp1UsartRxBuffer[i+4];
	}
	
	ascii2Int(dsp1UsartTxBuffer, ascii2int);
	int2Hex(usartRemoteRecvData, ascii2int);
	usart2I2c2SendData=1;
	return;
}

/***************************************************
 * 函数：				dohitech_mcu_dsp1_usart_volt_config
 * 功能：				电压阈值配置函数
 * 参数：                           in/out          说明
 *返回值              说明
 *
 * 说明：
 * 时间         版本          作者            说明
 * -------------------------------------------------
 * 2020/01/13   v1.0    shanght     创建
 **************************************************/
void dohitech_mcu_dsp1_usart_volt_config(void)
{
	uint8_t i;
	int ascii2int[2]={0};
	usart2TempVoltNum = dsp1UsartRxBuffer[3];
	for(i=0; i<4; i++)
	{
		dsp1UsartTxBuffer[i] = dsp1UsartRxBuffer[i+3];
	}
	
	ascii2Int(dsp1UsartTxBuffer, ascii2int);
	int2Hex(usartRemoteRecvData, ascii2int);
	usart2I2c2SendData=2;
	return;
}

/***************************************************
 * 函数：				dohitech_mcu_dsp1_usart_check_voltTemp_data
 * 功能：				查询温度电压数据
 * 参数：                           in/out          说明
 *返回值              说明
 *
 * 说明：
 * 时间         版本          作者            说明
 * -------------------------------------------------
 * 2020/01/13   v1.0    shanght     创建
 **************************************************/
void dohitech_mcu_dsp1_usart_check_voltTemp_data(void)
{
	usart2I2c2SendData=3;
}

/***************************************************
 * 函数：				dohitech_mcu_dsp1_usart_powerDown
 * 功能：				软件关机
 * 参数：                           in/out          说明
 *返回值              说明
 *
 * 说明：
 * 时间         版本          作者            说明
 * -------------------------------------------------
 * 2020/01/13   v1.0    shanght     创建
 **************************************************/
void dohitech_mcu_dsp1_usart_powerDown(void)
{
	usart2I2c2SendData=4;
}

/***************************************************
 * 函数：				void dohitech_mcu_dsp1_usart_powerOn()
 * 功能：				软件开机
 * 参数：                           in/out          说明
 *返回值              说明
 *
 * 说明：
 * 时间         版本          作者            说明
 * -------------------------------------------------
 * 2020/01/13   v1.0    shanght     创建
 **************************************************/
void dohitech_mcu_dsp1_usart_powerOn(void)
{
	usart2I2c2SendData=5;
}

/***************************************************
 * 函数：				 dohitech_mcu_dsp1_usart_cmd_analysis
* 功能：				对dsp通过usart发送的命令进行解析
 * 参数：                           in/out          说明
 * 						
 *返回值              说明
 *
 * 说明：
 * 时间         版本          作者            说明
 * -------------------------------------------------
 * 2018/09/26   v1.0    shanght     创建
 **************************************************/
void dohitech_mcu_dsp1_usart_cmd_analysis(void)
{
	if(dsp1UsartRxBuffer[0] == '@')
	{
		switch(dsp1UsartRxBuffer[1])
		{
			case 'S':
				if(dsp1UsartRxBuffer[2] == 'T')
					dohitech_mcu_dsp1_usart_Temp_config();
				else if(dsp1UsartRxBuffer[2] == 'V')
					dohitech_mcu_dsp1_usart_volt_config();
				else
					printf(" Cmd invalid : Set Volt/Temp\n");
				break;
			case 'C':
				if(dsp1UsartRxBuffer[2] == 'A')
					dohitech_mcu_dsp1_usart_check_voltTemp_data();
				else
					printf(" Cmd invalid : Check All pera\n");
				break;
			case 'P':
				if(dsp1UsartRxBuffer[2] == 'D')
					dohitech_mcu_dsp1_usart_powerDown();
				//poweronon
				else if(dsp1UsartRxBuffer[2] == 'O')
					dohitech_mcu_dsp1_usart_powerOn();
				else
					printf(" Cmd invalid :Power On/Off \n");
				break;
			default:
				printf("cmd platform fault\n");
				break;
		}
	}
}
#endif
