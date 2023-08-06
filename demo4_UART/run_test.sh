#!/bin/bash
avr-gcc -O3 -g -o UART_send.elf atmega32_UART_send_data.c -mmcu=atmega32
avr-gcc -O3 -g -o UART_recv.elf atmega32_UART_recv_data.c -mmcu=atmega32
gcc -o UART_test main.c -I/usr/lib/simavr/include -I/usr/lib/avr/include -L/usr/lib/simavr/lib -lsimavr
export LD_LIBRARY_PATH="/usr/lib/simavr/lib:$LD_LIBRARY_PATH"
./UART_test
