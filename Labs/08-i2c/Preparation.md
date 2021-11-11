## Preparation tasks

1. According to the [Arduino Uno schematic](../../Docs/arduino_shield.pdf), SDA and SCL signals are connected to pins:

   | **Signal** | **MCU pin** | **Arduino pins** |
   | :-: | :-: | :-: |
   | SDA (data)  | PC4 | A4, SDA |
   | SCL (clock) | PC5 | A5, SCL |

2. What is the general structure of I2C address and data frames?

   <table>
    <tr>
    <th>Frame type</th>
    <th>8</th>
    <th>7</th>
    <th>6</th>
    <th>5</th>
    <th>4</th>
    <th>3</th>
    <th>2</th>
    <th>1</th>
    <th>0</th>
    <th>Description</th>
    </tr>
    <tr>
    <td>Address</td>
    <td colspan="7">Slave address</td>
    <td>R/W</td>
    <td>ACK</td>
    <td>Sent by Master</td>
    </tr>
    <tr>
    <td>Data</td>
    <td colspan="8">Data to/from slave</td>
    <td>ACK</td>
    <td>Sent by Slave</td>
    </tr>
    </table>

### I2C scanner

1. According to the [`twi.h`](../../Examples/library/include/twi.h) header file from the I2C/TWI library:

   | **Function name** | **Function parameters** | **Description** | **Example** |
   | :-- | :-- | :-- | :-- |
   | `twi_init` | None | Initialize TWI, enable internal pull-up resistors, and set SCL frequency | `twi_init();` |
   | `twi_start` | `uint8_t slave_address` | Start communication on TWI bus and send address of TWI slave | `twi_start((addr<<1)+TWI_READ);` |
   | `twi_write` | `uint8_t data` | Send one data byte to TWI slave device | `twi_write(0xFF)` |
   | `twi_read_ack` | None | Read one byte from TWI slave device and acknowledge it by ACK | `twi_read_ack()` |
   | `twi_read_nack` | None | Read one byte from TWI slave device and acknowledge it by NACK | `twi_read_nack()` |
   | `twi_stop` | None | Generates stop condition on TWI bus | `twi_stop();` |