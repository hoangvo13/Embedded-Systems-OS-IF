/*
 * TASK - Lab 3
 *
 * Lab 3 - EEET2490
 * Semester 2017B
 *
 * Author : Hoang Vo
 * ID : s3575983
 *
 * Created: 8/8/2017 10:11:37 AM
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define TASK 0061

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void Timer_Frequency(uint8_t freq)
{
	TCCR1B |= (1<<WGM12) | (1<<CS12); // Turn on the CTC mode for Timer 1 and the prescaler of 256
	TIMSK1 = (1<<OCIE1A);			  // Enable Output Compare A Match Interrupt
//	OCR1A = 31249;					  // Set CTC compare value to 1Hz at 12 MHz AVR clock, with a prescaler of 256
	OCR1A = (F_CPU/(freq*2*256) - 1); 
	
//	TCCR1A |= (1<<COM1A1) | (1<<COM1A0); // Set OC1A on Compare Match (Set output to HIGH level)
	TCCR1A |= (1<<COM1A0);				 // Toggle OC1A on Compare Match
//	TCCR1A |= (1<<COM1A1);				 // Clear OC1A on Compare Match (Set output to LOW level)
	DDRB |= (1<<1);
}

unsigned int i = 0;

volatile int cliFlag = 0;

// Used for Question 2.1
unsigned int blinkLEDs(unsigned int blink)
{
	if (blink > 0)
	{
		for (int u=0; u<=4; u++)
		{
			PORTD = 0xC0;
			PORTB = 0x3F;
			_delay_ms(1000);
			PORTD = 0x00;
			PORTB = 0x00;
			_delay_ms(1000);
		}
	}
	return 1;
}

int main(void)
{
/*---------------------------------------------------------------- EXERCISE ---------------------------------------------------------------- */

#if TASK == 0061			// TEST CASE for CTC mode in Timer 2
	DDRD |= (1<<3);
	TCCR2A |= (1<<WGM21);								// CTC mode for Timer 2
	TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);		// prescaler of 1024
	TIMSK2 |= (1<<OCIE2B);								// Enable Output compare match B
	TCCR2A |= (1<<COM2B0);								// Toggle
	OCR2B = 100;
	
	sei();
	
	while(1)
	{
	}
	
#endif

#if TASK == 0060			/*--- EXERCISE --- */
	
	TCCR2A |= (1<<WGM21);								// CTC mode for Timer 2
	TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);		// prescaler of 1024
	TIMSK2 |= (1<<OCIE2B);								// Enable Output compare match B
	TCCR2A |= (1<<COM2B0);								// Toggle
	
	OCR2B = 250;
	
	EIMSK |= (1<<INT1);									// Enable INT1
	EICRA |= (1<<ISC10);								// Falling edge
	
	DDRB |= (1<<DDB0);
	DDRB |= (1<<DDB1);
	
	DDRD |= (1<<DDD3);
	
	
	sei();
	
	while(1)
	{
		
	}
	
#endif	
	
	
	
	
/*---------------------------------------------------------------- END EXERCISE ---------------------------------------------------------------- */	
	
	
/*---------------------------------------------------------------- TASK 5 ---------------------------------------------------------------- */

#if TASK == 0051		/*--- TASK 5 --- Question 5.1 ---*/
	DDRB |= (1<<DDB0) | (1<<DDB1) | (1<<DDB2);
	PCICR |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT8) | (1<<PCINT9) | (1<<PCINT10);
	sei();
	while(1)
	{
		
	}
	

#endif


#if TASK == 0050		/*--- TASK 5 --- TASK WORK ---*/

//	DDRB &= ~(1<<DDB2);				// Set PB2 pin (PCINT2) as input to receive interrupt request
	DDRB |= (1<<DDB0);	// Set PB0 as output for LED
	DDRB |= (1<<DDB1);	// Set PB1 as output for LED
	PCICR |= (1<<PCIE0);			// Set PCIE0 to enable PCMSK0 scan
	PCMSK0 |= (1<<PCINT2);			// Set PCINT2 to trigger an interrupt on state change
	sei();							// Turn on the Global Interrupt Enable Bit
	while(1)
	{
		
	} 

#endif

/*-------------------------------------------------------------- END TASK 5 ------------------------------------------------------------- */	

/*---------------------------------------------------------------- TASK 4 ---------------------------------------------------------------- */
#if TASK == 0040		/*--- TASK 4 --- TASK WORK ---*/
	
	DDRD &= ~((1<<DDD3) | (1<<DDD2));		// Clear PD2, PD3 pins for interrupt 0, 1
	DDRB |= (1<<DDB0) | (1<<DDB1);			// Set pins for LEDs
	EICRA |= (1<<ISC11) | (1<<ISC01);		// Set INT0, INT1 to trigger on falling edge
	EIMSK |= (1<<INT1) | (1<<INT0);			// Turns on INT0 and INT1
	PORTB &= ~((1<<PORTB0) | (1<<PORTB1));	// Turns off the LEDs after reset
	sei();									// Turn on global enable interrupt bit
	while(1)
	{
		
	}
	
#endif

/*-------------------------------------------------------------- END TASK 4 ------------------------------------------------------------- */	
	
/*---------------------------------------------------------------- TASK 3 ---------------------------------------------------------------- */	
#if TASK == 0030		/*--- TASK 3 --- TASK WORK ---*/
	
	DDRD &= ~(1<<DDD3);		// Set PD3 pin (INT1) as input to receive interrupt request
	DDRB |= (1<<DDB0);		// Set PB0 as output for LED
	EICRA |= (1<<ISC11);	// Set INT1 to trigger on falling edge
	EIMSK |= (1<<INT1);		// Turns on INT1
	sei();					// Turn on the Global Interrupt Enable Bit
	while(1)
	{
		if (cliFlag == 0)
		{
			sei();			// Turn on interrupts
		}
		else
		{
			cli();
			_delay_ms(2000);
			cliFlag = 0;
		}
	}
	
