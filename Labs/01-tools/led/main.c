/***********************************************************************
 * 
 * Blink a LED and use function from the delay library.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Copyright (c) 2021 Pavlo Shelemba
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
#define SHORT_DELAY 250 // Delay in milliseconds
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
 * Function: Dot function
 * Purpose:  Toggle LED to display dot in Morse code.
 * Returns:  none
 **********************************************************************/
void dot(void)
{
    // Set pin HIGH in Data Register (LED on)
    PORTB = PORTB | (1<<LED_GREEN);
    _delay_ms(SHORT_DELAY * 2);
    // Set pin LOW in Data Register (LED off)
    PORTB = PORTB & ~(1<<LED_GREEN);
    _delay_ms(SHORT_DELAY);
}
/**********************************************************************
 * Function: Dash function
 * Purpose:  Toggle LED to display dash in Morse code.
 * Returns:  none
 **********************************************************************/
void dash(void)
{
    PORTB = PORTB | (1<<LED_GREEN);
    _delay_ms(SHORT_DELAY * 4);
    PORTB = PORTB & ~(1<<LED_GREEN);
    _delay_ms(SHORT_DELAY);
}
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle LED to display Morse code.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Set pin as output in Data Direction Register
    DDRB = DDRB | (1<<LED_GREEN);
    PORTB = PORTB & ~(1<<LED_GREEN);

    // Infinite loop
    while (1)
    {
        // Pause several milliseconds
        // D in Morse code
        dash();
        dot(); 
        dot();
        
        _delay_ms(SHORT_DELAY * 8);
        
        // E in Morse code
        dot();
        
        _delay_ms(SHORT_DELAY * 8);
        
        // 2 in Morse code
        dot();
        dot();
        dash();
        dash();
        dash();
        
        _delay_ms(SHORT_DELAY * 8);
    }
}