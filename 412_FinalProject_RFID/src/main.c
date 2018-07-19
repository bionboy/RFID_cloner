//15 SPI_SS_A Slave A select for SPI

#include <asf.h>
#include <avr/io.h>
#include <stdlib.h>
//#include <util/delay.h>
#include "usart_mega.h"

//// UART CONST ////
#define FOSC 15974400 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

//// SPI CONST ////
#define SPI_DDR DDRB
//#define SS		DDB2
//#define SPCR	SPCR0
//#define SPDR	SPDR0
//#define SPSR	SPSR0

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
	SPI_MasterInit();
}


//// USART ////

void USART_Init(unsigned int ubrr)
{
	// Set baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Set frame format: 8data, 2stop bit
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
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
	SPI_DDR = (1 << SPI_MOSI) | (1 << SPI_SCK);
	// Enable SPI, Master, set clock rate fck/16
	SPCR0 = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_MasterTransmit(char cData)
{
	// Start transmission
	SPDR0 = cData;
	// Wait for transmission complete
	while(!(SPSR0 & (1 << SPIF))){};
}

char SPI_MasterReceive()
{
	// Wait for reception complete
	while(!(SPSR0 & (1 << SPIF))){};
	// Return Data Register	return SPDR0;
}