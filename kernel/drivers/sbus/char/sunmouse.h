/* $Id: sunmouse.h,v 1.1.1.1 2005/03/09 04:54:05 rex Exp $
 * sunmouse.h: Interface to the SUN mouse driver.
 *
 * Copyright (C) 1997  Eddie C. Dost  (ecd@skynet.be)
 */

#ifndef _SPARC_SUNMOUSE_H
#define _SPARC_SUNMOUSE_H 1

extern void sun_mouse_zsinit(void);
extern void sun_mouse_inbyte(unsigned char, int);

#endif /* !(_SPARC_SUNMOUSE_H) */
