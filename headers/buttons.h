#include <avr/interrupt.h>

#define INTERRUPT_ADD DDRD
#define INTERRUPT_PORT PORTD
#define INTERRUPT_PIN PIND
#define INTERRUPT_VECT PCINT2_vect
#define INTERRUPT_MASK PCMSK2
#define INTERRUPT_PCICR PCIE2

#define MAIN_NUMBER PD0
#define MAIN_PCINT PCINT16

#define LEFT_NUMBER PD1
#define LEFT_PCINT PCINT17

#define RIGHT_NUMBER PD2
#define RIGHT_PCINT PCINT18

#define BOTH_NUMBER PD3
#define BOTH_PCINT PCINT19

#define POWER_ADD DDRC
#define POWER_PORT PORTC
#define POWER_NUMBER PC4

void sleepNow(void);
void getState(void);
void mainHandler(void);
void leftHandler(void);
void setupInterrupts(void);
