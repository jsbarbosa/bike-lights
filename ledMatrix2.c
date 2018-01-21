/*
  One MAX7219 connected to an 8x8 LED matrix.
 */

#include <avr/io.h>
#include <util/delay.h>

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


void spi_send(uint8_t data)
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

void max7219_writec(uint8_t high_byte, uint8_t low_byte)
{
	spi_send(high_byte);
	spi_send(low_byte);
}

void max7219_clear(void)
{
	uint8_t i, j;
	for (i = 0; i < 8; i++)
	{
		CS_LOW();
		for(j = 0; j < 4; j++)
		{
			max7219_writec(i+1, 0);
		}
		CS_HIGH();
	}
}

void max7219_init(void)
{
	uint8_t i, n = 4;
    INIT_PORT();
    
    CS_LOW();
    for(i = 0; i<n; i++) max7219_writec(0x09, 0);
    CS_HIGH();
    
    CS_LOW();
    for(i = 0; i<n; i++) max7219_writec(0x0A, LIGHT_BRIGHT);
    CS_HIGH();
    
    CS_LOW();
    for(i = 0; i<n; i++) max7219_writec(0x0B, 7);
    CS_HIGH();
    
    CS_LOW();
    for(i = 0; i<n; i++) max7219_writec(0x0C, 1);
    CS_HIGH();
    
    CS_LOW();
    for(i = 0; i<n; i++) max7219_writec(0x0F, 0);
    CS_HIGH();
    
    max7219_clear();
}

void update_displays(void)
{
	uint8_t i, j;
	
	for(i = 0; i < 8; i++)
	{
		CS_LOW();
		for(j = 0; j < 4; j++)
		{
			spi_send(i + 1);
			spi_send(displays[i][j]);
		}
		CS_HIGH();
	}
}

void images(uint8_t im0[8], uint8_t im1[8], uint8_t im2[8], uint8_t im3[8], uint8_t inverse)
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
}

void leftLight(void)
{
	images(left, middle, left, middle, 0);
	update_displays();
	_delay_ms(LIGHT_DELAY);
	
	images(left, middle, left, middle, 1);
	update_displays();
	_delay_ms(LIGHT_DELAY);
}

void rightLight(void)
{
	images(middle, right, middle, right, 0);
	update_displays();
	_delay_ms(LIGHT_DELAY);
	
	images(middle, right, middle, right, 1);
	update_displays();
	_delay_ms(LIGHT_DELAY);
}

void bothLights(void)
{
	images(left, middle, middle, right, 0);
	update_displays();
	_delay_ms(LIGHT_DELAY);
	
	images(left, middle, middle, right, 1);
	update_displays();
	_delay_ms(LIGHT_DELAY);
}

int main(void)
{
	max7219_init();
	
	uint8_t i, n = 5;
	
	while(1)
	{
		for(i = 0; i < n; i++)
		{
			leftLight();
		}
		for(i = 0; i < n; i++)
		{
			rightLight();
		}
		for(i = 0; i < n; i++)
		{
			bothLights();
		}
	}
}
