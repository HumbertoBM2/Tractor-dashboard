/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "timer.h"
#include "ADC.h"
#include "uart.h"
#include "matrixkeyboard.h"
#include "lcd.h"
#include <math.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
	float throttleP;
	char operationFlag;
	uint32_t source;
} Data1;

typedef struct {
	float engine;
	float tractor;
	float gear;
	float throttleM;
	uint32_t source;
} Data2;

typedef struct{
	char* direction;
	uint32_t source;
} Data3;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* USER CODE BEGIN PV */
// Tasks
osThreadId Task1Handle;
osThreadId Task2Handle;
osThreadId Task3Handle;
osThreadId Task4Handle;
// Queues
osMessageQId Queue1Handle;
osMessageQId Queue2Handle;
osMessageQId Queue3Handle;
// Mutexes
osMutexId MutexUsartHandle;
osMutexId MutexPrintHandle;
// Variables
volatile uint8_t col = 16;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartDefaultTask(void const * argument);


/* USER CODE BEGIN PFP */
// Initialization functions
static void Task1_Init(void);
static void Task2_Init(void);
static void Task3_Init(void);
// Task functions
void StartTask1(void const * argument);
void StartTask2(void const * argument);
void StartTask3(void const * argument);
void StartTask4(void const * argument);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  Task1_Init();
  Task2_Init();
  Task3_Init();

  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  osMutexDef(MutexUsart);
  MutexUsartHandle = osMutexCreate(osMutex(MutexUsart));
  osMutexDef(MutexPrint);
  MutexPrintHandle = osMutexCreate(osMutex(MutexPrint));
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */

  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  osMessageQDef(Queue1, 30, Data1);
  Queue1Handle = osMessageCreate(osMessageQ(Queue1), 1);
  osMessageQDef(Queue2, 30, Data2);
  Queue2Handle = osMessageCreate(osMessageQ(Queue2), 1);
  osMessageQDef(Queue3, 30, Data3);
  Queue3Handle = osMessageCreate(osMessageQ(Queue3), 1);


  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityIdle, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  osThreadDef(Task2, StartTask2, osPriorityHigh, 0, 384);
  Task2Handle = osThreadCreate(osThread(Task2), NULL);

  osThreadDef(Task4, StartTask4, osPriorityNormal, 0, 384);
  Task4Handle = osThreadCreate(osThread(Task4), NULL);

  osThreadDef(Task3, StartTask3, osPriorityAboveNormal, 0, 384);
  Task3Handle = osThreadCreate(osThread(Task3), NULL);

  osThreadDef(Task1, StartTask1, osPriorityRealtime, 0, 384);
  Task1Handle = osThreadCreate(osThread(Task1), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}


/* USER CODE BEGIN 4 */

