#include <stdint.h>

typedef struct {
    volatile uint32_t MODER;    // Offset 0x00
    volatile uint32_t OTYPER;   // Offset 0x04
    volatile uint32_t OSPEEDR;  // Offset 0x08
    volatile uint32_t PUPDR;    // Offset 0x0C
    volatile uint32_t IDR;      // Offset 0x10
    volatile uint32_t ODR;      // Offset 0x14
} GPIO_RegDef_t;

// Cast the raw base addresses to our new structure type
#define pGPIOA ((GPIO_RegDef_t*) 0x40020000)
#define pGPIOD ((GPIO_RegDef_t*) 0x40020C00)
#define pRCC_AHB1ENR ((volatile uint32_t *)(0x40023800 + 0x30))

int main(void){
    //ENABLING CLOCK
    *pRCC_AHB1ENR |=(1<<0);
    *pRCC_AHB1ENR |=(1<<3);

    //GPIO CONFIGURATION
    pGPIOA -> MODER &= ~(3<<0);
    pGPIOD -> MODER &= ~(3<<24);
    pGPIOD -> MODER |=  (1<<24);

    //Executing the loop
    while(1){
        if((pGPIOA -> IDR) & (1<<0)){
            pGPIOD -> ODR |=(1<<12);
        }
        else{
            pGPIOD -> ODR &= ~(1<<12);
        }
    }
}