﻿/*
 * USART_driver.c
 *
 * Driver for the USART. Controls the data in SRAM for the 7 segment display.
 * Transmits a response after every incoming message.
 *
 * Created: 10/11/2020
 * Author: Emmanouil Petrakos
 * Developed with AtmelStudio 7.0.129
 */ 

#include <avr/io.h> // Required for the I/O registers macros
#include <avr/interrupt.h> // Required for the ISR macro
#include "program.h"

extern unsigned char data[8];
extern unsigned char transmitter_status;
extern unsigned char num_transmits_left;


//--------------------------------------------------------------------
// Interrupt service routine for USART receive completed.
// Communication is errorless and previous/next inputs/states are irrelevant so a state-machine isn't needed.
// Only the current input is needed to take the appropriate action.
//--------------------------------------------------------------------
ISR( USART_RXC_vect )
{
	// Receive frame
	unsigned char received_frame = UDR;
	
	if( received_frame == 0x43 ) // C
		for( unsigned char i = 0 ; i < num_of_data ; i++ )
			data[i] = 0x0A; // Clear data
			
	else if( received_frame == 0x4E ) // N
		for( unsigned char i = 0 ; i < num_of_data ; i++ )
			data[i] = 0x0A; // Clear data
	
	else if( received_frame == 0x41 ) // A
		return; // Do nothing
		
	else if( received_frame == 0x54 ) // T
		return; // Do nothing
		
	else if( received_frame == 0x0D ) // <CR>
		return; // Do nothing
		
	else if( received_frame == 0x0A ) // <LF>
	{
		 // Increase pending responses counter
		num_transmits_left++;
		// Enable transmitter interrupts to start the response. If it is already enabled, nothing changes.
		UCSRB |= ( 1 << UDRIE ); // breakpoint here to check memory after message
	}
	else // Frame is a number
	{
		// Moves all data one position forward. Top element gets discarded.
		for( unsigned char i = num_of_data - 2 ; i != 0xFF ; i-- ) // last element is 0xFF
			data[ i + 1 ] = data[i];
		// Save new data. Top half byte cleared, ascii -> bcd
		data[0] = received_frame & 0x0F;
	}
}


//--------------------------------------------------------------------
// Interrupt service routine for USART transmitter completed.
// FSM is one way (see report), input is irrelevant. Only current state is needed to compute next state.
//--------------------------------------------------------------------
ISR( USART_UDRE_vect )
{
	if( transmitter_status == none )
	{
		// Decrease pending responses
		num_transmits_left--;
		// Change State
		transmitter_status = 'O';
		// Send character
		UDR = 'O';
	}
	else if( transmitter_status == 'O' )
	{
		// Change State
		transmitter_status = 'K';
		// Send character
		UDR = 'K';
	}
	else if( transmitter_status == 'K' )
	{
		// Change State
		transmitter_status = 0x0D; // CR
		// Send character
		UDR = 0x0D;
	}
	else if( transmitter_status == 0x0D )
	{
		// Change State
		transmitter_status = 0x0A; // LF
		// Send character
		UDR = 0x0A;
	}
	else if( transmitter_status == 0x0A ) // LF
	{
		// Change State
		transmitter_status = none;
		// If no more transmits required, disable transmitter interrupts.
		if( num_transmits_left == 0 )
			UCSRB &= ~( 1 << UDRIE );
	}
}