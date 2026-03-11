#include<stdint.h>
#include<stm32f407xx.h>

#include "uart.h"

char data;

int main(void){
    usart2_setup();
    
    while(1){
        char c = read_buffer;
        if(c != '/0'){
            write_buffer(c);
        }
    }

}
