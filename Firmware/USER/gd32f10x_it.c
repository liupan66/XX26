/*!
    \file  gd32f10x_it.c
    \brief interrupt service routines
    
    \version 2014-12-26, V1.0.0, firmware for GD32F10x
    \version 2017-06-20, V2.0.0, firmware for GD32F10x
    \version 2018-07-31, V2.1.0, firmware for GD32F10x
*/

/*
    Copyright (c) 2018, GigaDevice Semiconductor Inc.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "global.h"
#include "FLASH.h"
#include "systick.h"


uint16_t GlRecvNum = 0;
uint8_t GlUart2RecvFlg = 0;
uint8_t GlTmpFlg = 0;
uint8_t GlTmpFlg_jump = 0;

/*MCU_GPIO6外部中断标志位，0xFF表示检测到上升沿，其它值表示未检测到上升沿*/
uint8_t gMCU_V7_GPIO6_Flg=0;
/*收到FPGA的在线升级命令标志*/
uint8_t gMCU_V7_GPIO6_UpdateFlg=0;

/*MCU_GPIO6高电平持续检测时间*/
uint16_t gMCU_V7_GPIO6_5msCnt=0;

/*MCU_GPIO6高电平检测间隔时间，如果500ms内未检测到连续的5ms高电平，则清空gMCU_V7_GPIO6_5msCnt计数值*/
uint16_t gMCU_V7_GPIO6_50msCnt=0;


uint16_t gMCU_V7_GPIO1_500msCnt=0;
uint16_t gMCU_V7_GPIO1_500msFlg=0;

#define ApplicationATarget 0x08002000
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

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
    delay_decrement();
}

/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART0_IRQHandler(void)
{
    if(
			(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) && 
       (RESET != usart_flag_get(USART0, USART_FLAG_RBNE))){
        /* receive data */
        debugUsartRxBuffer[debugUsartRxCnt++] = (usart_data_receive(USART0) & 0x7F);
//        if(debugUsartRxCnt == debugUsartRxSize){
					
					if(debugUsartRxBuffer[debugUsartRxCnt] == 0x26)
					{
						dohitech_mcu_dsp_reset();
						debugUsartRxCnt = 0;
						debugUsartRxBuffer[debugUsartRxCnt] = 0;
					}
					else if((debugUsartRxBuffer[debugUsartRxCnt] == 0x37) || (debugUsartRxBuffer[debugUsartRxCnt] == 0x73))
					{
						usart_data_transmit(USART1, debugUsartRxBuffer[debugUsartRxCnt]);
						while(RESET == usart_flag_get(USART1, USART_FLAG_TC));
						debugUsartRxCnt = 0;
						debugUsartRxBuffer[debugUsartRxCnt] = 0;
					}
//					else if((debugUsartRxBuffer[0] == 0xae)  && (debugUsartRxBuffer[1] == 0xea))
//					{
//						usart_data_transmit(USART1, 0x79);
//						while(RESET == usart_flag_get(USART1, USART_FLAG_TC));
////						dohitech_mcu_dsp_reset();		//boot 中有dsp复位 此处不需要
//						debugUsartRxBuffer[debugUsartRxCnt] = 0;
//						debugUsartRxCnt = 0;
//						NVIC_SystemReset();
//					}
					else if(debugUsartRxCnt > 2)
					{
						debugUsartRxCnt = 0;
						debugUsartRxBuffer[0] = 0;
						debugUsartRxBuffer[1] = 0;
					}
//        }
    }
    if((RESET != usart_flag_get(USART0, USART_FLAG_TBE)) && 
       (RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_TBE))){
           
        /* transmit data */
        usart_data_transmit(USART0, debugUsartTxBuffer[debugUsartTxCnt++]);
        if(debugUsartTxCnt == debugUsartTxSize){
						memset(debugUsartTxBuffer, 0, 80);
            usart_interrupt_disable(USART0, USART_INT_FLAG_TBE);
						usart_interrupt_enable(USART0, USART_INT_FLAG_RBNE);	
        }
    }
}
/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART1_IRQHandler(void)
{
	//	uint32_t va0;
    if((RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE)) && 
       (RESET != usart_flag_get(USART1, USART_FLAG_RBNE))){
        /* receive data */
        fpgaUsartRxBuffer[fpgaUsartRxCnt] = (usart_data_receive(USART1) & 0xFF);	
    }
    if((RESET != usart_flag_get(USART1, USART_FLAG_TBE)) && 
       (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_TBE))){
           
        /* transmit data */
        usart_data_transmit(USART1, fpgaUsartTxBuffer[fpgaUsartTxCnt++]);
    }
}

