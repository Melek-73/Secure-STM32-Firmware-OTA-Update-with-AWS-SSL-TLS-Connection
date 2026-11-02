#include "bsp.h"
#include "stm32f4xx.h"

#define GPIOGEN 		(1U<<6)   //write the AHB1 enable register to enable clock access
#define GPIOAEN 		(1U<<0)
#define PIN13			(1U<<13)
#define LED_PIN 		PIN13
#define PIN0			(1U<<0)
#define BTN_PIN			PIN0

void led_init(void){
	//enable clock access
	RCC->AHB1ENR |= GPIOGEN;
	//set PIN 13 to output mode
	GPIOG->MODER &= ~(3U << 26); // Clear bits 26 and 27
	GPIOG->MODER |=  (1U << 26); // Set bit 26
}

void led_on(void){
	/*Set PG13 high*/
	GPIOG->ODR |= LED_PIN;

}
void led_off(void){
	/*Set PG13 low*/
	GPIOG->ODR &=~LED_PIN;
}

void button_init(void){
	/*Enable Clock Access to PORTA*/
	RCC->AHB1ENR |= GPIOAEN;
	/*Set PA0 as input pin*/
	/* 2. Set PA0 as Input pin (00 in MODER bits 0-1) */
	GPIOA->MODER &= ~((1U << 0) | (1U << 1));
}

bool get_bin_state(void){
	/*Note BTN is active Low*/
	/*Check if button is pressed*/
	if (GPIOA->IDR & BTN_PIN) {return false;} else {return true;} ;
}
