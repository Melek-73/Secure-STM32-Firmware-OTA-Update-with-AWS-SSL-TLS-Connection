#include "stm32f4xx.h"
#include "fpu.h"
#include "uart.h"
#include "timebase.h"
#include "bsp.h"
#include "adc.h"
#include <stdio.h>
#include "circular_buffer.h"

bool btn_state;
uint32_t sensor_value;

/*Module
 * FPU
 * UART
 * TIMEBASE
 * GPIO (BSP)
 * ADC
 * */


int main(void)
{
	//Enable FPU
	fpu_enable();
	//Initialize debug UART
	debug_uart_init();
	slave_dev_uart_init();
	/*Initialize timebase*/
	timebase_init();
	/*Initialize LED*/
	led_init();
	/*Initialize push button*/
	button_init();
	/*Initialize ADC*/
	pa1_adc_init();
	/*start conversion*/
	start_conversion();
	/*Initialize circular buffer*/
	circular_buffer_init();
    while(1){
    	led_on();
    	//printf("Hello Debug from stm32\n\r");
    	btn_state = get_bin_state();
    	sensor_value = adc_read();
    	buffer_send_string("Hello Debug \n\r",DEBUG_PORT);
    	buffer_send_string("Hello Slave Device \n\r",SLAVE_DEV_PORT);
    	delay(1);
    };
}


