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
unsigned int sec_alarm = 0;
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

/*-------------------------------------------Alarm Preset-------------------------------------------*/
unsigned int alarm_preset(unsigned int alarm_val_temp)
{
	unsigned int alarm;										// 02:20
	if (alarm_val_temp == 140)						        // Use Binary
	{
		alarm ^= (1<<0);
	}
	return alarm;
}											    /*END*/
/*-------------------------------------------Alarm Preset-------------------------------------------*/

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
	if(!(PINB & (1<<PINB0)))
	{
		NoCount = 0;
		min = 0;
		hour = 0;
	}
}											  /*END*/
/*-----------------------------------------Reset Button--------------------------------------------*/

/*--------------------------------------Setting up ADC-LDR-----------------------------------------*/
void ADC_Init(void)
{
	ADMUX |= (1<<REFS0) | (1<<ADLAR);								// Select Reference voltage & ADC Left Adjust result
	ADMUX |= (1<<MUX0) | (1<<MUX2);									// Enable ADC5
	
	ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);					// Set prescaler of 128 - 125kHz sample rate @ 16MHz
	ADCSRA |= (1<<ADATE) | (1<<ADEN) | (1<<ADSC);					// Auto Trigger Enable && ADC Enable && Start Conversion
	ADCSRA |= (1<<ADIE);

	ADCSRB = 0x00;													// Free-running mode
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
//	OCR1A = 100;
}


/*----------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------START MAIN------------------------------------------------------------*/
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
//	TIMSK0 |= (1<<TOIE0);										// Enable Timer0 interrupt
	TIMSK0 |= (1<<OCIE0A);
	TCCR0A |= (1<<WGM01);
	OCR0A = 157;
	
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

//		PORTB = alarm_preset(alarm_val);					// Alarm for Preset Time	
				
		if (TCNT2 >= 157)									// 0.01 Timer2
		{
			TCNT2 = 0;
			NoCount++;
/*-----------------------Button for Setting Minutes-----------------------*/			
			if (!(PINB & (1<<PINB4)))
			{
				_delay_ms(150);
				if (min < 59)
				min++;
				else
				min = 0;
			}
/*-----------------------Button for Setting Minutes-----------------------*/

/*------------------------Button for Setting Hours------------------------*/			
			if (!(PINB & (1<<PINB5)))
			{
				_delay_ms(150);
				if (hour < 23)
				hour++;
				else
				hour = 0;
			}
/*------------------------Button for Setting Hours------------------------*/

/*------------------------------RESET BUTTON------------------------------*/			
			if (!(PIND & (1<<PIND7)))
			{
				NoCount = 0;
				min = 0;
				hour = 0;
			}
/*------------------------------RESET BUTTON------------------------------*/
			
			if (NoCount == 6000)							// 100 = 1s // 6K = 60s
			{
//				alarm_val++;
				min++;
				NoCount = 0;
				if (min == 60)
				{
					hour++;
					min = 0;
					if (hour == 24)
					{
//						alarm_val = 0;
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
//		PORTB &= ~(1<<PORTB1);
		OCR1A = 10;
	}
	else if (ADCH >= 64 && ADCH < 128)
	{
//		PORTB |= (1<<PORTB1);
		OCR1A = 200;
	}
	else if (ADCH >= 128 && ADCH < 192)
	{
		OCR1A = 1500;
	}
	else
	{
		OCR1A = 65000;
	}
}

ISR(TIMER0_COMPA_vect)
{
	alarm_val++;							// 0.016384 -> ~61 = 1s
	if (alarm_val == 100)
	{
		alarm_val = 0;
		sec_alarm++;
		if (sec_alarm == 60)
		{
			min_alarm++;
			sec_alarm = 0;
			if (min_alarm == 0x5A0)
			{
				min_alarm = 0;
			}
		}
	}
	
	if (min_alarm == 0x001E)
	{
		PORTB |= (1<<PORTB3);
	}
	if (min_alarm == 0x0020)
	{
		PORTB &= ~(1<<PORTB3);
	}
}
