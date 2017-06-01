/******************************************************************************  
 *  FILE          : daccmd.c 
 *  PROJECT       : CNTR8000 - LAB #4 ADC 
 *  PROGRAMMER    : Gabriel Yano, Artem Bordiuh
 *  FIRST VERSION : 2017-05-26
 *  DESCRIPTION   : 
 *    This file adds two more commands to the simple monitor program.
 *    The first command initializes the DAC peripheral that is taken as argument
 *    to the function.
 *    The second command sets the DAC value and prints the result to the desired 
 *    pin, the arguments taken are the channel and the pin.
 ******************************************************************************/

/* Includes ******************************************************************/
#include "daccmd.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"
#include "common.h"

/*If Board not specified*/
#ifndef STM32F303xC
#define STM32F303xC
#endif

/*ADC and GPIO HAL libs*/
#include "stm32f3xx_hal_gpio.h"
#include "stm32f3xx_hal_dac.h"
#include "stm32f3xx_hal_dac_ex.h"

#include "daccmd.h"

/* DAC global Handler declaration **********************************************/
static DAC_HandleTypeDef hdac;

void CMD_DAC_INIT(int mode) {

    if (mode != CMD_INTERACTIVE) {
        return;
    }
    //channel may be 1 or 2
    uint32_t channel = 1;

    if (fetch_uint32_arg(&channel)) {
        printf("No channel specified. Need to be 1 or 2. Default 1\r\n");
    }
	
	/*Enabling clock for GPIOA and DAC1 peripheral*/
	__GPIOA_CLK_ENABLE();
    __DAC1_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    /*Configure GPIO pin : */
    switch (channel) {
        case 1: GPIO_InitStruct.Pin = GPIO_PIN_4;
            channel = DAC_CHANNEL_1;
            break;
        case 2: GPIO_InitStruct.Pin = GPIO_PIN_5;
            channel = DAC_CHANNEL_2;
            break;
        default: GPIO_InitStruct.Pin = GPIO_PIN_4;
            channel = DAC_CHANNEL_1;
            break;
    }

    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 0;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Initialize DAC */
    HAL_StatusTypeDef rc;
    hdac.Instance = DAC1;
    rc = HAL_DAC_Init(&hdac);
    if (rc != HAL_OK) {
        printf("Unable to initialize DAC1, rc=%u\r\n", (unsigned int)rc);
        return;
    }
}


void CMD_DAC_ONOF(int mode) {
    if (mode != CMD_INTERACTIVE) {
        return;
    }
    //channel may be 1 or 2
    uint32_t channel = 1;
    //dacFormat: 1 - normal value, 2 - saw waveform, 3 - sinusoidal waveform
    uint32_t dacFormat = 0;
    //dacValue used to set the amplitude of output signal
    uint32_t dacValue = 0;
    //status 0 - off, 1 - on
    uint32_t status = 1;

	//Fetching all valuest from a terminal
    if (fetch_uint32_arg(&channel)) {
        printf("No Channel specified. Need to be 1 or 2. Default 1\r\n");
    }
    if (fetch_uint32_arg(&dacFormat)) {
        printf("No Pattern specified. Need to be 1 or 2. Default 1\r\n");
    }
    if (fetch_uint32_arg(&dacValue)) {
        printf("No Value specified. Need to be 0 or 4095. Default 1\r\n");
    }
    if (fetch_uint32_arg(&status)) {
        printf("No Status specified. Need to be 0 or 1. Default 0\r\n");

    }

    /*Configure GPIO pin : */
    switch (channel) {
        case 1:channel = DAC_CHANNEL_1;
            break;
        case 2: channel = DAC_CHANNEL_2;
            break;
        default: channel = DAC_CHANNEL_1;
            break;
    }
	
    HAL_StatusTypeDef rc;
	
    DAC_ChannelConfTypeDef dacConfigStruct;
    dacConfigStruct.DAC_Trigger = DAC_TRIGGER_NONE;
    dacConfigStruct.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	
    rc = HAL_DAC_ConfigChannel(&hdac, &dacConfigStruct, channel);
    if (rc != HAL_OK) {
        printf("Unable to configure DAC channel %u, rc=%u\n", (unsigned int)channel, (unsigned int)rc);
        return;
    }

    /* Turn on DAC */
    if (status == 1) {
        __HAL_DAC_ENABLE(&hdac, channel);
        /* Switching patterns 1 - linear, 2 - saw teeth  output*/
        switch (dacFormat) {
            case 1: rc = HAL_DAC_SetValue(&hdac, channel, DAC_ALIGN_12B_R, dacValue);
                if (rc != HAL_OK) {
                    printf("Unable to set value on DAC1 channel %u, rc=%u \r\n", (unsigned int)channel, (unsigned int)rc);
                    return;
                }
                printf("DAC linear output enabled on DAC1 Channel %d\r\n", (unsigned int)channel);
                break;
            case 2: rc = HAL_DACEx_TriangleWaveGenerate(&hdac, channel, DAC_TRIANGLEAMPLITUDE_4095);
                if (rc != HAL_OK) {
                    printf("Unable to generate triangle on DAC1 channel %u, rc=%u\n", (unsigned int)channel, (unsigned int)rc);
                    return;
                }
                printf("DAC triangle output enabled on DAC1 Channel %d\r\n", (unsigned int)channel);
 				break;
            case 3: break;
        }
    } else {
        __HAL_DAC_DISABLE(&hdac, channel);
        printf("DAC disabled on DAC1 channel %u\r\n", (unsigned int)channel);
    }
}



