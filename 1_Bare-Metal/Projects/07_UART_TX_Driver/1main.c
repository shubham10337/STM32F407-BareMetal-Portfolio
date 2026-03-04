#include<stdint.h>
#include<stm32f407xx.h>

#define USART2EN (1U << 17)
#define GPIOAEN  (1U << 0)
#define USARTEN  (1U << 13)
#define TX_EN    (1U << 3)
#define RX_EN    (1U << 2)
#define SR_TXE   (1U << 7)

#define RX_Pin 3
#define TX_Pin 2

#define Periph_clk 16000000
#define Baudrate 115200

static uint16_t calculateBR(uint32_t periph_clk,uint32_t baudrate);
static void send_data(int data);


int main(void){
    //Enable Clock
    RCC -> APB1ENR |= USART2EN;
    RCC -> AHB1ENR |= GPIOAEN;

    //GPIO MODE SETUP
    GPIOA -> MODER &= ~(1U <<(2*RX_Pin));
    GPIOA -> MODER &= ~(1U <<(2*TX_Pin));
    GPIOA -> MODER |= (2U <<(2*RX_Pin));
    GPIOA -> MODER |= (2U <<(2*TX_Pin));
    GPIOA -> AFR[0] &= ~(0xf << 8);
    GPIOA -> AFR[0] |= (7U << 8);
    GPIOA -> AFR[0] &= ~(0xf << 12);
    GPIOA -> AFR[0] |= (7U << 12);

    //UART SETUP
    USART2 -> CR1 |= USARTEN;
    USART2 -> CR1 |= TX_EN;

    USART2 -> BRR = calculateBR(Periph_clk,Baudrate);

    while(1){
         send_data(9);
    }

}

static uint16_t calculateBR(uint32_t periph_clk,uint32_t baudrate){
    return ((periph_clk + (baudrate/2))/baudrate);
}

static void send_data(int data){
    while(!(USART2 -> SR & SR_TXE)){

    }
    USART2 -> DR =  (data &0xff);
}