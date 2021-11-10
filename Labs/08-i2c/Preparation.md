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
    <td colspan="8">Slave data</td>
    <td>ACK</td>
    <td>Sent by Slave</td>
    </tr>
    </table>


