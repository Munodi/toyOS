/*
        I know this is non standard, but it will be important
*/
#include <cstdlib>
#include <cstring>


static void reverse(char s[]);

namespace std
{
    char *itoa(int value, char *str, int radix)
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
            str[i++] = nums[value % radix];
        }while((value /= radix) > 0);
        if(sign < 0)
            str[i++] = '-';
        str[i] = '\0';
        reverse(str);
        return str;
    }


    char *utoa(unsigned value, char *str, int radix)
    {
        if(value == 0)
        {
            str[0] = '0';
            str[1] = '\0';
            return str;
        }
        unsigned i;
        char nums[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        i = 0;
        do{
            str[i++] = nums[value % radix];
        }while((value /= radix) > 0);
        str[i] = '\0';
        reverse(str);
        return str;
    }
}



/* reverse:  reverse string s in place */
static void reverse(char s[])
{
    std::size_t i, j;
    char c;

    for (i = 0, j = std::strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
