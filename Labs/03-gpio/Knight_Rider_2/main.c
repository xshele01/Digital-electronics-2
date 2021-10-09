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

#define PUSH  PD4           // Push Button pin
#ifndef F_CPU
#define F_CPU 16000000UL    // CPU frequency in Hz for delay.h
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "gpio.h"

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
    // Configure LEDs at port C
    // Set first 5 pins as output in Data Direction Register
    for (int pin = 0; pin < 5; ++pin)
    {
        GPIO_config_output(&DDRC, pin);
    }
    // ...and turn first LED on in Data Register
    GPIO_write_high(&PORTC, 0);
    
    // Configure Push Button at port D and enable internal pull-up resistor
    GPIO_config_input_pullup(&DDRD, PUSH);
    
    while (1) 
    {
        if (buttonPushed(&PIND, PUSH))
        {
            while (1) 
            {
                // Loops through 5 LEDs
                for (int i = 0; i < 8; ++i)
                {
                    if (i < 4)
                    {
                        GPIO_toggle(&PORTC, i);
                        GPIO_toggle(&PORTC, i+1);
                        _delay_ms(500);
                    }
                    else
                    {
                        GPIO_toggle(&PORTC, 8-i);
                        GPIO_toggle(&PORTC, 8-i-1);
                        _delay_ms(500);
                    }
                }
            }
        }
    }
}