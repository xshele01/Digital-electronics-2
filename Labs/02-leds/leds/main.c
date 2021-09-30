/***********************************************************************
 * 
 * Alternately toggle two LEDs when a push button is pressed.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Copyright (c) 2021 Shelemba Pavlo
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_GREEN       PB5     // AVR pin where green LED is connected
#define LED_BREADBOAD   PC5     // AVR pin where green LED is connected
#define PUSH_BREADBOAD  PD4     // AVR pin where green LED is connected
#define BLINK_DELAY     500
#ifndef F_CPU
# define F_CPU          16000000     // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/sfr_defs.h>

/* Functions ---------------------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle two LEDs when a push button is pressed.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Green LED at port B
    // Set pin as output in Data Direction Register...
    DDRB = DDRB | (1<<LED_GREEN);
    // ...and turn LED off in Data Register
    PORTB = PORTB & ~(1<<LED_GREEN);

    // Configure the second LED at port C
    // Set pin as output in Data Direction Register...
    DDRC = DDRC | (1<<LED_BREADBOAD);
    // ...and turn LED off in Data Register
    PORTC = PORTC & ~(1<<LED_BREADBOAD);

    // Configure Push button at port D and enable internal pull-up resistor
    // Set pin as output in Data Direction Register...
    DDRD = DDRD & ~(1<<PUSH_BREADBOAD);
    // ...and turn LED off in Data Register
    PORTD = PORTD | (1<<PUSH_BREADBOAD);


    // Infinite loop
    while (1)
    {
        // Pause several milliseconds
        // _delay_ms(BLINK_DELAY);
    
        if(bit_is_clear(PIND, PUSH_BREADBOAD))
        {
            PORTB = PORTB ^ (1<<LED_GREEN);
            PORTC = PORTC ^ (1<<LED_BREADBOAD);
            loop_until_bit_is_set(PIND, PUSH_BREADBOAD);
        }
    }

    // Will never reach this
    return 0;
}