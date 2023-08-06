#include <stdio.h>
#include <simavr/sim_avr.h>
#include <simavr/avr_uart.h>
#include <simavr/avr_ioport.h>
#include <simavr/sim_elf.h>
#include <string.h>
#include <stdlib.h>
#include <simavr/sim_megax.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	avr_t *avr_trans = avr_make_mcu_by_name("atmega32");
	avr_t *avr_receiver = avr_make_mcu_by_name("atmega32");

	avr_init(avr_receiver);
	avr_init(avr_trans);

	// Load firmware vào chip ảo
	elf_firmware_t f_receiver, f_trans;
	elf_read_firmware("UART_recv.elf", &f_receiver);
	elf_read_firmware("UART_send.elf", &f_trans);

	avr_load_firmware(avr_receiver, &f_receiver);
	avr_load_firmware(avr_trans, &f_trans);

	avr_irq_t *irq_recv = avr_io_getirq(avr_receiver, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_INPUT);
	avr_irq_t *irq_send = avr_io_getirq(avr_trans, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_OUTPUT);

	avr_connect_irq(irq_send, irq_recv);

	while(1) {
		avr_run(avr_trans);
		avr_run(avr_receiver);
	}

	return 0;
}
