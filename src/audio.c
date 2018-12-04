/*
 * audio.c
 *
 *  Created on: Dec 2, 2018
 *      Author: Clayton
 */

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

int tim2_int = 0;

/*
 * Initlaize PA4, which is used as the audio output for the game
 */
void init_pa4(){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= (1<<8);
	GPIOA->ODR &= ~(1<<4);
}

/*
 * Toggle the enable bit of Timer 3 each time this interrupt is invoked.
 * Global variable "tim2_int" counts the number of invokes to this interrupt
 * and the interrupt turns itself off once the threshold is reached.
 */
void TIM2_IRQHandler(void) {
	tim2_int++;
	if(TIM3->CR1 & TIM_CR1_CEN){
		TIM3->CR1 &= ~TIM_CR1_CEN;	//DISABLE TIMER3
	}else{
		TIM3->CR1 |= TIM_CR1_CEN;	//ENABLE TIMER3
	}

	if(tim2_int == 10){
		TIM2->ARR = 1-1;
		TIM3->CR1 &= ~TIM_CR1_CEN;
		tim2_int = 0;
	}

	if((TIM2->SR & TIM_SR_UIF) != 0){
				TIM2->SR &= ~TIM_SR_UIF;
		}
}

/*
 * Initialize Timer 2
 */

void tim2_init(void) {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= 0x2;
	GPIOA->AFR[0] |= 0x2;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	TIM2->PSC = 48000 - 1;
	TIM2->ARR = 1 - 1;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CR1 |= TIM_CR1_CEN;

	NVIC->ISER[0] = 1<<TIM2_IRQn;
}

/*
 * Interrupt used to toggle the state of PA4, creating a square wave
 * at a frequency set by TIM3->PSC and TIM3->ARR.
 */

void TIM3_IRQHandler(void) {
	if(GPIOA->ODR & (1<<4)){
		GPIOA->ODR &= ~(1<<4);
	}else{
		GPIOA->ODR |= (1<<4);
	}

	if((TIM3->SR & TIM_SR_UIF) != 0){
		TIM3->SR &= ~TIM_SR_UIF;
	}
}

/*
 * Initialize Timer 3
 */

void tim3_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3->PSC = 4800-1;
	TIM3->ARR = 10-1;

	TIM3->DIER |= TIM_DIER_UIE;
	NVIC->ISER[0] = 1<<TIM3_IRQn;
}

/*
 * Audio for a collision between the ball and the walls or the
 * ball and a paddle.
 */

void collision(){
	tim2_int = 8;
	TIM2->ARR = 250-1;
}

/*
 * Audio for a score increase.
 */

void score(){
	tim2_int = 0;
	TIM2->ARR = 80-1;
	//ssnano_wait(2000000000);
}

void init_audio(){
	init_pa4();
	tim2_init();
	tim3_init();
	tim2_int = 0;
}