#include <stdint.h>

//Define pointers
#define pGPIOA_MODER            ((volatile uint32_t *)(0x40020000))
#define pGPIOD_MODER            ((volatile uint32_t *)(0x40020C00))
#define pGPIOD_MODER            ((volatile uint32_t *)(0x40020C00))
#define pGPIOA_IDR              ((volatile uint32_t *)(0x40020000 + 0x10))
#define pGPIOD_ODR              ((volatile uint32_t *)(0x40020C00 + 0x14))
#define pRCC_AHB1ENR            ((volatile uint32_t *)(0x40023800 + 0x30))
#define pRCC_APB2ENR            ((volatile uint32_t *)(0x40023800 + 0x44))
#define pSYSCFG_EXTICR1         ((volatile uint32_t *)(0x40013800 + 0x08))
#define pEXTI_IMR               ((volatile uint32_t *)(0x40013C00))
#define pEXTI_RTSR              ((volatile uint32_t *)(0x40013C00 + 0x08))
#define pEXTI_FTSR              ((volatile uint32_t *)(0x40013C00 + 0x0C))
#define pEXTI_PR                ((volatile uint32_t *)(0x40013C00 + 0x14))
#define pNVIC_ISER0             ((volatile uint32_t *)(0xE000E100))

//Interrupt handler protoype 
void EXTI0_IRQHandler(void);

int main(void){
    //Enable clock 
    *pRCC_AHB1ENR |= (9<<0);
    *pRCC_APB2ENR |= (1<<14);

    //Configure GPIO
    *pGPIOA_MODER &= ~(3<<0);
    *pGPIOD_MODER &= ~(3<<26);
    *pGPIOD_MODER |= (1<<26);
    *pGPIOD_MODER &= ~(3<<30);
    *pGPIOD_MODER |= (1<<30);

    //Configure Interrupts
    *pSYSCFG_EXTICR1 &= ~(0xF << 0);
    *pEXTI_IMR |= (1<<0);
    *pEXTI_FTSR |= (1<<0);
    *pEXTI_RTSR |= (1<<0);
    *pNVIC_ISER0 |= (1<<6);
    
    //clear the flag
    //*pEXTI_PR &= ~(1<<0);

    while(1){

    }
}

void EXTI0_IRQHandler(void){
    //for(volatile uint32_t i = 0 ; i < 10000 ; i++);
    if(*pGPIOA_IDR & (1<<0)){
        *pGPIOD_ODR &= ~(1<<13);
        *pGPIOD_ODR |= (1<<15);
    } else{
        *pGPIOD_ODR &= ~(1<<15);
        *pGPIOD_ODR |= (1<<13);
    }

    //clear flag
    *pEXTI_PR |= (1<<0);
}