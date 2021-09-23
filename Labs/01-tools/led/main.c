/***********************************************************************
 * 
 * Blink a LED and use function from the delay library.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
/* The preprocessor will process directives that are inserted into the C
 * source code. These directives allow additional actions to be taken on
 * the C source code before it is compiled into object code. Directives
 * are not part of the C language itself.
 *
 * Preprocessor directives begin with a pound (#) symbol and may have 
 * several arguments. Do NOT put a semicolon character at the end of 
 * directives. This is a common mistake.
 */
#define LED_GREEN   PB5 // AVR pin where green LED is connected
#define DOT 500         // Delay in milliseconds
#define COMMA 1000  
#define PAUSE_SHORT 250
#define PAUSE_LONG 2000
#ifndef F_CPU           // Preprocessor directive allows for conditional
                        // compilation. The #ifndef means "if not defined".
# define F_CPU 16000000 // CPU frequency in Hz required for delay
#endif                  // The #ifndef directive must be closed by #endif

/* Includes ----------------------------------------------------------*/
/* Include another C language file into the current file at the location
 * of the #include statement prior to compiling the source code.
 */
#include <util/delay.h> // Functions for busy-wait delay loops
#include <avr/io.h>     // AVR device-specific IO definitions

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle one LED and use delay library.
 * Returns:  none
 **********************************************************************/
void led_on(void)
{
    // Set pin as output in Data Direction Register
    // DDRB = DDRB or 0010 0000
    DDRB = DDRB | (1<<LED_GREEN);
}

void led_off(void)
{
    // Set pin LOW in Data Register (LED off)
    // PORTB = PORTB and 1101 1111
    PORTB = PORTB & ~(1<<LED_GREEN);
}

void dot(void)
{
    // Invert LED in Data Register
    // PORTB = PORTB or 0010 0000
    led_on();
    _delay_ms(DOT);
    led_off();
    _delay_ms(PAUSE_SHORT);
}

void comma(void)
{
    led_on();
    _delay_ms(COMMA);
    led_off();
    _delay_ms(PAUSE_SHORT);
}


int main(void)
{
    // Infinite loop
    while (1)
    {
        // Pause several milliseconds
        // D in Morse code
        comma();
        dot();
        dot();
        
        _delay_ms(PAUSE_LONG);
        
        // E in Morse code
        dot();
        
        _delay_ms(PAUSE_LONG);
        
        // 2 in Morse code
        dot();
        dot();
        comma();
        comma();
        comma();
        
        _delay_ms(PAUSE_LONG);
    }

    // Will never reach this
    return 0;
}