//15 SPI_SS_A Slave A select for SPI

//#define F_CPU 16000000 // Clock Speed
#define F_CPU 15974400 // Clock Speed
//#define F_CPU 8000000 // Clock Speed

#include <asf.h>
#include <avr/io.h>
//#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include "usart_mega.h"


////////////        UART CONST        ////////////
//#define F_CPU 16000000 // Clock Speed
#define BAUD 9600
//#define MYUBRR F_CPU / (9600 * 16UL) - 1 
#define MYUBRR 0x67

////////////        SPI  CONST        ////////////
#define SPI_DDR DDRB
//#define SS	DDB2
//#define SPCR	SPCR0
//#define SPDR	SPDR0
//#define SPSR	SPSR0

void Setup(void);
void USART_Init(void);
void USART_Transmit(unsigned char data);
void USART_PutS(const char *str);
unsigned char USART_Receive(void);
void SPI_MasterInit(void);
void SPI_MasterTransmit(char);
void SPI_ScanCard(void);
char SPI_MasterReceive(void);
char SPI_Trancieve(char);

int main (void)
{
	//cli();
	Setup();
	//sei();
	
	//
	//SPI_MasterTransmit(0b00000010); // Write command
	//SPI_MasterTransmit(0b00001100);	// Command = readcard
	char i = 0;
	int data = 0;
	
	while(1)
	{
		//USART_PutS("Hello");
		//USART_Transmit(SPI_MasterReceive());
		
		//USART_Transmit(SPI_Trancieve(USART_Receive()));
		//
		SPI_MasterTransmit(0b00000010); // Write command
		SPI_MasterTransmit(0b00001100);	// Command = mem
    	//SPI_MasterTransmit(0b00001100);	// Command = trans
		data = SPI_MasterReceive();
		i = data;
		//printf("%d", data);

		USART_Transmit(data);
		
		//if (data > 5){
			//USART_PutS("2");
		//}
		//else if (data > 3){
			//USART_PutS("1");
		//}
		//else if (data > 1){
		//USART_PutS("0");
		//}
		//else
		//{
		//USART_PutS("9");
		//}
		//
		//USART_Transmit(i++);
		
		//_delay_ms(500);
	};
}

void Setup()
{
	// INIT clock, board, USART, and SPI
	//sysclk_init();
	board_init();	USART_Init();	SPI_MasterInit();
}

////////////        USART        ////////////

void USART_Init()
{  
	// Set baud rate
	UBRR0H = (MYUBRR >> 8);
	UBRR0L = MYUBRR;
	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Set frame format: 8data, 2stop bit
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}void USART_Transmit(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1 << UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
}unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1 << RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}void USART_PutS(const char *str){	while(*str){		USART_Transmit(*str++);	}}////////////        SPI        ////////////
void SPI_MasterInit()
{
	// Set MOSI and SCK output, all others input
	SPI_DDR = SPI_MOSI | SPI_SCK;
	//DDRB = (1 << DDRB4) | (1 << DDRB5);
	
	// Enable SPI, Master, set clock rate fck/16
	SPCR0 = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	
	//PORTB = (0 << PORTB4);
}

void SPI_MasterTransmit(char cData)
{
	// Start transmission
	SPDR0 = cData;
	// Wait for transmission complete
	while(!(SPSR0 & (1 << SPIF))){};
}

void SPI_ScanCard(){
	SPI_MasterTransmit(1);
	SPI_MasterTransmit(1);
	SPI_MasterTransmit(0);
	SPI_MasterTransmit(0);
}

char SPI_MasterReceive()
{
	// Wait for reception complete
	while(!(SPSR0 & (1 << SPIF))){};
	// Return Data Register	return SPDR0;
}


char SPI_Trancieve(char data)
{
	SPDR0 = data;
	while(!(SPSR0 & (1 << SPIF)));
	return SPDR0;
}