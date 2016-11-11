#!/bin/bash

avr-g++ -g -Wall -mcall-prologues -mmcu=atmega328p  -Os   -c -o main.o main.cpp
avr-g++ -g -Wall -mcall-prologues -mmcu=atmega328p  -Os main.o -Wl,-gc-sections -lpololu_atmega328p -Wl,-relax -o main.obj
avr-objcopy -R .eeprom -O ihex main.obj main.hex
rm main.obj

echo "Done!"
