/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   This program will interface with a 64x128 pxl display and two joy
  *          sticks
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>
#include <stdio.h>

#include "display.h"
#include "joy_stick.h"
#include "serial_com.h"

#define NUMBER_OF_ADC_CHANNELS 2
void init_lcd();

void init_gpio(void);
void init_adc();
void init_dma();
uint16_t controllers[] = {0,0};


int main(void)
{
    init_lcd();
    init_gpio();
    // might need to calibrate: FRM: A.7.1
    init_adc();
    init_dma();
    init_audio();
    init_usart2();
    init_audio();
    //start_display();
    //write_display();
    //display1("hello");
    //run_adc();
    // PLEASE SEE MY COMMIT NOW
    // test
    testbench(&controllers);

}

/*
 *  Function: init Gpio
 *
 *  PB0-PB12 as GPIO outputs
 *  PB0-PB7 connected to DISPLAY DB0-DB7
 *  PB8 connected to DISPLAY Chip Select for IC2 (CS2)
 *  PB9 connected to DISPLAY Chip Select for IC1 (CS1)
 *  PB10 connected to DISPLAY Reset signal (RES)
 *  PB11 connected to DISPLAY instruction code (D/I)
 *  PB12 connected to DISPLAY Enable (E)
 *
 *  PA0-PA1 as GPIO Alternate Functions
 *  PA0 - ADC_IN0 - controller 1
 *  PA1 - ADC_IN1 - controller 2
 *
 *  //TBD - DAC - Speaker
 *
 *
 */

void init_gpio(void){
    // display INITIALIZATION
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER = ~(0x3ffffff);
    GPIOB->MODER |= 0x1555555; // set as outputs
    GPIOB->ODR &= ~(0x1fff); // initialize to 0;
    GPIOB->ODR |= EN;
    GPIOB->ODR |= CS1;

    // Controller INITIALIZATION
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= 0xf;


}

/*
 * Function: init_adc
 *
 */

void init_adc(){
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->CR2 |= RCC_CR2_HSI14ON;
    while(!(RCC->CR2 & RCC_CR2_HSI14RDY));
    calib_adc();
    ADC1->CR |= ADC_CR_ADEN;
    while(!(ADC1->ISR & ADC_ISR_ADRDY));
    while((ADC1->CR & ADC_CR_ADSTART));
    // Initializa DMA mode
}

/*
 * Function: init_dma
 *
 * init channels 0 and 1, for ADC in circ mode
 *
 *
 */

void init_dma(){
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    // configure ADC1 for DMA ENABLED
    ADC1->CFGR1 &= ~(ADC_CFGR1_SCANDIR);
    ADC1->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG;
    ADC1->CHSELR = 0;
    ADC1->CHSELR |= ADC_CHSELR_CHSEL0 | ADC_CHSELR_CHSEL1; // channels 0 & 1

    // CONFIGURE DMA CHANNEL 1 for Circ Mode, pointing to ADC1->DR
    // Note: Might have to configure two dma channels for second ADC channel
    DMA1_Channel1->CCR &= ~(DMA_CCR_EN);

    DMA1_Channel1->CPAR = (uint32_t) (&(ADC1->DR));
    DMA1_Channel1->CMAR = (uint32_t) controllers;
    DMA1_Channel1->CNDTR = 2;
    DMA1_Channel1->CCR |= DMA_CCR_MINC | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 |
            DMA_CCR_TEIE | DMA_CCR_CIRC;

    DMA1_Channel1->CCR |= DMA_CCR_EN;
}
