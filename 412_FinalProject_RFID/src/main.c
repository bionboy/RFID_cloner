/*
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */

#include <asf.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#include "usart_mega.h"

int main (void)
{
	// INIT CLOCK
	sysclk_init();
	
	// INIT BOARD
	board_init();
	
	//const usart_spi_options  SPI_OPTS = {
		//USART_BAUD_9600,
		//
	//};
	
	
	// LOGIC
	
	// TODO: Setup SPI for RFID
	//USART0_Init();
	
	while(1){};
}
