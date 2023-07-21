#include "simavr/sim_avr.h"
#include "simavr/sim_elf.h"
#include "simavr/sim_vcd_file.h"
#include "simavr/avr_ioport.h"
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <pthread.h>

avr_t *avr = NULL;
avr_vcd_t vcd_file;

volatile int8_t val = 0;
volatile uint8_t is_increasing = 1;
int main(int argc, char *argv[])
{
	elf_firmware_t f;
	elf_read_firmware("interrupt.elf", &f);
	f.frequency = 9600000;
	avr = avr_make_mcu_by_name("atmega8");
	avr_init(avr);
	avr_load_firmware(avr, &f);
	avr->ioend = 0x60;
	avr_vcd_init(avr, "gtkwave_output.vcd", &vcd_file, 1);
	avr_vcd_add_signal(&vcd_file,
					   avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('B'),
									 IOPORT_IRQ_PIN_ALL),
					   8, "PORTB");

	avr_vcd_add_signal(&vcd_file,
					   avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'),
									 IOPORT_IRQ_PIN_ALL),
					   8, "PORTC");

	avr_vcd_add_signal(&vcd_file,
					   avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'),
									 IOPORT_IRQ_PIN2),
					   1, "PD2");

	avr_vcd_add_signal(&vcd_file,
					   avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'),
									 IOPORT_IRQ_PIN3),
					   1, "PD3");


	// Kết nối ngắt INT0 và INT1 với hàm xử lý ngắt tương ứng
	avr_irq_t *int0_irq = avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), IOPORT_IRQ_PIN2);

	avr_irq_t *int1_irq = avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), IOPORT_IRQ_PIN3);

	avr_vcd_start(&vcd_file);
	while (1)
	{
		avr_run(avr);

		if (avr->cycle > 1000000)
		{ 
			break;
		}

		if (avr->cycle % 3000 == 0){
			avr_raise_irq(int1_irq, 1);
            avr_raise_irq(int1_irq, 0); 

		}else if (avr->cycle % 1000 == 0) { 
            avr_raise_irq(int0_irq, 1);
            avr_raise_irq(int0_irq, 0); 
        }
	}

	avr_vcd_stop(&vcd_file);
	avr_vcd_close(&vcd_file);
	return 0;
}
