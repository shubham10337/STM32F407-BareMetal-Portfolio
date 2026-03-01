#include <stdint.h>
#include <stm32f407xx.h>

#define GPIODEN (1U << 3)

#define LED3 13
#define LED4 12
#define LED5 14
#define LED6 15

int main(void){
  //Enable clock
  RCC -> AHB1ENR |= GPIODEN ;

  //GPIO setup
  GPIOD -> MODER &= ~(1U << (2*LED3));
  GPIOD -> MODER &= ~(1U << (2*LED4));
  GPIOD -> MODER &= ~(1U << (2*LED5));
  GPIOD -> MODER &= ~(1U << (2*LED6));
  GPIOD -> MODER |= (1U << (2*LED3));
  GPIOD -> MODER |= (1U << (2*LED4));
  GPIOD -> MODER |= (1U << (2*LED5));
  GPIOD -> MODER |= (1U << (2*LED6));

  while(1){
    GPIOD -> ODR |= (0xf << 12);
    for(uint32_t i = 0 ; i < 2000000 ; i++);
    
    GPIOD -> ODR &= ~(0xf << 12);
    for(uint32_t i = 0 ; i < 2000000 ; i++);

  }

}