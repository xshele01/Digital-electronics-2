/***********************************************************************
 * 
 * Analog-to-digital conversion with displaying result on LCD and 
 * transmitting via UART.
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
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function
#include "uart.h"           // Peter Fleury's UART library
#include <math.h>

/* Defines -----------------------------------------------------------*/
#ifndef F_CPU
# define F_CPU 16000000  // CPU frequency in Hz required for 
                         // UART_BAUD_SELECT
#endif

/* Variables ---------------------------------------------------------*/
enum Parity
{
    Odd,
    Even    
};

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Parity bit generator
 * Purpose:  Use input 8-bit data and provided parity type to calculate
 *           a parity bit
 * Returns:  Parity byte
 **********************************************************************/
uint8_t get_parity(uint16_t data, uint8_t type)
{
    uint8_t parity = type ? 0 : 1;
    
    for (int8_t i = 0; i < sizeof(data)*8; ++i)
    {
        if (data & 1)
            parity = !parity;
        
        data = data >> 1;
    }
    
    return parity;
}

/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion four times 
 *           per second. Send value to LCD and UART.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); lcd_puts("value:");
    lcd_gotoxy(3, 1); lcd_puts("key:");
    lcd_gotoxy(8, 0); lcd_puts("0    mV");    // Put ADC value in decimal
    lcd_gotoxy(8, 1); lcd_puts("none");    // Put button name here

    // Configure ADC to convert PC0[A0] analog value
    // Set ADC reference to AVcc with external capacitor
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);
   
    // Set input channel to ADC0 (default value)
    ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3));

    // Enable ADC module
    ADCSRA |= (1 << ADEN);

    // Enable conversion complete interrupt
    ADCSRA |= (1 << ADIE);

    // Set clock prescaler to 128
    ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 1 s and enable overflow interrupt
    TIM1_overflow_1s();
    TIM1_overflow_interrupt_enable();

    // Initialize UART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));
    
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
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion four times
 *           per second.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    // Start ADC conversion
    ADCSRA |= (1 << ADSC);
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display value on LCD and send it to UART.
 **********************************************************************/
ISR(ADC_vect)
{
    uint16_t mV = 0;
    uint32_t value = 0;
    char lcd_string[8] = "0000";

    value = ADC;                    // Copy ADC result to 32-bit variable
    mV = round((value*5*1000)/1023);  // Convert ADC value to mV

    itoa(mV, lcd_string, 10);       // Convert mV value to string
    
    uart_puts(lcd_string);
    uart_puts("\n\r");

    lcd_gotoxy(8, 0); lcd_puts("    ");
    lcd_gotoxy(8, 0); lcd_puts(lcd_string);    // Put mV value in decimal
    
    lcd_gotoxy(8, 1); lcd_puts("      ");
    lcd_gotoxy(8, 1); 
    
    if (value > 1000)
    {
        lcd_puts("none");
        uart_puts("none");
    }
    else if (value > 600)
    {
        lcd_puts("select");
        uart_puts("select");
    }
    else if (value > 300)
    {
        lcd_puts("left");
        uart_puts("left");
    }
    else if (value > 200)
    {
        lcd_puts("down");
        uart_puts("down");
    }
    else if (value > 90)
    {
        lcd_puts("up");
        uart_puts("up");
    }
    else if (value >= 0)
    {
        lcd_puts("right");
        uart_puts("right");
    }
    
    uart_puts("\n\r");
    
    itoa(get_parity(value, Odd), lcd_string, 10);
    
    lcd_gotoxy(14, 1); lcd_puts(" ");
    lcd_gotoxy(14, 1); lcd_puts(lcd_string);
    uart_puts(lcd_string);
    uart_puts("\n\r");
}