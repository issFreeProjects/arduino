#include <avr/io.h>
#include "ssh.h"
#include "serial.h"
#include "ssh_I.h"

char sh[29]         = "welcome to simple sh (ssh)!\r\n";
char s1[5]          = ">>>> ";
char newline[2]     = "\r\n";
char s2[5]          = "[  ] ";


void inc_ln()
{
	ln++;
	if(ln==100)
		ln = 0;
}


void sh_init()
{
	USART_Init(MYUBRR);
	USART_puts(sh, 29);
}


void set_line_number()
{
	// Note. acsii value of '0' is 48
	s2[2]=(ln%10)+48; // units digit
	s2[1]=(ln/10)%10==0 ? ' ' : (ln/10)%10+48; // tens digit
}


void sh_loop()
{
	USART_puts(s1,5);
	USART_egets(buff, 32);
	if(buff[31]!=0) // buffer if full
		USART_puts(newline, 1); // simulate press enter
	set_line_number(ln);
	USART_puts(s2,5);
	sh_run();
	USART_puts(res, 32);
	USART_puts(newline, 2);
	inc_ln();
}

