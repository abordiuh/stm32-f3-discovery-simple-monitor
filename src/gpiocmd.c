/**
 ******************************************************************************

 *  FILE          : gpiocmd.c
 *  PROJECT       : Hardware and Software Interfacing - Assignment  #2
 *  PROGRAMMER    : Artem Bordiuh & Gabriel Yano
 *  FIRST VERSION : 05-25-207
 *  DESCRIPTION   : The gpiocmd.c file will add two new command to the terminal
                    software of STM32 F3 Discovery. The commands will give user
                    possibility to deal with GPIO.

 ******************************************************************************
 **/

#include <stdio.h>
#include <string.h>

//STM32 Includes
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"

//User includes
#include "common.h"

/*
 *@brief The function will initialize the GPIO specified PIN and PORT
 *	as an output. User need to set port as an char A-E and pin as
 *	1-15. 
 *@param mode (the int variable for parser for a command line interface)
 *@return none
 */
void CMD_GPIO_INIT(int mode) {
    //Exit the function if mode is not CMD_INTERACTIVE	    
    if (mode != CMD_INTERACTIVE) {
        return;
    }

    //Variables to store pin and port of GPIO sent by User when calling command
    char * port = 0;
    uint32_t pin = 0;
    if (fetch_string_arg(&port)) {
        printf("No Port specified\r\n");
    }
    if (fetch_uint32_arg(&pin)) {
        printf("No Pin specified\r\n");
    }

    /*
     * Switching port and pin to fill a strucnture GPIO_InitStruct
     * Function allows to use differnet families of STM32 since GPIO_InitStruct
     * is general for different famelies
     */
    GPIO_InitTypeDef GPIO_InitStruct;
    /*Configure GPIO pin : */
    switch (pin) {
        case 0: GPIO_InitStruct.Pin = GPIO_PIN_0;
            break;
        case 1: GPIO_InitStruct.Pin = GPIO_PIN_1;
            break;
        case 2: GPIO_InitStruct.Pin = GPIO_PIN_2;
            break;
        case 3: GPIO_InitStruct.Pin = GPIO_PIN_3;
            break;
        case 4: GPIO_InitStruct.Pin = GPIO_PIN_4;
            break;
        case 5: GPIO_InitStruct.Pin = GPIO_PIN_5;
            break;
        case 6: GPIO_InitStruct.Pin = GPIO_PIN_6;
            break;
        case 7: GPIO_InitStruct.Pin = GPIO_PIN_7;
            break;
        case 8: GPIO_InitStruct.Pin = GPIO_PIN_8;
            break;
        case 9: GPIO_InitStruct.Pin = GPIO_PIN_9;
            break;
        case 10: GPIO_InitStruct.Pin = GPIO_PIN_10;
            break;
        case 11: GPIO_InitStruct.Pin = GPIO_PIN_11;
            break;
        case 12: GPIO_InitStruct.Pin = GPIO_PIN_12;
            break;
        case 13: GPIO_InitStruct.Pin = GPIO_PIN_13;
            break;
        case 14: GPIO_InitStruct.Pin = GPIO_PIN_14;
            break;
        case 15: GPIO_InitStruct.Pin = GPIO_PIN_15;
            break;
        default: printf("No such a pin! Must be 0 - 15");
    }
    /*Configure GPIO params*/
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 0;
    /*Configure GPIO port*/
    switch (*port) {
        case 65: __GPIOA_CLK_ENABLE();
            HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            break; //65 - port A
        case 66: __GPIOB_CLK_ENABLE();
            HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            break; //66 - port B
        case 67: __GPIOC_CLK_ENABLE();
            HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
            break; //67 - port C
        case 68: __GPIOD_CLK_ENABLE();
            HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
            break; //68 - port D
        case 69: __GPIOE_CLK_ENABLE();
            HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
            break; //69 - port E
        default: printf("No such a port! Must be A to E");
    }
}

/*
 *@brief The function will write high of low to the GPIO specified PIN and PORT
 *	User need to set port as an char A-E and pin as 1-15, also status as 1 or 0;
 * 
 *@param mode (the int variable for parser for a command line interface)
 *@return none
 */
void CMD_GPIO_TOGGLE(int mode) {
    if (mode != CMD_INTERACTIVE) {
        return;
    }
    //Variables to store status, pin and port of GPIO sent by User when calling command  
    uint32_t gpioPin = 0;
    uint32_t gpioStatus = 0;
    char * gpioPort = 0;
    if (fetch_string_arg(&gpioPort)) {
        printf("No Port specified\r\n");
    }
    if (fetch_uint32_arg(&gpioPin)) {
        printf("No Pin specified\r\n");
    }
    if (fetch_uint32_arg(&gpioStatus)) {
        printf("No Pin specified\r\n");
    }

    /*
     * Switching port and pin to fill a strucnture GPIO_InitStruct
     * Function allows to use differnet families of STM32 since GPIO_InitStruct
     * is general for different famelies
     */
    uint16_t pin = 0;
    /*Configure GPIO pin : */
    switch (gpioPin) {
        case 0: pin = GPIO_PIN_0;
            break;
        case 1: pin = GPIO_PIN_1;
            break;
        case 2: pin = GPIO_PIN_2;
            break;
        case 3: pin = GPIO_PIN_3;
            break;
        case 4: pin = GPIO_PIN_4;
            break;
        case 5: pin = GPIO_PIN_5;
            break;
        case 6: pin = GPIO_PIN_6;
            break;
        case 7: pin = GPIO_PIN_7;
            break;
        case 8: pin = GPIO_PIN_8;
            break;
        case 9: pin = GPIO_PIN_9;
            break;
        case 10: pin = GPIO_PIN_10;
            break;
        case 11: pin = GPIO_PIN_11;
            break;
        case 12: pin = GPIO_PIN_12;
            break;
        case 13: pin = GPIO_PIN_13;
            break;
        case 14: pin = GPIO_PIN_14;
            break;
        case 15: pin = GPIO_PIN_15;
            break;
        default: printf("No such a pin! Must be 0 - 15");
    }

    GPIO_TypeDef* port = GPIOE;

    switch (*gpioPort) {
        case 65: port = GPIOA;
            break;
        case 66: port = GPIOB;
            break;
        case 67: port = GPIOC;
            break;
        case 68: port = GPIOD;
            break;
        case 69: port = GPIOE;
            break;
        default: printf("No such a pin! Must be A to E");
    }

    GPIO_PinState status = 0;

    //Switching status and writing to a Pin High or Low        
    switch (gpioStatus) {
        case 0: status = 0;
            break;
        case 1: status = 1;
            break;
    }

    HAL_GPIO_WritePin(port, pin, status);
}


