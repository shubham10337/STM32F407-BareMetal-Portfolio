#ifndef STM32F411_H
#define STM32F411_H

#include <stdint.h>

// Base Addresses
#define RCC_BASE      0x40023800
#define GPIOA_BASE    0x40020000
#define GPIOC_BASE    0x40020800
#define SYSCFG_BASE   0x40013800
#define EXTI_BASE     0x40013C00
#define NVIC_BASE     0xE000E100

// Register Definitions
#define RCC_AHB1ENR   *((volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB2ENR   *((volatile uint32_t *)(RCC_BASE + 0x44))

#define GPIOC_MODER   *((volatile uint32_t *)(GPIOC_BASE + 0x00))
#define GPIOC_ODR     *((volatile uint32_t *)(GPIOC_BASE + 0x14))

#define GPIOA_MODER   *((volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_PUPD    *((volatile uint32_t *)(GPIOA_BASE + 0x0C))

#define SYSCFG_EXTICR1 *((volatile uint32_t *)(SYSCFG_BASE + 0x08))

#define EXTI_IMR      *((volatile uint32_t *)(EXTI_BASE + 0x00))
#define EXTI_FTSR     *((volatile uint32_t *)(EXTI_BASE + 0x0C))
#define EXTI_PR       *((volatile uint32_t *)(EXTI_BASE + 0x14))

#define NVIC_ISER0    *((volatile uint32_t *)(NVIC_BASE + 0x00))

#endif