/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
#if 0
void USART2_IRQHandler(void)
{
    if((RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE)) && 
       (RESET != usart_flag_get(USART2, USART_FLAG_RBNE))){
        /* receive data */
        dsp1UsartRxBuffer[dsp1UsartRxCnt] = (usart_data_receive(USART2) & 0xFF);				
#if 1
				if(dsp1UsartRxCnt == 1)
				{
					if((dsp1UsartRxBuffer[0] == 0x30) &&(dsp1UsartRxBuffer[1] == 0xa5))
					{
							if(*(__IO uint32_t*)ApplicationATarget == 0)
							{
								usart_data_transmit(USART2, 0x79);	
								while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
							}
							else
							{
								usart_data_transmit(USART2, 0xff);//0x97);	
								while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
							}						
					}
					else if((dsp1UsartRxBuffer[0] == 0x30)&& (dsp1UsartRxBuffer[1] == 0x5a))
					{
						dohitech_mcu_dsp_reset();
					}
					else if((dsp1UsartRxBuffer[0] == 0x31)&& (dsp1UsartRxBuffer[1] == 0xc1))
					{
						usart_data_transmit(USART2, 0x79);	
						while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
					}
					else if((dsp1UsartRxBuffer[0] == 0x3a)&& (dsp1UsartRxBuffer[1] == 0xa5))
					{
						uint32_t va0=1;
						while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
						usart_data_transmit(USART2, 0x79);
						
						erase_page(1, 4);
						if(FLASH_Program(ApplicationATarget, 4, &va0) != PASSED){
							if(FLASH_Program(ApplicationATarget, 4, &va0) != PASSED){
								printf("\t远程更新标志更新失败，无法切换远程更新模式\n");
								return;
							}
						}
						printf("\t准备切换到远程更新模式\n");
						dsp1UsartRxBuffer[0] = 0;
						dsp1UsartRxBuffer[1] = 0;
						dsp1UsartRxCnt = 0;
						usart_disable(USART0);
						usart_disable(USART2);
						i2c_disable(I2C0);
						i2c_disable(I2C1);
						dma_channel_disable(DMA0, DMA_CH0);
						NVIC_SystemReset();
					}
					else if((dsp1UsartRxBuffer[0] == 0x55) &&(dsp1UsartRxBuffer[1] == 0xAA))
					{
					  GlTmpFlg_jump = 1;		
					}
					
					if(GlTmpFlg_jump != 1)
					{
						 dsp1UsartRxCnt = 0;
					   dsp1UsartRxBuffer[0] = 0;
					   dsp1UsartRxBuffer[1] = 0;
					}

				}
				
				if(dsp1UsartRxCnt == 7)
				{
						if((GlTmpFlg_jump == 1) && (dsp1UsartRxBuffer[2] == 0x7E) &&(dsp1UsartRxBuffer[3] == 0x7E))
					  {
								 GlRecvNum = (dsp1UsartRxBuffer[5]<<8)|dsp1UsartRxBuffer[4];
								if(GlRecvNum == (dsp1UsartRxBuffer[7]<<8)|dsp1UsartRxBuffer[6])
								{
									GlTmpFlg = 0xff;
								}
					  }		
				}
				dsp1UsartRxCnt++;
				if((GlTmpFlg == 0xff) && (dsp1UsartRxCnt >= GlRecvNum + 8))
				{
					 GlTmpFlg = 0;
				   GlUart2RecvFlg = 0xff;
					 dsp1UsartRxCnt = 0;
					GlTmpFlg_jump = 0;
				}
				#endif

				if(dsp1UsartRxBuffer[0] != 0x55)
				{
						dsp1UsartRxCnt = 0;
					   dsp1UsartRxBuffer[0] = 0;
				}

    }
    if((RESET != usart_flag_get(USART2, USART_FLAG_TBE)) && 
       (RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_TBE))){
           
        /* transmit data */
        usart_data_transmit(USART2, dsp1UsartTxBuffer[dsp1UsartTxCnt++]);
    }
}
#endif

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
uint8_t erase_to_flag(void)
{
	uint32_t va0=1,res = 0;
	erase_page(1, 4);
	if(FLASH_Program(ApplicationATarget, 4, &va0) != PASSED)
	{
		if(FLASH_Program(ApplicationATarget, 4, &va0) != PASSED)
		{
			res = 1;
		}
	}
	if(FLASH_Program(ApplicationATarget+4, 4, &va0) != PASSED)//APP跳转Boot标记位
	{
		if(FLASH_Program(ApplicationATarget+4, 4, &va0) != PASSED)
		{
			res = 1;
		}
	}
	return res;
}

