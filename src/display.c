/*
 * display.c
 *
 *  Created on: Nov 7, 2018
 *      Author: zachwalters
 */

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "display.h"

/*
 *  PB0-PB12 as GPIO outputs
 *  PB0-B7 connected to DISPLAY DB0-DB7
 *  PB8 connected to DISPLAY Chip Select 2 (CS2)
 *  PB9 connected to DISPLAY Chip Select 1 (CS1)
 *  PB10 connected to DISPLAY Reset signal (RES)
 *  PB11 connected to DISPLAY instruction code (D/I)
 *  PB12 connected to DISPLAY Enable (E)
*/


void display_off(){
    GPIOB->BRR = EN; // enable off
    GPIOB->BRR |= RS; // set D/I (RS) off
    nano_wait(500);
    GPIOB->BSRR = EN | 0x3e; // enable on and set data
    nano_wait(8000);
    GPIOB->BRR = EN; // enable off
    nano_wait(1000000000);
    GPIOB->BRR = RS | 0x3e; // set D/I (RS) and data off
    nano_wait(500);
    GPIOB->BSRR = EN; // set enable on
    nano_wait(500);

//    GPIOB->BSRR |= 0x3e;
//    GPIOB->BSRR = RS << 16;
//    GPIOB->BSRR |= (0xC1 << 16);
//    GPIOB->BSRR |= (0x1000);
//    nano_wait(100000);
//    GPIOB->BSRR |= (0x1000 << 16);
}

void display_on(){
    GPIOB->BRR = EN; // enable off
    GPIOB->BRR = RS; // set D/I (RS) off
    nano_wait(500);
    GPIOB->BSRR = EN | 0x3f; // enable on and set data
    nano_wait(8000);
    GPIOB->BRR = EN; // enable off
    nano_wait(100000000);
    GPIOB->BRR = RS | 0x3f; // set D/I (RS) and data off
    nano_wait(500);
    GPIOB->BSRR = EN; // set enable on
    nano_wait(500);
    //while(1){
        //set_y_addr(0);
        //set_display_start(0);
        //nano_wait(10000);
        //write_display();
    //}
//    GPIOB->BSRR |= 0x3f;
//    GPIOB->BSRR = RS << 16;
//    //GPIOB->BSRR |= (0xC0 << 16);
//    GPIOB->BSRR |= (0x1000);
//    nano_wait(8000);
//    GPIOB->BSRR |= (0x1000 << 16);
}

void set_y_addr(int x){
    // x addr must b 5 bit;
    if (x >  63){
        return;
    }
    GPIOB->BRR = EN; // enable off
    GPIOB->BRR = RS; // set D/I (RS) on/off
    nano_wait(500);
    GPIOB->BSRR = EN | 0x40; // enable on and set data
    GPIOB->BSRR = x;
    nano_wait(10000);
    GPIOB->BRR = EN; // enable off
    nano_wait(15000);
    GPIOB->BRR = RS | 0xff; // set D/I (RS) and data off
    nano_wait(500);
    GPIOB->BSRR = EN; // set enable on
    nano_wait(500);

//    GPIOB->BSRR |= x;
//    GPIOB->BSRR |= 0x40;
//    GPIOB->BSRR = RS << 16;
//    GPIOB->BSRR |= (0x1000);
//    nano_wait(100000);
//    GPIOB->BSRR |= (0x1000 << 16);
}

void set_display_start(int x){
    if (x > 63){
        return;
    }
    GPIOB->BRR = EN; // enable off
    GPIOB->BRR = RS; // set D/I (RS) on/off
    nano_wait(500);
    GPIOB->BSRR = EN | 0xc0; // enable on and set data
    GPIOB->BSRR = x;
    nano_wait(10000);
    GPIOB->BRR = EN; // enable off
    nano_wait(15000);
    GPIOB->BRR = RS | 0xff; // set D/I (RS) and data off
    nano_wait(500);
    GPIOB->BSRR = EN; // set enable on
    nano_wait(500);

//    GPIOB->BSRR |= x;
//    GPIOB->BSRR |= 0xc0;
//    GPIOB->BSRR = RS << 16;
//    GPIOB->BSRR = EN;
//    nano_wait(100000);
//    GPIOB->BRR = EN;
}

void set_x_addr(int x){
    if (x > 7){
        return;
    }
    GPIOB->BRR = EN; // enable off
    GPIOB->BRR = RS; // set D/I (RS) on/off
    nano_wait(500);
    GPIOB->BSRR = EN | 0xb8; // enable on and set data
    GPIOB->BSRR = x;
    nano_wait(10000);
    GPIOB->BRR = EN; // enable off
    nano_wait(15000);
    GPIOB->BRR = RS | 0xff; // set D/I (RS) and data off
    nano_wait(500);
    GPIOB->BSRR = EN; // set enable on
    nano_wait(500);

//    GPIOB->BSRR |= x;
//    GPIOB->BSRR |= 0x38;
//    GPIOB->BSRR = RS << 16;
//    GPIOB->BSRR |= (0x1000);
//    nano_wait(100000);
//    GPIOB->BSRR |= (0x1000 << 16);
}

void start_display(){
    // turn display on
    GPIOB->BSRR |= 0x3f;
    GPIOB->BSRR |= (0x1 << 27);
    GPIOB->BSRR |= (0x1000);
    nano_wait(100000);
    GPIOB->BSRR |= (0x1000 << 16);
}

void write_display(int x){
    // write to display
    GPIOB->BRR = EN; // enable off
    GPIOB->BSRR = RS; // set D/I (RS) on/off
    nano_wait(500);
    GPIOB->BSRR = EN; // enable on and set data
    GPIOB->BSRR = x;
    //GPIOB->BRR = 0xff;
    nano_wait(10000);
    GPIOB->BRR = EN; // enable off
    nano_wait(15000);
    GPIOB->BRR = RS | 0xff; // set D/I (RS) and data off
    nano_wait(500);
    GPIOB->BSRR = EN; // set enable on
    nano_wait(500);


//    GPIOB->BSRR |= (0x800);
//    GPIOB->BSRR |= 0xff;
//    GPIOB->BSRR |= (0x1000);
//    nano_wait(8000);
//    GPIOB->BSRR |= (0x1000 << 16);
}

void write_alot(void){
    int x;
    int bits = 0xff;
    for(x = 0; x < 64;x++){
        write_display(bits);
        bits = bits << 1;
        if (bits == 0xff00){
            bits = 0xff;
        }
    }
}

void clear_screen(void){
    int y;
    int x;
    for (x = 0;x < 8; x++){
        set_x_addr(x);
        set_y_addr(0);
        for(y = 0;y < 64; y++){
            write_display(0x0);
        }
    }
}

void initilize_display(){
    display_on();
    clear_screen();
    // add dotted line down center of display
    // every other "page"
    int x;
    for(x = 0;x < 8;x++){

    }

}

void clear_screen(void);
void display_ball(int, int);
void display_pong1(int);
void display_pong2(int);
void display_score(int, int);
void display_start_screen();
void display_winner(int);
void display_start_wait();

