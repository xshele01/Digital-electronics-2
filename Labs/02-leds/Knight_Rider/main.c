/***********************************************************************
 * 
 * Knight Rider.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2021 Pavlo Shelemba
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

#define PUSH  PD4
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

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

int main(void)
{
	// Configure LEDs at port C
	// Set first 5 pins as output in Data Direction Register
    DDRC = 0x1F;
    // ...and turn first LED on in Data Register
	PORTC = 0x1;
	
	// Configure Push Button at port D and enable internal pull-up resistor
    DDRD = DDRD & ~(1 << PUSH);
    PORTD = PORTD | (1<< PUSH);
	
    while (1) 
    {
		if (buttonPushed(&PIND, PUSH))
		{
			while (1) 
			{
				for (int i = 0; i < 8; ++i)
				{
					if (i < 4)
					{
						PORTC = (PORTC << 1);
						_delay_ms(500);
					}
					else
					{
						PORTC = (PORTC >> 1);
						_delay_ms(500);
					}
				}
			}
		}
    }
}