void change_to_boot(void)
{
	uint8_t res = 0;
	res = erase_to_flag();
	if(!res)
	{
		dsp1UsartRxBuffer[0] = 0;
		dsp1UsartRxBuffer[1] = 0;
		dsp1UsartRxCnt = 0;
		usart_disable(USART0);
		usart_disable(USART2);
		i2c_disable(I2C0);
		i2c_disable(I2C1);
		dma_channel_disable(DMA0, DMA_CH0);
		NVIC_SystemReset();
	}

}

/**
  * @brief  This function handles TIMER2 interrupt request.
  * @param  None
  * @retval None
  */
uint16_t TimerTestCnt;
void TIMER2_IRQHandler(void)
{
	uint32_t va0=1;

	if(timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP) != RESET) 
	{
		/* clear channel 0 interrupt bit */
		timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);

		if(gMCU_V7_GPIO6_Flg==0xFFu)
		{
			if(gpio_input_bit_get(GPIOB, GPIO_PIN_4) == 0) 
			{
				/*检测到低电平*/
				gMCU_V7_GPIO6_Flg = 0;
			}

			/*检测到MCU_GPIO6的上升沿*/
			if(gMCU_V7_GPIO6_5msCnt>=50u)
			{
				/*持续5ms以上检测到高电平，认为是FPGA发送的在线更新命令*/
				gMCU_V7_GPIO6_Flg = 0u;

				#if 1
				erase_page(1, 4);
				if(FLASH_Program(ApplicationATarget, 4, &va0) != PASSED)
				{
					if(FLASH_Program(ApplicationATarget, 4, &va0) != PASSED)
					{
						printf("\t接收到FPGA的在线升级命令，但是写在线升级命令到FLASH失败\n");
						return;
					}
				}

				/*关闭串口功能，系统复位*/
				usart_disable(USART2);

				dma_channel_enable(DMA0, DMA_CH0);			/*使能DMA通道*/

				NVIC_SystemReset();											/*系统复位*/
				#endif
			}

			(gMCU_V7_GPIO6_5msCnt==0xFFFFu)?(gMCU_V7_GPIO6_5msCnt=0u):(gMCU_V7_GPIO6_5msCnt++);
			(gMCU_V7_GPIO6_50msCnt==0xFFFFu)?(gMCU_V7_GPIO6_50msCnt=0u):(gMCU_V7_GPIO6_50msCnt++);
		}
		else
		{
			gMCU_V7_GPIO6_5msCnt = 0u;
			gMCU_V7_GPIO6_50msCnt = 0u;
		}


		#if 1
		/*MCU_V7_GPIO1发送脉冲信号*/
		if(gMCU_V7_GPIO1_500msCnt >= 5000u)
		{
			TimerTestCnt++;
			/*间隔500ms向FPGA发送脉冲信号*/
			gMCU_V7_GPIO1_500msCnt = 0;
			gMCU_V7_GPIO1_500msFlg=~gMCU_V7_GPIO1_500msFlg;
			if(gMCU_V7_GPIO1_500msFlg>0u)
			{
				gpio_bit_set(GPIOB, GPIO_PIN_0);
				gpio_bit_set(GPIOC, GPIO_PIN_4);
				gpio_bit_set(GPIOC, GPIO_PIN_5);
			}
			else
			{
				gpio_bit_reset(GPIOB, GPIO_PIN_0);
				gpio_bit_reset(GPIOC, GPIO_PIN_4);
				gpio_bit_reset(GPIOC, GPIO_PIN_5);
			}
		}

		(gMCU_V7_GPIO1_500msCnt==0xFFFFu)?(gMCU_V7_GPIO1_500msCnt=0u):(gMCU_V7_GPIO1_500msCnt++);
		#endif
	}  
}

uint16_t ExtiTempCnt=0;
void EXTI4_IRQHandler(void)
{
	if(exti_interrupt_flag_get(EXTI_4) != RESET)
	{
		/*检测到MCU_GPIO6的上升沿，开启5ms定时器*/
		gMCU_V7_GPIO6_Flg=0xFFu;

		exti_interrupt_flag_clear(EXTI_4);
	}
}
