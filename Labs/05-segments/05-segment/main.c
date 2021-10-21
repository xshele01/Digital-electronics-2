/***********************************************************************
 * 
 * Decimal counter with 7-segment output
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Copyright (c) 2021 Pavlo Shelemba
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "segment.h"        // Seven-segment display library for AVR-GCC

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

    // Test of SSD: display number '3' at position 0
/*    SEG_update_shift_regs(3, 0);*/

    // Configure 16-bit Timer/Counter1 for Decimal counter
    TIM1_overflow_262ms();
    
    TIM0_overflow_4ms();
    
    // Set the overflow prescaler to 262 ms and enable interrupt
    TIM1_overflow_interrupt_enable();

    TIM0_overflow_interrupt_enable();

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

uint8_t count_0 = 0;
uint8_t count_1 = 0;
uint8_t count_2 = 0;
uint8_t count_3 = 0;

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Increment decimal counter value and display it on SSD.
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
