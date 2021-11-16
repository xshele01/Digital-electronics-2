# Lab 8: Pavlo Shelemba

Link to my `Digital-electronics-2` GitHub repository:

https://github.com/xshele01/Digital-electronics-2

### Arduino Uno pinout

1. Picture of the Arduino Uno board, marked with the pins that can be used for the following functions:
   * PWM generators from `Timer0`, `Timer1`, `Timer2`
   * analog channels for ADC
   * UART pins
   * I2C pins
   * SPI pins
   * external interrupt pins `INT0`, `INT1`

   ![Uno pinout](Images/circuit.svg)

### I2C

1. Code listing of `Timer1` overflow interrupt service routine for scanning I2C devices and rendering a clear table on the UART:

```c
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Update Finite State Machine and test I2C slave addresses 
 *           between 8 and 119.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    static state_t state = STATE_IDLE;  // Current state of the FSM
    static uint8_t addr = 7;            // I2C slave address
    uint8_t result = 1;                 // ACK result from the bus
    char uart_string[2] = "00"; // String for converting numbers by itoa()

    // FSM
    switch (state)
    {
    // Increment I2C slave address
    case STATE_IDLE:
        addr++;
        // If slave address is between 8 and 119 then move to SEND state
        if (addr >= 8 && addr <= 119)
            state = STATE_SEND;
        else if (addr > 119)
            addr = 7;

        break;
    
    // Transmit I2C slave address and get result
    case STATE_SEND:
        // I2C address frame:
        // +------------------------+------------+
        // |      from Master       | from Slave |
        // +------------------------+------------+
        // | 7  6  5  4  3  2  1  0 |     ACK    |
        // |a6 a5 a4 a3 a2 a1 a0 R/W|   result   |
        // +------------------------+------------+
        result = twi_start((addr<<1) + TWI_WRITE);
        twi_stop();
        /* Test result from I2C bus. If it is 0 then move to ACK state, 
         * otherwise move to IDLE */
        state = result ? STATE_IDLE : STATE_ACK;

        break;

    // A module connected to the bus was found
    case STATE_ACK:
        // Send info about active I2C slave to UART and move to IDLE
        art_puts("Found device on address: ");
        itoa(addr, uart_string, 16);
        uart_puts(uart_string);
        uart_puts("\r\n");

        state = STATE_IDLE;

        break;

    // If something unexpected happens then move to IDLE
    default:
        state = STATE_IDLE;

        break;
    }
}
```

2. Picture of I2C signals when reading checksum (only 1 byte) from DHT12 sensor:

![I2C signals for reading checksum from DHT12 sensor](Images/wavedrom.svg)

### Meteo station

An application for temperature, humidity measurement and display. Uses sensor DHT12, real time clock DS3231, LCD and one LED. Application displays time in hours:minutes:seconds at LCD, measures both temperature and humidity values once per minute, displays both values on LCD, and when the temperature is too high, the LED starts blinking.

1. FSM of meteo station:

    ![FSM of meteo station](Images/FSM.svg)
