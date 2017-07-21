/*
 * Exercise 2 - Lab 1
 *
 * Lab 1 - EEET2490
 * Semester: 2017B
 *
 * Author : Hoang Vo
 * ID : s3575983
 *
 * Created: 7/21/2017 8:55:11 AM
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 0xFF;
	DDRD = 0xFF;
	
    char status[9] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00};
    
    while(1)
    {
	    PORTB = status[7];
	    _delay_ms(1000);
	    PORTB = status[6];
	    _delay_ms(1000);
	    PORTB = status[5];
	    _delay_ms(1000);
	    PORTB = status[4];
	    _delay_ms(1000);
	    PORTB = status[3];
	    _delay_ms(1000);
	    PORTB = status[2];
	    _delay_ms(1000);
	    PORTB = status[8];
	    PORTD = status[1];
	    _delay_ms(1000);
	    PORTD = status[0];
	    _delay_ms(1000);
	    PORTD = status[8];
//	    _delay_ms(1000);
    }
}
