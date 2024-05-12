// uart.h

#ifndef UART_H_
#define UART_H_


// Define a USART protocol configuration
#define USART_CR1_UE     0x1UL << 13U 		// Enable USART protocol
#define USART_CR1_M      0x1UL << 12U  		// Set Word length of 8 bits
#define USART_CR2_STOP   0x3UL << 12U  		// Set 1 stop bit
#define USARTDIV         0x22B				// Set baudrate to 115200 bps with a frequency of 64 MHz
#define USART_CR1_TE     0x1UL <<  3U 		// Enable transmitter
#define USART_SR_TXE   	 0x1UL <<  7U 		// Transmit data register empty flag

// Functions
void USER_USART1_Init( void ); 	// Initialize USART1
int _write( int, char*, int );	// Redirect standard output to USART1 and print it in the serial monitor

#endif

