/***********************************************************************
 * 
 * Generate PWM signal
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2021 Pavlo Shelemba
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Generate PWM signal
 * Returns:  none
 **********************************************************************/
int main(void)
{   
    // Set Compare output mode, Fast PWM to non-inverting mode
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
    // Not needed, default value is already 0
    TCCR1A &= ~((1 << COM1A0) | (1 << COM1B0));
    
    // Select Waveform generation to Fast PWM, 10-bit
    TCCR1A |= (1 << WGM10) | (1 << WGM11);
    TCCR1B |= (1 << WGM12);
    // Not needed, default value is already 0
    TCCR1B &= ~(1 << WGM13);

    // Set overflow prescaler to 8
    TCCR1B |= (1 << CS11);
    // Not needed, default value is already 0
    TCCR1B &= ~((1 << CS12) | (1 << CS10)); 

    // Set default duty cycle in OCR1B to 50%
    OCR1B = 0x01FF;
    
    // Enable Output Compare B Match Interrupt
    TIMSK1 |= (1 << OCIE1B);
    
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {  
    }
    
    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 Compare B Match Interrupt
 * Purpose:  Change PWM signal duty cycle
 **********************************************************************/
ISR(TIMER1_COMPB_vect)
{
    ++OCR1B;
    if (OCR1B == 0x03FF)
        OCR1B = 0;
} 