# Lab 4: Interrupts, timers

### Part 1: Preparation tasks

1. Overflow times for three Timer/Counter modules that contain ATmega328P if CPU clock frequency is 16 MHz:

| **Module** | **Number of bits** | **1** | **8** | **32** | **64** | **128** | **256** | **1024** |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| Timer/Counter0 | 8  | 16u | 128u | -- | 1024u | -- | 4128u | 16384u |
| Timer/Counter1 | 16 | 4096u | 32768u | -- | 262144u | -- | 1056768u | 4194304u |
| Timer/Counter2 | 8  | 16u | 128u | 512u | 1024u | 2048u | 4128u | 16384u |

1. Connection of four LEDs of [Multi-function shield](../../Docs/arduino_shield.pdf):

| **LED** | **Pin** |
| :-: | :-: |
| D1 | PB5 |
| D2 | PB4 |
| D3 | PB3 |
| D4 | PB2 |

Connection of three push buttons and reset of [Multi-function shield](../../Docs/arduino_shield.pdf):

| **Push Button** | **Pin** |
| :-: | :-: |
| S1-A1 | PC1 |
| S2-A2 | PC2 |
| S3-A3 | PC3 |
| RESET | RESET |