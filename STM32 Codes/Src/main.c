// main.c

// Libraries and header files
#include "main.h"
#include "timer.h"
#include "ADC.h"
#include "uart.h"
#include "matrixkeyboard.h"
#include "lcd.h"
#include <math.h>


// Entity declarations

// Potentiometer
volatile uint16_t conversionData = 0;
volatile float voltageValue = 0, acelerador = 0;

// Matrix Keyboard
volatile char selectedKey;
volatile float freno;

// LCD
volatile uint8_t col = 16;


// Initialization Functions
void USER_RCC_ClockEnable( void );
void USER_GPIO_Init( void );


// Function to initialize clocks
void USER_RCC_ClockEnable( void ) {

  // Clock for Port A
  RCC->APB2ENR |= 	RCC_APB2ENR_IOPAEN;

  // Clock for Port B
  RCC->APB2ENR |= 	RCC_APB2ENR_IOPBEN;

  // Clock for port C
  RCC->APB2ENR	|=	RCC_APB2ENR_IOPCEN;

  // Clock for TIM2 (Timer)
  RCC->APB1ENR |= 	RCC_APB1ENR_TIM2EN;

  // Clock for the potentiometer
  RCC->APB2ENR	|=	RCC_APB2ENR_ADC1EN;
  RCC->CFGR	|=	RCC_CFGR_ADCPRE;

  // Clock for UART protocol
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;


  // Set system clock to 64 MHz
  FLASH->ACR	&=	~( 0x5UL << 0U );	
  FLASH->ACR	|=	( 0x2UL << 0U );	
  
  // PLL HSI clock /2 selected as PLL input clock
  RCC->CFGR	&=	~( 0x1UL << 16U )
			&	~( 0x7UL << 11U )
			&	~( 0x3UL << 8U )
			&	~( 0xFUL << 4U );

  // PLL input clock x 16 (PLLMUL bits)
  RCC->CFGR	|=	( 0xFUL << 18U )	
			|	( 0X4UL << 8U );
  RCC->CR	|=	( 0x1UL << 24U );

  // Wait until PLL is locked
  while( !(RCC->CR & ~( 0x1UL << 25U )));

  // PLL used as system clock (SW bits)
  RCC->CFGR	&=	~( 0x1UL << 0U );
  RCC->CFGR	|=	( 0x2UL << 0U );

  // Wait until PLL is switched
  while( 0x8UL != ( RCC->CFGR & 0xCUL ));

}


// Function to initialize the GPIOs
void USER_GPIO_Init( void ) {

  // Set PA0 As analog input
  GPIOA->CRL	&=	~( GPIO_CRL_MODE0 )
				&	~( GPIO_CRL_CNF0 );

  // Set PA9 as output push-pull
  GPIOA->CRH	&=	~( GPIO_CRH_CNF9_0 )
  				&	~( GPIO_CRH_MODE9 );
  GPIOA->CRH	|=	 ( GPIO_CRH_CNF9 )
  				|	 ( GPIO_CRH_MODE9_0 );
   		
  // Set PB10 as output
  GPIOB->CRH	&=	~( GPIO_CRH_CNF10 )
				&	~( GPIO_CRH_MODE10_0 );

  GPIOB->CRH 	|=	 ( GPIO_CRH_MODE10_1 );
  GPIOB->ODR 	|=	 ( GPIO_ODR_10 );

  // Set PB6 as output
  GPIOB->CRL	&=	~( GPIO_CRL_CNF6 )
				&	~( GPIO_CRL_MODE6_0 );

  GPIOB->CRL 	|=	 ( GPIO_CRL_MODE6_1 );
  GPIOB->ODR 	|=	 ( GPIO_ODR6 );

  // Set PB5 as output
  GPIOB->CRL	&=	~( GPIO_CRL_CNF5 )
				&	~( GPIO_CRL_MODE5_0 );
  GPIOB->CRL 	|=	 ( GPIO_CRL_MODE5_1 );
  GPIOB->ODR 	|=	 ( GPIO_ODR5 );

  // Set PB7 as output
  GPIOB->CRL	&=	~( GPIO_CRL_CNF7 )
				&	~( GPIO_CRL_MODE7_0 );

  GPIOB->CRL 	|=	 ( GPIO_CRL_MODE7_1 );
  GPIOB->ODR 	|=	 ( GPIO_ODR7 );

  // Set PB1 as input pull up
  GPIOB->CRL	&=	~( GPIO_CRL_MODE1 )
				&	~( GPIO_CRL_CNF1_0 );
  GPIOB->CRL	|= 	 ( GPIO_CRL_CNF1_1 );
  GPIOB->ODR	|=	 ( GPIO_ODR1 );

  // Set PB15 as input pull up
  GPIOB->CRH	&=	~( GPIO_CRH_MODE15 )
				&	~( GPIO_CRH_CNF15_0 );
  GPIOB->CRH	|= 	 ( GPIO_CRH_CNF15_1 );
  GPIOB->ODR	|=	 ( GPIO_ODR15 );

  // Set PB14 as input pull up
  GPIOB->CRH	&=	~( GPIO_CRH_MODE14 )
				&	~( GPIO_CRH_CNF14_0 );
  GPIOB->CRH	|= 	 ( GPIO_CRH_CNF14_1 );
  GPIOB->ODR	|=	 ( GPIO_ODR14 );

  // Set PB13 as input pull up
  GPIOB->CRH	&=	~( GPIO_CRH_MODE13 )
				&	~( GPIO_CRH_CNF13_0 );
  GPIOB->CRH	|= 	 ( GPIO_CRH_CNF13_1 );
  GPIOB->ODR	|=	 ( GPIO_ODR13 );
}





