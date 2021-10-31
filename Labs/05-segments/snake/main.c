/***********************************************************************
 * 
 * Snek
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Copyright (c) 2021 Pavlo Shelemba
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "segment.h"        // Seven-segment display library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC

/* Defines -----------------------------------------------------------*/
#define BTN_S1  PC1         // Push Button pin

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Display snek
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Configure SSD signals
    SEG_init();
	
	// Configure Push Button at port C and enable internal pull-up resistor
	GPIO_config_input_pullup(&DDRC, BTN_S1);

    // Configure 16-bit Timer/Counter1 for Decimal counter
    TIM1_overflow_262ms();
    // Set the overflow prescaler to 262 ms and enable interrupt
    TIM1_overflow_interrupt_enable();
	
	// Configure 8-bit Timer/Counter0 for Decimal counter
	TIM0_overflow_4ms();
	// Set the overflow prescaler to 4 ms and enable interrupt
	TIM0_overflow_interrupt_enable();

	// Enable pin change interrupt for the Push Button I/O pins
	PCICR |= (1<<PCIE1);
	// Enable pin change interrupt on the Push Button I/O pin
	PCMSK1 |= (1<<PCINT9);
	
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
// Counter values
uint8_t count_0 = 0;
uint8_t count_1 = 0;

/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Increment counter value from 00 to 59.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
	if (count_0 < 7)
		++count_0;
	else
		count_0 = 0;
	
	if (count_1 < 5)
		++count_1;
	else
		count_1 = 0;
}

/**********************************************************************
 * Function: Timer/Counter0 overflow interrupt
 * Purpose:  Display counter value on SSD
 **********************************************************************/
ISR(TIMER0_OVF_vect)
{
    static uint8_t count = 0;
    
    switch(count)
    {
        case 0:
			if (count_0 < 4)
				SEG_update_shift_regs(count_0, 0);
            else if (count_0 < 7)
				SEG_update_shift_regs(count_0 - 1, 1);
			else
				SEG_update_shift_regs(0, 1);
			count = 1;
            break;
        case 1:
            SEG_update_shift_regs(count_1, 3);
            count = 0;
            break;
    }
}

/**********************************************************************
 * Function: Pin Change Interrupt
 * Purpose:  Reset counter values
 **********************************************************************/
ISR(PCINT1_vect)
{
	count_0 = 0;
	count_1 = 0;
}
