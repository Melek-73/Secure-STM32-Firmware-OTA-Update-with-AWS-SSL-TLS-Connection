#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include "uart.h"
#include "stm32f4xx.h"

#define UART_BUFFER_SIZE	100
#define INIT_VAL			0

/*
Head (Write Pointer / Producer):
	• Tracks where the next piece of new data will be written into the buffer.
	• When you push or enqueue data, it gets stored at the head index, and then head moves forward.
Tail (Read Pointer / Consumer):
	• Tracks where the next piece of data will be read or removed from the buffer.
	• When you pop or dequeue data, it is fetched from the tail index, and then tail moves forward.

*/

typedef struct{
	unsigned char buffer[UART_BUFFER_SIZE];
	__IO uint32_t head;
	__IO uint32_t tail;
}circular_buffer;

typedef enum{
	DEBUG_PORT = 0,
	SLAVE_DEV_PORT
}portType;

void circular_buffer_init(void);
void buffer_send_string(const char *s,portType uart);

#endif /* CIRCULAR_BUFFER_H_ */
