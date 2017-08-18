/*
 * Digital Clock project
 * 
 * Assignment 1 - EEET2490
 * Semester: 2017B
 *
 * Author : Hoang Vo
 * ID : s3575983
 *
 * Created: 9/8/2017 22:18 PM
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Define DDR Registers
#define CtrSegsB	DDRB

// Define PORT Registers
#define segsB		PORTB
#define segsD		PORTD

unsigned int NoCount = 0;
unsigned int min = 0;
unsigned int hour = 0;

/*---------------------------------Bits Value to Display on 7-Segment---------------------------------*/
unsigned int number[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
/*---------------------------------------End Array Declaration----------------------------------------*/
		
/*-----------------------------------------Displaying A to F------------------------------------------*/	
unsigned int display(unsigned int time_val)
{
	unsigned int segs_temp = ~number[0];
	switch(time_val)
	{
		case 0: segs_temp = ~number[0];
				break;					
		case 1: segs_temp = ~number[1];
				break;
		case 2: segs_temp = ~number[2]; 
				break;
		case 3: segs_temp = ~number[3];	
				break;
		case 4: segs_temp = ~number[4];			
				break;
		case 5: segs_temp = ~number[5];	
				break;
		case 6: segs_temp = ~number[6];		
				break;
		case 7: segs_temp = ~number[7];			
				break;
		case 8: segs_temp = ~number[8];	
				break;
		case 9: segs_temp = ~number[9]; 
				break;
	}	
	return segs_temp;
}												/*END*/
/*-----------------------------------------Displaying A to F------------------------------------------*/

/*-------------------------------------------Displaying G---------------------------------------------*/
unsigned int displayG(unsigned int val)
{
	unsigned int segs_tempG; 
	
	if (val == 0)
	{
		segs_tempG |= (1<<6);
	}
	else if (val == 1)
	{
		segs_tempG |= (1<<6);
	}
	else if (val == 7)
	{
		segs_tempG |= (1<<6);
	}
	else segs_tempG &= ~(1<<6);
	
	return segs_tempG;
}											   /*END*/
/*-------------------------------------------Displaying G---------------------------------------------*/

/*-------------------------------------------Alarm Preset-------------------------------------------*/
unsigned int alarm_preset(unsigned int hh, unsigned int mm)
{
	unsigned int alarm;
	if (mm == 10 && hh == 01)
	{
		alarm |= (1<<7);
	}
	return alarm;
}											   /*END*/
/*------------------------------------------Alarm Preset----------------------------------------*/

/*----------------------------------------Displaying 4 Digits---------------------------------------*/
unsigned int display_digits(unsigned int digit)
{
	unsigned int disgit;
	switch(digit)
	{
		case 0: disgit |= (0x1<<0);
				break;
		case 1: disgit |= (0x2<<0);
				break;
		case 2: disgit |= (0x4<<0);
				break;
		case 3: disgit |= (0x8<<0);
	}
	return disgit;
}											   /*END*/
/*---------------------------------------Displaying 4 Digits-------------------------------------*/

/*-----------------------------------------Reset Button--------------------------------------------*/
void reset_button(void) 
{
	if(!(PINC & (1<<PINC4)))
	{
		NoCount = 0;
		min = 0;
		hour = 0;
	}
}											  /*END*/
/*-----------------------------------------Reset Button------------------------------------------*/

/*--------------------------------------Setting up ADC-LDR-----------------------------------------*/
void ADC_Init(void)
{
	ADMUX |= (1<<REFS0) | (1<<ADLAR);								// Select Reference voltage & ADC Left Adjust result
	ADMUX |= (1<<MUX0) | (1<<MUX2);									// Enable ADC5
	
	ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);					// Set prescaler of 128 - 125kHz sample rate @ 16MHz
	ADCSRA |= (1<<ADATE) | (1<<ADEN) | (1<<ADSC);					// Auto Trigger Enable && ADC Enable && Start Conversion
	
	ADCSRB = 0x00;													// Free-running mode
}											 /*END*/
/*--------------------------------------Setting up ADC-LDR---------------------------------------*/

/*-----------------------------------------Brightness--------------------------------------------*/
void check_brightness(void)
{
	if (ADCH < 128)
	{
		PORTC |= (1<<PORTC4);
	}
	else
	{
		PORTC &= ~(1<<PORTC4);
	}
}											 /*END*/
/*-----------------------------------------Brightness--------------------------------------------*/

/*------------------------------------------------------------START MAIN------------------------------------------------------------*/
int main(void)
{
	DDRC |= (1<<DDC0) | (1<<DDC1) | (1<<DDC2) | (1<<DDC3);	// PORTC 0 -> 3 for 4 Digits
	CtrSegsB  = 0x3F;										// PORTB 0 -> 5 for A to F
	DDRD |= (1<<DDD6);										// PORTD 6		for G
//	DDRC |= (1<<DDC4);										// ALARM for Preset time
	DDRC &= ~(1<<DDC4);										// Reset-time Button
	
// External Interrupt INT0
	EIMSK |= (1<<INT0);
	EICRA |= (1<<ISC01);
	
	// Timer 1
	TCCR1B |= (1<<CS12) | (1<<CS10);						// 1024


	ADC_Init();
	
	sei();
	
	while(1)
	{
		PORTC = display_digits(0);							// Displaying 1st Digit
		segsB  = display(min%10);
		PORTD  = displayG(min%10);
		_delay_us(50);
		
		PORTC = display_digits(1);							// Displaying 2nd Digit
		segsB  = display(min/10);
		PORTD  = displayG(min/10);
		_delay_us(50);
		
		PORTC = display_digits(2);							// Displaying 3rd Digit
		segsB  = display(hour%10);
		PORTD  = displayG(hour%10);
		_delay_us(50);

		PORTC = display_digits(3);							// Displaying 4th Digit
		segsB  = display(hour/10);
		PORTD  = displayG(hour/10);
		_delay_us(50);

		PORTD = alarm_preset(hour,min);						// Alarm for Preset Time
		reset_button();
//		check_brightness();
				
		if (TCNT1 >= 31249)									// 31249 = 2s .... 15624 = 1s
		{
			TCNT1 = 0;
			NoCount++;
			if (NoCount == 1)
			{
				min++;
				NoCount = 0;
				if (min == 60)
				{
					hour++;
					min = 0;
					if (hour == 24)
					{
						hour = 0;
					}
				}
			}	
		}	
	}
}/*-------------------------------------------------------------- END MAIN --------------------------------------------------------------*/ 


ISR(TIMER1_COMPA_vect)
{
	if (NoCount == 20)
	{
		PORTD |= (1<<PORTD7);
	}
}


ISR(INT0_vect)
{
	PORTC ^= (1<<PORTC5);
}


// back up
//		PORTC |= (1<<PORTC0);
//		PORTC &= ~((1<<PORTC1) | (1<<PORTC2) | (1<<PORTC3));
//		PORTC |= (1<<PORTC1);
//		PORTC &= ~((1<<PORTC0) | (1<<PORTC2) | (1<<PORTC3));
//		PORTC |= (1<<PORTC2);
//		PORTC &= ~((1<<PORTC0) | (1<<PORTC1) | (1<<PORTC3));
//		PORTC |= (1<<PORTC3);
//		PORTC &= ~((1<<PORTC0) | (1<<PORTC1) | (1<<PORTC2));
