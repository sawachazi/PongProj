/*
 * serial_com.c
 *
 *  Created on: Nov 14, 2018
 *      Author: zachwalters
 */
#include "serial_com.h"

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <string.h>
#include <stdlib.h> // for strtoul()


void init_usart2(void){
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER &= ~(0xf0);
    GPIOA->MODER |= 0xA0;
    GPIOA->AFR[0] &= ~(0xff00);
    GPIOA->AFR[0] |= 0x1100;
    GPIOA->OSPEEDR |= 0x30;

    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    USART2->CR1 &= ~(USART_CR1_UE); // disable usart
    USART2->CR2 &= ~USART_CR2_STOP; // one stop bit
    USART2->CR1 &= ~USART_CR1_PCE;

    USART2->CR1 &= ~USART_CR1_OVER8; // over sampling by 16 bits
    USART2->BRR = 0x1a1;
    USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE);
    USART2->CR1 |= USART_CR1_UE;

}

//============================================================================
// Configuration problem 5:
// Implement writechar so that it waits for the "transmitter empty" flag
// in the interrupt and status register of USART2 to be true.  Then it
// should output the character passed in the parameter c to the transmit
// data register of USART2.
// - writechar should not wait for the transmission to be complete.
// - writechar should check if the parameter is a newline ('\n') and first
//   send a carriage return ('\r').
//

void writechar(char c) {
    while(!(USART2->ISR & USART_ISR_TXE));
    if (c == '\n'){
        USART2->TDR = '\r' & 0xff;
        while(!(USART2->ISR & USART_ISR_TXE));
        USART2->TDR = '\n' & 0xff;
        return;
    }
    USART2->TDR = c & 0xff;
}

//============================================================================
// Configuration problem 6:
// Implement readchar in the following way:
// - If the USART2 ISR has the Overrun Error flag set, clear it by writing
//   the appropriate flag in the ICR.
// - Wait for the USART2 ISR to indicate that the read data register is
//   not empty.
// - Read a character from the USART2 read data register and return it.
//
char readchar(void) {
    if ((USART2->ISR & USART_ISR_ORE) == USART_ISR_ORE){
       USART2->ICR |= USART_ICR_ORECF;
    }
    while(!(USART2->ISR & USART_ISR_RXNE));
    return USART2->RDR;
}


//============================================================================
// print()
// Write a string to the serial port output.
// Return the length of the string.
//
int print(const char *s) {
    const char *ptr;
    for(ptr=s; *ptr; ptr+=1)
        writechar(*ptr);
    return ptr - s;
}
int println(const char *s) {
    int len = print(s);
    print("\n");
    return len;
}

//============================================================================
// readln()
// Read a line of input.  Echo characters.  Handle backspace and printing
// of control characters.
//
int readln(void *v, int len) {
    int i;
    char *c = v;
    for(i=0; i<len; i++) {
        char tmp = readchar();
        if (tmp == '\b' || tmp == '\177') {
            if (i == 0) {
                i -= 1;
            } else {
                if (c[i-1] < 32) {
                    print("\b\b  \b\b");
                } else {
                    print("\b \b");
                }
                i -= 2;
            }
            continue;
        }
        else if (tmp == '\r') {
            print("\r\n");
            c[i++] = '\n';
            return i;
        } else if (tmp == 0) {
            print("^0");
        } else if (tmp == 28) {
            print("^\\");
        } else if (tmp < 32) {
            writechar('^');
            writechar('A'-1+tmp);
        } else {
            writechar(tmp);
        }
        c[i] = tmp;
    }
    return i;
}


//===========================================================================
// Act on a command read by testbench().
void action(char **words, uint16_t * controllers) {
    if (words[0] != 0) {
        if (strcasecmp(words[0],"init") == 0) {
            if (strcasecmp(words[1],"lcd") == 0) {
                init_lcd();
                return;
            }
        }
        if (strcasecmp(words[0],"display1") == 0) {
            display1(words[1]);
            return;
        }
        if (strcasecmp(words[0],"display2") == 0) {
            display2(words[1]);
            return;
        }
        if (strcasecmp(words[0],"run") == 0) {
            if (strcasecmp(words[1],"adc") == 0){
                run_adc(controllers);
                return;
            }
        }
        if (strcasecmp(words[0],"start") == 0) {
            if (strcasecmp(words[1], "display") == 0){
                start_display();
                return;
            }
        }
        if (strcasecmp(words[0], "write") == 0){
            if (strcasecmp(words[1], "display") == 0){
                write_display();
                return;
            }

        }
        if (strcasecmp(words[0], "display") == 0){
            if (strcasecmp(words[1], "on") == 0){
                display_on();
                return;
            }
        }
        if (strcasecmp(words[0], "display") == 0){
            if (strcasecmp(words[1], "off") == 0){
                display_off();
                return;
            }
        }


        println("Unrecognized command.");
    }
}

//===========================================================================
// Interact with the hardware.
void testbench(uint16_t * controllers) {
    println("STM32 testbench.");
    for(;;) {
        char buf[60];
        print("> ");
        int sz = readln(buf, sizeof buf);
        if (sz > 0)
            buf[sz-1] = '\0';
        char *words[7] = { 0,0,0,0,0,0,0 };
        int i;
        char *cp = buf;
        for(i=0; i<6; i++) {
            // strtok tokenizes a string, splitting it up into words that
            // are divided by any characters in the second argument.
            words[i] = strtok(cp," \t");
            // Once strtok() is initialized with the buffer,
            // subsequent calls should be made with NULL.
            cp = 0;
            if (words[i] == 0)
                break;
            if (i==0 && strcasecmp(words[0], "display1") == 0) {
                words[1] = strtok(cp, ""); // words[1] is rest of string
                break;
            }
            if (i==0 && strcasecmp(words[0], "display2") == 0) {
                words[1] = strtok(cp, ""); // words[1] is rest of string
                break;
            }
        }
        action(words, controllers);
    }
}
