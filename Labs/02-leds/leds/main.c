/***********************************************************************
 * 
 * Alternately toggle two LEDs when a push button is pressed.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Copyright (c) 2021 Pavlo Shelemba
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_GREEN       PB5     // AVR pin where green LED is connected
#define LED_BREAD       PC5     // AVR pin where breadboard LED is connected
#define PUSH            PD4     // AVR pin where push button LED is connected
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
 * Function: buttonPushed
 * Purpose:  Detect button press with debouncing.
 * Returns:  State of a pin connected to the button.
 **********************************************************************/
uint8_t buttonPushed(volatile uint8_t *reg_name, uint8_t pin_num)
{
	int8_t debounce = 0;

	while (1)
	{
		if (bit_is_clear(*reg_name, pin_num))
			++debounce;
		else
			debounce = 0;

		_delay_ms(10);

		if (debounce == 4)
			return 1;
	}
}
/**********************************************************************
 * Function: Main function where the program execution begins.
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
    DDRC = DDRC | (1<<LED_BREAD);
    // ...and turn LED on in Data Register
    PORTC = PORTC & ~(1<<LED_BREAD);

    // Configure Push Button at port D and enable internal pull-up resistor
    DDRD = DDRD & ~(1<<PUSH);
    PORTD = PORTD | (1<<PUSH);

    // Infinite loop
    while (1)
    {
		// Pause several milliseconds
		_delay_ms(BLINK_DELAY);
				
        if(buttonPushed(&PIND, PUSH))
        {
            PORTB = PORTB ^ (1<<LED_GREEN);
            PORTC = PORTC ^ (1<<LED_BREAD);
            loop_until_bit_is_set(PIND, PUSH);
        }
    }

    // Will never reach this
    return 0;
}