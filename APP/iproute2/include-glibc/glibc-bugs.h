#include <features.h>
#include <sys/types.h>

#if defined(__GLIBC__) && __GLIBC__ >= 2

#ifndef __KERNEL_STRICT_NAMES
#define __KERNEL_STRICT_NAMES 1
#endif

#include <linux/types.h>

#endif
