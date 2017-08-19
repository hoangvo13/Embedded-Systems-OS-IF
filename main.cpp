/*
 * LDR_Adjust.cpp
 *
 * Created: 8/17/2017 1:20:11 PM
 * Author : Hoang
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void PWM_Init(void)
{
	TCCR1B |= (1<<WGM12);
	TCCR1A |= (1<<WGM11) | (1<<WGM10);
	
	TCCR1B |= (1<<CS11);
	TCCR1A |= (1<<COM1A1);
	
//	OCR1A = 200;
	
}

int main(void)
{
    DDRB |= (1<<DDB0) | (1<<DDB1) | (1<<DDB2);
	
	ADMUX |= (1<<REFS0) | (1<<ADLAR);
	
	ADCSRA |= (1<ADPS0) | (1<<ADPS1) | (1<<ADPS2);
	ADCSRA |= (1<<ADATE) | (1<<ADEN) | (1<<ADSC);
	
	ADCSRB = 0x00;
	
	PWM_Init();
	
//	OCR1A = 100;
	
    while (1) 
    {
//		PORTB &= ~(1<<PORTB0);
		if(ADCH < 128)
		{
			PORTB &= ~(1<<0);
			OCR1A = 100;
		}
		else
		{
			PORTB |= (1<<0);
			OCR1A = 5000;
		}
    }
}

