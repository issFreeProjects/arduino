/*
 *      @main.c, create date: Fri 09 Sep 2022
 *      this is just a test for cLcd library
 *      left arrow custom font and
 *      a simple progress bar are implemented
 *
 * */


#include <stdio.h>
#include <util/delay.h>
#include "cLcd.h"

// Custom chars set for lcd (custom fonts and symbols)
static uint8_t prog1[8]
    = { 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0};
static uint8_t left_arrow[8] 
    = {0x10, 0x10, 0x14, 0x12, 0x1F, 0x02, 0x04, 0x00};

void mk_progressbar_symbols(uint8_t i)  // i=0,1,2,3,4,5
{
    uint8_t tmp = 0;
    for(; i>0; i--)
        tmp |= (1<<(5-i));

    for(i=1; i<6; i++)
        prog1[i] = tmp;
}

void load_custom_symbols()
{
    // load custom fonts
    for(int i=0; i<6; i++){
        mk_progressbar_symbols(i);
        load_custom_sym(prog1, i);
    }

    load_custom_sym(left_arrow, 6);
}

void mk_progressbar(uint8_t per)  // per = 0,1,...,100
{
    uint8_t i,j;
    i = per/10;
    
    for(j=0; j<i; j++)  lcd_char(5);

    i = (per%10)/2;

    if(i!=0)  lcd_char(i);
}


void ad_mk_progressbar(uint8_t per, char *title_format, uint8_t title_len)
{
    char buff[title_len];

    sprintf(buff, title_format, per);
    lcd_str(buff);

    mk_progressbar(per);
}


int main()
{
    uint8_t p = 0;

    init();
    load_custom_symbols();
    lcd_clear_screen();

    lcd_char(6);
    lcd_str("Hello World!");
    lcd_goto(0xFF);
    lcd_char(']');

    while(1)
    {
        lcd_goto(0xF0);
        ad_mk_progressbar(p, "%3d%%[", 5);
        p++;

        _delay_ms(400);

        if(p>100)
        { 
            _delay_ms(1000);
            p=0;
            lcd_goto(0xF5);
            lcd_str("          ");
        }
    }
}
