#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

// chuong trinh con phat du lieu
void uart_char_tx(unsigned char chr)
{
    while (bit_is_clear(UCSRA, UDRE))
    {
    }; // cho den khi bit UDRE=1
    UDR = chr;
}

int main(void)
{
    // set baud, 57.6k ung voi f=8Mhz, xem bang 70 trang 165, Atmega32 datasheet
    UBRRH = 0;
    UBRRL = 8;

    // set khung truyen va kich hoat bo nhan du lieu
    UCSRA = 0x00;
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
    UCSRB = (1 << TXEN);

    char *str = "Anh chiu het noi roi, minh chia tay di\n";
    while (1)
    {
        int i;
        int size = strlen(str);
        for (i = 0; i < size; i++)
        {
            uart_char_tx(str[i]); // phat du lieu
            _delay_ms(200);
        }
    }
}