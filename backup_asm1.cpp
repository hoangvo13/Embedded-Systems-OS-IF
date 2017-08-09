/*
 * digital_clock_project.cpp
 *
 * Created: 8/3/2017 6:04:10 PM
 * Author : Hoang
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// DDR control 
#define digitDDR DDRC
#define AtoFletterDDR DDRB
#define GtoHletterDDR DDRD

// PORT control
#define digitPORT PORTC
#define AtoFletterPORT PORTB
#define GtoHlettterPORT PORTD

// unsigned int d1 = 0; // Digit 1 =>> 1st number for hours
// unsigned int d2 = 0; // Digit 2 =>> 2nd number for hours
// unsigned int d3 = 0; // Digit 3 =>> 1st number for minutes 
// unsigned int d4 = 0; // Digit 4 =>> 2nd number for minutes

unsigned char second = 0;
unsigned char minute = 0;
unsigned char hour = 0;

unsigned char setDigit(unsigned char value);
unsigned char checkValue(unsigned char mins, unsigned char hours);
ISR(TIMER1_COMPA_vect);

//				    0	  1     2     3     4     5     6     7     8     9 
char status[20] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

int main(void)
{
    // Timer 1
//	OCR1A = 15624;
	TCCR1B |= (1 << CS12) | (1 << CS00);	// Set prescaler to 1024 and start the timer
//	TCCR1B |= (1 << WGM12); 				// Mode 4, CTC on OCR1A
//	TIMSK1 |= (1 << OCIE1A);				// Set interrupt on compare match
	
//	sei();									// enable interrupts
	
    AtoFletterDDR = 0x3F;					// Set PB0 -> PB5 as OUTPUT .... A to F
	GtoHletterDDR = 0xC0;					// Set PD6 PD7 as OUTPUT	.... G & H
	digitDDR = 0x1E;						// Set PC1 -> PC4 as OUTPUT .... PC1 PC2 / PC3 PC4 .... hh/mm
	
	unsigned char NoCount = 0;

	while(1)
	{
		if (TCNT1 >= 15624)
		{
			TCNT1 = 0;
			NoCount++;
			
		}
		
		
		digitPORT = 0x10;
		AtoFletterPORT = setDigit(minute%10);
		GtoHlettterPORT = setDigit(minute%10);
		_delay_us(10);
		
		digitPORT = 0x08;
		AtoFletterPORT = setDigit(minute/10);
		GtoHlettterPORT = setDigit(minute/10);
		_delay_us(10);
		
		digitPORT = 0x04;
		AtoFletterPORT = setDigit(hour%10);
		GtoHlettterPORT = setDigit(hour%10);
		_delay_us(10);
		
		digitPORT = 0x02;
		AtoFletterPORT = setDigit(hour/10);
		GtoHlettterPORT = setDigit(hour/10);
		_delay_us(10);
	}
	
}


unsigned char checkValue(unsigned char mins, unsigned char hours)
{
	
}


unsigned char setDigit(unsigned char value)
{
	unsigned char segDisplay;
	switch(value)
	{
		case 0: segDisplay = ~status[0];
				break;	
		case 1: segDisplay = ~status[1];
				break;
		case 2: segDisplay = ~status[2];
				break;
		case 3: segDisplay = ~status[3];
				break;
		case 4: segDisplay = ~status[4];
				break;
		case 5: segDisplay = ~status[5];
				break;
		case 6: segDisplay = ~status[6];
				break;
		case 7: segDisplay = ~status[7];
				break;
		case 8: segDisplay = ~status[8];
				break;
		case 9: segDisplay = ~status[9];
				break;
	}
	return segDisplay;
}


/*
ISR(TIMER1_COMPA_vect)
{
	second++;
	if (second == 60)
	{
		minute++;
		second = 0;
		if (minute > 60)
		{
			minute = 0;
		}
	}
}
*/
