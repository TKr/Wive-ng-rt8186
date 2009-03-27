#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>


void*  memcpy(void * dest,const void *src,size_t count)
{
    unsigned long i;
    
    for(i=0; i<count; i++)
    {
		*(unsigned char *)(dest++)= *(unsigned char *)(src++);
    }
    return (void *)0;
}


