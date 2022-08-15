#include <stdint.h>
#include <string.h>
#include "ssprintf.h"
#include "utils.h"


const uint8_t MAXLEN_INT = sizeof(uint8_t)*8;
char SP = ' ';



static int8_t __ssprintf__s(char* tmp, char* val)
{
    uint8_t i;
    for(i=0; val[i]!='\0'; i++)
        tmp[i] = val[i];

    return i-1;
}

static int8_t __ssprintf__d(char* tmp, int8_t val)
{
    uint8_t i, c;

    for(i=0; i<MAXLEN_INT; i++){
        c = val%10;
        if(c>=10 || c<0) c = 9;

        tmp[i] = c + '0';
        val /= 10;
        if(val==0)
            break;
    }

    return i;
}

static int8_t __ssprintf__x(char* tmp, int8_t val)
{
    uint8_t i, c;

    for(i=0; i<MAXLEN_INT; i++){
        c = val%16;
        if(c>=16 || c<0) c = 15;

        if(c>9) tmp[i] = c + 'a' - 10;
        else tmp[i] = c + '0';
        val /= 16;
        if(val==0)
            break;
    }

    return i;
}


static int8_t __ssprintf__X(char* tmp, int8_t val)
{
    uint8_t i, c;

    for(i=0; i<MAXLEN_INT; i++){
        c = val%16;
        if(c>=16 || c<0) c = 15;

        if(c>9) tmp[i] = c + 'A' - 10;
        else tmp[i] = c + '0';
        val /= 16;
        if(val==0)
            break;
    }

    return i;
}

static int8_t __ssprintf__b(char* tmp, int8_t val)
{
    uint8_t i;

    for(i=0; i<MAXLEN_INT; i++){
        tmp[i] = val%2 + '0';
        val /= 2;
        if(val==0)
            break;
    }

    return i;
}

static int8_t __ssprintf__p(char* tmp, int8_t val)
{
    uint8_t i, c;
    int16_t v = (int16_t)&val;

    for(i=0; i<MAXLEN_INT; i++){
        c = v%16;
        if(c>9) tmp[i] = c + 'a' - 10;
        else tmp[i] = c + '0';
        v /= 16;
        if(v==0)
            break;
    }

    tmp[i+1] = 'x';
    tmp[i+2] = '0';

    return i+2;
}


static uint8_t __ssprintf__H__int(char* dest,
        int8_t val, uint8_t min_len, char flag)
{
    char tmp[MAXLEN_INT];
    char sign = ' ';
    uint8_t i=MAXLEN_INT-1, j=0, offset=0, c=0;

    memset(tmp, 0, MAXLEN_INT);
    
    if(val<0){
        sign = '-';
        val *= -1;
    }
    
    j = (__ssprintf__finder(flag))(tmp, val);

    while(tmp[i]==0)
        i--;

   
    if(min_len<=i && sign=='-'){
        offset++;
        dest[0] = '-'; 
    }
    if(min_len>i){
        offset = min_len-i-1;
        memset(dest, SP, min_len-i);
        if( sign=='-' ){
            i++;
            min_len++;
            dest[min_len-i-2] = '-';
        }

    }

    while(i>=0){
        dest[i+offset] = tmp[j-i];
        i--;
    }

    
    while(dest[c] != 0)
        c++;
    return c-1;
}


static uint8_t __ssprintf__H__str(char* dest,
        char *val, uint8_t min_len, char flag)
{
    char tmp[MAXLEN_INT];
    uint8_t i=MAXLEN_INT-1, offset=0, c=0;

    memset(tmp, 0, MAXLEN_INT);
    
    __ssprintf__s(tmp, val);

    while(tmp[i]==0)
        i--;
    
    if(min_len>i){
        offset = min_len-i-1;
        memset(dest, SP, min_len-i);
    }

    while(i>=0){
        dest[i+offset] = tmp[i];
        i--;
    }

    while(dest[c] != 0)
        c++;
    return c-1;
}


static uint8_t __ssprintf(char* dest,
        void *val, uint8_t min_len, char flag)
{

    if(flag=='s')
    {
            char *str = (char*) val;
            return __ssprintf__H__str(dest,str, min_len, flag);

    }else if(valid_numeric_flag(flag))
    {
            int *value = (int*) val;
            return __ssprintf__H__int(dest, *value, min_len, flag);
    }
    return 0;
}


void _ssprintf(char* dest, const char* format, void *val)
{
    uint8_t i=0, n=0, j=0;

    while(format[i]!='%' && format[i]!='\0')
    {
        dest[i] = format[i];
        i++;
    }

    if(format[i]=='%')
    {
        if(format[i+1]=='%')
            dest[i] = format[i+1];

        else if(isnumber(format[i+1]))
        {
            j=i;
            while(isnumber(format[j]))
                j++;

           if(format[i+1]=='0') SP='0';

           n = __ssprintf(dest+i, val,
                   atoi(format+i+1), format[j]);

           j -= i+1;
           SP = ' ';
        }

        else n = __ssprintf(dest+i, val, 0, format[i+1]);

        while(format[i]!='\0')
        {
            dest[n+i+1]=format[i+2+j];
            i++;
        }

    }
}




void ssprintf(char* dest, const char* format, void **val)
{
    uint8_t i=0, pi=0, n=0, valc=0;
    char tmp[64];

    while(format[i]!='\0' && format[i]!='%') i++;
    memcpy(dest, format, i);
    dest[i-pi]=0;
    n = strlen(dest);

    while(i<strlen(format))
    {
        pi = i;
        i++;
        while(format[i]!='\0' && format[i]!='%') i++;
        if(format[i+1]!='%' && format[i-1]=='%') i++;
        if(format[i]=='%' && format[i+1]=='%'&& format[i-1]=='%') i++;

        memcpy(tmp, format+pi, i-pi);
        tmp[i-pi]=0;
        _ssprintf(dest+n, tmp, val[valc]);

        if(tmp[0]=='%' && tmp[1]!='%') valc++;
        n = strlen(dest);
    }
}
