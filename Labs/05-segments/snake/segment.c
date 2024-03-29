/***********************************************************************
 * 
 * Seven-segment display library for AVR-GCC
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2019-Present Tomas Fryza
 * Copyright (c) 2021 Pavlo Shelemba
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license
 *
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#ifndef F_CPU
# define F_CPU 16000000
#endif
#include <util/delay.h>
#include "gpio.h"
#include "segment.h"

/* Variables ---------------------------------------------------------*/
// Active-low
uint8_t segment_value[] = {
	// Snek segments
	0b01111111, // A
	0b10111111,	// B
	0b11011111, // C
	0b11101111, // D
	0b11110111, // E
	0b11111011, // F
};

// Active-high position 0 to 3
uint8_t segment_position[] = {
	// p3 p2 p1 p0
	0b00010000,     // Position 0
	0b00100000,     // Position 1
	0b01000000,     // Position 2
	0b10000000      // Position 3
};

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: SEG_init()
 * Purpose:  Configure SSD signals LATCH, CLK, and DATA as output
 * Returns:  none
 **********************************************************************/
void SEG_init(void)
{
    /* Configuration of SSD signals */
    GPIO_config_output(&DDRD, SEG_LATCH);
    GPIO_config_output(&DDRD, SEG_CLK);
    GPIO_config_output(&DDRB, SEG_DATA);
}

/**********************************************************************
 * Function: SEG_update_shift_regs()
 * Purpose:  Display segments at one position of the SSD
 * Input:    segments - Segments to be displayed (abcdefgDP, active low)
 *           position - Position of the display where the segments are to 
 *                      be displayed (p3 p2 p1 p0 xxxx, active high)
 * Returns:  none
 **********************************************************************/
void SEG_update_shift_regs(uint8_t segments, uint8_t position)
{
	uint8_t bit_number;
	
    segments = segment_value[segments];     // 0, 1, ..., 9
    position = segment_position[position];  // 0, 1, 2, 3
    
    // Pull LATCH, CLK, and DATA low
    GPIO_write_low(&PORTD, SEG_LATCH);
    GPIO_write_low(&PORTD, SEG_CLK);
    GPIO_write_low(&PORTB, SEG_DATA);
    
    // Wait 1 us
    _delay_us(1);
    
    // Loop through the 1st byte (segments)
    // a b c d e f g DP (active low values)
    for (bit_number = 0; bit_number < 8; ++bit_number)
    {
        // Test LSB of "segments" by & (faster) or % (slower) and...
        if (!(segments & 00000001))
        // ...output DATA value
            GPIO_write_low(&PORTB, SEG_DATA);
        else
            GPIO_write_high(&PORTB, SEG_DATA);

		// Generate clock signal
        SEG_clk_2us();
        
        // Shift "segments"
        segments = segments >> 1;
    }

    // Loop through the 2nd byte (position)
    // p3 p2 p1 p0 . . . . (active high values)
    for (bit_number = 0; bit_number < 8; ++bit_number)
    {
        // Test LSB of "position" by & (faster) or % (slower) and...
        if (position & 00000001)
        // ...output DATA value
            GPIO_write_high(&PORTB, SEG_DATA);
        else
            GPIO_write_low(&PORTB, SEG_DATA);
            
		// Generate clock signal	
        SEG_clk_2us();

        // Shift "position"
        position = position >> 1;
    }

    // Pull LATCH high
    GPIO_write_high(&PORTD, SEG_LATCH);
    
    // Wait 1 us
    _delay_us(1);

}

/**********************************************************************
 * Function: SEG_clear()
 * Purpose:  Turn all segments of the SSD off
 * Input:    none
 * Returns:  none
 **********************************************************************/
void SEG_clear()
{
    // Pull LATCH, CLK, and DATA low
    GPIO_write_low(&PORTD, SEG_LATCH);
    GPIO_write_low(&PORTD, SEG_CLK);
    GPIO_write_low(&PORTB, SEG_DATA);
    
    // Wait 1 us
    _delay_us(1);
	
    for (uint8_t bit_number = 0; bit_number < 16; ++bit_number)
    {
		// Pull output DATA low
		GPIO_write_low(&PORTB, SEG_DATA);

		// Generate clock signal
	    SEG_clk_2us();
    }
	
	// Pull LATCH high
	GPIO_write_high(&PORTD, SEG_LATCH);
	
	// Wait 1 us
	_delay_us(1);
}

/**********************************************************************
 * Function: SEG_clk_2us()
 * Purpose:  Generate 1 period of a clock signal with a frequency of 
 *			 500 kHz
 * Input:    none
 * Returns:  none
 **********************************************************************/
void SEG_clk_2us()
{
	// Wait 1 us
	_delay_us(1);
		
	GPIO_write_high(&PORTD, SEG_CLK);
		    
	// Wait 1 us
	_delay_us(1);
		    
	// Pull CLK low
	GPIO_write_low(&PORTD, SEG_CLK);
}