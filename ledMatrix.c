/*
  One MAX7219 connected to an 8x8 LED matrix.
  * 
  * modified
 */

#include "headers/ledMatrix.h"

uint8_t INVERT = 0;
uint8_t LED_MATRICES[8][4];

uint8_t SMILE[8] = {
        0b00000000,
        0b01100110,
        0b01100110,
        0b00000000,
        0b01000010,
        0b01000010,
        0b00100100,
        0b00011000};

uint8_t SAD[8] = {
        0b00000000,
        0b01100110,
        0b01100110,
        0b00000000,
        0b00011000,
        0b00100100,
        0b01000010,
        0b01000010,
};

uint8_t LEFT[8] =  {
        0b00011000,
        0b00111000,
        0b01111111,
        0b11111111,
        0b01111111,
        0b00111111,
        0b00011000,
        0b00001000,
};

uint8_t RIGHT[8] =  {
        0b00011000,
        0b00011100,
        0b11111110,
        0b11111111,
        0b11111110,
        0b11111100,
        0b00011000,
        0b00010000,
};

uint8_t MIDDLE[8] = {
		0b00000000,
        0b00000000,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b00000000,
        0b00000000,
};


void spiSend(uint8_t data)
{
	uint8_t i;

	for (i = 0; i < 8; i++, data <<= 1)
	{
		CLK_LOW();
		
		if (data & 0x80) DATA_HIGH();
		else DATA_LOW();
		
		CLK_HIGH();
	}
    
}

void writeLED(uint8_t high_byte, uint8_t low_byte)
{
	spiSend(high_byte);
	spiSend(low_byte);
}

void clearLED(void)
{
	uint8_t i, j;
	for (i = 0; i < 8; i++)
	{
		CS_LOW();
		for(j = 0; j < 4; j++)
		{
			writeLED(i+1, 0);
		}
		CS_HIGH();
	}
}

void initLED(void)
{
	uint8_t i, n = 4;
    INIT_PORT();
    
    CS_LOW();
    for(i = 0; i<n; i++) writeLED(0x09, 0);
    CS_HIGH();
    
    CS_LOW();
    for(i = 0; i<n; i++) writeLED(0x0A, LIGHT_BRIGHT);
    CS_HIGH();
    
    CS_LOW();
    for(i = 0; i<n; i++) writeLED(0x0B, 7);
    CS_HIGH();
    
    CS_LOW();
    for(i = 0; i<n; i++) writeLED(0x0C, 1);
    CS_HIGH();
    
    CS_LOW();
    for(i = 0; i<n; i++) writeLED(0x0F, 0);
    CS_HIGH();
    
    clearLED();
}

void updateLED(void)
{
	uint8_t i, j;
	
	for(i = 0; i < 8; i++)
	{
		CS_LOW();
		for(j = 0; j < 4; j++)
		{
			spiSend(i + 1);
			spiSend(LED_MATRICES[i][j]);
		}
		CS_HIGH();
	}
}

void sendLED(uint8_t im0[8], uint8_t im1[8], uint8_t im2[8], uint8_t im3[8], uint8_t inverse)
{
	cli();
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		if(inverse)
		{
			LED_MATRICES[i][0] = ~im0[i];
			LED_MATRICES[i][1] = ~im1[i];
			LED_MATRICES[i][2] = ~im2[i];
			LED_MATRICES[i][3] = ~im3[i];
		}
		
		else
		{
			LED_MATRICES[i][0] = im0[i];
			LED_MATRICES[i][1] = im1[i];
			LED_MATRICES[i][2] = im2[i];
			LED_MATRICES[i][3] = im3[i];
		}
	}
	
	updateLED();
	sei();
}

void rightLight(void)
{
	sendLED(LEFT, MIDDLE, LEFT, MIDDLE, INVERT);
	INVERT ^= 1;
}

void leftLight(void)
{
	sendLED(MIDDLE, RIGHT, MIDDLE, RIGHT, INVERT);
	INVERT ^= 1;
}

void bothLights(void)
{
	sendLED(LEFT, MIDDLE, MIDDLE, RIGHT, INVERT);
	INVERT ^= 1;
}

void mainLight(void)
{
	sendLED(SMILE, SMILE, SMILE, SMILE, INVERT);
	INVERT ^= 1;
}

void initLEDTimer(void)
{
	TIMSK0 |= (1 << OCIE0B); 
    TCCR0B |= (1 << CS02) | (1 << CS00);
}

void stopLEDTimer(void)
{
	TIMSK0 &= ~(1 << OCIE0B); 
    TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
}
