#include<stdint.h>
#include<stm32f407xx.h>
#include "uart.h"

#define USART2EN (1U << 17)
#define GPIOAEN  (1U << 0)
#define USARTEN  (1U << 13)
#define TX_EN    (1U << 3)
#define RX_EN    (1U << 2)
#define SR_TXE   (1U << 7)
#define SR_RXNE  (1U << 5)
#define CR1_RXNEIE  (1U << 5)

#define RX_Pin 3
#define TX_Pin 2

#define Periph_clk 16000000
#define Baudrate 115200

uint16_t calculateBR(uint32_t periph_clk,uint32_t baudrate);
char Buffer[BufferSize];
uint8_t head = 0;
uint8_t tail = 0;
void usart2_setup(void){
        //Enable Clock
    RCC -> APB1ENR |= USART2EN;
    RCC -> AHB1ENR |= GPIOAEN;

    //GPIO MODE SETUP
    GPIOA -> MODER &= ~(1U <<(2*RX_Pin));
    GPIOA -> MODER &= ~(1U <<(2*TX_Pin));
    GPIOA -> MODER |= (2U <<(2*RX_Pin));
    GPIOA -> MODER |= (2U <<(2*TX_Pin));
    GPIOA -> AFR[0] &= ~(0xf << 8);
    GPIOA -> AFR[0] |= (7U << 8);
    GPIOA -> AFR[0] &= ~(0xf << 12);
    GPIOA -> AFR[0] |= (7U << 12);

    //UART SETUP
    USART2 -> CR1 |= USARTEN;
    USART2 -> CR1 |= TX_EN;
    USART2 -> CR1 |= RX_EN;

    USART2 -> BRR = calculateBR(Periph_clk,Baudrate);

    USART2 -> CR1 |= CR1_RXNEIE;
    NVIC -> ISER[1] = (1U << 6);
}


uint16_t calculateBR(uint32_t periph_clk,uint32_t baudrate){
    return ((periph_clk + (baudrate/2))/baudrate);
}


void write_buffer(char data){
    Buffer[head] = data;
    head = (head + 1) % BufferSize;
}

void read_buffer(void){
    if(head == tail){
        return '\0';
    }
    char return_data = Buffer[tail];
    tail = (tail + 1 ) % BufferSize;
    return return_data;
}

void USART2_IRQHandler(void){
    if(USART2 -> SR & SR_RXNE){
        write_buffer((char)(USART2 -> DR & 0xff));
    }
}