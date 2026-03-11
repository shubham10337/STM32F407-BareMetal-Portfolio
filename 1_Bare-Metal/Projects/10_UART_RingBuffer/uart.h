#ifndef UART_H_
#define UART_H_

#include<stdint.h>
#include<stm32f407xx.h>

#define BufferSize 64

void write_buffer(char data);
void read_buffer(void);
void usart2_setup(void);

#endif /* CD8D2605_3698_4F90_988B_D8ED101674F2 */
