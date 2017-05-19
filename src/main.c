/**
 ******************************************************************************
 *  FILE          : main.c
 *  PROJECT       : CNTR 8000 - Assignment #1
 *  PROGRAMMER    : Gabriel Yano & Artem Bordiuh
 *  REVIEW VERSION: 2017-05-18
 *  DESCRIPTION   : Implemented a new command for flashing LEDs in a specific
 *					pattern with delay and repetition times.
 *	IMPROVMENTS   :	Improve freezing of the terminal at the time some command is
 					executing by MCU. (Add some kind of job list for mcu and implement
					each command execution with posibility to read from CMD (interrupt))
					
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"
#include "stm32f3_discovery_accelerometer.h"
#include "stm32f3_discovery_gyroscope.h"

#include "common.h"


/* Private variables ---------------------------------------------------------*/
const Led_TypeDef LEDs[] = {LED3, LED4, LED5, LED6, LED7, LED8, LED9, LED10};
const uint32_t numLEDs = sizeof(LEDs)/sizeof(LEDs[0]);

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

int main(int argc, char **argv)
{
  uint32_t i;
  uint8_t accelRc, gyroRc;
  /* Configure the system clock */
  SystemClock_Config();

  HAL_Init();

  /* Start the Watchdog */


  TerminalInit();  /* Initialize UART and USB */
  /* Configure the LEDs... */
  for(i=0; i<numLEDs; i++) {
    BSP_LED_Init(LEDs[i]);
  }

  /* Initialize the pushbutton */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /* Initialize the Accelerometer */
  accelRc = BSP_ACCELERO_Init();
  if(accelRc != ACCELERO_OK) {
    printf("Failed to initialize acceleromter\n");
    Error_Handler();
  }

  /* Initialize the Gyroscope */
  gyroRc = BSP_GYRO_Init();
  if(gyroRc != GYRO_OK) {
    printf("Failed to initialize Gyroscope\n");
    Error_Handler();
  }

  my_Init();
  
  while(1) {
    TaskInput();
    my_Loop();
    /* Tickle the watchdog */
  }

  return 0;
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV                     = 1
  *            PLLMUL                         = RCC_PLL_MUL9 (9)
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    Error_Handler();
  }
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  BSP_LED_On(LED6);
  /* Infinite loop */
  while(1)
  {
  }
}

void SysTick_Handler(void)
{
    HAL_IncTick();
    my_Tick();
}

void CmdLED(int mode)
{
  uint32_t led,val;
  int rc;
  if(mode != CMD_INTERACTIVE) {
    return;
  }

  rc = fetch_uint32_arg(&led);
  if(rc) {
    printf("Missing LED index\n");
    return;
  }
    
  rc = fetch_uint32_arg(&val);
  if(rc) {
    printf("Missing state value, 0 for Off, 1 for On\n");
    return;
  }
  
  if((led < 3) || (led > 10)) {
    printf("Led index of %u is out of the range (3..10)\n",
	   (unsigned int)led);
    return;
  }

  led -= 3;
  if(val) {
    BSP_LED_On(LEDs[led]);
  } else {
    BSP_LED_Off(LEDs[led]);
  }

} 

ADD_CMD("led",CmdLED,"<index> <state> Turn off/on LED")

/**
  * @brief  Will flash LED pattern n times with specified delay
  *			Arguments for a console command are:
  *			First: Repetiton times
  *			Second: Pattern 1(downward flash) or 2(clockwise) or 3(counter clockwise)
  *			Third: Delay time, ms
  * @param  mode - variable for parser for a command line interface
  * @retval None
  */	
void CmdAllLed(int mode) {

	if (mode != CMD_INTERACTIVE) {
		return;
	}
	uint32_t repetition = 1;
	uint32_t pattern = 1;
	uint32_t delay = 50;
	
	//Parsing command line arguments (repetition, pattern, delay) and store it
	if (fetch_uint32_arg(&repetition)) {
		printf("Standard repetition times is 1\r\n");
	}
	if (fetch_uint32_arg(&pattern)) {
		printf("Standard pattern is 1\r\n");
	}
	if (fetch_uint32_arg(&delay)) {
		printf("Standard delay is 50ms\r\n");
	}
	
	//switching pattern type
	switch (pattern) {
		case 1: // Pattern flashing downwards
			for (int i = 0; i <= repetition; i++) {
				for (int j = 0; j < 8; j++) {
					BSP_LED_Toggle(LEDs[j]);
					HAL_Delay(delay);
				}
			}
			break;

		case 2: // Pattern flashing clockwise
			for (int i = 0; i <= repetition; i++) {
				uint32_t j = 0;
				int32_t countIndex = 2;
				while (j < 8 && j >= 0) {
					BSP_LED_Toggle(LEDs[j]);
					HAL_Delay(delay);

					if (j == 6) {
						j = 9;
						countIndex = -2;
					}

					j += countIndex;
				}
			}
			break;

		case 3: // Pattern flashing counter-clockwise
			for (int i = 0; i <= repetition; i++) {
				uint32_t j = 1;
				int32_t countIndex = 2;
				BSP_LED_Toggle(LEDs[0]);
				HAL_Delay(delay);
				while (j < 8 && j >= 0) {
					BSP_LED_Toggle(LEDs[j]);
					HAL_Delay(delay);

					if (j == 7) {
						j = 8;
						countIndex = -2;
					}

					j += countIndex;
				}
			}
			break;
		//if no correct pattern specified
		default: printf("No such a pattern\r\n");
	}
}

//Adding ledall command to flash specific pattern to a terminal
ADD_CMD("ledall",CmdAllLed,"<times> <pattern(1-3)> <delay> Turn on/off All Leds in sequence")

void CmdAccel(int mode)
{
  int16_t xyz[3];

  if(mode != CMD_INTERACTIVE) {
    return;
  }

  BSP_ACCELERO_GetXYZ(xyz);

  printf("Accelerometer returns:\n"
	 "   X: %d\n"
	 "   Y: %d\n"
	 "   Z: %d\n",
	 xyz[0],xyz[1],xyz[2]);


}

ADD_CMD("accel", CmdAccel,"                Read Accelerometer");

void CmdGyro(int mode)
{
  float xyz[3];

  if(mode != CMD_INTERACTIVE) {
    return;
  }

  BSP_GYRO_GetXYZ(xyz);

  printf("Gyroscope returns:\n"
	 "   X: %d\n"
	 "   Y: %d\n"
	 "   Z: %d\n",
	 (int)(xyz[0]*256),
	 (int)(xyz[1]*256),
	 (int)(xyz[2]*256));
}

ADD_CMD("gyro", CmdGyro,"                Read Gyroscope");

void CmdButton(int mode)
{
  uint32_t button;

  if(mode != CMD_INTERACTIVE) {
    return;
  }

  button = BSP_PB_GetState(BUTTON_USER);
  
  printf("Button is currently: %s\n",
	 button ? "Pressed" : "Released");

  return;
}

ADD_CMD("button", CmdButton,"                Print status of user button");
