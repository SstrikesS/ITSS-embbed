#define F_CPU 9600000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
// --- IMPORTANT: mmcu=atmega8 ---
volatile int8_t val = 0; // khai báo 1 biến val 8 bit, có dấu và giá trị khởi tạo bằng 0.
int main(void)
{

    DDRD = 0x00;  // khai báo PORTD là Input để sử dụng 2 chân ngắt.
    PORTD = 0xFF; // sử dụng điện trở nội kéo lên.
    DDRB = 0xFF;  // PORTB là Output để xuất LED 7 đoạn

    MCUCR |= (1 << ISC11) | (1 << ISC01); // cả 2 ngắt là ngắt cạnh xuống
    GICR |= (1 << INT1) | (1 << INT0);    // cho phép 2 ngắt hoạt động
    sei();                                // set bit I cho phép ngắt toàn cục

    DDRC = 0xFF; // PORTC là Output
    while (1)
    {            // vòng lặp vô tận
        PORTC++; // quét PORTC
        _delay_loop_2(60000);
    }
    return 0;
}

// Trình phục vụ ngắt của  INT0
ISR(INT0_vect)
{
    val++; // nếu có ngắt INT0 xảy ra, tăng val thêm 1
    if (val > 9)
        val = 0; // giới hạn không vượt quá 9
    PORTB = val;
}

// Trình phục vụ ngắt của  INT1
ISR(INT1_vect)
{
    val--; // nếu có ngắt INT1 xảy ra, giảm val đi 1
    if (val < 0)
        val = 9; // giới hạn không nhỏ hơn 0
    PORTB = val;
}