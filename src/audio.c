/*
 * audio.c
 *
 *  Created on: Dec 2, 2018
 *      Author: Clayton
 */

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

int tim2_int = 0;

void init_pa4(){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= (1<<8);
	GPIOA->ODR &= ~(1<<4);
}

void TIM2_IRQHandler(void) {
	tim2_int++;
	if(TIM3->CR1 & TIM_CR1_CEN){
		GPIOC->ODR &= ~(1<<8);
		//DISABLE TIMER3
		TIM3->CR1 &= ~TIM_CR1_CEN;
	}else{
		GPIOC->ODR |= (1<<8);
		//ENABLE TIMER3
		TIM3->CR1 |= TIM_CR1_CEN;
	}

	if(tim2_int == 10){
		TIM2->ARR = 1-1;
		TIM3->ARR = 1-1;
		TIM3->CR1 &= ~TIM_CR1_CEN;
		tim2_int = 0;
	}

	if((TIM2->SR & TIM_SR_UIF) != 0){
				TIM2->SR &= ~TIM_SR_UIF;
		}
}

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

void tim3_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3->PSC = 4800-1;
	TIM3->ARR = 10-1;

	TIM3->DIER |= TIM_DIER_UIE;
	NVIC->ISER[0] = 1<<TIM3_IRQn;
}

void collision(){
	tim2_int = 8;
	TIM2->ARR = 250-1;
}

void score(){
	tim2_int = 0;
	TIM2->ARR = 80-1;
	nano_wait(2000000000);
}
