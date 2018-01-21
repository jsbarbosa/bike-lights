#include <util/delay.h>
#include <avr/sleep.h>
#include "headers/buttons.h"
#include "headers/ledMatrix.h"

uint8_t BLINK_STATE = 0;

int main(void)
{
	DDRB = 0xFF;
	initLED();
	setupInterrupts();
	initLEDTimer();
	
	sei();
		
	while(1)
	{
		//~ if((TCCR0B & (1 << CS00)) == 0)	sleepNow();
		
		//~ leftHandler();
	}
	return 0;
}

void getState(uint8_t *left, uint8_t *right, uint8_t *both, uint8_t *mainl)
{
	*left = 0; *right = 0; *both = 0; *mainl = 0;
	if((INTERRUPT_PIN & (1 << LEFT_NUMBER)) == 0) *left = 1;
	if((INTERRUPT_PIN & (1 << RIGHT_NUMBER)) == 0) *right = 1;
	if((INTERRUPT_PIN & (1 << BOTH_NUMBER)) == 0) *both = 1;
	if((INTERRUPT_PIN & (1 << MAIN_NUMBER)) == 0) *mainl = 1;
}

ISR(LIGHT_VECT)
{
	uint8_t left, right, both, mainl;
	getState(&left, &right, &both, &mainl);
	
	if(left | right | both | mainl)
	{
		BLINK_STATE += 1;
		if(BLINK_STATE % BLINK_FACTOR)
		{
			if(both) bothLights();
			else if(left) leftLight();
			else if(right) rightLight();
			else if(mainl) mainLight();
		}
	}
	//else stopLEDTimer();
}

ISR(INTERRUPT_VECT)
{	
	PORTB ^= (1 << PB3);
	
	uint8_t left, right, both, mainl;
	getState(&left, &right, &both, &mainl);
	
	if((left | right | both | mainl) == 0)
	{
		clearLED();
	}
	//~ sleep_disable();
	//~ mainHandler();
}
