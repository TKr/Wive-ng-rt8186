/* 
 * Borrowed from arch/mips/r39xx/prom/init.c
 *
 */

/*
 * init.c: early initialisation code for R39XX Class PDAs
 *
 * Copyright (C) 1999 Harald Koerfgen
 *
 * $Id: prom_printf.c,v 1.1.1.1 2005/03/09 04:54:43 rex Exp $
 */

#include <stdarg.h>

#ifdef RTL8186
#include <asm/rtl8186.h>
#endif
#ifdef RTL8196
#include <asm/rtl8196.h>
#endif
       
void serial_outc(char c)
{
	int i=0;
    while (1)
    {
          i++;
                if (i >=0x6000)
                        break;

                if      (rtl_inb(UART_LSR) & 0x20)
                        break;
     }
     rtl_outb(UART_THR, c);
}
 

/*
 * Helpful for debugging :-)
 */
int prom_printf(const char * fmt, ...)
{
#ifdef CONFIG_SERIAL_CONSOLE
	//extern void serial_outc(char);
	static char buf[1024];
	va_list args;
	char c;
	int i = 0;

	/*
	 * Printing messages via serial port
	 */
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	for (i = 0; buf[i] != '\0'; i++) {
		c = buf[i];
		if (c == '\n')
			serial_outc('\r');
		serial_outc(c);
	}

	return i;
#else
	return 0;
#endif
}