// Task to initialize the MCU's clock, the clock for Port A, GPIOs, USART1, TIM2 and ADC1
static void Task1_Init (void)
{
	// Initialize system's clock
	// PLL HSI clock /2 selected as PLL input clock
	RCC->CFGR	&=	~( 0x1UL << 16U )
				&	~( 0x7UL << 11U )
				&	~( 0x3UL << 8U )
				&	~( 0xFUL << 4U );
	// PLL input clock x 16 (PLLMUL bits)
	RCC->CFGR	|=	( 0xFUL << 18U )
				|	( 0X4UL << 8U );
	RCC->CR		|=	( 0x1UL << 24U );
	// Wait until PLL is locked
	while( !(RCC->CR & ~( 0x1UL << 25U )));
	// PLL used as system clock (SW bits)
	RCC->CFGR	&=	~( 0x1UL << 0U );
	RCC->CFGR	|=	( 0x2UL << 0U );
	// Wait until PLL is switched
	while( 0x8UL != ( RCC->CFGR & 0xCUL ));
	// Initialize clock for ADC1
	RCC->APB2ENR	|=	RCC_APB2ENR_ADC1EN;
	RCC->CFGR		|=	RCC_CFGR_ADCPRE;
	// Initialize clock for TIM2
	RCC->APB1ENR |= 	RCC_APB1ENR_TIM2EN;
	// Initialize clock for Port A
	RCC->APB2ENR |= 	RCC_APB2ENR_IOPAEN;

	// Pin PA0 as analog input
	GPIOA->CRL	&=	~( GPIO_CRL_MODE0 )
	   			&	~( GPIO_CRL_CNF0 );
	// Initialize TIM2
	USER_TIM2_Init( );
	//Initialize ADC
	USER_ADC1_Init();
	// Initialize clock for USART1
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	// Pin PA9 (USART1_TX) as aoutput push-pull
	GPIOA->CRH	&=	~( GPIO_CRH_CNF9_0 )
	  	    	&	~( GPIO_CRH_MODE9 );
	GPIOA->CRH	|=	 ( GPIO_CRH_CNF9 )
	  	    	|	 ( GPIO_CRH_MODE9_0 );
	// Pin PA10 (USART1_RX) as input floating
	GPIOA->CRH &= ~(GPIO_CRH_CNF10_0) & ~(GPIO_CRH_MODE10);
	GPIOA->CRH |= GPIO_CRH_CNF10_1;
	// Configure built-in user LED PA5 as output
	GPIOA->BSRR = ( 0x1UL << 21U );
	GPIOA->CRL &= ~( 0x3UL << 22U )
	  	       & ~( 0x2UL << 20U );
	GPIOA->CRL |= ( 0x1UL << 20U );
	//Initialize USART1
	USER_USART1_Init();
}

// Task to initialize clock for Port B, GPIOs and the tractor's model
static void Task2_Init(void)
{
	// Initialize clock for Port B
	RCC->APB2ENR |= 	RCC_APB2ENR_IOPBEN;

	// Initialize pins for matrix keypad
	// Pin PB10 (Row 1) as output
	GPIOB->CRH	&=	~( GPIO_CRH_CNF10 )
	  			&	~( GPIO_CRH_MODE10_0 );
	GPIOB->CRH 	|=	 ( GPIO_CRH_MODE10_1 );
	GPIOB->ODR 	|=	 ( GPIO_ODR_10 );

	// Pin PB6 (Row 2) as output
	GPIOB->CRL	&=	~( GPIO_CRL_CNF6 )
	  			&	~( GPIO_CRL_MODE6_0 );
	GPIOB->CRL 	|=	 ( GPIO_CRL_MODE6_1 );
	GPIOB->ODR 	|=	 ( GPIO_ODR6 );

	// Pin PB5 (Row 3) as output
	GPIOB->CRL	&=	~( GPIO_CRL_CNF5 )
	  			&	~( GPIO_CRL_MODE5_0 );
	GPIOB->CRL 	|=	 ( GPIO_CRL_MODE5_1 );
	GPIOB->ODR 	|=	 ( GPIO_ODR5 );

	// Pin PB7 (Row 4) as output
	GPIOB->CRL	&=	~( GPIO_CRL_CNF7 )
	  			&	~( GPIO_CRL_MODE7_0 );
	GPIOB->CRL 	|=	 ( GPIO_CRL_MODE7_1 );
	GPIOB->ODR 	|=	 ( GPIO_ODR7 );

	// Pin PB1 (Col 1) as input pull up
	GPIOB->CRL	&=	~( GPIO_CRL_MODE1 )
	  			&	~( GPIO_CRL_CNF1_0 );
	GPIOB->CRL	|= 	 ( GPIO_CRL_CNF1_1 );
	GPIOB->ODR	|=	 ( GPIO_ODR1 );

	// Pin PB15 (Col 2) as input pull up
	GPIOB->CRH	&=	~( GPIO_CRH_MODE15 )
	  			&	~( GPIO_CRH_CNF15_0 );
	GPIOB->CRH	|= 	 ( GPIO_CRH_CNF15_1 );
	GPIOB->ODR	|=	 ( GPIO_ODR15 );

	// Pin PB14 (Col 3) as input pull up
	GPIOB->CRH	&=	~( GPIO_CRH_MODE14 )
	  			&	~( GPIO_CRH_CNF14_0 );
	GPIOB->CRH	|= 	 ( GPIO_CRH_CNF14_1 );
	GPIOB->ODR	|=	 ( GPIO_ODR14 );

	// Pin PB13 (Col 4) as input pull up
	GPIOB->CRH	&=	~( GPIO_CRH_MODE13 )
	  			&	~( GPIO_CRH_CNF13_0 );
	GPIOB->CRH	|= 	 ( GPIO_CRH_CNF13_1 );
	GPIOB->ODR	|=	 ( GPIO_ODR13 );

	// Initialize the tractor's model
	EngTrModel_initialize();

}


