/**
 ******************************************************************************

 *  FILE          : gpiocmd.h
 *  PROJECT       : Hardware and Software Interfacing - Assignment  #2
 *  PROGRAMMER    : Artem Bordiuh & Gabriel Yano
 *  FIRST VERSION : 05-25-207
 *  DESCRIPTION   : The gpiocmd.h file will add two new command to the terminal
                    software of STM32 F3 Discovery. The commands will give user
                    possibility to deal with GPIO.

 ******************************************************************************
 **/

#ifndef __COMMON_H
#define __COMON_H

void CMD_GPIO_INIT(int mode);
void CMD_GPIO_TOGGLE(int mode);

//Adding gpio commands to a terminal
//Need to define INCLUDE_GPIO_CMDS in order to use gpio commands
#ifdef INCLUDE_GPIO_CMDS
ADD_CMD("gpio", CMD_GPIO_TOGGLE, "<port> <pin> <status> Setting GPIO pin low or high(0 or 1)")
ADD_CMD("gpioinit", CMD_GPIO_INIT, "<port> <pin> Initializing GPIO pin")
#endif

#endif
