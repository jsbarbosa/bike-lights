#include <stdio.h>
#include <avr/sleep.h>
#include "headers/buttons.h"
#include "headers/ledMatrix.h"

uint8_t LEFT_STATE, RIGHT_STATE, BOTH_STATE, MAIN_STATE;
uint8_t POWER_BACK_COUNT = 0;
uint8_t BLINK_STATE = 0;

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
					
	TCNT2 = 0x00;
    TIMSK2 |= (1 << TOIE2);
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
    
    POWER_ADD |= (1 << POWER_NUMBER);
}

void getState(void)
{
	LEFT_STATE = 0;
	RIGHT_STATE = 0;
	BOTH_STATE = 0;
	MAIN_STATE = 0;
	
	if((INTERRUPT_PIN & (1 << LEFT_NUMBER)) == 0) LEFT_STATE = 1;
	if((INTERRUPT_PIN & (1 << RIGHT_NUMBER)) == 0) RIGHT_STATE = 1;
	if((INTERRUPT_PIN & (1 << BOTH_NUMBER)) == 0) BOTH_STATE = 1;
	if((INTERRUPT_PIN & (1 << MAIN_NUMBER)) == 0) MAIN_STATE = 1;
}

ISR(INTERRUPT_VECT)
{	
	//~ uint8_t left, right, both, mainl;
	//~ getState(&left, &right, &both, &mainl);
	//~ 
	//~ if((left | right | both | mainl)) 
	
	TCNT0 = 254;
	
	//~ else turnLEDSOff();	
}

ISR(LIGHT_VECT)
{
	getState();
	
	if(LEFT_STATE | RIGHT_STATE | BOTH_STATE)
	{
		BLINK_STATE += 1;
		if(BLINK_STATE % BLINK_FACTOR)
		{
			if(BOTH_STATE) bothLights();
			else if(LEFT_STATE) leftLight();
			else if(RIGHT_STATE) rightLight();
		}
	}
	
	else if(MAIN_STATE) mainLight();
	
	else turnLEDSOff();	
}

ISR(TIMER2_OVF_vect)
{
	TCNT2 = 0x00;
	uint8_t c = POWER_BACK_COUNT % 30;
	
	getState();
	
	if(LEFT_STATE | RIGHT_STATE | BOTH_STATE | MAIN_STATE) POWER_PORT &= ~(1 << POWER_NUMBER);
	
	else
	{
		if(c == 0)
		{
			POWER_PORT |= (1 << POWER_NUMBER);
		}
		else if(c == 3)
		{
			POWER_PORT &= ~(1 << POWER_NUMBER);
		}
	}
	
	POWER_BACK_COUNT += 1;
}