// Task to initialize Port C clock, GPIOs and the LCD
static void Task3_Init(void)
{
	// Initialize clock for Port C
	RCC->APB2ENR	|=	RCC_APB2ENR_IOPCEN;
	RCC->APB2ENR	|=	RCC_APB2ENR_IOPCEN;
	// Configure PC2 as output (left blinker)
	GPIOC->BSRR = (0x1UL << 9U);
	GPIOC->CRL &= ~(0x3UL << 10U)
			   & ~(0x2UL << 8U);
	GPIOC->CRL |= (0x1UL <<8U);
	// Configure PC3 as output (right blinker)
	GPIOC->BSRR = (0x1UL << 13U);
	GPIOC->CRL &= ~(0x3UL << 14U)
		  	   & ~(0x2UL << 12U);
	GPIOC->CRL |= (0x1UL <<12U);
	// Initialize the LCD
	LCD_Init();
}


// Task to read the ADC signal and calculate throttle's value
void StartTask1(void const * argument)
{
	uint16_t conversionData = 0;
	float voltageValue = 0;
	float acelerador = 0;
	Data1 messageForT2;
	char recibido='n';
	messageForT2.source=0x1000;
	for(;;)
	{
		osMutexWait(MutexUsartHandle, osWaitForever);
		conversionData = USER_ADC1_Convert(); // Read ADC's signal
		voltageValue = 0.00080586 * conversionData; // Make the conversion from analog to digital
		acelerador = scaleVoltageValue(voltageValue, 0, 3.3); // Calculate throttle's value
		recibido=USER_USART1_Receive(); // Evaluate data from the RPi module
		if (recibido == 'p') // If p received, add one to the current throttle value
		{
			acelerador = acelerador + 1.0;
		}
		else if (recibido == 'm') // If m received, susbstract one to the current throttle value
		{
			acelerador = acelerador - 1.0;
		}
		else
		{
			acelerador = acelerador; // If any other character is received, do not modify the throttle
		}
		messageForT2.throttleP=acelerador;
		messageForT2.operationFlag=recibido;
		// Put throttle's value and the received data from the RPi in a queue for Task2
		osMessagePut(Queue1Handle, (uint32_t)&messageForT2, 1);
		osMutexRelease(MutexUsartHandle);
	    osDelay(100);
	}
}

