#ifndef _ASM_PARAM_H
#define _ASM_PARAM_H

/* Safeguard against user stupidity  */
//#ifdef _SYS_PARAM_H
//#error Do not include <asm/param.h> with __KERNEL__ defined!
//#endif

#include <linux/config.h>

/*  
    This is the external value of HZ as seen by user programs.  Don't change
    unless you know what you're doing - changing breaks binary compatibility.
    MIPS (like X86 and ppc) is defined to provide userspace with a world where HZ=100
    We have to do this, (x*const)/const2 isnt optimised out because its not
    a null operation as it might overflow.. 
*/

#define HZ		CONFIG_HZ			 	/* internal kernel timer frequency   */
#define USER_HZ		CONFIG_HZ				/* some user interfaces are in ticks */
#define CLOCKS_PER_SEC	(100000/CONFIG_HZ*1000)			/* frequency at which times() counts */
#define CORRECT_HZ	((22090000)/CONFIG_HZ) 			/* kernel timer to cpu timer correct */
#define hz_to_std(a) (a)

#define EXEC_PAGESIZE	4096

#ifndef NGROUPS
#define NGROUPS		32
#endif

#ifndef NOGROUP
#define NOGROUP		(-1)
#endif

#define MAXHOSTNAMELEN	64	/* max length of hostname */


#endif /* _ASM_PARAM_H */
