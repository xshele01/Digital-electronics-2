/***********************************************************************
 * 
 * Decimal counter with 7-segment output
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
 * Purpose:  Display decimal counter values on SSD (Seven-segment 
 *           display) when 16-bit Timer/Counter1 overflows
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Configure SSD signals
    SEG_init();

    // Configure 16-bit Timer/Counter1 for Decimal counter
    // Set the overflow prescaler to 262 ms and enable interrupt
    TIM1_overflow_262ms();
    TIM1_overflow_interrupt_enable();

    // Configure 8-bit Timer/Counter0 for Decimal counter
    // Set the overflow prescaler to 4 ms and enable interrupt
    TIM0_overflow_4ms();
    TIM0_overflow_interrupt_enable();

	// Enable pin change interrupt for the Push Button I/O pins
	PCICR |= (1<<PCIE1);
	// Enable pin change interrupt on the Push Button I/O pin
	PCMSK1 |= (1<<PCINT9);
	
	// Configure Push Button at port C and enable internal pull-up resistor
    GPIO_config_input_pullup(&DDRC, BTN_S1);

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
uint8_t count_2 = 0;
uint8_t count_3 = 0;

/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Increment counter value from 00 to 59
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    ++count_0;
    
    if (count_0 == 10)
	{
        count_0 = 0;
        ++count_1;
       
        if (count_1 == 6)
        {
            count_0 = 0;
            count_1 = 0;
            ++count_2;
            
            if (count_2 == 10)
            {
                count_2 = 0;
                ++count_3;
                
                if (count_3 == 6)
                {
                    count_0 = 0;
                    count_1 = 0;
                    count_2 = 0;
                    count_3 = 0;
                }
            }
        }
    }
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
            SEG_update_shift_regs(count_2 + 10, pos++);
            break;
        case 3:
            SEG_update_shift_regs(count_3, pos);
            pos = 0;
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
	count_2 = 0;
	count_3 = 0;
}
