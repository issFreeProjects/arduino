#define FOSC 16000000 // Clock Speed
#define BAUD 57600
#define MYUBRR FOSC/16/BAUD-1

void USART_Init(int ubrr);


// inner methodes (not recommended to use)
void USART_Transmit(char data);
unsigned char USART_Receive(void);


// useful methodes
//
void USART_gets (char* buff, uint8_t len);
void USART_puts (char* buff, uint8_t len);
void USART_egets(char* buff, uint8_t len);
