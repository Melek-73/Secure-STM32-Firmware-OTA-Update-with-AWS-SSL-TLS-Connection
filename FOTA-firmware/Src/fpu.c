#include "stm32f4xx.h"

void fpu_enable(void){

	/*Enable FPU unit : enable CP10 and CP11 full access*/
	SCB->CPACR |= (1<<20);
	SCB->CPACR |= (1<<21);
	SCB->CPACR |= (1<<22);
	SCB->CPACR |= (1<<23);
}