// Task to determine the tractor's direction based on the matrix keypad input
// This task also manages the tractor's model by using the generated output values from Task1
void StartTask2(void const * argument)
{
	char selectedKey;
	float freno;
	float acelerador;
	char* dir;
	char flag;
	osEvent messageFromT1;
	Data2 messageForT3;
	messageForT3.source=0x2001;
	Data3 messageForT4;
	messageForT4.source=0x2002;
	for(;;)
	{
		selectedKey = USER_MATRIX_KEYBOARD_Read();
	    messageFromT1 = osMessageGet(Queue1Handle, 200); // Get the throttle's value from T1
	    acelerador=((Data1*)messageFromT1.value.p)->throttleP;
	    flag=((Data1*)messageFromT1.value.p)->operationFlag;
	    dir = "^"; // Set the tractor's default direction to forward
	    if(GPIOA->ODR & ( 0x1UL << 5U )) // Ensure all blinkers and stop lights are off
	    {
	    	GPIOA->ODR ^= ( 0x1UL << 5U );
	    }
	    if(GPIOC->ODR & ( 0x1UL << 2U )){
	    	GPIOC->ODR ^= ( 0x1UL << 2U );
	    }
	    if(GPIOC->ODR & ( 0x1UL << 3U )){
	    	GPIOC->ODR ^= ( 0x1UL << 3U );
	    }
	    if(flag=='b') // If b was received set break value to 5000
	    {
	    	freno = 5000.0;
	    	acelerador=acelerador/1000;
	    }
	    else if(selectedKey == '5' ) // If key 5 is pressed, set tractor's direction to break and turn on PA5
	    {
			freno = 5000.0;
			GPIOA->ODR ^= ( 0x1UL << 5U );
			acelerador=acelerador/2;
			dir = "*";
	    }
	    else if((selectedKey == '4') ) // If key 4 is pressed, set tractor's direction to left and turn on PC2
	    {
			dir = "<";
			GPIOC->ODR ^= ( 0x1UL << 2U );
	    }
	    else if((selectedKey == '6')) // If key 6 is pressed, set tractor's direction to right and turn on PC3
		{
			dir = ">";
			GPIOC->ODR ^= ( 0x1UL << 3U );
		}
	    else // If no key is pressed, reset tractor's direction and break value
	    {
			freno = 0.0;
			dir = "^";
	    }
	    EngTrModel_U.Throttle = acelerador;
	    EngTrModel_U.BrakeTorque = freno;

	    // Update the values into the vehicle model
	    EngTrModel_step();
	    // Put output values from the model into queues for tasks 3 and 4
	    messageForT3.engine=EngTrModel_Y.EngineSpeed;
	    messageForT3.tractor= EngTrModel_Y.VehicleSpeed;
	    messageForT3.gear=EngTrModel_Y.Gear;
	    messageForT3.throttleM=EngTrModel_U.Throttle;
	    messageForT4.direction=dir;
		osMessagePut(Queue2Handle, (uint32_t)&messageForT3, 250);
		osMessagePut(Queue3Handle, (uint32_t)&messageForT4, 250);
		HAL_Delay(2);
		osDelay(200);

	}
}

// Task to send the tractor's data via USART protocol
void StartTask3(void const * argument)
{
	osEvent messageFromT2;
	for(;;){
		osMutexWait(MutexUsartHandle, osWaitForever);
		messageFromT2 = osMessageGet(Queue2Handle, 300); // Grabs the output data from the model and sends it
		printf("%.1f,%.1f,%.1f,%.1f\n\r", ((Data2*)messageFromT2.value.p)->engine, ((Data2*)messageFromT2.value.p)->tractor,((Data2*)messageFromT2.value.p)->gear, ((Data2*)messageFromT2.value.p)->throttleM);
		osMutexRelease(MutexUsartHandle);
	    HAL_Delay(53);
	    osDelay(600);
	}
}

// Task to display the tractor's info on the LCD
// The first row will display thedirection, gear and throttle
// The second row will display the vehicle speed and the engine speed
void StartTask4(void const * argument)
{
	osEvent messageFromT2;
	char* dir;
	for(;;){
		messageFromT2 = osMessageGet(Queue3Handle, 300); // Grabs the direction value from the queue
		dir=((Data3*)messageFromT2.value.p)->direction;
		osMutexWait(MutexPrintHandle, osWaitForever);
		LCD_Set_Cursor( 1, 1 );
		LCD_Put_Str( "  ");
	    LCD_Put_Str( dir); // display direction
	    LCD_Put_Str( "  G:");
	    LCD_Put_Num( EngTrModel_Y.Gear ); // display gear
	    LCD_Put_Str( "  T:");
	    LCD_Put_Num( trunc(EngTrModel_U.Throttle) ); // display throttle
	    LCD_Put_Str( "  ");
	    LCD_Set_Cursor( 2, 0 );
	    LCD_Put_Num( EngTrModel_Y.VehicleSpeed ); // display vehicle speed
	    LCD_Put_Str( " km/h "); // display units
	    LCD_Put_Num( trunc(EngTrModel_Y.EngineSpeed) ); // diplay engine speed
	    LCD_Put_Str( " RPM"); // display units
	    LCD_Set_Cursor( 2, col-- );
	    osMutexRelease(MutexPrintHandle);
	    if( col == 0 ){
			 col = 16;
	   }
	    osDelay(600);
	}

}
/* USER CODE END 4 */


/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
