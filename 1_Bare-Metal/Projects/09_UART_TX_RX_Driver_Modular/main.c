#include<stdint.h>
#include<stm32f407xx.h>

#include "uart.h"

char data;

int main(void){
    usart2_setup();
    
    while(1){
        data = recieve_data();
         send_data(data);
    }

}
