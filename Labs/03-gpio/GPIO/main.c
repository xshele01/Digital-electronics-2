/***********************************************************************
 * 
 * Alternately toggle two LEDs when a push button is pressed. Use 
 * functions from GPIO library
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2019-Present Tomas Fryza
 * Copyright (c) 2021 Pavlo Shelemba
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_GREEN   PB5     // Green LED pin
#define LED_BREAD   PC5     // Breadboard LED pin
#define PUSH_BTN    PD4     // Push Button pin
#define BLINK_DELAY 500
#ifndef F_CPU
# define F_CPU 16000000     // CPU frequency in Hz for delay.h
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions
#include "gpio.h"           // GPIO library for AVR-GCC*/

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle two LEDs when a push button is pressed. Functions 
 *           from user-defined GPIO library is used
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Green LED at port B
    GPIO_config_output(&DDRB, LED_GREEN);
    GPIO_write_low(&PORTB, LED_GREEN);

    // Configure the second LED at port C
     GPIO_config_output(&DDRC, LED_BREAD);
     GPIO_write_low(&PORTC, LED_BREAD);

    // Configure Push button at port D and enable internal pull-up resistor
    GPIO_config_input_pullup(&DDRD, PUSH_BTN);

    // Infinite loop
    while (1)
    {
        if (!GPIO_read(&PIND, PUSH_BTN))
        {
			// Pause several milliseconds
			 _delay_ms(BLINK_DELAY);
			 
			 GPIO_toggle(&PORTB, LED_GREEN);
			 GPIO_toggle(&PORTC, LED_BREAD);
			 
            while (!GPIO_read(&PIND, PUSH_BTN))
			{
				 _delay_ms(BLINK_DELAY);
				 
				GPIO_toggle(&PORTB, LED_GREEN);
				GPIO_toggle(&PORTC, LED_BREAD);
			}
        }
    }

    // Will never reach this
    return 0;
}