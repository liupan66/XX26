/*!
    \file  main.c
    \brief TIMER1 oc inactive demo
    
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

/* configure the TIMER1 interrupt */
static void nvic_config(void);
/* configure the TIMER peripheral */
static void timer_config(void);


/**
    \brief      configure the TIMER1 interrupt
    \param[in]  none
    \param[out] none
    \retval     none
  */
static void nvic_config(void)
{
    nvic_irq_enable(TIMER2_IRQn, 0, 1);
}

/**
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
  */
static void timer_config(void)
{
	/* ---------------------------------------------------------------
    TIMER0 Configuration: 
    TIMER0CLK = SystemCoreClock / 54000 = 2KHz,
    And generate 3 signals with 3 different delays:
    TIMER0_CH0 delay = 2000/2000 = 1s
    TIMER0_CH1 delay = 4000/2000 = 2s
    TIMER0_CH2 delay = 6000/2000 = 3s
    --------------------------------------------------------------- */
   // timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER2);

    timer_deinit(TIMER2);
    /* initialize TIMER init parameter struct */
//    timer_struct_para_init(&timer_initpara);
    /* TIMER1 configuration */
	
	//delay=10us
    timer_initpara.prescaler         = 53;		//timer=100us
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 200;		
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2, &timer_initpara);

    timer_interrupt_enable(TIMER2, TIMER_INT_UP);

    timer_enable(TIMER2);
}

/*!
    \brief      dohitech_mcu_bsp_timer_init
    \param[in]  none
    \param[out] none
    \retval     none
*/
ErrStatus dohitech_mcu_bsp_timer_init(void)
{    
	nvic_config();
	timer_config();
	return SUCCESS;
}
