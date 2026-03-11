#include <stdint.h>
#include "stm32f407xx.h"
#include "uart.h" /* Your custom blueprint */

void USART2_Init(void) {
    /* 1. Clock Enablement */
    RCC->AHB1ENR |= (1U << 0);
    RCC->APB1ENR |= (1U << 17);

    /* 2. GPIO Multiplexing (PA2, PA3 -> AF7) */
    GPIOA->MODER &= ~((3U << 4) | (3U << 6));
    GPIOA->MODER |=  ((2U << 4) | (2U << 6));

    GPIOA->AFR[0] &= ~((0xFU << 8) | (0xFU << 12));
    GPIOA->AFR[0] |=  ((7U << 8)  | (7U << 12));

    /* 3. Baud Rate (115200 at 16MHz) */
    USART2->BRR = (8U << 4) | (11U);

    /* 4. Ignition (8-bit, TX enable, RX enable, Engine enable) */
    USART2->CR1 &= ~(1U << 12);
    USART2->CR1 |=  (1U << 3);
    USART2->CR1 |=  (1U << 2);
    USART2->CR1 |=  (1U << 13);
}

void USART2_SendChar(char data) {
    while(!(USART2->SR & (1U << 7))) {
    }
    USART2->DR = data;
}

char USART2_ReadChar(void) {
    while(!(USART2->SR & (1U << 5))) {
    }
    return USART2->DR;
}