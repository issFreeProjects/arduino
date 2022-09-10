#include <avr/io.h>
#include <util/delay.h>
#include "cLcd.h"


// use PORTD for lcd data lines (@LCD_DPort) and,
//     PORTD for lcd EN and RS  (@LCD_CPort)
#define LCD_DPort PORTD
#define LCD_CPort PORTD
// @LCD_DPort and @LCD_CPort directions
#define LCD_DPORT_DIR DDRD
#define LCD_CPORT_DIR DDRD


// lcd data lines offset from LCD_DPort
#define DPort_Offset 4
// RS and EN offsets from LCD_CPort
#define RS 2  // RS is connected to @LCD_CPort 2
#define EN 3  // EN is connected to @LCD_CPort 3

// XX_1 makes XX +5v and XX_0 makes it 0
#define RS_1 LCD_CPort |=  (1<<RS);
#define RS_0 LCD_CPort &= ~(1<<RS);
#define EN_1 LCD_CPort |=  (1<<EN);
#define EN_0 LCD_CPort &= ~(1<<EN);
// load 4-bit @data to the LCD_DPort (data lines)
#define LOAD(data) LCD_DPort = (LCD_DPort&0x0F) | (data<<DPort_Offset);

// delay
#define FLUSH_D 1
#define COMMAND_D 2
#define NIBBLE_D 20
#define INIT_D 15


/* apply changes */
static void flush()
{
    EN_1;
    _delay_us(FLUSH_D);
    EN_0;
    _delay_us(FLUSH_D);
}


void lcd_command(uint8_t comm)
{
    uint8_t c;

    c = (comm&0xF0)>>4;
    LOAD(c);
    RS_0;
    flush();

    _delay_us(NIBBLE_D);

    c = comm&0x0F;
    LOAD(c);
    flush();

    _delay_ms(COMMAND_D);
}


void lcd_char(uint8_t c)
{
    RS_1;
    uint8_t buff;

    buff = (c&0xF0)>>4;
    LOAD(buff);
    flush();
    _delay_us(NIBBLE_D);
    
    buff = c&0x0F;
    LOAD(buff);
    flush();

    _delay_ms(COMMAND_D);
}


void load_custom_sym(uint8_t *font, uint8_t index)
{
    uint8_t i;
    lcd_command(0x40 + 8*index);

    for(i=0;i<8;i++) lcd_char(font[i]);
}


void init()
{
    RS_0;
    /* make needed lcd data lines as output */
    LCD_DPORT_DIR |= (0xF << DPort_Offset);
    LCD_CPORT_DIR |= ( (1<<EN) | (1<<RS) );

    _delay_ms(INIT_D);

    lcd_command(0x02);
    lcd_command(0x28);
    lcd_command(0x0C);
    lcd_command(0x06);
    lcd_command(0x01);

    _delay_ms(COMMAND_D);
    lcd_command(0x80);
    _delay_ms(COMMAND_D);
}


void lcd_str(const char *str)
{
    while(*str!='\0')
    {
        lcd_char(*str);
        str++;
    }
}


void lcd_clear_screen()
{
     lcd_command(0x01);
     lcd_command(0x80);
}
