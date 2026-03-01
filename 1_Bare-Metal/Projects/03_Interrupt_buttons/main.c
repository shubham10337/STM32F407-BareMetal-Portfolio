#include<stdint.h>

//Defining pointers
#define pRCC_AHB1ENR          ((volatile uint32_t *)(0x40023800 + 0x30))
#define pRCC_APB2ENR          ((volatile uint32_t *)(0x40023800 + 0x44))
#define pGPIOA_MODER          ((volatile uint32_t *)(0x40020000))
#define pGPIOD_MODER          ((volatile uint32_t *)(0x40020C00))
#define pGPIOD_ODR            ((volatile uint32_t *)(0x40020C00 + 0X14))
#define pSYSCFG_EXTICR1       ((volatile uint32_t *)(0x40013800 + 0x08))
#define pEXTI_IMR             ((volatile uint32_t *)(0x40013C00))
#define pEXTI_RTSR            ((volatile uint32_t *)(0x40013C00 + 0x08))
#define pNVIC_ISER0           ((volatile uint32_t *)(0xE000E100))
#define pEXTI_PR              ((volatile uint32_t *)(0x40013C00 + 0x14))

//Interrupt Handler Prototype
void EXTI0_IRQHandler(void);

int main(void){
  //Enable Clock
  //*pRCC_AHB1ENR &= ~(9<<0);
  *pRCC_AHB1ENR |= (9<<0);
  *pRCC_APB2ENR |= (1<<14);
  
  //Configure GPIOs
  *pGPIOA_MODER &= ~(3<<0);
  *pGPIOD_MODER |= (1<<26);

  //configure interrupts
  *pSYSCFG_EXTICR1 &= ~(0xF << 0);
  *pEXTI_IMR |= (1<<0);
  *pEXTI_RTSR |= (1<<0);
  *pNVIC_ISER0 |=(1<<6);

  while(1){

  }
}

void EXTI0_IRQHandler(void){
 for(volatile uint32_t i=0;i<500000;i++);
 *pEXTI_PR |= (1<<0);
  *pGPIOD_ODR ^= (1<<13);

}
