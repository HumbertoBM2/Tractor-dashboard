// timer.h


#ifndef TIMER_H_
#define TIMER_H_

// Prescaler and initial count values for different timer via TIM2

// 500 ms
#define TIM_PSC_500MS	0X1E8
#define TIM_CNT_500MS	0X60

// 200 ms
#define TIM_PSC_200MS	0xC3
#define TIM_CNT_200MS	0xE6

// 50 ms
#define TIM_PSC_50MS	0X30
#define TIM_CNT_50MS	0XE6

// 5 ms
#define TIM_PSC_5MS	0X4
#define TIM_CNT_5MS	0X600

// 1 ms
#define TIM_PSC_1MS	0X0
#define TIM_CNT_1MS	0X600

// 10 us
#define TIM_PSC_10US	0X0
#define TIM_CNT_10US	0XFD80

// 100 us
#define TIM_PSC_100US	0X0
#define TIM_CNT_100US	0XE700

// Function prototypes
void USER_TIM2_Init( void );			    // Initialize TIM2 timer
void USER_TIM2_Delay( uint16_t, uint16_t );	// Function to generate delay via TIM2 timer

#endif