// Main function
int main( void ) {

  // Initialize clock, GPIOs, Timer, ADC, UART and the LCD
  USER_RCC_ClockEnable();
  USER_GPIO_Init();
  USER_TIM2_Init( );
  USER_ADC1_Init();
  USER_USART1_Init();
  LCD_Init();
  // Initialize Engine model
  EngTrModel_initialize();

  // Loop
  for(;;) {

	// Default tractor's direction
	char *dir = "Forward";

    // Potentiometer conversion cofig
    conversionData = USER_ADC1_Convert();
    voltageValue = 0.00080586 * conversionData;

    // Matrix Keyboard config
    selectedKey = USER_MATRIX_KEYBOARD_Read();

    // Check the pressed key in the Matrix Keyboard and set the
    // tractor's direction accordingly
    if(selectedKey == '5') {
		freno = 100.0;
		dir = "Break";
    }
    else if(selectedKey == '4') {
		voltageValue -= 1;
		dir = "Left";
    }
    else if(selectedKey == '6') {
		voltageValue -= 1;
		dir = "Right";
	}
    else {
		freno = 0.0;
    }

    // Define throttle's value
    acelerador = scaleVoltageValue(voltageValue, 0, 3.3);


    // Update the values for the Throttle and Brake commands
    EngTrModel_U.Throttle = acelerador;
    EngTrModel_U.BrakeTorque = freno;

    // Update the values into the vehicle model
    EngTrModel_step();

    // Print the output values
    printf("%f,%f,%f\n\r", EngTrModel_Y.EngineSpeed, EngTrModel_Y.VehicleSpeed, EngTrModel_Y.Gear);
    // implement a 200 ms delay
    USER_TIM2_Delay(TIM_PSC_200MS, TIM_CNT_200MS);

    // Format the output data for the LCD
    LCD_Set_Cursor( 1, 1 );
    LCD_Put_Str( dir);
    LCD_Put_Str( " m:");
    LCD_Put_Num( EngTrModel_Y.Gear );
    LCD_Put_Str( "      ");
    LCD_Set_Cursor( 2, 0 );
    LCD_Put_Str( "car:");
    LCD_Put_Num( trunc(EngTrModel_Y.VehicleSpeed) );
    LCD_Put_Str( " vel:");
    LCD_Put_Num( trunc(EngTrModel_Y.EngineSpeed) );
    LCD_Put_Str( "    ");
    LCD_Set_Cursor( 2, col-- );
    LCD_BarGraphic( 0, 64 );
    // Implement a 200 ms delay
    USER_TIM2_Delay(TIM_PSC_200MS, TIM_CNT_200MS);
    // Implement a 500 ms delay
    if( col == 0 ){
		USER_TIM2_Delay(TIM_PSC_500MS, TIM_CNT_500MS);
		 col = 16;
    }

  }

}



