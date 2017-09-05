/*
 * LCD_Display_new.c
 *
 * Created: 9/4/2017 10:22:16 PM
 * Author : Hoang Vo
 */ 

#ifdef F_CPU
#define F_CPU 12000000UL
#endif

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"

volatile char ReceivedByte;
volatile char WriteByte = 'b';


void USART_Init()
{
	UBRRH = (BAUD_PRESCALE>>8);						// Load upper 8-bit of the baud rate value into the high byte of the UBRR register
	UBRRL = BAUD_PRESCALE;							// Load lower 8-bit of the baud rate value into the low byte of the UBRR register
	UCSRC |= (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);	// Use 8-bit characters for data
	UCSRB |= (1<<RXEN) | (1<<TXEN);					// Turn on the transmission and reception hardware
//	UCSRB |= (1<<RXCIE);							// Turn on the Global Interrupt Enable bit
}

void char_display(char temp)
{
	char t = temp;	
	lcd_clrscr();				// clear screen of LCD
	lcd_home();					// bring cursor to 0,0	
	lcd_putc(t);				// type something random
	_delay_ms(50);				// wait 50ms
//	lcd_gotoxy(0,1);
//	lcd_command(temp);
//	lcd_puts("Transmitter");
}

int main(void)
{
	DDRA &= ~(1<<0);
	MCUCR |= (1<<ISC11);
	GICR |= (1<<INT1);
	
	USART_Init();
    lcd_init(LCD_DISP_ON_CURSOR);
	sei();
	
	while(1)
    {
		while((UCSRA & (1<<RXC)) == 0) {};			// Wait till data received and ready to be read from UDR
		ReceivedByte= UDR;
		char_display(ReceivedByte);
    }
}

ISR(INT1_vect)
{
	while((UCSRA & (1<<UDRE)) == 0) {};				// Wait till UDR is ready for more data to be written to it
	UDR = WriteByte;
}

ISR(USART_RXC_vect)
{
	ReceivedByte = UDR;
}

