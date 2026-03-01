#include<stdint.h>

//defining pointers
#define pRCC_AHB1ENR    ((volatile uint32_t *)(0x40023800 + 0x30))
#define pRCC_APB1ENR    ((volatile uint32_t *)(0x40023800 + 0x40))
#define pGPIOD_MODER    ((volatile uint32_t *)(0x40020C00))
#define pGPIOD_ODR      ((volatile uint32_t *)(0x40020C00 + 0x14))
#define pTIM4_PSC       ((volatile uint32_t *)(0x40000800 + 0x28))
#define pTIM4_ARR       ((volatile uint32_t *)(0x40000800 + 0x2C))
#define pTIM4_CNT       ((volatile uint32_t *)(0x40000800 + 0x24))
#define pTIM4_CR1       ((volatile uint32_t *)(0x40000800))
#define pTIM4_SR        ((volatile uint32_t *)(0x40000800 + 0X10))

#define PSC             (15999)

void delay(uint32_t ms);

int main(void){

//enabling clock
*pRCC_AHB1ENR |=(1<<3);
*pRCC_APB1ENR |=(1<<2);

//gpio configuration
*pGPIOD_MODER &= ~(3<<26);
*pGPIOD_MODER |=  (1<<26);

while(1){
    *pGPIOD_ODR ^=(1<<13);
    delay(500);

}
}

void delay(uint32_t ms){
    //configuring Timers
    *pTIM4_PSC = PSC;
    *pTIM4_ARR = ms;
    *pTIM4_CNT = 0;
    
    //enable counter
    *pTIM4_CR1 |= (1<<0);

    //waiting for entering flag
    while(!(*pTIM4_SR & (1<<0)));

    *pTIM4_SR &= ~(1<<0);
    *pTIM4_CR1 &= ~(1<<0);
}
