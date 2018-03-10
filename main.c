#include <avr/io.h>
#include <util/delay.h>

#define LED_PORT PB1
 
int main(void) 
{
    DDRB |= (1 << LED_PORT);
    
    while (1)
    {
        PORTB ^= (1 << LED_PORT);
        _delay_ms(250);
        //~ PORTB &= ~(1 << LED_PORT);
        //~ _delay_ms(400);
    }
 
    return (0);
}








