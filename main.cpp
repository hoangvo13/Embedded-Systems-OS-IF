/*
 * Lab_1.cpp
 *
 * Created: 7/11/2017 11:13:17 AM
 * Author : Hoang Vo
 */ 

/* Define a macro for the delay.h header file. F_CPU is the micro-controller frequency 
value for the delay.h header file
Default value of F_CPU in delay.h header file is 1000000 (1MHz)*/

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// char status[9];

#include <avr/io.h>		// This header defines all the Input/Output Registers for all AVR micro-controllers
#include <util/delay.h>		// This header file defines two functions, _delay_ms (millisecond delay) and _delay_us (microsecond delay)

#define LED_ON PORTB |= (1<<PORTB0)
#define LED_OFF PORTB &= ~(1<<PORTB0)

int main(void)
{
	DDRB = 0xFF;	// Set PORTB as Output
//	DDRB |= (1<<DDB0); // Set Pin 0 of the Port as output
//	DDRB &= ~(1<<DDB1); // Set Pin 1 of Port B as input
//	DDRB &= ~(1<<DDB2); // Set Pin 2 of Port B as input
	DDRD = 0xFF;	// Set PORTD as Output	
/*  // PREVIOUS TASKS
	while (1) 
    {
//		PORTB = 0xFF;		//		
		if (!(PINB & (1<<PINB1))) // check if the pin 1 port B is low (button pressed)
		{
//			PORTB |= (1<<PORTB0); // Turn on the pin 0
			LED_ON;
		}
//		_delay_ms(1000);
//		PORTB = 0x00;
		else
		{
//			PORTB &= ~(1<<PORTB0); // Turn off the pin 0
			LED_OFF;
		}
//		_delay_ms(1000);
    }
*/

//...............................................................

/*  // TASK 5 APPROACH 1	
	while(1)
	{
//		PORTB |= (1<<PORTB1) ;

//		PORTB = 0b10000000;
//		_delay_ms(1000);
//		PORTB = 0b01000000;
//		_delay_ms(1000);
		PORTB = 0b00100000;
		_delay_ms(1000);
		PORTB = 0b00010000;
		_delay_ms(1000);
		PORTB = 0b00001000;
		_delay_ms(1000);
		PORTB = 0b00000100;
		_delay_ms(1000);
		PORTB = 0b00000010;
		_delay_ms(1000);
		PORTB = 0b00000001;
		_delay_ms(1000);
		PORTB = 0b00000000;
	}
*/

//...............................................

/*	// TASK 5 APPROACH 2
	while(1)
	{
		for (int i=0; i<6; i++)
		{
			PORTB = 0x01<<i; // Left shifting the value 0x01 and turning On one 
//			LED at a time from left to right in LED array
//			PORTB = 0x20>>i; 
//			Right shifting the value 0x20 and turning On
//			one LED at a time from right to left in LED array
			_delay_ms(100);
		}
	}
*/
			
//................................................

// Exercise 1
/*
	while(1)
	{
		if (!(PINB & (1<<PINB1))) // check if the pin 1 port B is low (button pressed)
		{
//			PORTB |= (1<<PORTB0); // Turn on the pin 0
			for (int i=0; i<3; i++)
			{
				LED_ON;
				_delay_ms(1000);
				LED_OFF;
				_delay_ms(1000);	
			}
		}
		
		if (!(PINB & (1<<PINB2)))
		{
			for (int y=0; y<10; y++)
			{
				LED_ON;
				_delay_ms(1000);
				LED_OFF;
				_delay_ms(1000);
			}
		}
	}
*/


/*	
	PORTB = 0b01110011;
	PORTB = 0x73;
	PORTB = (1<<0) | (1<<1) |  (1<<4)  | (1<<5) | (1<<6);
	PORTB ^= (1<<7) | (1<<5) | (1<<6);
*/

// Exercise 2

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
//		_delay_ms(1000);
	}
	
// Exercise 3



}

