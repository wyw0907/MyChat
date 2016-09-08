#ifndef __SYSTICK_H
#define __SYSTICK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#define TICK_FREQ (1000u)

extern unsigned long SEC;

void SysTick_init(void);
unsigned long get_tick_count(unsigned long *count);
unsigned long get_timer(void);
void mdelay(unsigned long nTime);
void udelay(unsigned long nTime);
void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif
	 
#endif	/* __SYSTICK_H */

