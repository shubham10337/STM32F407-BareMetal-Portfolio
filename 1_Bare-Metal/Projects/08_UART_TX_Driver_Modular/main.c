#include<stdint.h>
#include<stm32f407xx.h>

#include "uart.h"

int main(void){
    usart2_setup();
    
    while(1){
         send_data(9);
    }

}
