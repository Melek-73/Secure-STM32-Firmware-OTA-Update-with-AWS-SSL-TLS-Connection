#include "uart.h"
#include <stdint.h>

#define GPIOAEN			(1U<<0)
#define GPIOCEN			(1U<<2)
#define UART1EN			(1U<<4)
#define UART6EN			(1U<<5)

#define DBG_UART_BAUDRATE 115200
#define SYS_FREQ		  16000000U
#define APB2_CLK		  SYS_FREQ
#define CR1_TE			  (1U<<3)
#define CR1_RE			  (1U<<2)
#define CR1_UE			  (1U<<13)
#define SR_TXE			  (1U<<7)

static  uint16_t compute_uart_bd(uint32_t periph_clk,uint32_t baudrate);
static  uint16_t compute_uart_bd(uint32_t periph_clk,uint32_t baudrate);
static void uart_set_baudrate(uint32_t periph_clk,uint32_t baudrate);
static void uart_write(int ch);

int __io_putchar(int ch){
	uart_write(ch);
	return ch;
}

void debug_uart_init(){
	/*Enable Clock Access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN ;
	/*Set the mode of PA9 to AF (USART1_Tx)*/
	GPIOA->MODER &=~(1U<<18);
	GPIOA->MODER |= (1U<<19);
	/*Set the mode of PA10 to AF (USART1_Rx)*/
	GPIOA->MODER &=~(1U<<20);
	GPIOA->MODER |= (1U<<21);
	/*Set AF type of PA9 to AF*/
	GPIOA->AFR[1] &= ~(0xFU << 4); // Clear bits 7..4 (Pin 9)
	GPIOA->AFR[1] |=  (7U << 4);   // Set AF7 (USART1)
	/*Set AF type of PA10 to AF*/
	GPIOA->AFR[1] &= ~(0xFU << 8); // Clear bits 7..4 (Pin 10)
	GPIOA->AFR[1] |=  (7U << 8);   // Set AF7 (USART1)
	/*Enable Clock Access to UART1*/
	RCC->APB2ENR |= UART1EN;
	/*Disbale UART module*/
	USART1->CR1 &=~CR1_UE;
	/*Configure uart baudrate*/
	uart_set_baudrate(APB2_CLK,DBG_UART_BAUDRATE);
	/*Configure transfer direction*/
	USART1->CR1 = CR1_TE | CR1_RE;
	/*Enable interrupt in NVIC*/
	NVIC_EnableIRQ(USART1_IRQn);
	/*Enable UART module*/
	USART1->CR1 |= CR1_UE;
}

/*	STM32 ----------->slave device
 *	3.3V  ----------->VCC
 *  GND   ----------->GND
 *  PC6   ----------->RX
 * 	PC7   ----------->TX
 * */


// very important : green wire must put in PC7 and white wire must put into PC6 , black wire into GND
void slave_dev_uart_init(){
	/*Enable Clock Access to GPIOA*/
	RCC->AHB1ENR |= GPIOCEN ;
	/*Set the mode of PC6 to AF (USART6_Tx)*/
	GPIOC->MODER &=~(3U<<12);
	GPIOC->MODER |= (2U<<12);
	/*Set the mode of PC7 to AF (USART6_Rx)*/
	GPIOC->MODER &=~(3U<<14);
	GPIOC->MODER |= (2U<<14);
	/*Set AF type of PC6 to AF*/
	GPIOC->AFR[0] &= ~(0xFU << 24); // Clear bits 7..4 (Pin 6)
	GPIOC->AFR[0] |=  (8U << 24);   // Set AF7 (USART6)
	/*Set AF type of PC7 to AF*/
	GPIOC->AFR[0] &= ~(0xFU << 28); // Clear bits 7..4 (Pin 7)
	GPIOC->AFR[0] |=  (8U << 28);   // Set AF7 (USART6)
	/*Enable Clock Access to UART6*/
	RCC->APB2ENR |= UART6EN;
	/*Disable UART module*/
	USART6->CR1 &=~CR1_UE;
	/*Configure uart baudrate*/
	USART6->BRR = compute_uart_bd(APB2_CLK,DBG_UART_BAUDRATE);
	/*Configure transfer direction*/
	USART6->CR1 = CR1_TE | CR1_RE;
	/*Enable interrupt in NVIC*/
	NVIC_EnableIRQ(USART6_IRQn);
	/*Enable UART module*/
	USART6->CR1 |= CR1_UE;
}

static void uart_write(int ch){
	/*Make sure transmit data register is empty*/
	while(!(USART6->SR & SR_TXE)){} ;
	/*Write to transmit data register*/
	USART6->DR =(ch & 0xFF);
}
static  uint16_t compute_uart_bd(uint32_t periph_clk,uint32_t baudrate){
	return ((periph_clk + (baudrate/2U))/baudrate);
}
static void uart_set_baudrate(uint32_t periph_clk,uint32_t baudrate){
	USART1->BRR = compute_uart_bd(periph_clk,baudrate);
}

