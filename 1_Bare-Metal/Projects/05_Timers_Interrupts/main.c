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
#define pTIM4_DIER      ((volatile uint32_t *)(0x40000800 + 0x0C))
#define pNVIC_ISER0     ((volatile uint32_t *)(0xE000E100))
#define pTIM4_SR        ((volatile uint32_t *)(0x40000800 + 0X10))

#define PSC             (15999)

//interrupt handler prototype
void TIM4_IRQHandler(void);

int main(void){

    //enabling clock
    *pRCC_AHB1ENR |=(1<<3);
    *pRCC_APB1ENR |=(1<<2);

    //gpio configuration
    *pGPIOD_MODER &= ~(3<<26);
    *pGPIOD_MODER |=  (1<<26);

    //CONFIGURING TIMERS
    *pTIM4_PSC = PSC;
    *pTIM4_ARR = 500;
    *pTIM4_CNT = 0;
    
    //ENABLING INTERRUPT
    *pTIM4_DIER |= (1<<0);
    *pNVIC_ISER0 |= (1<<30);

    //enable counter
    *pTIM4_CR1 |= (1<<0);

while(1){
    //DO USEFUL WORK
}
}

void TIM4_IRQHandler(void){
    *pTIM4_SR &= ~(1<<0);
    *pGPIOD_ODR ^=(1<<13);
}


