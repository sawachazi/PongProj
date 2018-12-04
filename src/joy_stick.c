/*
 * joy_stick.c
 *
 *  Created on: Nov 1, 2018
 *      Author: zachwalters
 */


#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

void display1(const char *s);
void display2(const char *s);


// 'controllers' initialized in main as global variable

void run_adc(uint16_t* controllers){
    char line[16];
//    ADC1->CHSELR = 0;
    //ADC1->CHSELR |= ADC_CHSELR_CHSEL1;
    while(1){
        while(!(ADC1->ISR & ADC_ISR_ADRDY));
        ADC1->CR |= ADC_CR_ADSTART;
        //while(!(ADC1->ISR & ADC_ISR_EOC));
        //sprintf(line, "Integer: 0x%x", controller);
        //display1(line);
        //sprintf(line, "Integer: 0x%x", controllers[1]);
        sprintf(line, "Voltage: %0.3f", controllers[0] * 3 / 4095.0 );
        display1(line);
        sprintf(line, "Voltage: %0.3f", controllers[1] * 3 / 4095.0 );
        display2(line);
    }
}

void update_controllers(uint16_t * controllers){
	while(!(ADC1->ISR & ADC_ISR_ADRDY));
	ADC1->CR |= ADC_CR_ADSTART;
	while(!(ADC1->ISR & ADC_ISR_EOC));
}

void calib_adc(){
    if ((ADC1->CR & ADC_CR_ADEN) != 0) /* (1) */
    {
        ADC1->CR &= (uint32_t)(~ADC_CR_ADEN); /* (2) */
    }
    ADC1->CR |= ADC_CR_ADCAL; /* (3) */
    while ((ADC1->CR & ADC_CR_ADCAL) != 0); // (4)

}
