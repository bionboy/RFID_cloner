/*
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
//
//15 SPI_SS_A Slave A select for SPI
//16 SPI_MOSI Master out slave in line of serial peripheral interface
//17 SPI_MISO Master in slave out line of serial peripheral interface
//18 SPI_SCK Clock for serial peripheral interface

#include <asf.h>
#include <avr/io.h>
#include <stdlib.h>
//#include <util/delay.h>

#include "usart_mega.h"

// UART
//#define FOSC 1843200 // Clock Speed
#define FOSC 15974400 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

// SPI pins
#define DDR_SPI DDRB
#define SS		DDB2
#define DD_MOSI DDB3
#define DD_MISO DDB4
#define DD_SCK	DDB5
#define SPCR	SPCR0
#define SPDR	SPDR0
#define SPSR	SPSR0

void Setup(void);
void USART_Init(unsigned int);
void USART_Transmit(unsigned char data);
void USART_PutS(const char *str);
unsigned char USART_Receive(void);
void SPI_MasterInit(void);
void SPI_MasterTransmit(char);
char SPI_MasterReceive(void);

int main (void)
{
	cli();
	Setup();
	sei();
	
	while(1)
	{
		//USART_Transmit(SPI_MasterReceive());
		
		USART_Transmit('F');
		
		//SPI_MasterTransmit(0xff);
	};
}


void Setup()
{
	// INIT CLOCK
	sysclk_init();
	// INIT BOARD
	board_init();	// INIT USART	USART_Init(MYUBRR);	// INIT SPI
	//SPI_MasterInit();
}


//// USART ////

void USART_Init(unsigned int ubrr)
{
	// Set baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// Set frame format: 8data, 2stop bit
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}void USART_Transmit(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1 << UDRE0))){};
	/* Put data into buffer, sends the data */
	UDR0 = data;
}unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1 << RXC0))){};
	/* Get and return received data from buffer */
	return UDR0;
}void USART_PutS(const char *str){	while(*str){		USART_Transmit(*str);	}}//// SPI ////
void SPI_MasterInit()
{
	// Set MOSI and SCK output, all others input
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
	// Enable SPI, Master, set clock rate fck/16
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_MasterTransmit(char cData)
{
	// Start transmission
	SPDR = cData;
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF))){};
}

char SPI_MasterReceive()
{
	// Wait for reception complete
	while(!(SPSR & (1<<SPIF))){};
	// Return Data Register	return SPDR;
}