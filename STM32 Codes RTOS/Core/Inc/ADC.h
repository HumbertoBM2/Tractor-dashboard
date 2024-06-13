// ADC.h


#ifndef ADC_H_
#define ADC_H_

// Function prototypes
void USER_ADC1_Init( void ); // Initialize ADC1
uint16_t USER_ADC1_Convert( void ); // Function for signal conversion
float scaleVoltageValue( float, float, float ); // Function for voltage scaling

#endif
