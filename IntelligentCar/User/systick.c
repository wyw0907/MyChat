/**
  ******************************************************************************
  * @file    Project/User/systick.c 
  * @author  yeyinglong
  * @version V1.3.0
  * @date    11-4-2015
  * @brief   something about time
  ******************************************************************************/
	
	
/* Includes ------------------------------------------------------------------*/
#include "systick.h"
#include "stm32f10x_rcc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint32_t SystemCoreClock; 
static volatile uint32_t g_ul_ms_ticks=0;
static volatile uint32_t TimingDelay=0;
unsigned long SEC=0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void SysTick_init(void)
{
	if (SysTick_Config (SystemCoreClock / TICK_FREQ)) {     // Setup SysTick Timer for 1 msec interrupts
		while (1);                                          // Handle Error
	}
}
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void mdelay(unsigned long nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in us.
  * @retval None
  */
void udelay(unsigned long nTime)
{
//	uint32_t us = SystemCoreClock/1000000;
//	uint32_t endtm = nTime*us + SysTick->VAL;
//	uint32_t ms_tick = g_ul_ms_ticks;
//	while(endtm < (g_ul_ms_ticks-ms_tick)*1000*us + SysTick->VAL);
	u8 j;
	while(nTime--)
	for(j=0;j<12;j++);
}

unsigned long get_tick_count(unsigned long *count)
{
  count[0] = g_ul_ms_ticks;
	return 0;
}

unsigned long get_timer(void)
{
	return(g_ul_ms_ticks);
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  if (TimingDelay != 0x00)
		TimingDelay--;
  g_ul_ms_ticks++;
	if(g_ul_ms_ticks%1000 == 0)
		SEC++;
}
