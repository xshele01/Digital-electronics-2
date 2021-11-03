# Lab 7: ADC and UART serial communication

### Praparation tasks

1. According to the [LCD keypad shield](../../Docs/arduino_shield.pdf), the five push buttons (Select, Left, Up, Down, and Right) are connected using a single `PC0[A0]` Arduino pin and a voltage divider circuit.
2. Voltage values on pin PC0[A0] for each button pressed:

    ![Right button](Images/Right.svg)
    ![Up button](Images/Up.svg)
    ![Down button](Images/Down.svg)
    ![Left button](Images/Left.svg)
    ![Select button](Images/Select.svg)
3. When none of the push buttons are pressed:
   
    ![No button](Images/None.svg)
4. ADC values for these voltages according to the following equation if reference is Vref = 5&nbsp;V and number of bits for analog to digital conversion is n = 10:
   
    ![ADC equation](Images/ADC.svg)

    | **Push button** | **PC0[A0] voltage** | **ADC value (calculated)** | **ADC value (measured)** |
    | :-: | :-: | :-: | :-: |
    | Right  | 0&nbsp;V | 0   |  |
    | Up     | 0.496&nbsp;V | 102 |  |
    | Down   | 1.203&nbsp;V | 246 |  |
    | Left   | 1.907&nbsp;V | 390 |  |
    | Select | 3.182&nbsp;V | 651 |  |
    | none   | 5&nbsp;V | 1023 |  |
