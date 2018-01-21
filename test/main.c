#include <util/delay.h>
#include "headers/buttons.h"
#include "headers/ledMatrix.h"



int main(void)
{	
	/*setupInterrupts();
	
	while(1)
	{
		sleepNow();
		mainHandler();
		leftHandler();
	}
	return 0;*/
	
	//Inits
	
	initSPI();
	initMatrix();
	clearMatrix();
	initBuffer();

	// Pointer to beginning of message
	const char *messagePointer = &message[0];

	// Size of message matrix
	uint16_t messageSize = sizeof(message);

	// Event loop
	while (1) 
	{

		displayMessage(messagePointer, messageSize);	// Display the message

	}                                                 
	return (0);     
}
