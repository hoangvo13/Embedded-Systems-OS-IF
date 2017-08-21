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

unsigned int NoCount = 0;
unsigned int min = 0;
unsigned int hour = 0;

unsigned int alarm_val = 0;
unsigned int min_alarm = 0;

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

/*----------------------------------------Displaying 4 Digits-----------------------------------------*/
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
				break;
	}
	return disgit;
}											   /*END*/
/*---------------------------------------Displaying 4 Digits---------------------------------------*/

/*--------------------------------------Setting up ADC-LDR-----------------------------------------*/
void ADC_Init(void)
{
	ADMUX |= (1<<REFS0) | (1<<ADLAR);					// Select Reference voltage & ADC Left Adjust result
	ADMUX |= (1<<MUX0) | (1<<MUX2);						// Enable ADC5
	
	ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);		// Set prescaler of 128 - 125kHz sample rate @ 16MHz
	ADCSRA |= (1<<ADATE) | (1<<ADEN) | (1<<ADSC);		// Auto Trigger Enable && ADC Enable && Start Conversion
	ADCSRA |= (1<<ADIE);

	ADCSRB = 0x00;										// Free-running mode
}											 /*END*/
/*--------------------------------------Setting up ADC-LDR---------------------------------------*/

/*-----------------------------------------Brightness--------------------------------------------*/
void check_brightness(void)
{
	if (ADCH < 128)
	{
		PORTB |= (1<<PORTB1);
	}
	else
	{
		PORTB &= ~(1<<PORTB1);
	}
}											 /*END*/
/*-----------------------------------------Brightness--------------------------------------------*/

void PWM_Init(void)
{
	TCCR1B |= (1<<WGM12);
	TCCR1A |= (1<<WGM11) | (1<<WGM10);
	TCCR1B |= (1<<CS11);
	TCCR1A |= (1<<COM1A1);
}

/*----------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------START MAIN------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/
int main(void)
{
	DDRC |= (1<<DDC0) | (1<<DDC1) | (1<<DDC2) | (1<<DDC3);		// PORTC 0 -> 3 for 4 Digits
	DDRD  = 0x7F;												// PORTD 0 -> 7 for 7 segments
	DDRB |= (1<<DDB0);											// LED for Alarm-Preset time
	DDRB |= (1<<DDB1);											// LED for Brightness											
	DDRB |= (1<<DDB3);											// might be used as indicator for temperature

	DDRB &= ~(1<<DDD7);											// Reset-Button
	DDRB &= ~(1<<DDD4);											// Button for Setting Minutes
	DDRB &= ~(1<<DDD5);											// Button for Setting Hours
	
	TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);				// Timer2 prescaler of 1024 used for counting real time
	
	TCCR0B |= (1<<CS02) | (1<<CS00);							// Timer0 prescaler of 1024 used for setting alarm-preset time
	TIMSK0 |= (1<<OCIE0A);
	TCCR0A |= (1<<WGM01);
	OCR0A = 155;
	
	ADC_Init();													// LDR		
	PWM_Init();													// Brightness		
	
	sei();														// Enable global interrupt
	
	while(1)
	{	
		PORTC = display_digits(0);							// Displaying 1st Digit
		PORTD  = display(min%10);							// Converting minute value to the 1st Digit
		_delay_us(50);
		
		PORTC = display_digits(1);							// Displaying 2nd Digit
		PORTD  = display(min/10);							// Converting minute value to the 2nd Digit
		_delay_us(50);
		
		PORTC = display_digits(2);							// Displaying 3rd Digit
		PORTD  = display(hour%10);							// Converting hour value to the 3rd Digit
		_delay_us(50);

		PORTC = display_digits(3);							// Displaying 4th Digit
		PORTD  = display(hour/10);							// Converting hour value to the 4th Digit
		_delay_us(50);
				
		if (TCNT2 >= 155)									// 0.01s Timer2
		{
			TCNT2 = 0;
			NoCount++;
			if (!(PINB & (1<<PINB4)))					/*-----------------------Button for Setting Minutes-----------------------*/
			{
				_delay_ms(150);
				if (min < 59)
				{
					min++;
				}
				else
				{
					min = 0;
				}
			}
			if (!(PINB & (1<<PINB5)))					/*------------------------Button for Setting Hours------------------------*/		
			{
				_delay_ms(150);
				if (hour < 23)
				{
					hour++;
				}
				else
				{
					hour = 0;
				}

			}
			if (!(PIND & (1<<PIND7)))					/*------------------------------RESET BUTTON------------------------------*/
			{
				NoCount = 0;
				min = 0;
				hour = 0;
				alarm_val = 0;
				min_alarm = 0;
			}
			
			if (NoCount == 6000)							// 100 = 1s // 6k = 60s
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

ISR(ADC_vect)
{
	if (ADCH < 64)
	{
		OCR1A = 50;
	}
	else if (ADCH >= 64 && ADCH < 128)
	{
		OCR1A = 300;
	}
	else if (ADCH >= 128 && ADCH < 192)
	{
		OCR1A = 1000;
	}
	else
	{
		OCR1A = 65000;
	}
}

ISR(TIMER0_COMPA_vect)
{
	alarm_val++;
	if (alarm_val == 6000)				// 0.01s -> 100 = 1s -> 6000 = 60s
	{
		alarm_val = 0;
		min_alarm++;
		if (min_alarm == 0x5A0)			// 1440 minutes = 24 hours...... preset_time = (hour*60 + minute)
		{
			min_alarm = 0;
		}
	}
/*---------Preset Time---------*/	
	if (min_alarm == 0x008C)			// 140 mins
	{
		PORTB |= (1<<PORTB3);
	}
	if (min_alarm == 0x008E)			// 31
	{
		PORTB &= ~(1<<PORTB3);
	}
}


