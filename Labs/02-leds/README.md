# Lab 2 Control of GPIO, LED, push button
## Preparation tasks

1. Two basic ways to connect a LED to the output pin of the microcontroller: 
   
* LED active-low:
  
* LED active-high:

2. LED resistor values for typical red and blue LEDs can be calculated according to Ohm's law:

![Ohm's law](Images/resistor.svg)

| **LED color** | **Supply voltage** | **LED current** | **LED voltage** | **Resistor value** |
| :-: | :-: | :-: | :-: | :-: |
| red | 5&nbsp;V | 20&nbsp;mA | 2&nbsp;V | 150 Ω |
| blue | 5&nbsp;V | 20&nbsp;mA | 4&nbsp;V | 50 Ω |


## Active-low and active-high LEDs

1. Complete tables according to the AVR manual.

| **DDRB** | **Description** |
| :-: | :-- |
| 0 | Input pin |
| 1 |  |

| **PORTB** | **Description** |
| :-: | :-- |
| 0 | Output low value |
| 1 | |

| **DDRB** | **PORTB** | **Direction** | **Internal pull-up resistor** | **Description** |
| :-: | :-: | :-: | :-: | :-- |
| 0 | 0 | input | no | Tri-state, high-impedance |
| 0 | 1 | input | yes | Pxn will source current if ext. pulled low |
| 1 | 0 | | | |
| 1 | 1 | | | |

| **Port** | **Pin** | **Input/output usage?** |
| :-: | :-: | :-: |
| A | x | Microcontroller ATmega328P does not contain port A | 
| B | 0 | Pin 8 |
|  | 1 | Pin ~9 |
|  | 2 | Pin ~10 |
|  | 3 | Pin ~11 | 
|  | 4 | Pin 12 |
|  | 5 | Pin 13 |
|  | 6 |  |
|  | 7 |  | 
| C | 0 | Pin A0 |
|  | 1 | Pin A1 |
|  | 2 | Pin A2 |
|  | 3 | Pin A3 | 
|  | 4 | Pin A4 |
|  | 5 | Pin A5 |
|  | 6 |  |
|  | 7 |  | 
| D | 0 | Pin RX<-0 |
|  | 1 | Pin TX->1 |
|  | 2 | 2 |
|  | 3 | ~3 |
|  | 4 | 4 |
|  | 5 | ~5 |
|  | 6 | ~6 |
|  | 7 | 7 |

2. Part of the C code listing with syntax highlighting, which blinks alternately with a pair of LEDs; let one LED is connected to port B and the other to port C:

```c
int main(void)
{
    // Green LED at port B
    // Set pin as output in Data Direction Register...
    DDRB = DDRB | (1<<LED_GREEN);
    // ...and turn LED off in Data Register
    PORTB = PORTB & ~(1<<LED_GREEN);

    // Configure the second LED at port C
    // WRITE YOUR CODE HERE

    // Infinite loop
    while (1)
    {
        // Pause several milliseconds
        _delay_ms(BLINK_DELAY);

        // WRITE YOUR CODE HERE
    }

    // Will never reach this
    return 0;
}
```