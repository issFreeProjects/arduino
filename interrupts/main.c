#include <avr/io.h>
#include <avr/interrupt.h>

/**
 *      blinking LED on B5 (internal arduino LED).
 *      speed increaser button is connected to D2 port.
 *
 *              ____ D2
 *             |
 *          +---+
 *          | O |
 *          +---+
 *           | |_____|1kΩ resistor|____ +5V
 *           |_______________________________GND
 *
 *      default blinking B5 (built in - pin D13)
 *          blinking interval is 1 second.
 *      pressing speed increaser button reduces
 *          this time by 651/15624 ≈ 0.042(s)
 *          so, 24 times pressing this button
 *          will reset the speed to 1 second.
 *
 *
 * */

#define F_CPU 16000000L
// Maximum of Counter1 (= 2^16)
#define C1_MAX 65536        
// Minimum of Counter1 must be C1_MIN to get 1Hz 
// interrupt frequency, for more details see timer.pdf
#define C1_MIN 49911
#define C1_INTERVAL 15624   // equals C1_MAX-C1_MIN
// pressing speed increaser button will increase C1_MIN by
// spbtn_counter*SPEED_SCALE that means at each time,
// will reduce timer1's interrupt time interval by
// SPEED_SCALE/C1_INTERVAL (s) = 0.041666 (s)
// Note. 24 times pressing speed increaser button, 
//       will set spbtn_counter=0.
#define SPEED_SCALE 651
// between any speed increaser button interrupts, you need to
// make a delay about 200(ms) until button has released.
// delay = BTN_DELAY/C1_INTERVAL = 0.199(s).
#define BTN_DELAY 3124

uint8_t sib_c;   // speed increaser button counter



ISR(TIMER1_COMPA_vect)
{
    // toggleing B5 (D13 port)
    PORTB ^= _BV(PORTB5);

    if(sib_c*SPEED_SCALE >= C1_INTERVAL)
        sib_c = 0;

    // increase counter1's minimum by SPEED_SCALE
    TCNT1  = C1_MIN + sib_c*SPEED_SCALE;
}


ISR(INT0_vect)
{
    int current_tcnt1;

    current_tcnt1 = TCNT1;
    sib_c++;

    // about 200(ms) delay
    while(TCNT1-current_tcnt1 < BTN_DELAY){};
}



void init_timer1()
{
    // initial value of counter1
    TCNT1   = C1_MIN;            
    // set timer1 to CTC mode
    TCCR1A |= (1<<WGM12);   
    // set F_counter = F_CPU / 1024
    TCCR1B |= (1<<CS10) | (1<<CS12);
    // enable output compare interrupt
    TIMSK1 |= (1<<OCIE1A); 
}


void init_ex_hwINT0()
{
    // Enable INT0
    EICRA = 0;
	EIMSK |= (1<<INT0);
    // The low level on the D2 pin generates
    // an interrupt.
    MCUCR = 0;
}



int main(void)
{
    cli();

    sib_c = 0;

    // make D13 (B5) as output and D* as input
    DDRB |= _BV(PORTB5);
    DDRD  = 0;

    // initilaize timer 1 interrupt
    init_timer1();
    // initialize hardware interrupt
    init_ex_hwINT0();

    sei();

    while(1){} // don't go away!
}
