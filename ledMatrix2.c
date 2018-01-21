/*
  One MAX7219 connected to an 8x8 LED matrix.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define CLK_HIGH()  PORTB |= (1<<PB5)
#define CLK_LOW()   PORTB &= ~(1<<PB5)

#define CS_HIGH()   PORTB |= (1<<PB2)
#define CS_LOW()    PORTB &= ~(1<<PB2)

#define DATA_HIGH() PORTB |= (1<<PB4)
#define DATA_LOW()  PORTB &= ~(1<<PB4)

#define INIT_PORT() DDRB |= (1<<PB2) | (1<<PB4) | (1<<PB5)

#define LIGHT_DELAY 750
#define LIGHT_BRIGHT 8

uint8_t display[8], displays[8][4];

uint8_t BLINK_FACTOR = 2, BLINK_STATE = 0;
uint8_t INVERT = 0;

uint8_t smile[8] = {
        0b00000010,
        0b01100110,
        0b01100110,
        0b00000000,
        0b01000010,
        0b01000010,
        0b00100100,
        0b00011000};

uint8_t sad[8] = {
        0b00000000,
        0b01100110,
        0b01100110,
        0b00000000,
        0b00011000,
        0b00100100,
        0b01000010,
        0b01000010,
};

uint8_t left[8] =  {
        0b00011000,
        0b00111000,
        0b01111111,
        0b11111111,
        0b01111111,
        0b00111111,
        0b00011000,
        0b00001000,
};

uint8_t right[8] =  {
        0b00011000,
        0b00011100,
        0b11111110,
        0b11111111,
        0b11111110,
        0b11111100,
        0b00011000,
        0b00010000,
};

uint8_t middle[8] = {
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
			spiSend(displays[i][j]);
		}
		CS_HIGH();
	}
}

void sendLED(uint8_t im0[8], uint8_t im1[8], uint8_t im2[8], uint8_t im3[8], uint8_t inverse)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		if(inverse)
		{
			displays[i][0] = ~im0[i];
			displays[i][1] = ~im1[i];
			displays[i][2] = ~im2[i];
			displays[i][3] = ~im3[i];
		}
		
		else
		{
			displays[i][0] = im0[i];
			displays[i][1] = im1[i];
			displays[i][2] = im2[i];
			displays[i][3] = im3[i];
		}
	}
	
	updateLED();
}

void leftLight(void)
{
	sendLED(left, middle, left, middle, INVERT);
	INVERT ^= 1;
}

void rightLight(void)
{
	sendLED(middle, right, middle, right, INVERT);
	INVERT ^= 1;
}

void bothLights(void)
{
	sendLED(left, middle, middle, right, INVERT);
	INVERT ^= 1;
}





int main(void)
{
    TIMSK0 |= (1 << OCIE0A); 
    sei();

    TCCR0B |= (1 << CS02) | (1 << CS00);

	initLED();
		
	while(1)
	{
		
	}
}


ISR (TIMER0_COMPA_vect)
{
	if(BLINK_STATE % BLINK_FACTOR) leftLight();
	
	BLINK_STATE += 1;
}
