/*
 * Exercise 3 - Lab 1
 *
 * Lab 1 - EEET2490
 * Semester: 2017B
 *
 * Author : Hoang Vo
 * ID : s3575983
 *
 * Created: 7/21/2017 11:20:44 PM
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#define LED_ON PORTB |= (1<<PORTB0)
#define LED_OFF PORTB &= ~(1<<PORTB0)

int main(void)
{
     while (1)
     {
	     if (!(PINB & (1<<PINB1))) // check if the pin 1 port B is low (button pressed)
	     {
			 _delay_us(10000); // delay for 0.01 second
			 
			 if (!(PINB & (1<<PINB1))) // After delay in microsecond => check again to make sure the input pin is still low (no fluctuation)
			 {
				for (int i=0; i<3; i++) // Blinking the LED 3 times with 1Hz frequency
				{
					LED_ON;
					_delay_ms(1000);
					LED_OFF;
					_delay_ms(1000);
				}
			 }
	     }
	 }
}

