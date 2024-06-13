// lcd.c

// Includes
#include "main.h"
#include "lcd.h"
#include "timer.h"


// Function definitions


// User-defined character to load into the CGRAM memory of the LCD
const int8_t UserFont[8][8] =
{
		{ 0x11, 0x0A, 0x04, 0x1B, 0x11, 0x11, 0x11, 0x0E },
		{ 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 },
		{ 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 },
		{ 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C },
		{ 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E },
		{ 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

// Function that initializes the LCD to 4 bits
void LCD_Init(void){

	int8_t const *p;

	// Configuration of all pins to the LCD as general-purpose output push-pull, 10 MHz speed

	GPIOC->CRL	&=	~( 0x3UL << 30U ) & ~( 0x2UL << 28U )
			& 	~( 0x3UL << 26U ) & ~( 0x2UL << 24U );
	GPIOC->CRL 	|= 	 ( 0x1UL << 28U )
			|  	 ( 0x1UL << 24U );
	GPIOC->CRH	&=	~( 0x3UL << 18U ) & ~( 0x2UL << 16U )
			& 	~( 0x3UL << 14U ) & ~( 0x2UL << 12U )
			&	~( 0x3UL << 10U ) & ~( 0x2UL <<  8U )
			& 	~( 0x3UL <<  6U ) & ~( 0x2UL <<  4U )
			& 	~( 0x3UL <<  2U ) & ~( 0x2UL <<  0U );
	GPIOC->CRH	|= 	 ( 0x1UL << 16U )
			|  	 ( 0x1UL << 12U )
			| 	 ( 0x1UL <<  8U )
			|  	 ( 0x1UL <<  4U )
			|  	 ( 0x1UL <<  0U );


	// LCD Initialization
	// https://web.alfredstate.edu/faculty/weimandn/lcd/lcd_initialization/lcd_initialization_index.html
	// Power ON


	GPIOC->BSRR	 =	 LCD_RS_PIN_LOW;
	GPIOC->BSRR	 =	 LCD_RW_PIN_LOW;
	GPIOC->BSRR	 =	 LCD_EN_PIN_LOW;
	GPIOC->BSRR	 =	 LCD_D4_PIN_LOW;
	GPIOC->BSRR	 =	 LCD_D5_PIN_LOW;
	GPIOC->BSRR	 =	 LCD_D6_PIN_LOW;
	GPIOC->BSRR	 =	 LCD_D7_PIN_LOW;

	USER_TIM2_Delay(TIM_PSC_50MS, TIM_CNT_50MS);

	// Special case of 'Function Set'

	GPIOC->BSRR	 =	 LCD_D4_PIN_HIGH;
	GPIOC->BSRR	 =	 LCD_D5_PIN_HIGH;
	GPIOC->BSRR	 =	 LCD_D6_PIN_LOW;
	GPIOC->BSRR	 =	 LCD_D7_PIN_LOW;
	LCD_Pulse_EN( );

	USER_TIM2_Delay(TIM_PSC_5MS, TIM_CNT_5MS);

	// Special case of 'Function Set'

	GPIOC->BSRR	 =	 LCD_D4_PIN_HIGH;
	GPIOC->BSRR	 =	 LCD_D5_PIN_HIGH;
	GPIOC->BSRR	 =	 LCD_D6_PIN_LOW;
	GPIOC->BSRR	 =	 LCD_D7_PIN_LOW;
	LCD_Pulse_EN( );

	USER_TIM2_Delay(TIM_PSC_100US, TIM_CNT_100US);

	// Special case of 'Function Set'

	GPIOC->BSRR	 =	 LCD_D4_PIN_HIGH;
	GPIOC->BSRR	 =	 LCD_D5_PIN_HIGH;
	GPIOC->BSRR	 =	 LCD_D6_PIN_LOW;
	GPIOC->BSRR	 =	 LCD_D7_PIN_LOW;
	LCD_Pulse_EN( );
	while( LCD_Busy( ) );					//	Checking the busy flag

	// Initial 'Function Set' to change 4-bit mode

	GPIOC->BSRR	 =	 LCD_D4_PIN_LOW;
	GPIOC->BSRR	 =	 LCD_D5_PIN_HIGH;
	GPIOC->BSRR	 =	 LCD_D6_PIN_LOW;
	GPIOC->BSRR	 =	 LCD_D7_PIN_LOW;
	LCD_Pulse_EN( );
	while( LCD_Busy( ) );					//	Checking the busy flag

	// 'Function Set' (I=1, N and F as required)

	LCD_Write_Cmd( 0x28U );					//	2-line display, 5x7 dot

	// 'Display ON/OFF Control' (D=0, C=0, B=0)

	LCD_Write_Cmd( 0x08U );					//	Display, cursor and blinking off

	// 'Clear Display'

	LCD_Write_Cmd( 0x01U );//

	// 'Entry Mode Set' (I/D and S as required)

	LCD_Write_Cmd( 0x06U );					//	Cursor increment by 1, shift off

	// Initialization Ends

	LCD_Write_Cmd( 0x0FU );					//	Display, cursor and blinking on

	// Load the character defined by the user into the CGRAM

	LCD_Write_Cmd( 0x40 );					//	Set the CGRAM address to 0
	p = &UserFont[0][0];

	for( int i = 0; i < sizeof( UserFont ); i++, p++ )
		LCD_Put_Char( *p );

	//	Set DDRAM address in address

	LCD_Write_Cmd( 0x80 );//
}

// Function that generates a strobe on the LCD
void LCD_Out_Data4(uint8_t val)
{
	if( ( val & 0x01U ) == 0x01U )				//	Bit[0]
		GPIOC->BSRR	=	LCD_D4_PIN_HIGH;
	else
		GPIOC->BSRR	=	LCD_D4_PIN_LOW;

	if( ( val & 0x02U ) == 0x02U )				//	Bit[1]
		GPIOC->BSRR	=	LCD_D5_PIN_HIGH;
	else
		GPIOC->BSRR	=	LCD_D5_PIN_LOW;

	if( ( val & 0x04U ) == 0x04U )				//	Bit[2]
		GPIOC->BSRR	=	LCD_D6_PIN_HIGH;
	else
		GPIOC->BSRR	=	LCD_D6_PIN_LOW;

	if( ( val & 0x08U ) == 0x08U )				//	Bit[3]
		GPIOC->BSRR	=	LCD_D7_PIN_HIGH;
	else
		GPIOC->BSRR	=	LCD_D7_PIN_LOW;
}

// Function that writes 1 byte of data to the LCD
void LCD_Write_Byte(uint8_t val)
{
	LCD_Out_Data4( ( val >> 4 ) & 0x0FU );
	LCD_Pulse_EN( );
	LCD_Out_Data4( val & 0x0FU );
	LCD_Pulse_EN( );
	while( LCD_Busy( ) );
}

// Function that writes a command to the LCD
void LCD_Write_Cmd( uint8_t val )
{
	GPIOC->BSRR	=	LCD_RS_PIN_LOW;			//	RS=0 (character selection)
	LCD_Write_Byte( val );
}

// Write an ASCII character to the LCD
void LCD_Put_Char( uint8_t c )
{
	GPIOC->BSRR	=	LCD_RS_PIN_HIGH;		//	RS=1 (character selection)
	LCD_Write_Byte( c );
}

// Function that sets the cursor to a position on the LCD screen
// Minimum values for line and column must be 1
void LCD_Set_Cursor( uint8_t line, uint8_t column )
{
	uint8_t address;
	column--;
	line--;
	address = ( line * 0x40U ) + column;
	address = 0x80U + ( address & 0x7FU );
	LCD_Write_Cmd( address );
}

// Function that sends a string of ASCII characters to the LCD
void LCD_Put_Str( char * str )
{
	for( int16_t i = 0; i < 16 && str[ i ] != 0; i++ )
		LCD_Put_Char( str[ i ] );			//	Send 1 byte to the LCD
}

// Function that sends a numeric character to the LCD
// The number must be integer and maximum of 5 digits
void LCD_Put_Num( int16_t num )
{
	int16_t p;
	int16_t f = 0;
	int8_t ch[ 5 ];

	for( int16_t i = 0; i < 5; i++ ){
		p = 1;
		for( int16_t j = 4 - i; j > 0; j-- )
			p = p * 10;
		ch[ i ] = ( num / p );
		if( num >= p && !f )
			f = 1;
		num = num - ch[ i ] * p;
		ch[ i ] = ch[ i ] + 48;
		if( f )
			LCD_Put_Char( ch[ i ] );
	}
}

// Function that causes delays on the LCD

char LCD_Busy( void )
{
	// Configuration of D7 as input floating

	GPIOC->CRH	&=	~( 0x2UL << 18U ) & ~( 0x3UL << 16U );
	GPIOC->CRH	|=   	 ( 0x1UL << 18U );
	GPIOC->BSRR	 =	 LCD_RS_PIN_LOW;
	GPIOC->BSRR	 =	 LCD_RW_PIN_HIGH;
	GPIOC->BSRR	 =	 LCD_EN_PIN_HIGH;

	USER_TIM2_Delay(TIM_PSC_100US, TIM_CNT_100US);		//	100 us

	if(( GPIOC->IDR	& LCD_D7_PIN_HIGH )) 			//	If D7 is set, then
	{
		GPIOC->BSRR	= 	LCD_EN_PIN_LOW;
		GPIOC->BSRR	=	LCD_RW_PIN_LOW;

	// Configuration of D7 as output push-pull, 10 MHz speed

		GPIOC->CRH	&=	~( 0x3UL << 18U ) & ~( 0x2UL << 16U );
		GPIOC->CRH	|=   	 ( 0x1UL << 16U );

		return 1;
	}
	else
	{
		GPIOC->BSRR	= 	LCD_EN_PIN_LOW;
		GPIOC->BSRR	=	LCD_RW_PIN_LOW;

	// Configuration of D7 as output push-pull, 10 MHz speed

		GPIOC->CRH	&=	~( 0x3UL << 18U ) & ~( 0x2UL << 16U );
		GPIOC->CRH	|=   	 ( 0x1UL << 16U );

		return 0;
	}
}

// Function that generates a pulse on the EN pin of the LCD
void LCD_Pulse_EN( void )
{
	GPIOC->BSRR	=	LCD_EN_PIN_LOW;

	USER_TIM2_Delay(TIM_PSC_10US, TIM_CNT_10US);

	GPIOC->BSRR	=	LCD_EN_PIN_HIGH;		//	Enable pin EN ON

	USER_TIM2_Delay(TIM_PSC_10US, TIM_CNT_10US);

	GPIOC->BSRR	=	LCD_EN_PIN_LOW;			//	Enable pin EN OFF

	USER_TIM2_Delay(TIM_PSC_1MS, TIM_CNT_1MS);
}



// Function that displays a graphic character on the LCD,
// 'value' contains the value of its position in CGRAM, and
// 'size' specifies its size.
void LCD_BarGraphic( int16_t value, int16_t size )
{
	value = value * size / 20;				//	5x8 pixel matrix

	for( int16_t i = 0; i < size; i++ ){
		if( value > 5 )
		{
		    LCD_Put_Char( 0x05U );
		    value -= 5;
		}
		else
		{
		    LCD_Put_Char( value );
		    break;
		}
	}
}


// Function that displays a graphic character on the LCD specifying
//the starting horizontal position pos_x and the vertical position pos_y on the LCD screen.
void LCD_BarGraphicXY( int16_t pos_x, int16_t pos_y, int16_t value )
{
	LCD_Set_Cursor( pos_x, pos_y );
	for( int16_t i = 0; i < 16; i++ ){
		if( value > 5 )
		{
		    LCD_Put_Char( 0x05U );
		    value -= 5;
		}
		else
		{
		    LCD_Put_Char( value );

		    while( i++ < 16 )
			LCD_Put_Char( 0 );
		}
	}
}
