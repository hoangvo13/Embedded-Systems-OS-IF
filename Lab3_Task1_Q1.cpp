/*
 * Question 1.1 - Lab3
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

void Timer_Frequency(uint8_t freq)
{
	TCCR1B |= (1<<WGM12) | (1<<CS12);		      // CTC mode & 256 prescaler
	TIMSK1 |= (1<<OCIE1A);					          // Enable Output compare match A
//	TCCR1A |= (1<<COM1A0);					        // Toggle OC1A on Compare Match
//	TCCR1A |= (1<<COM1A1);					        // Clear OC1A on Compare Match
	TCCR1A |= (1<<COM1A0) | (1<<COM1A1);	    // Set OC1A on Compare Match

	OCR1A = (F_CPU/(freq*2*256)-1);			      // Calculate value for OCR1A
	
}

int main(void)
{
	DDRB |= (1<<0);                           // Set PB0 as OUTPUT
	DDRB |= (1<<1);                           // Set PB1 as OUTPUT

	Timer_Frequency(10);
	sei();
	
    while (1) 
    {

    }
}

ISR(TIMER1_COMPA_vect)
{
	PORTB ^= (1<<0);				// Toggle LED
}

