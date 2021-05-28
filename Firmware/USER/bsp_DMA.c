#include "global.h"
/* DMA���ջ����� */
uint8_t dma_buffer[600];
/* �����������ݻ����� */
uint8_t rx0_date_buf[600];
/* ���������ݸ���������1Ϊ�����ݴ�����0Ϊû�����ݴ�����*/
uint32_t USART_RX_NUM = 0;
void DMA_config(void)
{
    dma_parameter_struct dma_init_struct;
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA0);
    
    /* deinitialize DMA channel2 */
    dma_deinit(DMA0, DMA_CH2);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)dma_buffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = sizeof(dma_buffer);//(uint32_t)num_of_data_write;
    
    dma_init_struct.periph_addr = 0x40004804;//0x40013804;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH2, dma_init_struct);
    dma_circulation_disable(DMA0,DMA_CH2);
	dma_memory_to_memory_disable(DMA0,DMA_CH2);
	dma_channel_enable(DMA0, DMA_CH2);
   // dma_flag_clear(DMA0, DMA_CH2, DMA_FLAG_FTF);
   // dma_interrupt_enable(DMA0, DMA_CH2, DMA_INT_FTF);
}
