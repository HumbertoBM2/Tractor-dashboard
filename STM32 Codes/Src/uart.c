// uart.c

// Libraries and header files
#include "main.h"
#include "uart.h"

// Function to initialize UART protocol
void USER_USART1_Init( void ) {
	
  USART1->CR1	|=	 USART_CR1_UE;		//	Enable USART
  USART1->CR1	&=	~USART_CR1_M;		//	Set data width to 8 bits
  USART1->CR2	&=	~USART_CR2_STOP;	//	Set 1 stop bit
  USART1->BRR	 =	 USARTDIV;			//	Baud rate
  USART1->CR1	|= 	 USART_CR1_TE;		//	Enable transmitter
	
}


// Function to redirect output to USART protocol and then print it on a serial monitor
int _write( int file, char *ptr, int len ) {

  int DataIdx;

  for( DataIdx = 0 ; DataIdx < len; DataIdx++ ) {
    while(!( USART1->SR & USART_SR_TXE ));
    USART1->DR = *ptr++;
  }

  return len;
	
}
