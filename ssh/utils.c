#include <stdint.h>
#include "utils.h"


void memzero(void* dest, uint8_t n)
{
	uint8_t i;
	char *cdest = (char*)dest;

	for(i=0; i<n; i++)
		cdest[i] = 0;
}


void memcopy(void* dest, void* src, uint8_t n)
{
	uint8_t i;
	char *csrc  = (char*)src;
	char *cdest = (char*)dest;

	for(i=0; i<n; i++)
		cdest[i] = csrc[i];
}


uint8_t memcomp(void* pt1, void* pt2, uint8_t n)
{
	uint8_t i;
	char *cpt1 = (char*)pt1;
	char *cpt2 = (char*)pt2;
	
	for(i=0; i<n; i++)
		if(cpt1[i] != cpt2[i])
			return 0;
	return 1;
}


int8_t atoi(const char* str)
{
	int8_t result=0, i=0;
	while(str[i]!='\0' && str[i]!='\r' && str[i]!='\n'){
		result = result*10 + str[i] - '0';
		i++;
	}
	return result;
}
