// uart.h

#ifndef UART_H_
#define UART_H_

// Defines
#define USARTDIV         0x22B		// Baud rate divisor for 115200 bps

// Function prototypes
void USER_USART1_Init( void ); 		// Initialize USART1
int _write( int, char*, int );		// Send data through USART1
uint8_t USER_USART1_Receive(void); 	// Function to receive data via USART1


#endif

