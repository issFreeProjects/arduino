#include <avr/io.h>
#include <util/delay.h>
#include "ssh_I.h"
#include "utils.h"


uint8_t i, n;


static void error(char* message, uint8_t len)
{
    memcopy(res, "[ERROR] ", 8);
    memcopy(res+8, message, len);
}



/**
 *      Helper functions
 **/
static void sr_DDRx()
{
    switch(buff[3]){
        case 'D':
            memzero(res, BUFFER_SIZE);
            memcopy(res, "DDRD = ", 7);
            for(i=0; i<8; i++)
                res[14-i] = ((DDRD&(1<<i))>>(i)) + '0';
            break;
        case 'B':
            memzero(res, BUFFER_SIZE);
            memcopy(res, "DDRB = ", 7);
            for(i=0; i<8; i++)
                res[14-i] = ((DDRB&(1<<i))>>(i)) + '0';
            break;
        case 'C':
            memzero(res, BUFFER_SIZE);
            memcopy(res, "DDRC = ", 7);
            for(i=0; i<8; i++)
                res[14-i] = ((DDRC&(1<<i))>>(i)) + '0';
            break;
        default:
            error("use: DDRx, x={B,C,D}", 20);
            break;
    }
}

static void sr_PINx()
{
    switch(buff[3]){
        case 'D':
            memzero(res, BUFFER_SIZE);
            memcopy(res, "D0-D7 = ", 8);
            for(i=0; i<8; i++)
                res[15-i] = ((DDRD&(1<<i))>>(i)) + '0';
            break;
        case 'B':
            memzero(res, BUFFER_SIZE);
            memcopy(res, "D8-D13 = ", 9);
            for(i=0; i<8; i++)
                res[16-i] = ((DDRB&(1<<i))>>(i)) + '0';
            break;
        case 'C':
            memzero(res, BUFFER_SIZE);
            memcopy(res, "A0-A5 = ", 8);
            for(i=0; i<8; i++)
                res[15-i] = ((DDRC&(1<<i))>>(i)) + '0';
            break;
        default:
            error("use: PINx, x={B,C,D}", 20);
            break;
    }
}

static void sr_PORTx()
{
    switch(buff[4]){
        case 'D':
            memzero(res, BUFFER_SIZE);
            memcopy(res, "D0-D7 = ", 8);
            for(i=0; i<8; i++)
                res[15-i] = ((PORTD&(1<<i))>>(i)) + '0';
            break;
        case 'B':
            memzero(res, BUFFER_SIZE);
            memcopy(res, "B8-B13 = ", 9);
            for(i=0; i<8; i++)
                res[16-i] = ((PORTB&(1<<i))>>(i)) + '0';
            break;
        case 'C':
            memzero(res, BUFFER_SIZE);
            memcopy(res, "A0-A5 = ", 8);
            for(i=0; i<8; i++)
                res[15-i] = ((PORTC&(1<<i))>>(i)) + '0';
            break;
        default:
            error("use: PORTx, x={B,C,D}", 21);
            break;
    }

}

static void sr_set()
{
    n = atoi(buff+5);

    if(n<8  && n>=0)
        PORTD |= (1<<n);
    else if(n<14 && n>=8)
        PORTB |= (1<<(n-8));
    else error("use set Dx, 0<x<13", 18);
}

static void sr_unset()
{
    n = atoi(buff+7);

    if(n<8  && n>=0)
        PORTD &= ~(1<<n);
    else if(n<14 && n>=8)
        PORTB &= ~(1<<(n-8));
    else error("use set Dx, 0<x<13", 18);
}

static void sr_sleep(uint8_t sec)
{
    _delay_ms(sec * 1000);

    res[0] = '\n';
    memcopy(res+1, "Done\n", 8);
}


void sh_run()
{
    if     (memcomp(buff, "DDR",     3))    // DDR (B,C,D)
        sr_DDRx();
    else if(memcomp(buff, "PIN",     3))    // PIN (B,C,D)
        sr_PINx();
    else if(memcomp(buff, "PORT",    4))    // PORT(B,C,D)
        sr_PORTx();
    else if(memcomp(buff, "set D",   5))    // set D0-D13 = 1
        sr_set();
    else if(memcomp(buff, "unset D", 7))    // set D0-D13 = 0
        sr_unset();
    else if(memcomp(buff, "sleep1",  6))    // sleep 1000 (ms)
        sr_sleep(1);
    else if(memcomp(buff, "sleep2",  6))    // sleep 2000 (ms)
        sr_sleep(2);
    else if(buff[0] != '\n' && buff[0] != '\r' && buff[0] != '\0')
            memcopy(res, "command not found.", 18);
    else    memzero(res, BUFFER_SIZE);
}
