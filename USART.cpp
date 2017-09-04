/*
 * Task 2 - Lab5
 * Interrupt 
 *
 * EEET2490
 * 2017B
 *
 * Author : Hoang Vo
 * ID : s3575983
 *
 * Created: 8/28/2017 7:50:20 PM 
 */ 

#ifndef F_CPU
#define F_CPU 12000000UL
#endif

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


int main(void)
{
	UBRRH = (BAUD_PRESCALE>>8);	// Load upper 8-bit of the baud rate value into the high byte of the UBRR register
	UBRRL = BAUD_PRESCALE;		// Load lower 8-bit of the baud rate value into the low byte of the UBRR register
	UCSRC |= (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);	// Use 8-bit characters for data
	UCSRB |= (1<<RXEN) | (1<<TXEN);	// Turn on the transmission and reception hardware
 
	UCSRB |= (1<<RXCIE);		// Enable RX Complete Interrupt
	sei();						// Turn on the Global Interrupt Enable bit
 
	char ReceivedByte;	// Variable to store the data (1 byte) read from the register
 
    while (1) 
    {
//		while((UCSRA & (1<<RXC)) == 0) {}; // Wait till data received and ready to be read from UDR
//		ReceivedByte = UDR;	// Read the received byte value	
		
//		while((UCSRA & (1<<UDRE)) == 0) {}; // Wait till UDR is ready for more data to be written to it
//		UDR = ReceivedByte; // Write (send) back the data to the computer - same content received earlier
    }
}

ISR(USART_RXC_vect)
{
	char ReceivedByte;
	ReceivedByte = UDR;	// Read the received byte value
	UDR = ReceivedByte;	// Write (send) back the data to the computer - same content received earlier
}


