#include <stdio.h>
#include <string.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"

#include "common.h"


void CMD_GPIO_INIT(int mode) {
    
    if (mode != CMD_INTERACTIVE) {
        return;
    }
    
    uint32_t port = 0;
    uint32_t pin = 0;
    
    if (fetch_uint32_arg(&port)) {
        printf("No Port specified\r\n");
    }
    if (fetch_uint32_arg(&pin)) {
        printf("No Pin specified\r\n");
    }
    
	printf("The port is %d", (int)port);

    GPIO_InitTypeDef GPIO_InitStruct;
	/*Configure GPIO pin : */
	switch(pin)
	{
		case 0: GPIO_InitStruct.Pin = GPIO_PIN_0; break;
		case 1: GPIO_InitStruct.Pin = GPIO_PIN_1; break;
		case 2: GPIO_InitStruct.Pin = GPIO_PIN_2; break;
		case 3: GPIO_InitStruct.Pin = GPIO_PIN_3; break;
		case 4: GPIO_InitStruct.Pin = GPIO_PIN_4; break;
		case 5: GPIO_InitStruct.Pin = GPIO_PIN_5; break;
		case 6: GPIO_InitStruct.Pin = GPIO_PIN_6; break;
		case 7: GPIO_InitStruct.Pin = GPIO_PIN_7; break;
		case 8: GPIO_InitStruct.Pin = GPIO_PIN_8; break;
		case 9: GPIO_InitStruct.Pin = GPIO_PIN_9; break;
		case 10: GPIO_InitStruct.Pin = GPIO_PIN_10; break;
		case 11: GPIO_InitStruct.Pin = GPIO_PIN_11; break;
		case 12: GPIO_InitStruct.Pin = GPIO_PIN_12; break;
		case 13: GPIO_InitStruct.Pin = GPIO_PIN_13; break;
		case 14: GPIO_InitStruct.Pin = GPIO_PIN_14; break;
		case 15: GPIO_InitStruct.Pin = GPIO_PIN_15; break;
            default: printf("No such a pin! Must be 0 - 15");
	}
        
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = 0;
        
	switch (port)
	{
                case 'A': __GPIOA_CLK_ENABLE(); HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); break;
                case 'B': __GPIOB_CLK_ENABLE(); HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); break;
                case 'C': __GPIOC_CLK_ENABLE(); HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); break;
                case 'D': __GPIOD_CLK_ENABLE(); HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); break;
                case 'E': __GPIOE_CLK_ENABLE(); HAL_GPIO_Init(GPIOE, &GPIO_InitStruct); break;
            default: printf("No such a pin! Must be A to E");
	}
}

ADD_CMD("gpioinit", CMD_GPIO_INIT, "<times> <pattern(1-3)> <delay> Turn on/off All Leds in sequence")

void CMD_GPIO_TOGGLE(int mode) {
    if (mode != CMD_INTERACTIVE) {
        return;
    }
    
    uint32_t gpioPort = 0;
    uint32_t gpioPin = 0;
    uint32_t gpioStatus = 0;
    
    if (fetch_uint32_arg(&gpioPort)) {
        printf("No Port specified\r\n");
    }
    if (fetch_uint32_arg(&gpioPin)) {
        printf("No Pin specified\r\n");
    }
    if (fetch_uint32_arg(&gpioStatus)) {
        printf("No Pin specified\r\n");
    }
    
    uint16_t pin = 0;
	/*Configure GPIO pin : */
	switch(gpioPin)
	{
		case 0: pin = GPIO_PIN_0; break;
		case 1: pin = GPIO_PIN_1; break;
		case 2: pin = GPIO_PIN_2; break;
		case 3: pin = GPIO_PIN_3; break;
		case 4: pin = GPIO_PIN_4; break;
		case 5: pin = GPIO_PIN_5; break;
		case 6: pin = GPIO_PIN_6; break;
		case 7: pin = GPIO_PIN_7; break;
		case 8: pin = GPIO_PIN_8; break;
		case 9: pin = GPIO_PIN_9; break;
		case 10: pin = GPIO_PIN_10; break;
		case 11: pin = GPIO_PIN_11; break;
		case 12: pin = GPIO_PIN_12; break;
		case 13: pin = GPIO_PIN_13; break;
		case 14: pin = GPIO_PIN_14; break;
		case 15: pin= GPIO_PIN_15; break;
            default: printf("No such a pin! Must be 0 - 15");
	}
        
        GPIO_TypeDef* port = GPIOE;
        
	switch (gpioPort)
	{
                case 'A': port = GPIOA; break;
                case 'B': port = GPIOB; break;
                case 'C': port = GPIOC; break;
                case 'D': port = GPIOD; break;
                case 'E': port = GPIOE; break;
            default: printf("No such a pin! Must be A to E");
	}
        printf("The port is %d", (int)port);
        
        GPIO_PinState status = GPIO_PIN_RESET;
        
        switch(gpioStatus) {
            case 0 : status = GPIO_PIN_RESET; break;
            case 1 : status = GPIO_PIN_SET; break;
        }
        
    HAL_GPIO_WritePin(port, pin, status);
}
//Adding ledall command to flash specific pattern to a terminal

ADD_CMD("gpio", CMD_GPIO_TOGGLE, "<times> <pattern(1-3)> <delay> Turn on/off All Leds in sequence")
