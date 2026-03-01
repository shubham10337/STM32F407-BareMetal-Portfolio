#include <stdint.h>

#define pRCC_AHB1ENR ((volatile uint32_t *)(0x40023800 + 0x30))

#define pGPIOD_MODER ((volatile uint32_t *)(0x40020C00))
#define pGPIOA_MODER ((volatile uint32_t *)(0x40020000))

#define pGPIOA_IDR   ((volatile uint32_t *)(0x40020000 + 0x10))
#define pGPIOD_ODR   ((volatile uint32_t *)(0x40020C00 + 0x14))

int main(void){
    //Enabling clock
    *pRCC_AHB1ENR |= (1<<0);
    *pRCC_AHB1ENR |= (1<<3);

    //REGISTERS CONFIGURATION

    *pGPIOA_MODER &= ~(3<<0);

    *pGPIOD_MODER &= ~(3<<24);
    *pGPIOD_MODER |=  (1<<24);

    while(1){
        if((*pGPIOA_IDR & (1<<0))){
            *pGPIOD_ODR |= (1<<12);
        }
        else{
            *pGPIOD_ODR &= ~(1<<12);
        }
    }

    
}
