#include <util/delay.h>
#include <avr/sleep.h>
#include "headers/buttons.h"
#include "headers/ledMatrix.h"


int main(void)
{
	initLED();
	initLEDTimer();
	setupInterrupts();
    
    sei();
	
	while(1)
	{
	}
	return 0;
}









