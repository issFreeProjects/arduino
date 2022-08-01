#include <avr/io.h>
#include "serial.h"

void USART_Init(int ubrr)
{
    /*Set baud rate */
    UBRR0H = (char)(ubrr>>8);
    UBRR0L = (char)ubrr;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}


void USART_Transmit(char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)))
    ;
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

unsigned char USART_Receive(void)
{
    /* Wait for data to be received */
    while (!(UCSR0A & (1<<RXC0)))
    ;
    /* Get and return received data from buffer */
    return UDR0;
}


void USART_gets(char* buff, uint8_t len)
{
    uint8_t c = 0;
    char res = 0;
    while( c<len ){
        res = USART_Receive();
        if(res=='\n' || res=='\r')
            break;
        buff[c] = res;
        c++;
    }
    while(c<len-1){
        buff[c] = 0;
        c++;
    }

}

void USART_egets(char* buff, uint8_t len)
{
    uint8_t c = 0;
    unsigned char res = 0;
    while( c<len ){
        res = USART_Receive();
        USART_Transmit(res);
        if(res=='\n' || res=='\r')
            break;
        buff[c] = res;
        c++;
    }
    while(c<len-1){
        buff[c] = 0;
        c++;
    }
}

void USART_puts(char* buff, uint8_t len)
{
    uint8_t c = 0;
    while(c<len){
        if(buff[c]==0) break;
        USART_Transmit(buff[c]);
        c++;
    }
}
