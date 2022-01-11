# Lab 7: ADC and UART serial communication

### Praparation tasks

1. According to the [LCD keypad shield](../../Docs/arduino_shield.pdf), the five push buttons (Select, Left, Up, Down, and Right) are connected using a single `PC0[A0]` Arduino pin and a voltage divider circuit.

2. Voltage values on pin `PC0[A0]` for each button pressed:

    ![Right button](Images/Right.svg)

    ![Up button](Images/Up.svg)

    ![Down button](Images/Down.svg)

    ![Left button](Images/Left.svg)

    ![Select button](Images/Select.svg)

3. When none of the push buttons are pressed:
   
    ![No button](Images/None.svg)

4. ADC values for these voltages according to the following equation if reference is Vref = 5&nbsp;V and number of bits for analog to digital conversion is n = 10:
   
    ![ADC equation](Images/ADC.svg)

    | **Push button** | **`PC0[A0]` voltage** | **ADC value (calculated)** | **ADC value (measured)** |
    | :-- | :-: | :-: | :-: |
    | Right  | 0&nbsp;V | 0 | 0 |
    | Up     | 0.496&nbsp;V | 102 | 98 |
    | Down   | 1.203&nbsp;V | 246 | 256 |
    | Left   | 1.907&nbsp;V | 390 | 408 |
    | Select | 3.182&nbsp;V | 651 | 639 |
    | none   | 5&nbsp;V | 1023 | 1023 |


### Part 2: Analog-to-Digital Conversion

| **Operation** | **Register(s)** | **Bit(s)** | **Description** |
| :-- | :-: | :-: | :-- |
| Voltage reference    | `ADMUX`  | `REFS1:0` | `00`: External voltage reference, `01`: AVcc voltage reference (5V), `10`: (reserved), `11`: 1.1V voltage reference   |
| Input channel        | `ADMUX`  | `MUX3:0`  | `0000`: ADC0, `0001`: ADC1, `0010`: ADC2, `0011`: ADC3, `0100`: ADC4, `0101`: ADC5, `0110`: ADC6, `0111`: ADC7, `1000`: ADC8, `1001:1101`: (reserved), `1110`: 1.1V, `1111`: 0V |
| ADC enable           | `ADCSRA` | `ADEN`    | `0`: Off, `1`: On |
| Start conversion     | `ADCSRA` | `ADSC`    | `0`: No effect, `1`: Start |
| ADC interrupt enable | `ADCSRA` | `ADIE`    | `1`: activate *ADC Conversion Complete Interrupt* |
| ADC clock prescaler  | `ADCSRA` | `ADPS2:0` | `000`: Division factor 2, `001`: 2, `010`: 4, `011`: 8, `100`: 16, `101`: 32, `110`: 64, `111`: 128|
| ADC 10-bit result    | `ADCL` and `ADCH` | `ADC9:0` | These bits represent the result from the conversion, the exact way the value is stored depends on whether the `ADLAR` bit in `ADMUX` is set |


### Part 3: UART communication

| **Function name** | **Function parameter(s)** | **Description** | **Example** |
| :-- | :-- | :-- | :-- |
| `uart_init` | `UART_BAUD_SELECT(9600, F_CPU)` | Initialize UART to 8N1 and set baudrate to 9600&nbsp;Bd | `uart_init(UART_BAUD_SELECT(9600, F_CPU));` |
| `uart_getc` | `void` | Get received byte from ringbuffer | `uart_getc();` |
| `uart_putc` | `unsigned char data` | Put byte to ringbuffer for transmitting via UART | `uart_putc('C');` |
| `uart_puts` | `const char* s` | Put string to ringbuffer for transmitting via UART | `uart_puts("Hello, world!");` |