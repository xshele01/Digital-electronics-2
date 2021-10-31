/***********************************************************************
 * 
 * Kitchen Alarm
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2019-Present Tomas Fryza
 * Copyright (c) 2021 Pavlo Shelemba
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_GREEN   PB5     // LED pin
#define BTN_S1		PC1        // Push Button pins
#define BTN_S2		PC2
#define BTN_S3		PC3 
#define DOT			10    
#define BLINK_DELAY 500
#ifndef F_CPU
# define F_CPU 16000000     // CPU frequency in Hz for delay.h
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <avr/io.h>         // AVR device-specific IO definitions
#include "gpio.h"           // GPIO library for AVR-GCC*/
#include "segment.h"        // Seven-segment display library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC

// Counter values
static uint8_t count_0 = 0;
static uint8_t count_1 = 0;
static uint8_t count_2 = 0;
static uint8_t count_3 = 0;

static uint8_t button_start = 0;
static uint8_t button_minus = 0;
static uint8_t button_plus = 0;

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle two LEDs when a push button is pressed. Functions 
 *           from user-defined GPIO library is used
 * Returns:  none
 **********************************************************************/
int main(void)
{
	// Configure SSD signals
	SEG_init();
	
    // Green LED at port B
    GPIO_config_output(&DDRB, LED_GREEN);
    GPIO_write_low(&PORTB, LED_GREEN);

    // Configure Push button at port D and enable internal pull-up resistor
    GPIO_config_input_pullup(&DDRC, BTN_S3);
	GPIO_config_input_pullup(&DDRC, BTN_S2);
	GPIO_config_input_pullup(&DDRC, BTN_S1);
	
	// Configure 8-bit Timer/Counter0 for Decimal counter
	TIM0_overflow_4ms();
	// Set the overflow prescaler to 4 ms and enable interrupt
	TIM0_overflow_interrupt_enable();
	
	// Configure 16-bit Timer/Counter0 for Decimal counter
	TIM1_overflow_33ms();
	// Set the overflow prescaler to 4 ms and enable interrupt
	TIM1_overflow_interrupt_enable();
	
	// Configure 8-bit Timer/Counter0 for Decimal counter
	TIM2_overflow_4ms();
	// Set the overflow prescaler to 4 ms and enable interrupt
	TIM2_overflow_interrupt_enable();
	
	// Enable pin change interrupt for the Push Button I/O pins
	PCICR |= (1<<PCIE1);
	// Enable pin change interrupts on the Push Button I/O pins
	PCMSK1 |= (1<<PCINT9) | (1<<PCINT10) | (1<<PCINT11);
	
	// Enables interrupts by setting the global interrupt mask
	sei();

    // Infinite loop
    while (1)
    {
    }

    // Will never reach this
    return 0;
}

/**********************************************************************
 * Function: Timer/Counter0 overflow interrupt
 * Purpose:  Display counter value on SSD
 **********************************************************************/
ISR(TIMER0_OVF_vect)
{	
    static uint8_t pos = 0;
    
    switch(pos)
    {
        case 0:
			SEG_update_shift_regs(count_0, pos++);
            break;
        case 1:
            SEG_update_shift_regs(count_1, pos++);
            break;
        case 2:
            SEG_update_shift_regs(count_2 + DOT, pos++);
            break;
        case 3:
            SEG_update_shift_regs(count_3, pos);
            pos = 0;
            break;
    }
}

ISR(TIMER1_OVF_vect)
{
}

ISR(TIMER2_OVF_vect)
{
	button_start = !GPIO_read(&PINC, BTN_S1);
	button_minus = !GPIO_read(&PINC, BTN_S2);
	button_plus = !GPIO_read(&PINC, BTN_S3);
}

ISR(PCINT1_vect)
{
	if (button_plus)
	{
		if (count_2 < 9)
		{
			++count_2;
		}
		else
		{
			count_2 = 0;
			
			if (count_3 < 9)
			{
				++count_3;
			}
			else
			{
				count_3 = 0;
			}
		}
	}
	else if (button_minus)
	{
		if (count_2 > 0)
		{
			--count_2;
		}
		else
		{
			count_2 = 9;
			
			if (count_3 > 0)
			{
				--count_3;
			}
			else
			{
				count_3 = 9;
			}
		}
	}
	else if (button_start)
	{
	}
}