/***********************************************************************
 * 
 * Traffic light
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2021 Pavlo Shelemba
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

#define LED_RED_P   PB3     // Red LED pin for pedestrians
#define LED_GRN_P   PB2     // Green LED pin for pedestrians
#define LED_RED_C   PC4     // Red LED pin for cars
#define LED_YLW_C   PC3     // Yellow LED pin for cars
#define LED_GRN_C   PC2     // Green LED pin for cars
#define PUSH_BTN    PD4     // Push Button pin
#define BLINK_DELAY 500
#ifndef F_CPU
# define F_CPU 16000000     // CPU frequency in Hz for delay
#endif

#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions
#include "gpio.h"           // GPIO library

//Button press detector with debouncing
uint8_t buttonPushed(volatile uint8_t *reg_name, uint8_t pin_num)
{
	int8_t debounce = 0;
	
	while (1)
	{
		if (!GPIO_read(reg_name, pin_num))
			++debounce;
		else
			return 0;
		
		_delay_ms(10);
		
		if (debounce == 4)
			return 1;
	}
}

int main(void)
{
    // Configure pedestrian traffic light and set Red LED HIGH
	GPIO_config_output(&DDRB, LED_RED_P);
	GPIO_write_high(&PORTB, LED_RED_P);
	GPIO_config_output(&DDRB, LED_GRN_P);
	GPIO_write_low(&PORTB, LED_GRN_P);
    
    // Configure car traffic light and set Green LED HIGH
    GPIO_config_output(&DDRC, LED_RED_C);
    GPIO_write_low(&PORTC, LED_RED_C);
    GPIO_config_output(&DDRC, LED_YLW_C);
    GPIO_write_low(&PORTC, LED_YLW_C);
    GPIO_config_output(&DDRC, LED_GRN_C);
    GPIO_write_high(&PORTC, LED_GRN_C);

    // Configure Push Button
    GPIO_config_input_pullup(&DDRD, PUSH_BTN);

    while (1)
    {
		// Cycle through traffic light states once
        if (buttonPushed(&PIND, PUSH_BTN))
        {
			_delay_ms(BLINK_DELAY);
			GPIO_toggle(&PORTC, LED_GRN_C); // Car        Green  LED 1 -> 0
			GPIO_toggle(&PORTC, LED_YLW_C); // Car        Yellow LED 0 -> 1
			_delay_ms(BLINK_DELAY * 3);     // Wait on    Yellow
			GPIO_toggle(&PORTC, LED_YLW_C); // Car        Yellow LED 1 -> 0
			GPIO_toggle(&PORTC, LED_RED_C); // Car        Red    LED 0 -> 1
			GPIO_toggle(&PORTB, LED_RED_P); // Pedestrian Red    LED 1 -> 0
			GPIO_toggle(&PORTB, LED_GRN_P); // Pedestrian Green  LED 0 -> 1
			_delay_ms(BLINK_DELAY * 5);     // Wait on    Red
			GPIO_toggle(&PORTC, LED_RED_C);	// Car        Red    LED 1 -> 0 
			GPIO_toggle(&PORTC, LED_YLW_C);	// Car        Yellow LED 0 -> 1
			_delay_ms(BLINK_DELAY * 3);     // Wait on    Yellow
			GPIO_toggle(&PORTC, LED_YLW_C); // Car        Yellow LED 1 -> 0
			GPIO_toggle(&PORTC, LED_GRN_C); // Car        Green  LED 0 -> 1
			GPIO_toggle(&PORTB, LED_GRN_P); // Pedestrian Green  LED 1 -> 0
			GPIO_toggle(&PORTB, LED_RED_P); // Pedestrian Red    LED 0 -> 1
			_delay_ms(BLINK_DELAY);
			
			while (!buttonPushed(&PIND, PUSH_BTN))
				continue;
        }
    }
}