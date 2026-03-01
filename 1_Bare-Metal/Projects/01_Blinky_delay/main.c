#include <stdint.h>

#define pRCC_AHB1ENR     ((volatile uint32_t*) (0x40023800 + 0x30))
#define pGPIOD_MODER     ((volatile uint32_t*) (0x40020C00))
#define pGPIOD_ODR       ((volatile uint32_t*) (0x40020C00 + 0x14))

int main(void){
    // Enabling clock
    *pRCC_AHB1ENR |= (1<<3);

    //configuring registers
    *pGPIOD_MODER &= ~(3<<24); //LED 4 (pin 12)
    *pGPIOD_MODER &= ~(3<<26); //Led 3 (pin 13)
    *pGPIOD_MODER &= ~(3<<28); //Led 5 (pin 14)
    *pGPIOD_MODER &= ~(3<<30); //Led 6 (pin 15)
    *pGPIOD_MODER |=  (1<<24);
    *pGPIOD_MODER |=  (1<<26);
    *pGPIOD_MODER |=  (1<<28);
    *pGPIOD_MODER |=  (1<<30);
    *pGPIOD_ODR   |=  (1<<12); //set bit 12 for pin 12
    *pGPIOD_ODR   |=  (1<<13); //set bit 13 for pin 13
    *pGPIOD_ODR   |=  (1<<14); //set bit 14 for pin 14
    *pGPIOD_ODR   |=  (1<<15); //set bit 15 for pin 15

   while (1)
   {
    *pGPIOD_ODR ^= (1<<12);
    *pGPIOD_ODR ^= (1<<13);
    *pGPIOD_ODR ^= (1<<14);
    *pGPIOD_ODR ^= (1<<15);
    for(volatile uint32_t i = 1; i<=4000000;i++){
        
    }
   }
   
}

