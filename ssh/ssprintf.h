#define isnumber(i) i-'0'<=9 && i-'0'>=0

#define __ssprintf__finder(c)           \
    ( c=='d' ? __ssprintf__d            \
    : c=='x' ? __ssprintf__x            \
    : c=='X' ? __ssprintf__X            \
    : c=='p' ? __ssprintf__p            \
    :          __ssprintf__b )

#define valid_numeric_flag(c)           \
    ( c=='d' ? 1                        \
    : c=='x' ? 1                        \
    : c=='X' ? 1                        \
    : c=='p' ? 1                        \
    : c=='b' ? 1 : 0 )


static int8_t __ssprintf__d(char *tmp, int8_t val);


static int8_t __ssprintf__x(char *tmp, int8_t val);


static int8_t __ssprintf__X(char *tmp, int8_t val);


static int8_t __ssprintf__b(char *tmp, int8_t val);


static int8_t __ssprintf__p(char *tmp, int8_t val);


static uint8_t __ssprintf(char *dest, void *val,
         uint8_t min_len, char flag);


void _ssprintf(char *dest, const char *format, void *val);


void ssprintf(char *dest, const char *format, void **val);
