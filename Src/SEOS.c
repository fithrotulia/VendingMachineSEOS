/*
 * SEOS.c
 *
 *  Created on: Nov 24, 2019
 *      Author: fithrotul
 */

#include "main.h"
#include "Task.h"
#include "SEOS.h"

void TIM3_IRQHandler(void)
{
	Task_Run();
}
void SCH_Init(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3->PSC = 0;
	TIM3->ARR = 72;
	TIM3->CR1 |= TIM_CR1_URS;
	TIM3->DIER |= TIM_DIER_UIE;
	TIM3->EGR |= TIM_EGR_UG;

	NVIC_EnableIRQ(TIM3_IRQn);
}
