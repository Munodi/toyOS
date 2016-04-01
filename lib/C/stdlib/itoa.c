/*
        I know this is non standard, but it will be important
*/
#include <stdlib.h>
#include <string.h>


static void reverse(char s[]);

char *itoa(int value, char *str, int base)
{
    if(value == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }
    int sign, i;
    char nums[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if(value < 0)
    {
        sign = -1;
        value = abs(value);
    }
    sign = i = 0;
    do{
        str[i++] = nums[value % base];
    }while((value /= base) > 0);
    if(sign < 0)
        str[i++] = '-';
    str[i] = '\0';
    reverse(str);
    return str;
}

/* reverse:  reverse string s in place */
static void reverse(char s[])
{
    size_t i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
