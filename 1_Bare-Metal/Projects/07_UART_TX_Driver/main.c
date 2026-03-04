#include <stdio.h>
#include <stdint.h>
#include "stm32f407xx.h" /* Official STMicroelectronics Device Header */

/* ------------------------------------------------------------------------- */
/* 1. THE TRANSMISSION ENGINE                                                */
/* ------------------------------------------------------------------------- */
void USART2_SendChar(char data) {
    /* The Roadblock: Wait until Transmit Data Register is Empty (TXE = 1) */
    while (!(USART2->SR & (1 << 7)));
    USART2->DR = data;
}

/* ------------------------------------------------------------------------- */
/* 2. THE STANDARD LIBRARY BRIDGE                                            */
/* ------------------------------------------------------------------------- */
int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        USART2_SendChar(ptr[i]);
    }
    return len;
}

/* ------------------------------------------------------------------------- */
/* 3. MAIN HARDWARE INITIALIZATION                                           */
/* ------------------------------------------------------------------------- */
int main(void) {
    /* Phase 1: Clock Routing */
    RCC->AHB1ENR |= (1 << 0);  /* Enable GPIOA */
    RCC->APB1ENR |= (1 << 17); /* Enable USART2 */

    /* Phase 2: Alternate Function Multiplexing (PA2 = TX, PA3 = RX) */
    GPIOA->MODER &= ~((3 << 4) | (3 << 6)); /* Scrub Mode */
    GPIOA->MODER |=  ((2 << 4) | (2 << 6)); /* Set to AF Mode (10) */
    
    GPIOA->AFRL &= ~((0xF << 8) | (0xF << 12)); /* Scrub AFRL for Pins 2 & 3 */
    GPIOA->AFRL |=  ((7 << 8) | (7 << 12));     /* Inject AF7 */

    /* Phase 3: Baud Rate Configuration (115200 at 16MHz) */
    /* USARTDIV = 16MHz / (16 * 115200) = 8.68 -> Mantissa 8, Fraction 11 */
    USART2->BRR = (8 << 4) | (11);

    /* Phase 4: Ignition Sequence */
    USART2->CR1 &= ~(1 << 12); /* 8 Data Bits (M = 0) */
    USART2->CR1 |=  (1 << 3);  /* Transmitter Enable (TE = 1) */
    USART2->CR1 |=  (1 << 13); /* Main Engine Enable (UE = 1) */

    /* Phase 5: Execution */
    printf("Bare-Metal USART2 Initialized using CMSIS.\r\n");
    printf("System frequency: 16 MHz. Baud: 115200.\r\n");
    printf("Hardware Routing: PA2 (TX), PA3 (RX).\r\n");

    while (1) {
        /* Main operating loop */
    }
}