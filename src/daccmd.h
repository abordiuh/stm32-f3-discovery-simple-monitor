/**
 ******************************************************************************
 *  FILE          : daccmd.h
 *  PROJECT       : Hardware and Software Interfacing - Assignment  #4
 *  PROGRAMMER    : Artem Bordiuh & Gabriel Yano
 *  REVISED	  	  : 06-01-207
 *  DESCRIPTION   : This file adds two more commands to the simple monitor program.
 *    The first command initializes the DAC peripheral that is taken as argument
 *    to the function.
 *    The second command sets the DAC value and prints the result to the desired 
 *    pin, the arguments taken are the channel and the pin.
 ******************************************************************************
 **/

#ifndef DACCMD_H
#define DACCMD_H

void CMD_DAC_INIT(int mode);
void CMD_DAC_ONOF(int mode);

//Adding dac commands to a terminal
//Need to define INCLUDE_DAC_CMDS in order to use dac commands
#ifdef INCLUDE_DAC_CMDS
ADD_CMD("dacinit", CMD_DAC_INIT, "<channel> Initialize DAC1 on Channel 1 or 2")
ADD_CMD("dac", CMD_DAC_ONOF, "<channel(1-2)> <pattern(1-3)> <value (0-4095)> <status (1/0)> Turn on/off All Leds in sequence")
#endif


#endif /* DACCMD_H */

