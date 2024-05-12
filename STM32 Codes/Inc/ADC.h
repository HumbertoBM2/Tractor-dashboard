// ADC.h

#ifndef ADC_H_
#define ADC_H_

#define ADC_CR1_DUALMOD		0xFUL  << 16U
#define ADC_CR2_CONT		0x1UL  << 1U
#define ADC_CR2_ALIGN		0x1UL  << 11U
#define ADC_SMPR2_SMP0		0x7UL  << 0U
#define ADC_SQR1_L		  	0xFUL  << 20U
#define ADC_SQR3_SQ1		0x1FUL << 0U
#define ADC_CR2_ADON		0x1UL  << 0U
#define ADC_CR2_CAL		  	0x1UL  << 2U
#define ADC_SR_EOC		  	0x1UL  << 1U

void USER_ADC1_Init( void );
uint16_t USER_ADC1_Convert( void );
float scaleVoltageValue( float, float, float );

#endif
