#include "stm32f4xx.h"
#include "timebase.h"

#define CTRL_ENABLE			(1U<<0)
#define	CTRL_TICKINT		(1U<<1)
#define CTRL_CLCKSRC		(1U<<2)
#define	CTRL_COUNTFLAG		(1U<<16)

#define ONE_SEC_LOAD		16000000
#define TICK_FREQ			1
#define MAX_DELAY			0xFFFFFFFF

volatile uint32_t g_curr_tick;
volatile uint32_t g_curr_tick_p;

uint32_t get_tick(void);

void delay(uint32_t delay){
	uint32_t tickstart = get_tick();
	 while ((uint32_t)(get_tick() - tickstart) < delay){}
}

uint32_t get_tick(void){
	__disable_irq();
	g_curr_tick_p = g_curr_tick;
	__enable_irq();

	return g_curr_tick_p;
}

void tick_increment(void){
	g_curr_tick += TICK_FREQ;
}

void timebase_init(void){
	/*Disable global interrupts*/
	__disable_irq();
	/*Load the timer with number of clock cycles per second*/
	SysTick->LOAD = ONE_SEC_LOAD -1;
	/*Clear SysTick current value register*/
	SysTick->VAL = 0;
	/*Select internal clock source*/
	SysTick->CTRL = CTRL_CLCKSRC;
	/*Enable interrupt*/
	SysTick->CTRL |= CTRL_TICKINT;
	/*Enable SysTick*/
	SysTick->CTRL |= CTRL_ENABLE;
	/*Enable global interrupts*/
	__enable_irq();
}

void SysTick_Handler(void){
	tick_increment();
}
