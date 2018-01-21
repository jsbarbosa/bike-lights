#include <stdio.h>
#include <avr/sleep.h>
#include "headers/buttons.h"

void setupInterrupts(void)
{
	// ports as inputs
	INTERRUPT_ADD &= ~((1 << MAIN_NUMBER) | (1 << LEFT_NUMBER)
						| (1 << RIGHT_NUMBER) | (1 << BOTH_NUMBER));

	// pull up resistors
	INTERRUPT_PORT |= ((1 << MAIN_NUMBER) | (1 << LEFT_NUMBER) 
					| (1 << RIGHT_NUMBER) | (1 << BOTH_NUMBER));
	
	// set interrupts
	PCICR |= (1 << INTERRUPT_PCICR);
	INTERRUPT_MASK |= ((1 << MAIN_PCINT) | (1 << LEFT_PCINT)
					| (1 << RIGHT_PCINT) | (1 << BOTH_PCINT));
}

void sleepNow(void)
{
	cli();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sei();
	sleep_cpu();
}

void mainHandler(void)
{
	cli();
	DDRB = 0xFF;
	//~ if((INTERRUPT_PIN & (1 << MAIN_NUMBER)) == 0)
	//~ {
		PORTB ^= (1 << PB3);
	//~ }
	sei();
}

void leftHandler(void)
{
	//~ DDRB = 0xFF;
	if((INTERRUPT_PIN & (1 << LEFT_NUMBER)) == 0)
	{
		PORTB |= (1 << PB4);
	}
}
