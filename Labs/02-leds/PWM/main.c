/***********************************************************************
 * 
 * LED brightness controller using PWM
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2021 Pavlo Shelemba
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_GREEN       PB5      // Green LED pin
#define DUTY0			0 
#define DUTY25			125
#define DUTY50			250
#define DUTY75			375
#define DUTY100			500
#ifndef F_CPU
# define F_CPU          16000000 // CPU frequency in Hz for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Busy-wait delay loops
#include <avr/io.h>         // IO definitions

/* Functions ---------------------------------------------------------*/
int main(void)
{
    // Configure green LED at port B
    DDRB = DDRB | (1<<LED_GREEN);
  // Set LED off
    PORTB = PORTB & ~(1<<LED_GREEN);

    while (1)
    {
    PORTB = PORTB | (1<<LED_GREEN);
    _delay_us(DUTY50);
    PORTB = PORTB & ~(1<<LED_GREEN);
    _delay_us(DUTY50);
    }
}