#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "GPL/GPL_circular_buffer.h"
#include "GPL/GPL_scratch_pad_protocle.h"



GPL_ScratchPad_t scratchPad = { 0 };
GPL_CircularBuffer_t RX_circularBuffer = { 0 };
GPL_CircularBuffer_t TX_circularBuffer = { 0 };

void USART_Transmit( char data ) {
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)))
	;
	/* Put data into buffer, sends the data */
	UDR = data;
}

void USART_Transmit_buffer( char *data ) {
	while( *data )
		USART_Transmit( *data++ );
}


int main(void) {
	char scratchPad_memory[32] = { 0 };
	scratchPad = GPL_ScratchPad_create( scratchPad_memory, sizeof( scratchPad_memory ) );
	
	uint8_t RX_circularBuffer_size = 32;
	RX_circularBuffer = GPL_CircularBuffer_create( RX_circularBuffer_size );
	GPL_CircularBuffer_reset( RX_circularBuffer );
	
	uint8_t TX_circularBuffer_size = 32;
	TX_circularBuffer = GPL_CircularBuffer_create( TX_circularBuffer_size );
	GPL_CircularBuffer_reset( TX_circularBuffer );
	

	/* UART Init */
	// Baud 9600kbs on 8MHz clock
	// Frame 8n1
	// Global interrupts on RX complete
	uint16_t baud = 51UL;
	UBRRH = (uint8_t)( baud >> 8 );
	UBRRL = (uint8_t)baud;
	UCSRB = ( 1 << RXEN ) | ( 1 << TXEN ) | ( 1 << RXCIE );
	UCSRC = ( 1 << URSEL ) | ( 1 << UCSZ1 ) | ( 1 << UCSZ0 );
	
	/* Counter0 Init */
	// CTC Mode
	// clk/256
	// Every 100Hz
	TCCR0 |= ( 1 << WGM01 ) | ( 1 << CS02 );
	TIMSK |= ( 1 << OCIE0 );
	OCR0 = 155;

	sei();
	
	DDRA = 0xFF;
	for( uint8_t i = 0 ; i < sizeof(scratchPad_memory); i++)
		scratchPad_memory[i] = i;
	
	while (1)
	{
		_delay_ms(500);
		PORTA ^= ( 1 << PA0 );
		
	}
}

ISR(USART_RXC_vect) {
	/* Read data from register */
	char RX_byte = UDR;
	GPL_CircularBuffer_write( RX_circularBuffer, RX_byte );
}

ISR(TIMER0_COMP_vect) {
	if( !GPL_CircularBuffer_empty( TX_circularBuffer ) ) {
		char TX_byte = GPL_CircularBuffer_read( TX_circularBuffer );
		USART_Transmit(TX_byte);
		return;
	}
	
	if( !GPL_CircularBuffer_empty( RX_circularBuffer ) ) {
		char byte = GPL_CircularBuffer_read( RX_circularBuffer );
		if( GPL_ScratchPad_processByte( scratchPad, byte ) ){
			char value = GPL_ScratchPad_read( scratchPad, byte );
			char buffer[32] = "\0";
			snprintf(buffer, sizeof(buffer), "0x%x:0x%x\r\n", byte, value);
			
			char *buffer_ptr = buffer;
			while(*buffer_ptr)
				GPL_CircularBuffer_write( TX_circularBuffer,*buffer_ptr++ );
		}
	}
	

}
