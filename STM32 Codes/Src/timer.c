// timer.c

// Libraries and header files
#include "main.h"
#include "timer.h"

// Initialize TIM2 (Timer)
void USER_TIM2_Init( void ) {

	TIM2->SMCR	&=	~(TIM_SMCR_SMS);	//	Enable internal clock source
	TIM2->CR1	&=	~(TIM_CR1_UDIS);	//	Enable UEV
	TIM2->CR1	&=	~(TIM_CR1_DIR);		// 	Set counter as upcounter
	TIM2->CR1	&=	~(TIM_CR1_CMS);		//	Set Edge-aligned mode
	TIM2->SR	&=	~(TIM_SR_UIF);		//	Clear UIF
}

// Function to delay with TIM2 based on the function's input parameters
void USER_TIM2_Delay( uint16_t TIM_PSC, uint16_t TIM_CNT ) {

	TIM2->PSC	=	TIM_PSC;			//  Prescaler value for the delay
	TIM2->CNT	=	TIM_CNT;			//  Initual count for the delay
	TIM2->CR1	|=	TIM_CR1_CEN;		//	Enable TIM2
	while(!( TIM2->SR & TIM_SR_UIF ));	//	Wait for UIF
	TIM2->CR1	&=	~(TIM_CR1_CEN);		//	Stop TIM2 timer
	TIM2->SR	&=	~(TIM_SR_UIF);		//	Clear UIF
}
