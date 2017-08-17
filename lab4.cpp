/*
 * Tasks - Lab 4
 *
 * Lab 4 - EEET2490
 * Semester: 2017B
 *
 * Author : Hoang Vo
 * ID : s3575983
 * 
 * Created: 8/12/2017 10:05:02 AM
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define TASK 001

void PWM_Init(void)
{
	TCCR1B |= (1<<WGM12);					 
//	TCCR1B |= (1<<CS12) | (1<<CS10);		// CTC mode for Timer 1 and prescaler of 1024
	TCCR1B |= (1<<CS11);
	TCCR1A |= (1<<WGM11) | (1<<WGM10);		// Set up the Fast PWM, 10-bit (Mode 7)
//	TCCR1B |= (1<<WGM13);
	
	TCCR1A |= (1<<COM1A1);					// Clear OC1A on the Compare Match, set OC1A/OC1B at BOTTOM (non-inverting mode)
//	TCCR1A |= (1<<COM1A0);
//	TCCR1A |= (1<<COM1B1);
	
	OCR1A = 200; // Set duty cycle
	
//	OCR1B = 32; // Set the duty cycle = OCR1A/OCR1B
}

int main(void)
{
	
#if TASK == 004
	DDRB |= (1<<1); // Set PINB1 as Output
	PWM_Init();

	while(1)
	{
//		PORTB |= (1<<PORTB0);
	}
	
#endif
	
	
#if TASK == 003
	DDRB |= (1<<0);
//	sei();
	PWM_Init();
	while(1)
	{
		
	}
#endif
	

#if TASK == 001	
	DDRB	|= (1<<DDB0) | (1<<DDB1) | (1<<DDB2);					// PB0 and PB1 for two LEDs
	
	ADMUX	|= (1<<REFS0);									// Reference Selection : AVcc with external capacitor at AREF pin
	ADMUX	|= (1<<ADLAR);									// ADC Left Adjusted to be 8-bit
//	ADMUX   |= (1<<MUX2);															// ADC PC0 -> MUX[3:0] = 0000
	
	ADCSRA	|= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);		// Set prescaler of 128 - 125kHz sample rate @ 16MHz
	ADCSRA	|= (1<<ADATE);									// ADC Auto Trigger Enable
	ADCSRA	|= (1<<ADEN);									// Enable the ADC
	ADCSRA	|= (1<<ADSC);									// Start conversion
//	ADCSRA  |= (1<<ADIE);									// ADC Interrupt Enable
															
	ADCSRB	 = 0x00;										// Set ADC to Free-Running Mode
//	sei();
	
	
//	unsigned int adc_value;
	
	while (1) 
    {
		if(ADCH <= 85)
		{
			PORTB &= ~(1<<PORTB0);
			PORTB |= (1<<PORTB1); 
			PORTB |= (1<<PORTB2);
		}
		else if((ADCH < 171) && (ADCH > 85))
		{
			PORTB &= ~(1<<PORTB1);
			PORTB |= (1<<PORTB0);  
			PORTB |= (1<<PORTB2);
		}
		else if(ADCH >= 171)
		{
			PORTB &= ~(1<<PORTB2);
			PORTB |= (1<<PORTB0);
			PORTB |= (1<<PORTB1);
		}
		
				
    }
#endif


	


}

/*
ISR(TIMER1_COMPA_vect)
{
	PORTB |= (1<<PORTB0);
}

ISR(TIMER1_COMPB_vect)
{
	PORTB &= ~(1<<PORTB0);
}
*/


/*
ISR(ADC_vect)
{
	if (ADCH < 128)
	{
		PORTB |= (1<<PORTB0);	// Turn on LED0
		PORTB &= ~(1<<PORTB1);	// Turn off LED1
	}
	else
	{
		PORTB &= ~(1<<PORTB0);
		PORTB |= (1<<PORTB1);
	}
}
*/

