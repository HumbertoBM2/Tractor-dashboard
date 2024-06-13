//  uart.c

// Includes
#include "main.h"
#include "uart.h"

// Function definitions

// Initialize USART1
void USER_USART1_Init( void ){
	USART1->CR1	|=	 USART_CR1_UE;		// USART enabled
	USART1->CR1	&=	~USART_CR1_M;		// 8 Data bits
	USART1->CR2	&=	~USART_CR2_STOP;	// 1 Stop bit
	USART1->BRR	 =	 USARTDIV;			// Desired baud rate
	USART1->CR1	|= 	 USART_CR1_TE;		// Transmitter enabled
	USART1->CR1 |=	USART_CR1_RE;		// Receiver enabled
}


// Function to send data via USART1
int _write( int file, char *ptr, int len )
{
  int DataIdx;
  for( DataIdx = 0 ; DataIdx < len; DataIdx++ ) {
    while(!( USART1->SR & USART_SR_TXE ));
    USART1->DR = *ptr++;
  }
  return len;
}

// Function to receive data via USART1
uint8_t USER_USART1_Receive(void) {
	 if (USART1->SR & USART_SR_RXNE)
	 {
		 return (uint8_t)USART1->DR;
	 }
	 else
	 {
	     return 'n'; // No value received
	 }
}
