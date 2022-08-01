#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ssh.h"

uint8_t lst=0;


int main(void){
	DDRD  = 0b01111000;     // set D3-D6  as output
	DDRB  = 0b00100000;		// set D13    as output
	DDRC  = 0;
	PORTC = 0;

    cli();
    PCICR  |= (1 << 2);  	// for D (D0-D7)
    PCMSK2 |= (1 << 7);  	// D7
    sei();

	sh_init();

	while(1){
		sh_loop();
		PORTD = ln<<3;  	// show line number bits on D3-D6
	}

}


ISR (PCINT2_vect){
	if( lst==0 ){
		ln = 0;
		PORTD = 0;
	}
	lst = PIND&(1<<7);
}
