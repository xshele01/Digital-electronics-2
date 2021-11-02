/***********************************************************************
 * 
 * Stopwatch with LCD display output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-Present Tomas Fryza
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
#include <stdlib.h>         // C library for conversion function
#include <string.h>         // C library for substring copies

/* Variables ---------------------------------------------------------*/
// Custom character definition for progress bar using https://omerk.github.io/lcdchargen/
uint8_t loadChar0[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
};

uint8_t loadChar1[8] = {
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000
};

uint8_t loadChar2[8] = {
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000
};

uint8_t loadChar3[8] = {
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100
};

uint8_t loadChar4[8] = {
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110
};

uint8_t loadChar5[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};

/* Function definitions ----------------------------------------------*/
void storeChar(uint8_t ch[8])
{
    // Store all new chars to memory line by line
    for (uint8_t i = 0; i < 8; ++i)
        lcd_data(ch[i]);
}

/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Update stopwatch value on LCD display when 8-bit 
 *           Timer/Counter2 overflows.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);

    // Set pointer to beginning of CGRAM memory
    lcd_command(1 << LCD_CGRAM);
    
    storeChar(loadChar0);
    storeChar(loadChar1);
    storeChar(loadChar2);
    storeChar(loadChar3);
    storeChar(loadChar4);
    storeChar(loadChar5);
    
    // Set DDRAM address
    lcd_command(1 << LCD_DDRAM);

    // Put chars at LCD display
    lcd_gotoxy(1, 0);
    lcd_puts("00:00.0");

    lcd_gotoxy(6, 0);
    lcd_putc('.');
	
	lcd_gotoxy(11, 0);
    lcd_putc('0');

    // Configure 8-bit Timer/Counter2 for Stopwatch
    // Set the overflow prescaler to 16 ms and enable interrupt
    TIM2_overflow_16ms();
    TIM2_overflow_interrupt_enable();
    
    // Configure 8-bit Timer/Counter0 for Stopwatch
    // Set the overflow prescaler to 16 ms and enable interrupt
    TIM0_overflow_16ms();
    TIM0_overflow_interrupt_enable();
    
    // Configure 16-bit Timer/Counter1 for Stopwatch
    // Set the overflow prescaler to 262 ms and enable interrupt
    TIM1_overflow_262ms();
    TIM1_overflow_interrupt_enable();

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
 * Function: Timer/Counter2 overflow interrupt
 * Purpose:  Update the stopwatch on LCD display every sixth overflow,
 *           ie approximately every 100 ms (6 x 16 ms = 100 ms).
 **********************************************************************/
ISR(TIMER2_OVF_vect)
{
    static uint8_t number_of_overflows = 0;
    static uint8_t tens = 0;        // Tenths of a second
    static uint8_t secs = 0;       // Seconds
    static uint8_t mins = 0;       // Minutes
    static uint16_t secs_2 = 0;     // Square seconds
    char lcd_str[2] = "  ";         // Strings for converting numbers by itoa()
    char lcd_2_str[4] = "    ";
    
    ++number_of_overflows;

    if (number_of_overflows >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        number_of_overflows = 0;
        
        ++tens;
        
        if (tens == 10)
        {
            tens = 0;
            ++secs; 
        }
        
        lcd_gotoxy(7,0);
        lcd_putc(tens + '0');
        
        itoa(secs, lcd_str, 10);
        
        if (secs < 10)
            lcd_gotoxy(5, 0);
        else if (secs < 60)
            lcd_gotoxy(4, 0);
        else
        {
            secs = 0;
            ++mins;
            
            lcd_gotoxy(12,0);
            lcd_puts("   ");
                        
            lcd_gotoxy(4, 0);
            strcpy(lcd_str, "00");
        }
        
        lcd_puts(lcd_str);
        
        itoa(mins, lcd_str, 10);
        
        if (mins < 10)
            lcd_gotoxy(2, 0);
        else if (mins < 60)
            lcd_gotoxy(1, 0);
        else
        {
            lcd_gotoxy(1, 0);
            
            mins = 0;
            strcpy(lcd_str, "00");
        }
        
        lcd_puts(lcd_str);

        secs_2 = secs * secs;
        
        lcd_gotoxy(11,0);
        itoa(secs_2, lcd_2_str, 10);
        lcd_puts(lcd_2_str);
    }
}

ISR(TIMER0_OVF_vect)
{
    static uint8_t number_of_overflows = 0;
    static uint8_t lcd_section = 1;
    
    if (number_of_overflows >= 6)
    {
        number_of_overflows = 0;
        ++lcd_section;
    }
    
    if (lcd_section == 11)
    {
        lcd_section = 1;
        
        lcd_gotoxy(1, 1);
        lcd_puts("          ");
    }
    
    lcd_gotoxy(lcd_section, 1);
    lcd_putc(number_of_overflows);
    
    ++number_of_overflows;
}    

ISR(TIMER1_OVF_vect)
{
    static char *running_text = "    I like Digital electronics!    \0";
    static char lcd_text[5];
    static uint8_t count = 1;
    static uint8_t pointer = 0;
    
    if (count)
    {
        memcpy(lcd_text, &running_text[pointer], 4);
        lcd_text[4] = '\0';
        count = 0;
        ++pointer;
    }
    else
    {
        count = 1;
    }
    
    if (pointer == strlen(running_text) - 4)
    {
        pointer = 0;
    }
    
    lcd_gotoxy(11, 1);
    lcd_puts(lcd_text);
}