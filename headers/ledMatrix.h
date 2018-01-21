#include <avr/io.h>
#include <avr/interrupt.h>

#define CLK_HIGH()  PORTB |= (1<<PB5)
#define CLK_LOW()   PORTB &= ~(1<<PB5)

#define CS_HIGH()   PORTB |= (1<<PB2)
#define CS_LOW()    PORTB &= ~(1<<PB2)

#define DATA_HIGH() PORTB |= (1<<PB4)
#define DATA_LOW()  PORTB &= ~(1<<PB4)

#define INIT_PORT() DDRB |= (1<<PB2) | (1<<PB4) | (1<<PB5)

#define LIGHT_BRIGHT 8

#define LIGHT_VECT TIMER0_COMPB_vect

#define BLINK_FACTOR 2

void initLED(void);
void clearLED(void);
void mainLight(void);
void updateLED(void);
void leftLight(void);
void rightLight(void);
void bothLights(void);
void initLEDTimer(void);
void stopLEDTimer(void);


void spiSend(uint8_t data);
void writeLED(uint8_t high_byte, uint8_t low_byte);
void sendLED(uint8_t im0[8], uint8_t im1[8], uint8_t im2[8], uint8_t im3[8], uint8_t inverse);

