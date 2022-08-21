#define __valid_hex_char(c) (c-'0'>=0 && c-'0'<=9) || (c-'A'>=0 && c-'A'<=5) || (c-'a'>=0 && c-'a'<=5)
#define __value_of_hexchar(c) (c-'0'>=0 && c-'0'<=9) ? c-'0' : (c-'A'>=0 && c-'A'<=5) ? c-'A' : c-'a'


void    memzero(void* dest, uint8_t n);
void    memcopy(void* dest, void* src, uint8_t n);
uint8_t memcomp(void* pt1,  void* pt2, uint8_t n);
int8_t     atoi(const char* str);
uint16_t hex2charaddr(const char* hex);
