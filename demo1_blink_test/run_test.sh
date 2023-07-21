#!/bin/bash
avr-gcc -O3 -g -o blink.elf blink_led_atmega8.c -mmcu=atmega8
gcc -o blink_test main.c -I/usr/lib/simavr/include  -L/usr/lib/simavr/lib -lsimavr
export LD_LIBRARY_PATH="/usr/lib/simavr/lib:$LD_LIBRARY_PATH"
./blink_test
