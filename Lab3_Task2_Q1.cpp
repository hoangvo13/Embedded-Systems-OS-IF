/*
 * Question 2.1 - Lab3
 * 
 * EEET2490
 * 2017B
 *
 * Author : Hoang Vo
 * ID : s3575983
 *
 * Created: 8/24/2017 9:01:20 AM 
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void Timer_Frequency(uint8_t freq)
{
	TCCR1B |= (1<<WGM12) | (1<<CS12);				// CTC mode & 256 prescaler
	TIMSK1 |= (1<<OCIE1A);					        // Enable Output compare match A
//	TCCR1A |= (1<<COM1A0);					        // Toggle OC1A on Compare Match
//	TCCR1A |= (1<<COM1A1);					        // Clear OC1A on Compare Match
	TCCR1A |= (1<<COM1A0) | (1<<COM1A1);			// Set OC1A on Compare Match

	OCR1A = (F_CPU/(freq*2*256)-1);					// Calculate value for OCR1A
}

int main(void)
{
	DDRB |= (0x3F<<0);
	DDRC |= (1<<0) | (1<<1);

//	EICRA |= (1<<ISC10);							// Set INT1 to trigger on ANY logic change
	EICRA |= (1<<ISC11);							// Set INT1 to trigger on the falling edge
//	EICRA |= (1<<ISC11) | (1<<ISC10);				// Set INT1 to trigger on the rising edge
	
	EIMSK |= (1<<INT1);								// Turns on INT1
	sei();
	
	while(1)
	{
		PORTC &= ~(1<<1);
		PORTB |= (1<<0);			// LED 0
		_delay_ms(1000);
		PORTB &= ~(1<<0);
		PORTB |= (1<<1);			// LED 1	
		_delay_ms(1000);
		PORTB &= ~(1<<1);			
		PORTB |= (1<<2);			// LED 2
		_delay_ms(1000);
		PORTB &= ~(1<<2);
		PORTB |= (1<<3);			// LED 3
		_delay_ms(1000);
		PORTB &= ~(1<<3);
		PORTB |= (1<<4);			// LED 4
		_delay_ms(1000);
		PORTB &= ~(1<<4);
		PORTB |= (1<<5);			// LED 5
		_delay_ms(1000);
		PORTB &= ~(0x3F<<0);
		PORTC |= (1<<0);			// LED 6		
		_delay_ms(1000);
		PORTC &= ~(1<<0);
		PORTC |= (1<<1);			// LED 7
		_delay_ms(1000);
	}
}

ISR(INT1_vect)
{
	for(int i=0; i<5; i++)
	{
		PORTB |= (0x3F<<0);
		PORTC |= (0x03<<0);
		_delay_ms(1000);
		PORTB &= ~(0x3F<<0);
		PORTC &= ~(0x03<<0);
		_delay_ms(1000);
	}
}