#endif	
	
/*-------------------------------------------------------------- END TASK 3 ------------------------------------------------------------- */	
	
/*---------------------------------------------------------------- TASK 2 ---------------------------------------------------------------- */	
#if TASK == 0023					// Question 2.3 & 2.4 - Task 2
	
	EIMSK |= (1<<INT0);
	EIMSK |= (1<<INT1);				// Enable INT0 & INT1  
	EICRA |= (1<<ISC01);			// Falling edge in INT0
	EICRA |= (1<<ISC11);			// Falling edge in INT1
	sei();
	DDRB |= (1<<DDB1);
	DDRB |= (1<<DDB0);
	
	while(1)
	{
		PORTB ^= (1<<PORTB1);
		_delay_ms(1000);
	}
	
#endif
	
	
#if TASK == 0021					// Question 2.1 - Task 2
	
//	DDRD &= ~(1<<DDD3);
	EICRA |= (1<<ISC11);
	EIMSK |= (1<<INT1);
	sei();
	DDRB = 0x3F;
	DDRD = 0xC0;
	
	while(1)
	{
		PORTD = 0x00;
		PORTB = 0x01;
		_delay_ms(1000);
		PORTB = 0x02;
		_delay_ms(1000);
		PORTB = 0x04;
		_delay_ms(1000);
		PORTB = 0x08;
		_delay_ms(1000);
		PORTB = 0x10;
		_delay_ms(1000);
		PORTB = 0x20;
		_delay_ms(1000);
		PORTB = 0x00;
		PORTD = 0x40;
		_delay_ms(1000);
		PORTD = 0x80;
		_delay_ms(1000);
	}

#endif


#if TASK == 0020
//	DDRD &= ~(1<<DDD3);		// Set D3 PIN (INT1) as input to receive interrupt request
	DDRB |= (1<<DDB0);		// Set PB0 as output for LED
/* 
	ISC11 ISC10  :   **EICRA**
	  0		0    :	Low level	
	  0     1    :	Logic change
	  1     0    :  Falling edge
	  1     1	 :  Rising edge
*/	
	EICRA |= (1<<ISC11);	
	EIMSK |= (1<<INT1);		// Turns on INT1
	sei();					// Turn on the Global Interrupt Enable Bit
	
	while(1)
	{
			
	}
#endif	
/*--------------------------------------------------------------- END TASK 2 ------------------------------------------------------------- */	
	
	
/*---------------------------------------------------------------- TASK 1 ---------------------------------------------------------------- */	
#if TASK == 0012
	
	DDRB |= (1<<0);
	Timer_Frequency(1);
	sei();
	while(1)
	{
		
	}	

#endif
	
#if TASK == 0011	
	
	DDRB |= (1<<1);
	DDRB |= (1<<0);
	TCCR1B |= (1<<CS12);
	TCCR1B |= (1<<WGM12);
//	OCR1A = 62499; // duration = 1s   .... frequency = 500 mHz
	OCR1A = 31249; // duration = 0.5s .... frequency = 1 Hz
	TCCR1A |= (1<<COM1A0);
	TIMSK1 |= (1<<OCIE1A);
	sei();
 
    while (1) 
    {
		
    }
#endif
/*-------------------------------------------------------------- END TASK 1 --------------------------------------------------------- */

} /*------------------------------------------------------------ END main() ----------------------------------------------------------*/



/*------------------------------------------------------ Interrupt Service Routine ---------------------------------------------------*/

/*
ISR(PCINT1_vect)
{
	if (!(PINC & (1<<PINC0)) == 1)
	{
		PORTB ^= (1<<PORTB0);
	}
	else if (!(PINC & (1<<PINC1)) == 1)
	{
		PORTB ^= (1<<PORTB1);
	}
	else if (!(PINC & (1<<PINC2)) == 1)
	{
		PORTB ^= (1<<PORTB2);
	}
}
*/

/*
ISR(PCINT0_vect)
{
	if (!(PINB & (1<<PINB2)) == 1)	// LOW to HIGH pin change 
	{
		PORTB ^= (1<<PORTB0);		// Toggle the PB0 connecting to the LED
	}
	else                            // HIGH to LOW pin change 
	{
		PORTB ^= (1<<PORTB1);		// Toggle the PB1 connecting to the LED
	}								
}
*/


ISR(INT0_vect)
{
//	PORTB ^= (1<<PORTB1);	// Question 2.3

//	PORTB &= ~(1<<PORTB0);	// Question 2.4 --- Turn off the LED when INT0 is pressed

/* TASK 4 - task work 
	sei();
	PORTB |= (1<<PORTB0);
	_delay_ms(2000);
	PORTB &= ~(1<<PORTB0); 
//*/
}

ISR(INT1_vect)
{
/* TASK 4- task work
	PORTB |= (1<<PORTB1);
	_delay_ms(5000);
	PORTB &= ~(1<<PORTB1);
//*/	
	
/* TASK 3 --- TASK WORK */	
//	PORTB ^= (1<<PORTB0);	// Toggle the PB0 connecting to the LED
//	cliFlag = 1;
		
/* TASK 2 --- Question 2.4*/	
	PORTB ^= (1<<PORTB0);
	
/* TASK 2 --- Question 2.1 --- 8 LEDs */	
//	i++;
//	blinkLEDs(i);
}

ISR(TIMER2_COMPB_vect)
{
//	PORTB ^= (1<<1);
}

ISR(TIMER1_COMPA_vect)
{
//	PORTB ^= (1<<0);
}

