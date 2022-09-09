/*
 *      @main.c, created date: Fri 09 Sep 2022
 *      this is just a test for cLcd library
 *      left arrow custom font and
 *      a simple progress bar are implemented
 *
 * */


#include <util/delay.h>
#include "cLcd.h"

// Custom chars set for lcd (custom fonts and symbols)
static uint8_t prog1[8];
static uint8_t left_arrow[8] 
    = {0x10, 0x10, 0x14, 0x12, 0x1F, 0x02, 0x04, 0x00};

void mk_progressbar(uint8_t i)  // i=0,1,2,3,4,5
{
    uint8_t tmp = 0;
    for(; i>0; i--)
        tmp |= (1<<(5-i));
    for(i=0; i<8; i++)
        prog1[i] = tmp;
}

void load_custom_stuff()
{
    // load custom fonts
    for(int i=0; i<6; i++){
        mk_progressbar(i);
        load_custom_sym(prog1, i);
    }

    load_custom_sym(left_arrow, 6);
}


int main()
{
    init();
    load_custom_stuff();
    lcd_clear_screen();

    lcd_char(6);
    lcd_str("Hello World!");
    lcd_command(0xC0); // newline

    int8_t c1, c2=0, c3=1;
    while(1){
        for(c1=0; c1<6; c1++){
            lcd_command(0xC0 + c2);
            if(c3==1)
                lcd_char(c1);
            else lcd_char(5-c1);
            _delay_ms(50);
        }
        
        c1=0;
        c2+=c3;
        if(c2==15)
            c3=-1;
        if(c2==0)
            c3=1;
    } 
}
