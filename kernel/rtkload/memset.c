
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>



void * memset(void * s, int c, size_t count)
{
    char *xs = (char *) s;

    while (count--)
        *xs++ = (char) c;

    return s;
}


