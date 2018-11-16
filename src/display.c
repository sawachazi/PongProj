/*
 * display.c
 *
 *  Created on: Nov 7, 2018
 *      Author: zachwalters
 */

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"


/*
 *  PB0-PB12 as GPIO outputs
 *  PB0-B7 connected to DISPLAY DB0-DB7
 *  PB8 connected to DISPLAY Chip Select 2 (CS2)
 *  PB9 connected to DISPLAY Chip Select 1 (CS1)
 *  PB10 connected to DISPLAY Reset signal (RES)
 *  PB11 connected to DISPLAY instruction code (D/I)
 *  PB12 connected to DISPLAY Enable (E)
*/

void start_display(void);
void write_display(void);

void display_off(){
    GPIOB->BSRR |= 0x3e;
    GPIOB->BSRR |= 0x800 << 16;
    GPIOB->BSRR |= (0xC1 << 16);
    GPIOB->BSRR |= (0x1000);
    nano_wait(2000);
    GPIOB->BSRR |= (0x1000 << 16);
}

void display_on(){
    GPIOB->BSRR |= 0x3f;
    GPIOB->BSRR |= 0x1 << 27;
    GPIOB->BSRR |= (0xC0 << 16);
    GPIOB->BSRR |= (0x1000);
    nano_wait(1500);
    GPIOB->BSRR |= (0x1000 << 16);

//    GPIOB->ODR |= 0x1f;
//    GPIOB->ODR |= 1 << 12;
}

void start_display(){
    // turn display on
    GPIOB->BSRR |= 0x3f;
    GPIOB->BSRR |= (0x1 << 27);
    GPIOB->BSRR |= (0x1000);
    nano_wait(2000);
    GPIOB->BSRR |= (0x10000000);

}

void write_display(void){

    // write to display
    GPIOB->BSRR |= 0xff;
    GPIOB->BSRR |= (0x800);
    GPIOB->BSRR |= (0x1000);
    nano_wait(2000);
    GPIOB->BSRR |= (0x1000 << 16);



    //start_display();
}
