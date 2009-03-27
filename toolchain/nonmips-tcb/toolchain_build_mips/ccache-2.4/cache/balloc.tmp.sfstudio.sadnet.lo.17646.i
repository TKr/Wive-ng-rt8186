# 1 "balloc.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "balloc.c"
# 14 "balloc.c"
# 1 "/opt/Wive/kernel/include/linux/config.h" 1



# 1 "/opt/Wive/kernel/include/linux/autoconf.h" 1
# 5 "/opt/Wive/kernel/include/linux/config.h" 2
# 15 "balloc.c" 2
# 1 "/opt/Wive/kernel/include/linux/fs.h" 1
# 10 "/opt/Wive/kernel/include/linux/fs.h"
# 1 "/opt/Wive/kernel/include/linux/linkage.h" 1
# 11 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/limits.h" 1
# 12 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/wait.h" 1
# 13 "/opt/Wive/kernel/include/linux/wait.h"
# 1 "/opt/Wive/kernel/include/linux/kernel.h" 1
# 10 "/opt/Wive/kernel/include/linux/kernel.h"
# 1 "/opt/Wive/kernel/include/asm/gcc/stdarg.h" 1
# 43 "/opt/Wive/kernel/include/asm/gcc/stdarg.h"
typedef __builtin_va_list __gnuc_va_list;
# 110 "/opt/Wive/kernel/include/asm/gcc/stdarg.h"
typedef __gnuc_va_list va_list;
# 11 "/opt/Wive/kernel/include/linux/kernel.h" 2

# 1 "/opt/Wive/kernel/include/linux/stddef.h" 1
# 13 "/opt/Wive/kernel/include/linux/kernel.h" 2
# 1 "/opt/Wive/kernel/include/linux/types.h" 1







# 1 "/opt/Wive/kernel/include/linux/posix_types.h" 1
# 36 "/opt/Wive/kernel/include/linux/posix_types.h"
typedef struct {
        unsigned long fds_bits [(1024/(8 * sizeof(unsigned long)))];
} __kernel_fd_set;


typedef void (*__kernel_sighandler_t)(int);


typedef int __kernel_key_t;

# 1 "/opt/Wive/kernel/include/asm/posix_types.h" 1
# 17 "/opt/Wive/kernel/include/asm/posix_types.h"
typedef unsigned int __kernel_dev_t;
typedef unsigned long __kernel_ino_t;
typedef unsigned int __kernel_mode_t;
typedef unsigned long __kernel_nlink_t;
typedef long __kernel_off_t;
typedef int __kernel_pid_t;
typedef int __kernel_ipc_pid_t;
typedef int __kernel_uid_t;
typedef int __kernel_gid_t;
typedef unsigned int __kernel_size_t;
typedef int __kernel_ssize_t;
typedef int __kernel_ptrdiff_t;
typedef long __kernel_time_t;
typedef long __kernel_suseconds_t;
typedef long __kernel_clock_t;
typedef long __kernel_daddr_t;
typedef char * __kernel_caddr_t;

typedef unsigned short __kernel_uid16_t;
typedef unsigned short __kernel_gid16_t;
typedef int __kernel_uid32_t;
typedef int __kernel_gid32_t;
typedef __kernel_uid_t __kernel_old_uid_t;
typedef __kernel_gid_t __kernel_old_gid_t;


typedef long long __kernel_loff_t;


typedef struct {
        long val[2];
} __kernel_fsid_t;




static __inline__ void __FD_SET(unsigned long __fd, __kernel_fd_set *__fdsetp)
{
        unsigned long __tmp = __fd / (8 * sizeof(unsigned long));
        unsigned long __rem = __fd % (8 * sizeof(unsigned long));
        __fdsetp->fds_bits[__tmp] |= (1UL<<__rem);
}


static __inline__ void __FD_CLR(unsigned long __fd, __kernel_fd_set *__fdsetp)
{
        unsigned long __tmp = __fd / (8 * sizeof(unsigned long));
        unsigned long __rem = __fd % (8 * sizeof(unsigned long));
        __fdsetp->fds_bits[__tmp] &= ~(1UL<<__rem);
}


static __inline__ int __FD_ISSET(unsigned long __fd, const __kernel_fd_set *__p)
{
        unsigned long __tmp = __fd / (8 * sizeof(unsigned long));
        unsigned long __rem = __fd % (8 * sizeof(unsigned long));
        return (__p->fds_bits[__tmp] & (1UL<<__rem)) != 0;
}






static __inline__ void __FD_ZERO(__kernel_fd_set *__p)
{
        unsigned long *__tmp = __p->fds_bits;
        int __i;

        if (__builtin_constant_p((1024/(8 * sizeof(unsigned long))))) {
                switch ((1024/(8 * sizeof(unsigned long)))) {
                case 16:
                        __tmp[ 0] = 0; __tmp[ 1] = 0;
                        __tmp[ 2] = 0; __tmp[ 3] = 0;
                        __tmp[ 4] = 0; __tmp[ 5] = 0;
                        __tmp[ 6] = 0; __tmp[ 7] = 0;
                        __tmp[ 8] = 0; __tmp[ 9] = 0;
                        __tmp[10] = 0; __tmp[11] = 0;
                        __tmp[12] = 0; __tmp[13] = 0;
                        __tmp[14] = 0; __tmp[15] = 0;
                        return;

                case 8:
                        __tmp[ 0] = 0; __tmp[ 1] = 0;
                        __tmp[ 2] = 0; __tmp[ 3] = 0;
                        __tmp[ 4] = 0; __tmp[ 5] = 0;
                        __tmp[ 6] = 0; __tmp[ 7] = 0;
                        return;

                case 4:
                        __tmp[ 0] = 0; __tmp[ 1] = 0;
                        __tmp[ 2] = 0; __tmp[ 3] = 0;
                        return;
                }
        }
        __i = (1024/(8 * sizeof(unsigned long)));
        while (__i) {
                __i--;
                *__tmp = 0;
                __tmp++;
        }
}
# 47 "/opt/Wive/kernel/include/linux/posix_types.h" 2
# 9 "/opt/Wive/kernel/include/linux/types.h" 2
# 1 "/opt/Wive/kernel/include/asm/types.h" 1
# 14 "/opt/Wive/kernel/include/asm/types.h"
typedef unsigned short umode_t;






typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
typedef unsigned int __u32;
# 38 "/opt/Wive/kernel/include/asm/types.h"
typedef __signed__ long long __s64;
typedef unsigned long long __u64;
# 49 "/opt/Wive/kernel/include/asm/types.h"
typedef __signed char s8;
typedef unsigned char u8;

typedef __signed short s16;
typedef unsigned short u16;

typedef __signed int s32;
typedef unsigned int u32;
# 66 "/opt/Wive/kernel/include/asm/types.h"
typedef __signed__ long long s64;
typedef unsigned long long u64;
# 77 "/opt/Wive/kernel/include/asm/types.h"
typedef u32 dma_addr_t;

typedef u64 dma64_addr_t;




typedef unsigned long phys_t;
# 10 "/opt/Wive/kernel/include/linux/types.h" 2



typedef __kernel_fd_set fd_set;
typedef __kernel_dev_t dev_t;
typedef __kernel_ino_t ino_t;
typedef __kernel_mode_t mode_t;
typedef __kernel_nlink_t nlink_t;
typedef __kernel_off_t off_t;
typedef __kernel_pid_t pid_t;
typedef __kernel_daddr_t daddr_t;
typedef __kernel_key_t key_t;
typedef __kernel_suseconds_t suseconds_t;


typedef __kernel_uid32_t uid_t;
typedef __kernel_gid32_t gid_t;
typedef __kernel_uid16_t uid16_t;
typedef __kernel_gid16_t gid16_t;
# 45 "/opt/Wive/kernel/include/linux/types.h"
typedef __kernel_loff_t loff_t;
# 54 "/opt/Wive/kernel/include/linux/types.h"
typedef __kernel_size_t size_t;




typedef __kernel_ssize_t ssize_t;




typedef __kernel_ptrdiff_t ptrdiff_t;




typedef __kernel_time_t time_t;




typedef __kernel_clock_t clock_t;




typedef __kernel_caddr_t caddr_t;



typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;


typedef unsigned char unchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;




typedef __u8 u_int8_t;
typedef __s8 int8_t;
typedef __u16 u_int16_t;
typedef __s16 int16_t;
typedef __u32 u_int32_t;
typedef __s32 int32_t;



typedef __u8 uint8_t;
typedef __u16 uint16_t;
typedef __u32 uint32_t;


typedef __u64 uint64_t;
typedef __u64 u_int64_t;
typedef __s64 int64_t;
# 123 "/opt/Wive/kernel/include/linux/types.h"
struct ustat {
        __kernel_daddr_t f_tfree;
        __kernel_ino_t f_tinode;
        char f_fname[6];
        char f_fpack[6];
};
# 14 "/opt/Wive/kernel/include/linux/kernel.h" 2
# 1 "/opt/Wive/kernel/include/linux/compiler.h" 1
# 15 "/opt/Wive/kernel/include/linux/kernel.h" 2
# 1 "/opt/Wive/kernel/include/asm/byteorder.h" 1
# 23 "/opt/Wive/kernel/include/asm/byteorder.h"
# 1 "/opt/Wive/kernel/include/linux/byteorder/big_endian.h" 1
# 11 "/opt/Wive/kernel/include/linux/byteorder/big_endian.h"
# 1 "/opt/Wive/kernel/include/linux/byteorder/swab.h" 1
# 131 "/opt/Wive/kernel/include/linux/byteorder/swab.h"
static __inline__ __const__ __u16 __fswab16(__u16 x)
{
        return ({ __u16 __tmp = (x) ; ({ __u16 __x = (__tmp); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }); });
}
static __inline__ __u16 __swab16p(__u16 *x)
{
        return ({ __u16 __tmp = (*(x)) ; ({ __u16 __x = (__tmp); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }); });
}
static __inline__ void __swab16s(__u16 *addr)
{
        do { *(addr) = ({ __u16 __tmp = (*((addr))) ; ({ __u16 __x = (__tmp); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }); }); } while (0);
}

static __inline__ __const__ __u32 __fswab32(__u32 x)
{
        return ({ __u32 __tmp = (x) ; ({ __u32 __x = (__tmp); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }); });
}
static __inline__ __u32 __swab32p(__u32 *x)
{
        return ({ __u32 __tmp = (*(x)) ; ({ __u32 __x = (__tmp); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }); });
}
static __inline__ void __swab32s(__u32 *addr)
{
        do { *(addr) = ({ __u32 __tmp = (*((addr))) ; ({ __u32 __x = (__tmp); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }); }); } while (0);
}


static __inline__ __const__ __u64 __fswab64(__u64 x)
{

        __u32 h = x >> 32;
        __u32 l = x & ((1ULL<<32)-1);
        return (((__u64)(__builtin_constant_p((__u32)(l)) ? ({ __u32 __x = ((l)); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32((l)))) << 32) | ((__u64)((__builtin_constant_p((__u32)(h)) ? ({ __u32 __x = ((h)); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32((h)))));



}
static __inline__ __u64 __swab64p(__u64 *x)
{
        return ({ __u64 __tmp = (*(x)) ; ({ __u64 __x = (__tmp); ((__u64)( (__u64)(((__u64)(__x) & (__u64)0x00000000000000ffULL) << 56) | (__u64)(((__u64)(__x) & (__u64)0x000000000000ff00ULL) << 40) | (__u64)(((__u64)(__x) & (__u64)0x0000000000ff0000ULL) << 24) | (__u64)(((__u64)(__x) & (__u64)0x00000000ff000000ULL) << 8) | (__u64)(((__u64)(__x) & (__u64)0x000000ff00000000ULL) >> 8) | (__u64)(((__u64)(__x) & (__u64)0x0000ff0000000000ULL) >> 24) | (__u64)(((__u64)(__x) & (__u64)0x00ff000000000000ULL) >> 40) | (__u64)(((__u64)(__x) & (__u64)0xff00000000000000ULL) >> 56) )); }); });
}
static __inline__ void __swab64s(__u64 *addr)
{
        do { *(addr) = ({ __u64 __tmp = (*((addr))) ; ({ __u64 __x = (__tmp); ((__u64)( (__u64)(((__u64)(__x) & (__u64)0x00000000000000ffULL) << 56) | (__u64)(((__u64)(__x) & (__u64)0x000000000000ff00ULL) << 40) | (__u64)(((__u64)(__x) & (__u64)0x0000000000ff0000ULL) << 24) | (__u64)(((__u64)(__x) & (__u64)0x00000000ff000000ULL) << 8) | (__u64)(((__u64)(__x) & (__u64)0x000000ff00000000ULL) >> 8) | (__u64)(((__u64)(__x) & (__u64)0x0000ff0000000000ULL) >> 24) | (__u64)(((__u64)(__x) & (__u64)0x00ff000000000000ULL) >> 40) | (__u64)(((__u64)(__x) & (__u64)0xff00000000000000ULL) >> 56) )); }); }); } while (0);
}
# 12 "/opt/Wive/kernel/include/linux/byteorder/big_endian.h" 2
# 82 "/opt/Wive/kernel/include/linux/byteorder/big_endian.h"
# 1 "/opt/Wive/kernel/include/linux/byteorder/generic.h" 1
# 149 "/opt/Wive/kernel/include/linux/byteorder/generic.h"
extern __u32 ntohl(__u32);
extern __u32 htonl(__u32);
extern unsigned short int ntohs(unsigned short int);
extern unsigned short int htons(unsigned short int);
# 83 "/opt/Wive/kernel/include/linux/byteorder/big_endian.h" 2
# 24 "/opt/Wive/kernel/include/asm/byteorder.h" 2
# 16 "/opt/Wive/kernel/include/linux/kernel.h" 2
# 42 "/opt/Wive/kernel/include/linux/kernel.h"
extern int console_printk[];
# 61 "/opt/Wive/kernel/include/linux/kernel.h"
struct completion;

extern struct notifier_block *panic_notifier_list;
 void panic(const char * fmt, ...)
        __attribute__ ((noreturn, format (printf, 1, 2)));
 void do_exit(long error_code)
        __attribute__((noreturn));
 void complete_and_exit(struct completion *, long)
        __attribute__((noreturn));
extern int abs(int);
extern unsigned long simple_strtoul(const char *,char **,unsigned int);
extern long simple_strtol(const char *,char **,unsigned int);
extern unsigned long long simple_strtoull(const char *,char **,unsigned int);
extern long long simple_strtoll(const char *,char **,unsigned int);
extern int sprintf(char * buf, const char * fmt, ...)
        __attribute__ ((format (printf, 2, 3)));
extern int vsprintf(char *buf, const char *, va_list)
        __attribute__ ((format (printf, 2, 0)));
extern int snprintf(char * buf, size_t size, const char * fmt, ...)
        __attribute__ ((format (printf, 3, 4)));
extern int vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
        __attribute__ ((format (printf, 3, 0)));

extern int sscanf(const char *, const char *, ...)
        __attribute__ ((format (scanf, 2, 3)));
extern int vsscanf(const char *, const char *, va_list)
        __attribute__ ((format (scanf, 2, 0)));

extern int get_option(char **str, int *pint);
extern char *get_options(char *str, int nints, int *ints);
extern unsigned long long memparse(char *ptr, char **retptr);
extern void dev_probe_lock(void);
extern void dev_probe_unlock(void);

extern int session_of_pgrp(int pgrp);




 int printk(const char * fmt, ...)
        __attribute__ ((format (printf, 1, 2)));


static inline void console_silent(void)
{
        (console_printk[0]) = 0;
}

static inline void console_verbose(void)
{
        if ((console_printk[0]))
                (console_printk[0]) = 15;
}

extern void bust_spinlocks(int yes);
extern int oops_in_progress;

extern int tainted;
extern const char *print_tainted(void);

extern void dump_stack(void);
# 178 "/opt/Wive/kernel/include/linux/kernel.h"
extern void __out_of_line_bug(int line) __attribute__((noreturn));





struct sysinfo {
        long uptime;
        unsigned long loads[3];
        unsigned long totalram;
        unsigned long freeram;
        unsigned long sharedram;
        unsigned long bufferram;
        unsigned long totalswap;
        unsigned long freeswap;
        unsigned short procs;
        unsigned short pad;
        unsigned long totalhigh;
        unsigned long freehigh;
        unsigned int mem_unit;
        char _f[20-2*sizeof(long)-sizeof(int)];
};
# 14 "/opt/Wive/kernel/include/linux/wait.h" 2
# 1 "/opt/Wive/kernel/include/linux/list.h" 1






# 1 "/opt/Wive/kernel/include/linux/prefetch.h" 1
# 13 "/opt/Wive/kernel/include/linux/prefetch.h"
# 1 "/opt/Wive/kernel/include/asm/processor.h" 1
# 15 "/opt/Wive/kernel/include/asm/processor.h"
# 1 "/opt/Wive/kernel/include/linux/cache.h" 1




# 1 "/opt/Wive/kernel/include/asm/cache.h" 1
# 18 "/opt/Wive/kernel/include/asm/cache.h"
struct cache_desc {
        int linesz;
        int sets;
        int ways;
        int flags;
};
# 6 "/opt/Wive/kernel/include/linux/cache.h" 2
# 16 "/opt/Wive/kernel/include/asm/processor.h" 2
# 1 "/opt/Wive/kernel/include/asm/isadep.h" 1
# 17 "/opt/Wive/kernel/include/asm/processor.h" 2
# 25 "/opt/Wive/kernel/include/asm/processor.h"
# 1 "/opt/Wive/kernel/include/linux/threads.h" 1
# 26 "/opt/Wive/kernel/include/asm/processor.h" 2
# 1 "/opt/Wive/kernel/include/asm/cachectl.h" 1
# 27 "/opt/Wive/kernel/include/asm/processor.h" 2
# 1 "/opt/Wive/kernel/include/asm/mipsregs.h" 1
# 452 "/opt/Wive/kernel/include/asm/mipsregs.h"
# 1 "/opt/Wive/kernel/include/asm/system.h" 1
# 20 "/opt/Wive/kernel/include/asm/system.h"
# 1 "/opt/Wive/kernel/include/asm/sgidefs.h" 1
# 21 "/opt/Wive/kernel/include/asm/system.h" 2
# 1 "/opt/Wive/kernel/include/asm/ptrace.h" 1
# 14 "/opt/Wive/kernel/include/asm/ptrace.h"
# 1 "/opt/Wive/kernel/include/asm/isadep.h" 1
# 15 "/opt/Wive/kernel/include/asm/ptrace.h" 2
# 31 "/opt/Wive/kernel/include/asm/ptrace.h"
struct pt_regs {

        unsigned long pad0[6];


        unsigned long regs[32];


        unsigned long cp0_status;
        unsigned long lo;
        unsigned long hi;
        unsigned long cp0_badvaddr;
        unsigned long cp0_cause;
        unsigned long cp0_epc;
};
# 102 "/opt/Wive/kernel/include/asm/ptrace.h"
extern void show_regs(struct pt_regs *);
# 22 "/opt/Wive/kernel/include/asm/system.h" 2


__asm__ (
        ".macro\t__sti\n\t"
        ".set\tpush\n\t"
        ".set\treorder\n\t"
        ".set\tnoat\n\t"
        "mfc0\t$1,$12\n\t"
        "ori\t$1,0x1f\n\t"
        "xori\t$1,0x1e\n\t"
        "mtc0\t$1,$12\n\t"
        ".set\tpop\n\t"
        ".endm");

extern __inline__ void
__sti(void)
{
        __asm__ __volatile__(
                "__sti"
                :
                :
                : "memory");
}
# 53 "/opt/Wive/kernel/include/asm/system.h"
__asm__ (
        ".macro\t__cli\n\t"
        ".set\tpush\n\t"
        ".set\tnoat\n\t"
        "mfc0\t$1,$12\n\t"
        "ori\t$1,1\n\t"
        "xori\t$1,1\n\t"
        ".set\tnoreorder\n\t"
        "mtc0\t$1,$12\n\t"
        "sll\t$0, $0, 1\t\t\t# nop\n\t"
        "sll\t$0, $0, 1\t\t\t# nop\n\t"
        "sll\t$0, $0, 1\t\t\t# nop\n\t"
        ".set\tpop\n\t"
        ".endm");

extern __inline__ void
__cli(void)
{
        __asm__ __volatile__(
                "__cli"
                :
                :
                : "memory");
}

__asm__ (
        ".macro\t__save_flags flags\n\t"
        ".set\tpush\n\t"
        ".set\treorder\n\t"
        "mfc0\t\\flags, $12\n\t"
        ".set\tpop\n\t"
        ".endm");






__asm__ (
        ".macro\t__save_and_cli result\n\t"
        ".set\tpush\n\t"
        ".set\treorder\n\t"
        ".set\tnoat\n\t"
        "mfc0\t\\result, $12\n\t"
        "ori\t$1, \\result, 1\n\t"
        "xori\t$1, 1\n\t"
        ".set\tnoreorder\n\t"
        "mtc0\t$1, $12\n\t"
        "sll\t$0, $0, 1\t\t\t# nop\n\t"
        "sll\t$0, $0, 1\t\t\t# nop\n\t"
        "sll\t$0, $0, 1\t\t\t# nop\n\t"
        ".set\tpop\n\t"
        ".endm");
# 114 "/opt/Wive/kernel/include/asm/system.h"
__asm__(".macro\t__restore_flags flags\n\t"
        ".set\tnoreorder\n\t"
        ".set\tnoat\n\t"
        "mfc0\t$1, $12\n\t"
        "andi\t\\flags, 1\n\t"
        "ori\t$1, 1\n\t"
        "xori\t$1, 1\n\t"
        "or\t\\flags, $1\n\t"
        "mtc0\t\\flags, $12\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        ".set\tat\n\t"
        ".set\treorder\n\t"
        ".endm");
# 214 "/opt/Wive/kernel/include/asm/system.h"
extern void *resume(void *last, void *next);
# 227 "/opt/Wive/kernel/include/asm/system.h"
extern __inline__ unsigned long xchg_u32(volatile int * m, unsigned long val)
{
# 249 "/opt/Wive/kernel/include/asm/system.h"
        unsigned long flags, retval;

        __asm__ __volatile__( "__save_flags %0" : "=r" (flags));
        __cli();
        retval = *m;
        *m = val;
        do { unsigned long __tmp1; __asm__ __volatile__( "__restore_flags\t%0" : "=r" (__tmp1) : "0" (flags) : "memory"); } while(0);
        return retval;

}




static __inline__ unsigned long
__xchg(unsigned long x, volatile void * ptr, int size)
{
        switch (size) {
                case 4:
                        return xchg_u32(ptr, x);
        }
        return x;
}

extern void *set_except_vector(int n, void *addr);

extern void __die(const char *, struct pt_regs *, const char *where,
        unsigned long line) __attribute__((noreturn));
extern void __die_if_kernel(const char *, struct pt_regs *, const char *where,
        unsigned long line);
# 453 "/opt/Wive/kernel/include/asm/mipsregs.h" 2
# 548 "/opt/Wive/kernel/include/asm/mipsregs.h"
static inline void tlb_probe(void)
{
        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "tlbp\n\t"
                ".set pop");
}

static inline void tlb_read(void)
{
        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "tlbr\n\t"
                ".set pop");
}

static inline void tlb_write_indexed(void)
{
        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "tlbwi\n\t"
                ".set pop");
}

static inline void tlb_write_random(void)
{
        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "tlbwr\n\t"
                ".set pop");
}




static inline unsigned long get_pagemask(void)
{
        unsigned long val;

        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mfc0 %0, $5\n\t"
                ".set pop"
                : "=r" (val));
        return val;
}

static inline void set_pagemask(unsigned long val)
{
        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mtc0 %z0, $5\n\t"
                ".set pop"
                : : "Jr" (val));
}
# 688 "/opt/Wive/kernel/include/asm/mipsregs.h"
static inline void set_entrylo0(unsigned long val)
{
        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mtc0 %z0, $2\n\t"
                ".set pop"
                : : "Jr" (val));
}

static inline void set_entrylo1(unsigned long val)
{
        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mtc0 %z0, $3\n\t"
                ".set pop"
                : : "Jr" (val));
}

static inline unsigned long get_entrylo0(void)
{
        unsigned long val;

        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mfc0 %0, $2\n\t"
                ".set pop" : "=r" (val));

        return val;
}

static inline unsigned long get_entrylo1(void)
{
        unsigned long val;

        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mfc0 %0, $3\n\t"
                ".set pop" : "=r" (val));

        return val;
}




static inline unsigned long get_entryhi(void)
{
        unsigned long val;

        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mfc0 %0, $10\n\t"
                ".set pop"
                : "=r" (val));

        return val;
}

static inline void set_entryhi(unsigned long val)
{
        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mtc0 %z0, $10\n\t"
                ".set pop"
                : : "Jr" (val));
}


static inline unsigned long get_index(void)
{
        unsigned long val;

        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mfc0 %0, $0\n\t"
                ".set pop"
                : "=r" (val));
        return val;
}

static inline void set_index(unsigned long val)
{
        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mtc0 %z0, $0\n\t"
                ".set pop"
                : : "Jr" (val));
}


static inline unsigned long get_wired(void)
{
        unsigned long val;

        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mfc0 %0, $6\n\t"
                ".set pop"
                : "=r" (val));
        return val;
}

static inline void set_wired(unsigned long val)
{
        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mtc0 %z0, $6\n\t"
                ".set pop"
                : : "Jr" (val));
}


static inline unsigned int get_status(void)
{
        unsigned long val;

        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mfc0 %0, $12\n\t"
                ".set pop"
                : "=r" (val));
        return val;
}

static inline void set_status(unsigned long val)
{
        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mtc0 %z0, $12\n\t"
                ".set pop"
                : : "Jr" (val));
}

static inline unsigned long get_info(void)
{
        unsigned long val;

        __asm__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mfc0 %0, $7\n\t"
                ".set pop"
                : "=r" (val));
        return val;
}


static inline unsigned long get_taglo(void)
{
        unsigned long val;

        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mfc0 %0, $28\n\t"
                ".set pop"
                : "=r" (val));
        return val;
}

static inline void set_taglo(unsigned long val)
{
        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mtc0 %z0, $28\n\t"
                ".set pop"
                : : "Jr" (val));
}

static inline unsigned long get_taghi(void)
{
        unsigned long val;

        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mfc0 %0, $29\n\t"
                ".set pop"
                : "=r" (val));
        return val;
}

static inline void set_taghi(unsigned long val)
{
        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mtc0 %z0, $29\n\t"
                ".set pop"
                : : "Jr" (val));
}

static inline unsigned long get_context(void)
{
        unsigned long val;

        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mfc0 %0, $4\n\t"
                ".set pop"
                : "=r" (val));

        return val;
}

static inline void set_context(unsigned long val)
{
        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mtc0 %z0, $4\n\t"
                ".set pop"
                : : "Jr" (val));
}

static inline unsigned long get_errorepc(void)
{
        unsigned long val;

        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mfc0 %0, $30\n\t"
                ".set pop"
                : "=r" (val));

        return val;
}

static inline void set_errorepc(unsigned long val)
{
        __asm__ __volatile__(
                ".set push\n\t"
                ".set reorder\n\t"
                "mtc0 %z0, $30\n\t"
                ".set pop"
                : : "Jr" (val));
}
# 983 "/opt/Wive/kernel/include/asm/mipsregs.h"
static inline unsigned int set_cp0_status(unsigned int set) { unsigned int res; res = ({ int __res; __asm__ __volatile__( ".set\tpush\n\t" ".set\treorder\n\t" "mfc0\t%0,""$12""\n\t" ".set\tpop" : "=r" (__res)); __res;}); res |= set; __asm__ __volatile__( "mtc0\t%0,""$12""\n\t" "nop" : : "r" (res));; return res; } static inline unsigned int clear_cp0_status(unsigned int clear) { unsigned int res; res = ({ int __res; __asm__ __volatile__( ".set\tpush\n\t" ".set\treorder\n\t" "mfc0\t%0,""$12""\n\t" ".set\tpop" : "=r" (__res)); __res;}); res &= ~clear; __asm__ __volatile__( "mtc0\t%0,""$12""\n\t" "nop" : : "r" (res));; return res; } static inline unsigned int change_cp0_status(unsigned int change, unsigned int new) { unsigned int res; res = ({ int __res; __asm__ __volatile__( ".set\tpush\n\t" ".set\treorder\n\t" "mfc0\t%0,""$12""\n\t" ".set\tpop" : "=r" (__res)); __res;}); res &= ~change; res |= (new & change); __asm__ __volatile__( "mtc0\t%0,""$12""\n\t" "nop" : : "r" (res));; return res; }
static inline unsigned int set_cp0_cause(unsigned int set) { unsigned int res; res = ({ int __res; __asm__ __volatile__( ".set\tpush\n\t" ".set\treorder\n\t" "mfc0\t%0,""$13""\n\t" ".set\tpop" : "=r" (__res)); __res;}); res |= set; __asm__ __volatile__( "mtc0\t%0,""$13""\n\t" "nop" : : "r" (res));; return res; } static inline unsigned int clear_cp0_cause(unsigned int clear) { unsigned int res; res = ({ int __res; __asm__ __volatile__( ".set\tpush\n\t" ".set\treorder\n\t" "mfc0\t%0,""$13""\n\t" ".set\tpop" : "=r" (__res)); __res;}); res &= ~clear; __asm__ __volatile__( "mtc0\t%0,""$13""\n\t" "nop" : : "r" (res));; return res; } static inline unsigned int change_cp0_cause(unsigned int change, unsigned int new) { unsigned int res; res = ({ int __res; __asm__ __volatile__( ".set\tpush\n\t" ".set\treorder\n\t" "mfc0\t%0,""$13""\n\t" ".set\tpop" : "=r" (__res)); __res;}); res &= ~change; res |= (new & change); __asm__ __volatile__( "mtc0\t%0,""$13""\n\t" "nop" : : "r" (res));; return res; }
static inline unsigned int set_cp0_config(unsigned int set) { unsigned int res; res = ({ int __res; __asm__ __volatile__( ".set\tpush\n\t" ".set\treorder\n\t" "mfc0\t%0,""$16""\n\t" ".set\tpop" : "=r" (__res)); __res;}); res |= set; __asm__ __volatile__( "mtc0\t%0,""$16""\n\t" "nop" : : "r" (res));; return res; } static inline unsigned int clear_cp0_config(unsigned int clear) { unsigned int res; res = ({ int __res; __asm__ __volatile__( ".set\tpush\n\t" ".set\treorder\n\t" "mfc0\t%0,""$16""\n\t" ".set\tpop" : "=r" (__res)); __res;}); res &= ~clear; __asm__ __volatile__( "mtc0\t%0,""$16""\n\t" "nop" : : "r" (res));; return res; } static inline unsigned int change_cp0_config(unsigned int change, unsigned int new) { unsigned int res; res = ({ int __res; __asm__ __volatile__( ".set\tpush\n\t" ".set\treorder\n\t" "mfc0\t%0,""$16""\n\t" ".set\tpop" : "=r" (__res)); __res;}); res &= ~change; res |= (new & change); __asm__ __volatile__( "mtc0\t%0,""$16""\n\t" "nop" : : "r" (res));; return res; }
# 28 "/opt/Wive/kernel/include/asm/processor.h" 2
# 1 "/opt/Wive/kernel/include/asm/reg.h" 1
# 29 "/opt/Wive/kernel/include/asm/processor.h" 2


struct cpuinfo_mips {
        unsigned long udelay_val;
        unsigned long *pgd_quick;
        unsigned long *pte_quick;
        unsigned long pgtable_cache_sz;
        unsigned long asid_cache;
} __attribute__((__aligned__(16)));




extern void (*cpu_wait)(void);
extern void r3081_wait(void);
extern void r39xx_wait(void);
extern void r4k_wait(void);

extern struct cpuinfo_mips cpu_data[];
extern unsigned int vced_count, vcei_count;
# 63 "/opt/Wive/kernel/include/asm/processor.h"
extern int EISA_bus;
# 74 "/opt/Wive/kernel/include/asm/processor.h"
extern struct task_struct *last_task_used_math;
# 96 "/opt/Wive/kernel/include/asm/processor.h"
struct mips_fpu_hard_struct {
        double fp_regs[32];
        unsigned int control;
};







typedef u64 fpureg_t;
struct mips_fpu_soft_struct {
        fpureg_t regs[32];
        unsigned int sr;
};

union mips_fpu_union {
        struct mips_fpu_hard_struct hard;
        struct mips_fpu_soft_struct soft;
};





typedef struct {
        unsigned long seg;
} mm_segment_t;




struct thread_struct {

        unsigned long reg16;
        unsigned long reg17, reg18, reg19, reg20, reg21, reg22, reg23;
        unsigned long reg29, reg30, reg31;


        unsigned long cp0_status;


        union mips_fpu_union fpu;


        unsigned long cp0_badvaddr;
        unsigned long cp0_baduaddr;
        unsigned long error_code;
        unsigned long trap_no;


        unsigned long mflags;
        mm_segment_t current_ds;
        unsigned long irix_trampoline;
        unsigned long irix_oldctx;
};
# 189 "/opt/Wive/kernel/include/asm/processor.h"
extern int kernel_thread(int (*fn)(void *), void * arg, unsigned long flags);
# 198 "/opt/Wive/kernel/include/asm/processor.h"
static inline unsigned long thread_saved_pc(struct thread_struct *t)
{
        extern void ret_from_fork(void);


        if (t->reg31 == (unsigned long) ret_from_fork)
                return t->reg31;

        return ((unsigned long *)t->reg29)[10];
}
# 221 "/opt/Wive/kernel/include/asm/processor.h"
unsigned long get_wchan(struct task_struct *p);
# 14 "/opt/Wive/kernel/include/linux/prefetch.h" 2
# 43 "/opt/Wive/kernel/include/linux/prefetch.h"
static inline void prefetch(const void *x) {;}




static inline void prefetchw(const void *x) {;}
# 8 "/opt/Wive/kernel/include/linux/list.h" 2
# 26 "/opt/Wive/kernel/include/linux/list.h"
struct list_head {
        struct list_head *next, *prev;
};
# 45 "/opt/Wive/kernel/include/linux/list.h"
static inline void __list_add(struct list_head *new,
                              struct list_head *prev,
                              struct list_head *next)
{
        next->prev = new;
        new->next = next;
        new->prev = prev;
        prev->next = new;
}
# 63 "/opt/Wive/kernel/include/linux/list.h"
static inline void list_add(struct list_head *new, struct list_head *head)
{
        __list_add(new, head, head->next);
}
# 76 "/opt/Wive/kernel/include/linux/list.h"
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
        __list_add(new, head->prev, head);
}
# 88 "/opt/Wive/kernel/include/linux/list.h"
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
        next->prev = prev;
        prev->next = next;
}






static inline void list_del(struct list_head *entry)
{
        __list_del(entry->prev, entry->next);
        entry->next = (void *) 0;
        entry->prev = (void *) 0;
}





static inline void list_del_init(struct list_head *entry)
{
        __list_del(entry->prev, entry->next);
        do { (entry)->next = (entry); (entry)->prev = (entry); } while (0);
}






static inline void list_move(struct list_head *list, struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add(list, head);
}






static inline void list_move_tail(struct list_head *list,
                                  struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add_tail(list, head);
}





static inline int list_empty(struct list_head *head)
{
        return head->next == head;
}

static inline void __list_splice(struct list_head *list,
                                 struct list_head *head)
{
        struct list_head *first = list->next;
        struct list_head *last = list->prev;
        struct list_head *at = head->next;

        first->prev = head;
        head->next = first;

        last->next = at;
        at->prev = last;
}






static inline void list_splice(struct list_head *list, struct list_head *head)
{
        if (!list_empty(list))
                __list_splice(list, head);
}
# 180 "/opt/Wive/kernel/include/linux/list.h"
static inline void list_splice_init(struct list_head *list,
                                    struct list_head *head)
{
        if (!list_empty(list)) {
                __list_splice(list, head);
                do { (list)->next = (list); (list)->prev = (list); } while (0);
        }
}
# 280 "/opt/Wive/kernel/include/linux/list.h"
struct hlist_head {
        struct hlist_node *first;
};

struct hlist_node {
        struct hlist_node *next, **pprev;
};






static inline int hlist_unhashed(const struct hlist_node *h)
{
        return !h->pprev;
}

static inline int hlist_empty(const struct hlist_head *h)
{
        return !h->first;
}

static inline void __hlist_del(struct hlist_node *n)
{
        struct hlist_node *next = n->next;
        struct hlist_node **pprev = n->pprev;
        *pprev = next;
        if (next)
                next->pprev = pprev;
}

static inline void hlist_del(struct hlist_node *n)
{
        __hlist_del(n);
        n->next = ((void *) 0x00100100);
        n->pprev = ((void *) 0x00200200);
}

static inline void hlist_del_init(struct hlist_node *n)
{
        if (n->pprev) {
                __hlist_del(n);
                ((n)->next = ((void *)0), (n)->pprev = ((void *)0));
        }
}

static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
{
        struct hlist_node *first = h->first;
        n->next = first;
        if (first)
                first->pprev = &n->next;
        h->first = n;
        n->pprev = &h->first;
}


static inline void hlist_add_before(struct hlist_node *n,
                                        struct hlist_node *next)
{
        n->pprev = next->pprev;
        n->next = next;
        next->pprev = &n->next;
        *(n->pprev) = n;
}

static inline void hlist_add_after(struct hlist_node *n,
                                        struct hlist_node *next)
{
        next->next = n->next;
        n->next = next;
        next->pprev = &n->next;

        if(next->next)
                next->next->pprev = &next->next;
}
# 15 "/opt/Wive/kernel/include/linux/wait.h" 2

# 1 "/opt/Wive/kernel/include/linux/spinlock.h" 1
# 55 "/opt/Wive/kernel/include/linux/spinlock.h"
  typedef struct { } spinlock_t;
# 118 "/opt/Wive/kernel/include/linux/spinlock.h"
  typedef struct { } rwlock_t;
# 151 "/opt/Wive/kernel/include/linux/spinlock.h"
# 1 "/opt/Wive/kernel/include/asm/atomic.h" 1
# 19 "/opt/Wive/kernel/include/asm/atomic.h"
typedef struct { volatile int counter; } atomic_t;
# 58 "/opt/Wive/kernel/include/asm/atomic.h"
extern __inline__ void atomic_add(int i, atomic_t * v)
{
        int flags;

        __asm__ __volatile__( "__save_flags %0" : "=r" (flags));
        __cli();
        v->counter += i;
        do { unsigned long __tmp1; __asm__ __volatile__( "__restore_flags\t%0" : "=r" (__tmp1) : "0" (flags) : "memory"); } while(0);
}
# 76 "/opt/Wive/kernel/include/asm/atomic.h"
extern __inline__ void atomic_sub(int i, atomic_t * v)
{
        int flags;

        __asm__ __volatile__( "__save_flags %0" : "=r" (flags));
        __cli();
        v->counter -= i;
        do { unsigned long __tmp1; __asm__ __volatile__( "__restore_flags\t%0" : "=r" (__tmp1) : "0" (flags) : "memory"); } while(0);
}

extern __inline__ int atomic_add_return(int i, atomic_t * v)
{
        int temp, flags;

        __asm__ __volatile__( "__save_flags %0" : "=r" (flags));
        __cli();
        temp = v->counter;
        temp += i;
        v->counter = temp;
        do { unsigned long __tmp1; __asm__ __volatile__( "__restore_flags\t%0" : "=r" (__tmp1) : "0" (flags) : "memory"); } while(0);

        return temp;
}

extern __inline__ int atomic_sub_return(int i, atomic_t * v)
{
        int temp, flags;

        __asm__ __volatile__( "__save_flags %0" : "=r" (flags));
        __cli();
        temp = v->counter;
        temp -= i;
        v->counter = temp;
        do { unsigned long __tmp1; __asm__ __volatile__( "__restore_flags\t%0" : "=r" (__tmp1) : "0" (flags) : "memory"); } while(0);

        return temp;
}
# 152 "/opt/Wive/kernel/include/linux/spinlock.h" 2
extern int atomic_dec_and_lock(atomic_t *atomic, spinlock_t *lock);
# 17 "/opt/Wive/kernel/include/linux/wait.h" 2


# 1 "/opt/Wive/kernel/include/asm/page.h" 1
# 29 "/opt/Wive/kernel/include/asm/page.h"
void andes_clear_page(void * page);
void mips32_clear_page_dc(unsigned long page);
void mips32_clear_page_sc(unsigned long page);
void r3k_clear_page(void * page);
void r4k_clear_page_d16(void * page);
void r4k_clear_page_d32(void * page);
void r4k_clear_page_r4600_v1(void * page);
void r4k_clear_page_r4600_v2(void * page);
void r4k_clear_page_s16(void * page);
void r4k_clear_page_s32(void * page);
void r4k_clear_page_s64(void * page);
void r4k_clear_page_s128(void * page);
void r5432_clear_page_d32(void * page);
void rm7k_clear_page(void * page);
void sb1_clear_page(void * page);
void andes_copy_page(void * to, void * from);
void mips32_copy_page_dc(unsigned long to, unsigned long from);
void mips32_copy_page_sc(unsigned long to, unsigned long from);
void r3k_copy_page(void * to, void * from);
void r4k_copy_page_d16(void * to, void * from);
void r4k_copy_page_d32(void * to, void * from);
void r4k_copy_page_r4600_v1(void * to, void * from);
void r4k_copy_page_r4600_v2(void * to, void * from);
void r4k_copy_page_s16(void * to, void * from);
void r4k_copy_page_s32(void * to, void * from);
void r4k_copy_page_s64(void * to, void * from);
void r4k_copy_page_s128(void * to, void * from);
void r5432_copy_page_d32(void * to, void * from);
void rm7k_copy_page(void * to, void * from);
void sb1_copy_page(void * to, void * from);

extern void (*_clear_page)(void * page);
extern void (*_copy_page)(void * to, void * from);
# 74 "/opt/Wive/kernel/include/asm/page.h"
typedef struct { unsigned long pte; } pte_t;

typedef struct { unsigned long pmd; } pmd_t;
typedef struct { unsigned long pgd; } pgd_t;
typedef struct { unsigned long pgprot; } pgprot_t;
# 91 "/opt/Wive/kernel/include/asm/page.h"
extern __inline__ int get_order(unsigned long size)
{
        int order;

        size = (size-1) >> (12 -1);
        order = -1;
        do {
                size >>= 1;
                order++;
        } while (size);
        return order;
}
# 20 "/opt/Wive/kernel/include/linux/wait.h" 2
# 31 "/opt/Wive/kernel/include/linux/wait.h"
struct __wait_queue {
        unsigned int flags;

        struct task_struct * task;
        struct list_head task_list;




};
typedef struct __wait_queue wait_queue_t;
# 77 "/opt/Wive/kernel/include/linux/wait.h"
struct __wait_queue_head {
        spinlock_t lock;
        struct list_head task_list;




};
typedef struct __wait_queue_head wait_queue_head_t;
# 155 "/opt/Wive/kernel/include/linux/wait.h"
static inline void init_waitqueue_head(wait_queue_head_t *q)
{




        q->lock = (spinlock_t) { };
        do { (&q->task_list)->next = (&q->task_list); (&q->task_list)->prev = (&q->task_list); } while (0);




}

static inline void init_waitqueue_entry(wait_queue_t *q, struct task_struct *p)
{




        q->flags = 0;
        q->task = p;



}

static inline int waitqueue_active(wait_queue_head_t *q)
{






        return !list_empty(&q->task_list);
}

static inline void __add_wait_queue(wait_queue_head_t *head, wait_queue_t *new)
{
# 203 "/opt/Wive/kernel/include/linux/wait.h"
        list_add(&new->task_list, &head->task_list);
}




static inline void __add_wait_queue_tail(wait_queue_head_t *head,
                                                wait_queue_t *new)
{
# 220 "/opt/Wive/kernel/include/linux/wait.h"
        list_add_tail(&new->task_list, &head->task_list);
}

static inline void __remove_wait_queue(wait_queue_head_t *head,
                                                        wait_queue_t *old)
{





        list_del(&old->task_list);
}
# 13 "/opt/Wive/kernel/include/linux/fs.h" 2

# 1 "/opt/Wive/kernel/include/linux/vfs.h" 1



# 1 "/opt/Wive/kernel/include/asm/statfs.h" 1
# 19 "/opt/Wive/kernel/include/asm/statfs.h"
typedef __kernel_fsid_t fsid_t;



struct statfs {
        long f_type;

        long f_bsize;
        long f_frsize;
        long f_blocks;
        long f_bfree;
        long f_files;
        long f_ffree;


        long f_bavail;
        __kernel_fsid_t f_fsid;
        long f_namelen;
        long f_spare[6];
};
# 5 "/opt/Wive/kernel/include/linux/vfs.h" 2
# 15 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/net.h" 1
# 22 "/opt/Wive/kernel/include/linux/net.h"
# 1 "/opt/Wive/kernel/include/linux/socket.h" 1
# 11 "/opt/Wive/kernel/include/linux/socket.h"
struct __kernel_sockaddr_storage {
        unsigned short ss_family;

        char __data[128 - sizeof(unsigned short)];


} __attribute__ ((aligned((__alignof__ (struct sockaddr *)))));





# 1 "/opt/Wive/kernel/include/asm/socket.h" 1



# 1 "/opt/Wive/kernel/include/asm/sockios.h" 1
# 13 "/opt/Wive/kernel/include/asm/sockios.h"
# 1 "/opt/Wive/kernel/include/asm/ioctl.h" 1
# 14 "/opt/Wive/kernel/include/asm/sockios.h" 2
# 5 "/opt/Wive/kernel/include/asm/socket.h" 2
# 24 "/opt/Wive/kernel/include/linux/socket.h" 2
# 1 "/opt/Wive/kernel/include/linux/sockios.h" 1
# 25 "/opt/Wive/kernel/include/linux/socket.h" 2
# 1 "/opt/Wive/kernel/include/linux/uio.h" 1
# 19 "/opt/Wive/kernel/include/linux/uio.h"
struct iovec
{
        void *iov_base;
        __kernel_size_t iov_len;
};
# 26 "/opt/Wive/kernel/include/linux/socket.h" 2

# 1 "/opt/Wive/kernel/include/linux/version.h" 1
# 28 "/opt/Wive/kernel/include/linux/socket.h" 2

typedef unsigned short sa_family_t;





struct sockaddr {
        sa_family_t sa_family;
        char sa_data[14];
};

struct linger {
        int l_onoff;
        int l_linger;
};
# 55 "/opt/Wive/kernel/include/linux/socket.h"
struct sockaddr_storage {
       sa_family_t ss_family;

       char __data[128 - sizeof(sa_family_t)];


} __attribute__ ((aligned((__alignof__ (struct sockaddr *)))));
# 70 "/opt/Wive/kernel/include/linux/socket.h"
struct msghdr {
        void * msg_name;
        int msg_namelen;
        struct iovec * msg_iov;
        __kernel_size_t msg_iovlen;
        void * msg_control;
        __kernel_size_t msg_controllen;
        unsigned msg_flags;
};







struct cmsghdr {
        __kernel_size_t cmsg_len;
        int cmsg_level;
        int cmsg_type;
};
# 139 "/opt/Wive/kernel/include/linux/socket.h"
static inline struct cmsghdr * __cmsg_nxthdr(void *__ctl, __kernel_size_t __size,
                                               struct cmsghdr *__cmsg)
{
        struct cmsghdr * __ptr;

        __ptr = (struct cmsghdr*)(((unsigned char *) __cmsg) + ( ((__cmsg->cmsg_len)+sizeof(long)-1) & ~(sizeof(long)-1) ));
        if ((unsigned long)((char*)(__ptr+1) - (char *) __ctl) > __size)
                return (struct cmsghdr *)0;

        return __ptr;
}

static inline struct cmsghdr * cmsg_nxthdr (struct msghdr *__msg, struct cmsghdr *__cmsg)
{
        return __cmsg_nxthdr(__msg->msg_control, __msg->msg_controllen, __cmsg);
}







struct ucred {
        __u32 pid;
        __u32 uid;
        __u32 gid;
};
# 297 "/opt/Wive/kernel/include/linux/socket.h"
extern int memcpy_fromiovec(unsigned char *kdata, struct iovec *iov, int len);
extern int memcpy_fromiovecend(unsigned char *kdata, struct iovec *iov,
                                int offset, int len);
extern int csum_partial_copy_fromiovecend(unsigned char *kdata,
                                          struct iovec *iov,
                                          int offset,
                                          unsigned int len, int *csump);

extern int verify_iovec(struct msghdr *m, struct iovec *iov, char *address, int mode);
extern int memcpy_toiovec(struct iovec *v, unsigned char *kdata, int len);
extern void memcpy_tokerneliovec(struct iovec *iov, unsigned char *kdata, int len);
extern int move_addr_to_user(void *kaddr, int klen, void *uaddr, int *ulen);
extern int move_addr_to_kernel(void *uaddr, int ulen, void *kaddr);
extern int put_cmsg(struct msghdr*, int level, int type, int len, void *data);
# 23 "/opt/Wive/kernel/include/linux/net.h" 2


struct poll_table_struct;
# 49 "/opt/Wive/kernel/include/linux/net.h"
typedef enum {
  SS_FREE = 0,
  SS_UNCONNECTED,
  SS_CONNECTING,
  SS_CONNECTED,
  SS_DISCONNECTING
} socket_state;
# 65 "/opt/Wive/kernel/include/linux/net.h"
struct socket
{
        socket_state state;

        unsigned long flags;
        struct proto_ops *ops;
        struct inode *inode;
        struct fasync_struct *fasync_list;
        struct file *file;
        struct sock *sk;
        wait_queue_head_t wait;

        short type;
        unsigned char passcred;
};



struct scm_cookie;
struct vm_area_struct;
struct page;

struct proto_ops {
  int family;

  int (*release) (struct socket *sock);
  int (*bind) (struct socket *sock, struct sockaddr *umyaddr,
                         int sockaddr_len);
  int (*connect) (struct socket *sock, struct sockaddr *uservaddr,
                         int sockaddr_len, int flags);
  int (*socketpair) (struct socket *sock1, struct socket *sock2);
  int (*accept) (struct socket *sock, struct socket *newsock,
                         int flags);
  int (*getname) (struct socket *sock, struct sockaddr *uaddr,
                         int *usockaddr_len, int peer);
  unsigned int (*poll) (struct file *file, struct socket *sock, struct poll_table_struct *wait);
  int (*ioctl) (struct socket *sock, unsigned int cmd,
                         unsigned long arg);
  int (*listen) (struct socket *sock, int len);
  int (*shutdown) (struct socket *sock, int flags);
  int (*setsockopt) (struct socket *sock, int level, int optname,
                         char *optval, int optlen);
  int (*getsockopt) (struct socket *sock, int level, int optname,
                         char *optval, int *optlen);
  int (*sendmsg) (struct socket *sock, struct msghdr *m, int total_len, struct scm_cookie *scm);
  int (*recvmsg) (struct socket *sock, struct msghdr *m, int total_len, int flags, struct scm_cookie *scm);
  int (*mmap) (struct file *file, struct socket *sock, struct vm_area_struct * vma);
  ssize_t (*sendpage) (struct socket *sock, struct page *page, int offset, size_t size, int flags);
};

struct net_proto_family
{
        int family;
        int (*create)(struct socket *sock, int protocol);


        short authentication;
        short encryption;
        short encrypt_net;
};

struct net_proto
{
        const char *name;
        void (*init_func)(struct net_proto *);
};

extern int sock_wake_async(struct socket *sk, int how, int band);
extern int sock_register(struct net_proto_family *fam);
extern int sock_unregister(int family);
extern struct socket *sock_alloc(void);
extern int sock_create(int family, int type, int proto, struct socket **);
extern void sock_release(struct socket *);
extern int sock_sendmsg(struct socket *, struct msghdr *m, int len);
extern int sock_recvmsg(struct socket *, struct msghdr *m, int len, int flags);
extern int sock_readv_writev(int type, struct inode * inode, struct file * file,
                                  const struct iovec * iov, long count, long size);

extern int net_ratelimit(void);
extern unsigned long net_random(void);
extern void net_srandom(unsigned long);
# 16 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/kdev_t.h" 1
# 67 "/opt/Wive/kernel/include/linux/kdev_t.h"
typedef unsigned short kdev_t;
# 76 "/opt/Wive/kernel/include/linux/kdev_t.h"
extern const char * kdevname(kdev_t);
# 91 "/opt/Wive/kernel/include/linux/kdev_t.h"
static inline unsigned int kdev_t_to_nr(kdev_t dev) {
        return (((unsigned int) ((dev) >> 8))<<8) | ((unsigned int) ((dev) & ((1U << 8) - 1)));
}

static inline kdev_t to_kdev_t(int dev)
{
        int major, minor;
# 106 "/opt/Wive/kernel/include/linux/kdev_t.h"
        major = (dev >> 8);
        minor = (dev & 0xff);

        return (((major) << 8) | (minor));
}
# 17 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/ioctl.h" 1
# 18 "/opt/Wive/kernel/include/linux/fs.h" 2

# 1 "/opt/Wive/kernel/include/linux/dcache.h" 1






# 1 "/opt/Wive/kernel/include/linux/mount.h" 1
# 19 "/opt/Wive/kernel/include/linux/mount.h"
struct vfsmount
{
        struct list_head mnt_hash;
        struct vfsmount *mnt_parent;
        struct dentry *mnt_mountpoint;
        struct dentry *mnt_root;
        struct super_block *mnt_sb;
        struct list_head mnt_mounts;
        struct list_head mnt_child;
        atomic_t mnt_count;
        int mnt_flags;
        char *mnt_devname;
        struct list_head mnt_list;
};

static inline struct vfsmount *mntget(struct vfsmount *mnt)
{
        if (mnt)
                atomic_add(1,(&mnt->mnt_count));
        return mnt;
}

extern void __mntput(struct vfsmount *mnt);

static inline void mntput(struct vfsmount *mnt)
{
        if (mnt) {
                if ((atomic_sub_return(1, (&mnt->mnt_count)) == 0))
                        __mntput(mnt);
        }
}
# 8 "/opt/Wive/kernel/include/linux/dcache.h" 2
# 24 "/opt/Wive/kernel/include/linux/dcache.h"
struct qstr {
        const unsigned char * name;
        unsigned int len;
        unsigned int hash;
};

struct dentry_stat_t {
        int nr_dentry;
        int nr_unused;
        int age_limit;
        int want_pages;
        int dummy[2];
};
extern struct dentry_stat_t dentry_stat;






static __inline__ unsigned long partial_name_hash(unsigned long c, unsigned long prevhash)
{
        return (prevhash + (c << 4) + (c >> 4)) * 11;
}


static __inline__ unsigned long end_name_hash(unsigned long hash)
{
        return (unsigned int) hash;
}


static __inline__ unsigned int full_name_hash(const unsigned char * name, unsigned int len)
{
        unsigned long hash = 0;
        while (len--)
                hash = partial_name_hash(*name++, hash);
        return end_name_hash(hash);
}



struct dentry {
        atomic_t d_count;
        unsigned int d_flags;
        struct inode * d_inode;
        struct dentry * d_parent;
        struct list_head d_hash;
        struct list_head d_lru;
        struct list_head d_child;
        struct list_head d_subdirs;
        struct list_head d_alias;
        int d_mounted;
        struct qstr d_name;
        unsigned long d_time;
        struct dentry_operations *d_op;
        struct super_block * d_sb;
        unsigned long d_vfs_flags;
        void * d_fsdata;
        unsigned char d_iname[16];
};

struct dentry_operations {
        int (*d_revalidate)(struct dentry *, int);
        int (*d_hash) (struct dentry *, struct qstr *);
        int (*d_compare) (struct dentry *, struct qstr *, struct qstr *);
        int (*d_delete)(struct dentry *);
        void (*d_release)(struct dentry *);
        void (*d_iput)(struct dentry *, struct inode *);
};
# 128 "/opt/Wive/kernel/include/linux/dcache.h"
extern spinlock_t dcache_lock;

static __inline__ int dname_external(struct dentry *d)
{
        return d->d_name.name != d->d_iname;
}




extern void d_instantiate(struct dentry *, struct inode *);
extern void d_delete(struct dentry *);


extern struct dentry * d_alloc(struct dentry *, const struct qstr *);
extern void shrink_dcache_sb(struct super_block *);
extern void shrink_dcache_parent(struct dentry *);
extern int d_invalidate(struct dentry *);


struct zone_struct;

extern int shrink_dcache_memory(int, unsigned int);
extern void prune_dcache(int);


extern int shrink_icache_memory(int, int);
extern void prune_icache(int);


extern int shrink_dqcache_memory(int, unsigned int);


extern struct dentry * d_alloc_root(struct inode *);


extern void d_genocide(struct dentry *);

extern struct dentry *d_find_alias(struct inode *);
extern void d_prune_aliases(struct inode *);


extern int have_submounts(struct dentry *);




extern void d_rehash(struct dentry *);
# 186 "/opt/Wive/kernel/include/linux/dcache.h"
static __inline__ void d_add(struct dentry * entry, struct inode * inode)
{
        d_instantiate(entry, inode);
        d_rehash(entry);
}


extern void d_move(struct dentry *, struct dentry *);


extern struct dentry * d_lookup(struct dentry *, struct qstr *);


extern int d_validate(struct dentry *, struct dentry *);

extern char * __d_path(struct dentry *, struct vfsmount *, struct dentry *,
        struct vfsmount *, char *, int);
# 219 "/opt/Wive/kernel/include/linux/dcache.h"
static __inline__ struct dentry * dget(struct dentry *dentry)
{
        if (dentry) {
                if (!((&dentry->d_count)->counter))
                        __out_of_line_bug(223);
                atomic_add(1,(&dentry->d_count));
        }
        return dentry;
}

extern struct dentry * dget_locked(struct dentry *);
# 238 "/opt/Wive/kernel/include/linux/dcache.h"
static __inline__ int d_unhashed(struct dentry *dentry)
{
        return list_empty(&dentry->d_hash);
}

extern void dput(struct dentry *);

static __inline__ int d_mountpoint(struct dentry *dentry)
{
        return dentry->d_mounted;
}

extern struct vfsmount *lookup_mnt(struct vfsmount *, struct dentry *);
# 20 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/stat.h" 1





# 1 "/opt/Wive/kernel/include/asm/stat.h" 1





struct __old_kernel_stat {
        unsigned int st_dev;
        unsigned int st_ino;
        unsigned int st_mode;
        unsigned int st_nlink;
        unsigned int st_uid;
        unsigned int st_gid;
        unsigned int st_rdev;
        long st_size;
        unsigned int st_atime, st_res1;
        unsigned int st_mtime, st_res2;
        unsigned int st_ctime, st_res3;
        unsigned int st_blksize;
        int st_blocks;
        unsigned int st_unused0[2];
};

struct stat {
        dev_t st_dev;
        long st_pad1[3];
        ino_t st_ino;
        mode_t st_mode;
        nlink_t st_nlink;
        uid_t st_uid;
        gid_t st_gid;
        dev_t st_rdev;
        long st_pad2[2];
        off_t st_size;
        long st_pad3;




        time_t st_atime;
        long reserved0;
        time_t st_mtime;
        long reserved1;
        time_t st_ctime;
        long reserved2;
        long st_blksize;
        long st_blocks;
        long st_pad4[14];
};







struct stat64 {
        unsigned long st_dev;
        unsigned long st_pad0[3];

        unsigned long long st_ino;

        mode_t st_mode;
        nlink_t st_nlink;

        uid_t st_uid;
        gid_t st_gid;

        unsigned long st_rdev;
        unsigned long st_pad1[3];

        long long st_size;





        time_t st_atime;
        unsigned long reserved0;

        time_t st_mtime;
        unsigned long reserved1;

        time_t st_ctime;
        unsigned long reserved2;

        unsigned long st_blksize;
        unsigned long st_pad2;

        long long st_blocks;
};
# 7 "/opt/Wive/kernel/include/linux/stat.h" 2
# 21 "/opt/Wive/kernel/include/linux/fs.h" 2


# 1 "/opt/Wive/kernel/include/linux/string.h" 1
# 15 "/opt/Wive/kernel/include/linux/string.h"
extern char * ___strtok;
extern char * strpbrk(const char *,const char *);
extern char * strtok(char *,const char *);
extern char * strsep(char **,const char *);
extern __kernel_size_t strspn(const char *,const char *);





# 1 "/opt/Wive/kernel/include/asm/string.h" 1
# 15 "/opt/Wive/kernel/include/asm/string.h"
static __inline__ char *strcpy(char *__dest, __const__ char *__src)
{
  char *__xdest = __dest;

  __asm__ __volatile__(
        ".set\tnoreorder\n\t"
        ".set\tnoat\n"
        "1:\tlbu\t$1,(%1)\n\t"
        "addiu\t%1,1\n\t"
        "sb\t$1,(%0)\n\t"
        "bnez\t$1,1b\n\t"
        "addiu\t%0,1\n\t"
        ".set\tat\n\t"
        ".set\treorder"
        : "=r" (__dest), "=r" (__src)
        : "0" (__dest), "1" (__src)
        : "memory");

  return __xdest;
}


static __inline__ char *strncpy(char *__dest, __const__ char *__src, size_t __n)
{
  char *__xdest = __dest;

  if (__n == 0)
    return __xdest;

  __asm__ __volatile__(
        ".set\tnoreorder\n\t"
        ".set\tnoat\n"
        "1:\tlbu\t$1,(%1)\n\t"
        "subu\t%2,1\n\t"
        "sb\t$1,(%0)\n\t"
        "beqz\t$1,2f\n\t"
        "addiu\t%0,1\n\t"
        "bnez\t%2,1b\n\t"
        "addiu\t%1,1\n"
        "2:\n\t"
        ".set\tat\n\t"
        ".set\treorder"
        : "=r" (__dest), "=r" (__src), "=r" (__n)
        : "0" (__dest), "1" (__src), "2" (__n)
        : "memory");

  return __xdest;
}


static __inline__ int strcmp(__const__ char *__cs, __const__ char *__ct)
{
  int __res;

  __asm__ __volatile__(
        ".set\tnoreorder\n\t"
        ".set\tnoat\n\t"
        "lbu\t%2,(%0)\n"
        "1:\tlbu\t$1,(%1)\n\t"
        "addiu\t%0,1\n\t"
        "bne\t$1,%2,2f\n\t"
        "addiu\t%1,1\n\t"
        "bnez\t%2,1b\n\t"
        "lbu\t%2,(%0)\n\t"

        "nop\n\t"

        "move\t%2,$1\n"
        "2:\tsubu\t%2,$1\n"
        "3:\t.set\tat\n\t"
        ".set\treorder"
        : "=r" (__cs), "=r" (__ct), "=r" (__res)
        : "0" (__cs), "1" (__ct));

  return __res;
}


static __inline__ int
strncmp(__const__ char *__cs, __const__ char *__ct, size_t __count)
{
        int __res;

        __asm__ __volatile__(
        ".set\tnoreorder\n\t"
        ".set\tnoat\n"
        "1:\tlbu\t%3,(%0)\n\t"
        "beqz\t%2,2f\n\t"
        "lbu\t$1,(%1)\n\t"
        "subu\t%2,1\n\t"
        "bne\t$1,%3,3f\n\t"
        "addiu\t%0,1\n\t"
        "bnez\t%3,1b\n\t"
        "addiu\t%1,1\n"
        "2:\n\t"

        "nop\n\t"

        "move\t%3,$1\n"
        "3:\tsubu\t%3,$1\n\t"
        ".set\tat\n\t"
        ".set\treorder"
        : "=r" (__cs), "=r" (__ct), "=r" (__count), "=r" (__res)
        : "0" (__cs), "1" (__ct), "2" (__count));

        return __res;
}


extern void *memset(void *__s, int __c, size_t __count);


extern void *memcpy(void *__to, __const__ void *__from, size_t __n);


extern void *memmove(void *__dest, __const__ void *__src, size_t __n);





static __inline__ void *memscan(void *__addr, int __c, size_t __size)
{
        char *__end = (char *)__addr + __size;
        unsigned char __uc = (unsigned char) __c;

        __asm__(".set\tpush\n\t"
                ".set\tnoat\n\t"
                ".set\treorder\n\t"
                "1:\tbeq\t%0,%1,2f\n\t"
                "addiu\t%0,1\n\t"
                "lbu\t$1,-1(%0)\n\t"
                "bne\t$1,%z4,1b\n"
                "2:\t.set\tpop"
                : "=r" (__addr), "=r" (__end)
                : "0" (__addr), "1" (__end), "Jr" (__uc));

        return __addr;
}
# 26 "/opt/Wive/kernel/include/linux/string.h" 2
# 34 "/opt/Wive/kernel/include/linux/string.h"
extern char * strcat(char *, const char *);


extern char * strncat(char *, const char *, __kernel_size_t);
# 46 "/opt/Wive/kernel/include/linux/string.h"
extern int strnicmp(const char *, const char *, __kernel_size_t);


extern char * strchr(const char *,int);


extern char * strrchr(const char *,int);


extern char * strstr(const char *,const char *);


extern __kernel_size_t strlen(const char *);


extern __kernel_size_t strnlen(const char *,__kernel_size_t);
# 77 "/opt/Wive/kernel/include/linux/string.h"
extern int memcmp(const void *,const void *,__kernel_size_t);


extern void * memchr(const void *,int,__kernel_size_t);
# 24 "/opt/Wive/kernel/include/linux/fs.h" 2


# 1 "/opt/Wive/kernel/include/asm/bitops.h" 1
# 323 "/opt/Wive/kernel/include/asm/bitops.h"
extern __inline__ void set_bit(int nr, volatile void * addr)
{
        int mask;
        volatile int *a = addr;
        unsigned long flags;

        a += nr >> 5;
        mask = 1 << (nr & 0x1f);
        __asm__ __volatile__( "__save_and_cli\t%0" : "=r" (flags) : : "memory");
        *a |= mask;
        do { unsigned long __tmp1; __asm__ __volatile__( "__restore_flags\t%0" : "=r" (__tmp1) : "0" (flags) : "memory"); } while(0);
}
# 345 "/opt/Wive/kernel/include/asm/bitops.h"
extern __inline__ void __set_bit(int nr, volatile void * addr)
{
        int mask;
        volatile int *a = addr;

        a += nr >> 5;
        mask = 1 << (nr & 0x1f);
        *a |= mask;
}
# 365 "/opt/Wive/kernel/include/asm/bitops.h"
extern __inline__ void clear_bit(int nr, volatile void * addr)
{
        int mask;
        volatile int *a = addr;
        unsigned long flags;

        a += nr >> 5;
        mask = 1 << (nr & 0x1f);
        __asm__ __volatile__( "__save_and_cli\t%0" : "=r" (flags) : : "memory");
        *a &= ~mask;
        do { unsigned long __tmp1; __asm__ __volatile__( "__restore_flags\t%0" : "=r" (__tmp1) : "0" (flags) : "memory"); } while(0);
}
# 387 "/opt/Wive/kernel/include/asm/bitops.h"
extern __inline__ void change_bit(int nr, volatile void * addr)
{
        int mask;
        volatile int *a = addr;
        unsigned long flags;

        a += nr >> 5;
        mask = 1 << (nr & 0x1f);
        __asm__ __volatile__( "__save_and_cli\t%0" : "=r" (flags) : : "memory");
        *a ^= mask;
        do { unsigned long __tmp1; __asm__ __volatile__( "__restore_flags\t%0" : "=r" (__tmp1) : "0" (flags) : "memory"); } while(0);
}
# 409 "/opt/Wive/kernel/include/asm/bitops.h"
extern __inline__ void __change_bit(int nr, volatile void * addr)
{
        unsigned long * m = ((unsigned long *) addr) + (nr >> 5);

        *m ^= 1UL << (nr & 31);
}
# 424 "/opt/Wive/kernel/include/asm/bitops.h"
extern __inline__ int test_and_set_bit(int nr, volatile void * addr)
{
        int mask, retval;
        volatile int *a = addr;
        unsigned long flags;

        a += nr >> 5;
        mask = 1 << (nr & 0x1f);
        __asm__ __volatile__( "__save_and_cli\t%0" : "=r" (flags) : : "memory");
        retval = (mask & *a) != 0;
        *a |= mask;
        do { unsigned long __tmp1; __asm__ __volatile__( "__restore_flags\t%0" : "=r" (__tmp1) : "0" (flags) : "memory"); } while(0);

        return retval;
}
# 449 "/opt/Wive/kernel/include/asm/bitops.h"
extern __inline__ int __test_and_set_bit(int nr, volatile void * addr)
{
        int mask, retval;
        volatile int *a = addr;

        a += nr >> 5;
        mask = 1 << (nr & 0x1f);
        retval = (mask & *a) != 0;
        *a |= mask;

        return retval;
}
# 470 "/opt/Wive/kernel/include/asm/bitops.h"
extern __inline__ int test_and_clear_bit(int nr, volatile void * addr)
{
        int mask, retval;
        volatile int *a = addr;
        unsigned long flags;

        a += nr >> 5;
        mask = 1 << (nr & 0x1f);
        __asm__ __volatile__( "__save_and_cli\t%0" : "=r" (flags) : : "memory");
        retval = (mask & *a) != 0;
        *a &= ~mask;
        do { unsigned long __tmp1; __asm__ __volatile__( "__restore_flags\t%0" : "=r" (__tmp1) : "0" (flags) : "memory"); } while(0);

        return retval;
}
# 495 "/opt/Wive/kernel/include/asm/bitops.h"
extern __inline__ int __test_and_clear_bit(int nr, volatile void * addr)
{
        int mask, retval;
        volatile int *a = addr;

        a += nr >> 5;
        mask = 1 << (nr & 0x1f);
        retval = (mask & *a) != 0;
        *a &= ~mask;

        return retval;
}
# 516 "/opt/Wive/kernel/include/asm/bitops.h"
extern __inline__ int test_and_change_bit(int nr, volatile void * addr)
{
        int mask, retval;
        volatile int *a = addr;
        unsigned long flags;

        a += nr >> 5;
        mask = 1 << (nr & 0x1f);
        __asm__ __volatile__( "__save_and_cli\t%0" : "=r" (flags) : : "memory");
        retval = (mask & *a) != 0;
        *a ^= mask;
        do { unsigned long __tmp1; __asm__ __volatile__( "__restore_flags\t%0" : "=r" (__tmp1) : "0" (flags) : "memory"); } while(0);

        return retval;
}
# 541 "/opt/Wive/kernel/include/asm/bitops.h"
extern __inline__ int __test_and_change_bit(int nr, volatile void * addr)
{
        int mask, retval;
        volatile int *a = addr;

        a += nr >> 5;
        mask = 1 << (nr & 0x1f);
        retval = (mask & *a) != 0;
        *a ^= mask;

        return retval;
}
# 566 "/opt/Wive/kernel/include/asm/bitops.h"
extern __inline__ int test_bit(int nr, volatile void *addr)
{
        return ((1UL << (nr & 31)) & (((const unsigned int *) addr)[nr >> 5])) != 0;
}
# 678 "/opt/Wive/kernel/include/asm/bitops.h"
static __inline__ unsigned long ffz(unsigned long word)
{
        int b = 0, s;

        word = ~word;
        s = 16; if (word << 16 != 0) s = 0; b += s; word >>= s;
        s = 8; if (word << 24 != 0) s = 0; b += s; word >>= s;
        s = 4; if (word << 28 != 0) s = 0; b += s; word >>= s;
        s = 2; if (word << 30 != 0) s = 0; b += s; word >>= s;
        s = 1; if (word << 31 != 0) s = 0; b += s;

        return b;
}
# 726 "/opt/Wive/kernel/include/asm/bitops.h"
extern __inline__ int find_next_zero_bit(void *addr, int size, int offset)
{
        unsigned long *p = ((unsigned long *) addr) + (offset >> 5);
        unsigned long result = offset & ~31UL;
        unsigned long tmp;

        if (offset >= size)
                return size;
        size -= result;
        offset &= 31UL;
        if (offset) {
                tmp = *(p++);
                tmp |= ~0UL >> (32-offset);
                if (size < 32)
                        goto found_first;
                if (~tmp)
                        goto found_middle;
                size -= 32;
                result += 32;
        }
        while (size & ~31UL) {
                if (~(tmp = *(p++)))
                        goto found_middle;
                result += 32;
                size -= 32;
        }
        if (!size)
                return result;
        tmp = *p;

found_first:
        tmp |= ~0UL << size;
found_middle:
        return result + ffz(tmp);
}
# 786 "/opt/Wive/kernel/include/asm/bitops.h"
extern __inline__ int ext2_set_bit(int nr, void * addr)
{
        int mask, retval, flags;
        unsigned char *ADDR = (unsigned char *) addr;

        ADDR += nr >> 3;
        mask = 1 << (nr & 0x07);
        __asm__ __volatile__( "__save_and_cli\t%0" : "=r" (flags) : : "memory");
        retval = (mask & *ADDR) != 0;
        *ADDR |= mask;
        do { unsigned long __tmp1; __asm__ __volatile__( "__restore_flags\t%0" : "=r" (__tmp1) : "0" (flags) : "memory"); } while(0);
        return retval;
}

extern __inline__ int ext2_clear_bit(int nr, void * addr)
{
        int mask, retval, flags;
        unsigned char *ADDR = (unsigned char *) addr;

        ADDR += nr >> 3;
        mask = 1 << (nr & 0x07);
        __asm__ __volatile__( "__save_and_cli\t%0" : "=r" (flags) : : "memory");
        retval = (mask & *ADDR) != 0;
        *ADDR &= ~mask;
        do { unsigned long __tmp1; __asm__ __volatile__( "__restore_flags\t%0" : "=r" (__tmp1) : "0" (flags) : "memory"); } while(0);
        return retval;
}

extern __inline__ int ext2_test_bit(int nr, const void * addr)
{
        int mask;
        const unsigned char *ADDR = (const unsigned char *) addr;

        ADDR += nr >> 3;
        mask = 1 << (nr & 0x07);
        return ((mask & *ADDR) != 0);
}




extern __inline__ unsigned long ext2_find_next_zero_bit(void *addr, unsigned long size, unsigned long offset)
{
        unsigned long *p = ((unsigned long *) addr) + (offset >> 5);
        unsigned long result = offset & ~31UL;
        unsigned long tmp;

        if (offset >= size)
                return size;
        size -= result;
        offset &= 31UL;
        if(offset) {
# 848 "/opt/Wive/kernel/include/asm/bitops.h"
                tmp = *(p++);
                tmp |= (__builtin_constant_p((__u32)(~0UL >> (32-offset))) ? ({ __u32 __x = ((~0UL >> (32-offset))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32((~0UL >> (32-offset))));
                if(size < 32)
                        goto found_first;
                if(~tmp)
                        goto found_middle;
                size -= 32;
                result += 32;
        }
        while(size & ~31UL) {
                if(~(tmp = *(p++)))
                        goto found_middle;
                result += 32;
                size -= 32;
        }
        if(!size)
                return result;
        tmp = *p;

found_first:




        return result + ffz((__builtin_constant_p((__u32)(tmp)) ? ({ __u32 __x = ((tmp)); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32((tmp))) | (~0UL << size));
found_middle:
        return result + ffz((__builtin_constant_p((__u32)(tmp)) ? ({ __u32 __x = ((tmp)); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32((tmp))));
}
# 27 "/opt/Wive/kernel/include/linux/fs.h" 2

struct poll_table_struct;
# 50 "/opt/Wive/kernel/include/linux/fs.h"
struct files_stat_struct {
        int nr_files;
        int nr_free_files;
        int max_files;
};
extern struct files_stat_struct files_stat;

struct inodes_stat_t {
        int nr_inodes;
        int nr_unused;
        int dummy[5];
};
extern struct inodes_stat_t inodes_stat;

extern int leases_enable, dir_notify_enable, lease_break_time;
# 200 "/opt/Wive/kernel/include/linux/fs.h"
# 1 "/opt/Wive/kernel/include/asm/semaphore.h" 1
# 21 "/opt/Wive/kernel/include/asm/semaphore.h"
# 1 "/opt/Wive/kernel/include/linux/rwsem.h" 1
# 22 "/opt/Wive/kernel/include/linux/rwsem.h"
struct rw_semaphore;


# 1 "/opt/Wive/kernel/include/linux/rwsem-spinlock.h" 1
# 22 "/opt/Wive/kernel/include/linux/rwsem-spinlock.h"
struct rwsem_waiter;
# 31 "/opt/Wive/kernel/include/linux/rwsem-spinlock.h"
struct rw_semaphore {
        __s32 activity;
        spinlock_t wait_lock;
        struct list_head wait_list;



};
# 55 "/opt/Wive/kernel/include/linux/rwsem-spinlock.h"
extern void init_rwsem(struct rw_semaphore *sem);
extern void __down_read(struct rw_semaphore *sem);
extern void __down_write(struct rw_semaphore *sem);
extern void __up_read(struct rw_semaphore *sem);
extern void __up_write(struct rw_semaphore *sem);
# 26 "/opt/Wive/kernel/include/linux/rwsem.h" 2
# 41 "/opt/Wive/kernel/include/linux/rwsem.h"
static inline void down_read(struct rw_semaphore *sem)
{
        ;
        __down_read(sem);
        ;
}




static inline void down_write(struct rw_semaphore *sem)
{
        ;
        __down_write(sem);
        ;
}




static inline void up_read(struct rw_semaphore *sem)
{
        ;
        __up_read(sem);
        ;
}




static inline void up_write(struct rw_semaphore *sem)
{
        ;
        __up_write(sem);
        ;
}
# 22 "/opt/Wive/kernel/include/asm/semaphore.h" 2

struct semaphore {

        atomic_t count;
        atomic_t waking;




        wait_queue_head_t wait;



} __attribute__((aligned(8)));
# 63 "/opt/Wive/kernel/include/asm/semaphore.h"
static inline void sema_init (struct semaphore *sem, int val)
{
        ((&sem->count)->counter = (val));
        ((&sem->waking)->counter = (0));
        init_waitqueue_head(&sem->wait);



}

static inline void init_MUTEX (struct semaphore *sem)
{
        sema_init(sem, 1);
}

static inline void init_MUTEX_LOCKED (struct semaphore *sem)
{
        sema_init(sem, 0);
}

 void __down(struct semaphore * sem);
 int __down_interruptible(struct semaphore * sem);
 int __down_trylock(struct semaphore * sem);
 void __up(struct semaphore * sem);

static inline void down(struct semaphore * sem)
{



        if (atomic_sub_return(1,(&sem->count)) < 0)
                __down(sem);
}





static inline int down_interruptible(struct semaphore * sem)
{
        int ret = 0;




        if (atomic_sub_return(1,(&sem->count)) < 0)
                ret = __down_interruptible(sem);
        return ret;
}







static inline int down_trylock(struct semaphore * sem)
{
        int ret = 0;
        if (atomic_sub_return(1,(&sem->count)) < 0)
                ret = __down_trylock(sem);
        return ret;
}
# 188 "/opt/Wive/kernel/include/asm/semaphore.h"
static inline void up(struct semaphore * sem)
{



        if (atomic_add_return(1,(&sem->count)) <= 0)
                __up(sem);
}
# 201 "/opt/Wive/kernel/include/linux/fs.h" 2


extern void update_atime (struct inode *);


extern void buffer_init(unsigned long);
extern void inode_init(unsigned long);
extern void mnt_init(unsigned long);


enum bh_state_bits {
        BH_Uptodate,
        BH_Dirty,
        BH_Lock,
        BH_Req,
        BH_Mapped,
        BH_New,
        BH_Async,
        BH_Wait_IO,
        BH_launder,
        BH_JBD,

        BH_PrivateStart,


};
# 239 "/opt/Wive/kernel/include/linux/fs.h"
struct buffer_head {

        struct buffer_head *b_next;
        unsigned long b_blocknr;
        unsigned short b_size;
        unsigned short b_list;
        kdev_t b_dev;

        atomic_t b_count;
        kdev_t b_rdev;
        unsigned long b_state;
        unsigned long b_flushtime;

        struct buffer_head *b_next_free;
        struct buffer_head *b_prev_free;
        struct buffer_head *b_this_page;
        struct buffer_head *b_reqnext;

        struct buffer_head **b_pprev;
        char * b_data;
        struct page *b_page;
        void (*b_end_io)(struct buffer_head *bh, int uptodate);
        void *b_private;

        unsigned long b_rsector;
        wait_queue_head_t b_wait;

        struct inode * b_inode;
        struct list_head b_inode_buffers;
};

typedef void (bh_end_io_t)(struct buffer_head *bh, int uptodate);
void init_buffer(struct buffer_head *, bh_end_io_t *, void *);
# 285 "/opt/Wive/kernel/include/linux/fs.h"
extern void set_bh_page(struct buffer_head *bh, struct page *page, unsigned long offset);




# 1 "/opt/Wive/kernel/include/linux/pipe_fs_i.h" 1




struct pipe_inode_info {
        wait_queue_head_t wait;
        char *base;
        unsigned int len;
        unsigned int start;
        unsigned int readers;
        unsigned int writers;
        unsigned int waiting_readers;
        unsigned int waiting_writers;
        unsigned int r_counter;
        unsigned int w_counter;
};
# 42 "/opt/Wive/kernel/include/linux/pipe_fs_i.h"
void pipe_wait(struct inode * inode);

struct inode* pipe_new(struct inode* inode);
# 291 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/minix_fs_i.h" 1






struct minix_inode_info {
        union {
                __u16 i1_data[16];
                __u32 i2_data[16];
        } u;
};
# 292 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/ext2_fs_i.h" 1
# 22 "/opt/Wive/kernel/include/linux/ext2_fs_i.h"
struct ext2_inode_info {
        __u32 i_data[15];
        __u32 i_flags;
        __u32 i_faddr;
        __u8 i_frag_no;
        __u8 i_frag_size;
        __u16 i_osync;
        __u32 i_file_acl;
        __u32 i_dir_acl;
        __u32 i_dtime;
        __u32 i_block_group;
        __u32 i_next_alloc_block;
        __u32 i_next_alloc_goal;
        __u32 i_prealloc_block;
        __u32 i_prealloc_count;
        __u32 i_dir_start_lookup;
        int i_new_inode:1;
};
# 293 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/ext3_fs_i.h" 1
# 24 "/opt/Wive/kernel/include/linux/ext3_fs_i.h"
struct ext3_inode_info {
        __u32 i_data[15];
        __u32 i_flags;






        __u32 i_file_acl;
        __u32 i_dir_acl;
        __u32 i_dtime;
        __u32 i_block_group;
        __u32 i_state;
        __u32 i_next_alloc_block;
        __u32 i_next_alloc_goal;




        __u32 i_dir_start_lookup;

        struct list_head i_orphan;
# 63 "/opt/Wive/kernel/include/linux/ext3_fs_i.h"
        loff_t i_disksize;
# 75 "/opt/Wive/kernel/include/linux/ext3_fs_i.h"
        struct rw_semaphore truncate_sem;
};
# 294 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/hpfs_fs_i.h" 1



struct hpfs_inode_info {
        unsigned long mmu_private;
        ino_t i_parent_dir;
        unsigned i_dno;
        unsigned i_dpos;
        unsigned i_dsubdno;
        unsigned i_file_sec;
        unsigned i_disk_sec;
        unsigned i_n_secs;
        unsigned i_ea_size;
        unsigned i_conv : 2;
        unsigned i_ea_mode : 1;
        unsigned i_ea_uid : 1;
        unsigned i_ea_gid : 1;
        unsigned i_dirty : 1;
        struct semaphore i_sem;
        loff_t **i_rddir_off;
};
# 295 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/ntfs_fs_i.h" 1






struct ntfs_attribute;
struct ntfs_sb_info;




typedef u8 ntfs_u8;
typedef u16 ntfs_u16;
typedef u32 ntfs_u32;
typedef u64 ntfs_u64;
typedef s8 ntfs_s8;
typedef s16 ntfs_s16;
typedef s32 ntfs_s32;
typedef s64 ntfs_s64;




typedef __kernel_mode_t ntmode_t;



typedef uid_t ntfs_uid_t;



typedef gid_t ntfs_gid_t;



typedef __kernel_size_t ntfs_size_t;



typedef __kernel_time_t ntfs_time_t;





typedef u16 ntfs_wchar_t;




typedef s64 ntfs_offset_t;




typedef u64 ntfs_time64_t;
# 69 "/opt/Wive/kernel/include/linux/ntfs_fs_i.h"
typedef s32 ntfs_cluster_t;



struct ntfs_inode_info {
        struct ntfs_sb_info *vol;
        unsigned long i_number;
        __u16 sequence_number;
        unsigned char *attr;
        int attr_count;
        struct ntfs_attribute *attrs;
        int record_count;
        int *records;

        union {
                struct {
                        int recordsize;
                        int clusters_per_record;
                } index;
        } u;
};
# 296 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/msdos_fs_i.h" 1







struct msdos_inode_info {
        unsigned long mmu_private;
        int i_start;
        int i_logstart;
        int i_attrs;
        int i_ctime_ms;
        int i_location;
        struct inode *i_fat_inode;
        struct list_head i_fat_hash;
};
# 297 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/umsdos_fs_i.h" 1
# 39 "/opt/Wive/kernel/include/linux/umsdos_fs_i.h"
struct dir_locking_info {
        wait_queue_head_t p;
        short int looking;
        short int creating;



        long pid;

};

struct umsdos_inode_info {
        struct msdos_inode_info msdos_info;
        struct dir_locking_info dir_info;
        int i_patched;
        int i_is_hlink;
        off_t pos;
};
# 298 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/iso_fs_i.h" 1



enum isofs_file_format {
        isofs_file_normal = 0,
        isofs_file_sparse = 1,
        isofs_file_compressed = 2,
};




struct iso_inode_info {
        unsigned int i_first_extent;
        unsigned char i_file_format;
        unsigned char i_format_parm[3];
        unsigned long i_next_section_ino;
        off_t i_section_size;
};
# 299 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/nfs_fs_i.h" 1





# 1 "/opt/Wive/kernel/include/linux/nfs.h" 1
# 10 "/opt/Wive/kernel/include/linux/nfs.h"
# 1 "/opt/Wive/kernel/include/linux/sunrpc/msg_prot.h" 1
# 14 "/opt/Wive/kernel/include/linux/sunrpc/msg_prot.h"
enum rpc_auth_flavor {
        RPC_AUTH_NULL = 0,
        RPC_AUTH_UNIX = 1,
        RPC_AUTH_SHORT = 2,
        RPC_AUTH_DES = 3,
        RPC_AUTH_KRB = 4,
};

enum rpc_msg_type {
        RPC_CALL = 0,
        RPC_REPLY = 1
};

enum rpc_reply_stat {
        RPC_MSG_ACCEPTED = 0,
        RPC_MSG_DENIED = 1
};

enum rpc_accept_stat {
        RPC_SUCCESS = 0,
        RPC_PROG_UNAVAIL = 1,
        RPC_PROG_MISMATCH = 2,
        RPC_PROC_UNAVAIL = 3,
        RPC_GARBAGE_ARGS = 4,
        RPC_SYSTEM_ERR = 5
};

enum rpc_reject_stat {
        RPC_MISMATCH = 0,
        RPC_AUTH_ERROR = 1
};

enum rpc_auth_stat {
        RPC_AUTH_OK = 0,
        RPC_AUTH_BADCRED = 1,
        RPC_AUTH_REJECTEDCRED = 2,
        RPC_AUTH_BADVERF = 3,
        RPC_AUTH_REJECTEDVERF = 4,
        RPC_AUTH_TOOWEAK = 5
};
# 11 "/opt/Wive/kernel/include/linux/nfs.h" 2
# 41 "/opt/Wive/kernel/include/linux/nfs.h"
 enum nfs_stat {
        NFS_OK = 0,
        NFSERR_PERM = 1,
        NFSERR_NOENT = 2,
        NFSERR_IO = 5,
        NFSERR_NXIO = 6,
        NFSERR_EAGAIN = 11,
        NFSERR_ACCES = 13,
        NFSERR_EXIST = 17,
        NFSERR_XDEV = 18,
        NFSERR_NODEV = 19,
        NFSERR_NOTDIR = 20,
        NFSERR_ISDIR = 21,
        NFSERR_INVAL = 22,
        NFSERR_FBIG = 27,
        NFSERR_NOSPC = 28,
        NFSERR_ROFS = 30,
        NFSERR_MLINK = 31,
        NFSERR_OPNOTSUPP = 45,
        NFSERR_NAMETOOLONG = 63,
        NFSERR_NOTEMPTY = 66,
        NFSERR_DQUOT = 69,
        NFSERR_STALE = 70,
        NFSERR_REMOTE = 71,
        NFSERR_WFLUSH = 99,
        NFSERR_BADHANDLE = 10001,
        NFSERR_NOT_SYNC = 10002,
        NFSERR_BAD_COOKIE = 10003,
        NFSERR_NOTSUPP = 10004,
        NFSERR_TOOSMALL = 10005,
        NFSERR_SERVERFAULT = 10006,
        NFSERR_BADTYPE = 10007,
        NFSERR_JUKEBOX = 10008
 };



enum nfs_ftype {
        NFNON = 0,
        NFREG = 1,
        NFDIR = 2,
        NFBLK = 3,
        NFCHR = 4,
        NFLNK = 5,
        NFSOCK = 6,
        NFBAD = 7,
        NFFIFO = 8
};






struct nfs_fh {
        unsigned short size;
        unsigned char data[64];
};
# 107 "/opt/Wive/kernel/include/linux/nfs.h"
enum nfs3_stable_how {
        NFS_UNSTABLE = 0,
        NFS_DATA_SYNC = 1,
        NFS_FILE_SYNC = 2
};
# 7 "/opt/Wive/kernel/include/linux/nfs_fs_i.h" 2




struct nfs_inode_info {



        __u64 fileid;




        struct nfs_fh fh;




        unsigned short flags;
# 44 "/opt/Wive/kernel/include/linux/nfs_fs_i.h"
        unsigned long read_cache_jiffies;
        __u64 read_cache_ctime;
        __u64 read_cache_mtime;
        __u64 read_cache_isize;
        unsigned long attrtimeo;
        unsigned long attrtimeo_timestamp;





        unsigned long cache_mtime_jiffies;





        __u32 cookieverf[2];




        struct list_head read;
        struct list_head dirty;
        struct list_head commit;
        struct list_head writeback;

        unsigned int nread,
                                ndirty,
                                ncommit,
                                npages;


        struct rpc_cred *mm_cred;
};
# 92 "/opt/Wive/kernel/include/linux/nfs_fs_i.h"
struct nfs_lock_info {
        u32 state;
        u32 flags;
        struct nlm_host *host;
};
# 300 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/sysv_fs_i.h" 1






struct sysv_inode_info {
        u32 i_data[10+1+1+1];




        u32 i_dir_start_lookup;
};
# 301 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/affs_fs_i.h" 1



# 1 "/opt/Wive/kernel/include/linux/a.out.h" 1







# 1 "/opt/Wive/kernel/include/asm/a.out.h" 1



struct exec
{
  unsigned long a_info;
  unsigned a_text;
  unsigned a_data;
  unsigned a_bss;
  unsigned a_syms;
  unsigned a_entry;
  unsigned a_trsize;
  unsigned a_drsize;
};
# 9 "/opt/Wive/kernel/include/linux/a.out.h" 2




enum machine_type {



  M_OLDSUN2 = 0,




  M_68010 = 1,




  M_68020 = 2,




  M_SPARC = 3,


  M_386 = 100,
  M_MIPS1 = 151,
  M_MIPS2 = 152
};
# 157 "/opt/Wive/kernel/include/linux/a.out.h"
struct nlist {
  union {
    char *n_name;
    struct nlist *n_next;
    long n_strx;
  } n_un;
  unsigned char n_type;
  char n_other;
  short n_desc;
  unsigned long n_value;
};
# 235 "/opt/Wive/kernel/include/linux/a.out.h"
struct relocation_info
{

  int r_address;

  unsigned int r_symbolnum:24;



  unsigned int r_pcrel:1;


  unsigned int r_length:2;






  unsigned int r_extern:1;







  unsigned int r_pad:4;

};
# 5 "/opt/Wive/kernel/include/linux/affs_fs_i.h" 2


# 1 "/opt/Wive/kernel/include/linux/time.h" 1



# 1 "/opt/Wive/kernel/include/asm/param.h" 1
# 5 "/opt/Wive/kernel/include/linux/time.h" 2





struct timespec {
        time_t tv_sec;
        long tv_nsec;
};
# 32 "/opt/Wive/kernel/include/linux/time.h"
static __inline__ unsigned long
timespec_to_jiffies(struct timespec *value)
{
        unsigned long sec = value->tv_sec;
        long nsec = value->tv_nsec;

        if (sec >= (((~0UL >> 1)-1) / (100)))
                return ((~0UL >> 1)-1);
        nsec += 1000000000L / (100) - 1;
        nsec /= 1000000000L / (100);
        return (100) * sec + nsec;
}

static __inline__ void
jiffies_to_timespec(unsigned long jiffies, struct timespec *value)
{
        value->tv_nsec = (jiffies % (100)) * (1000000000L / (100));
        value->tv_sec = jiffies / (100);
}

static __inline__ int
timespec_before(struct timespec a, struct timespec b)
{
        if (a.tv_sec == b.tv_sec)
                return a.tv_nsec < b.tv_nsec;
        return a.tv_sec < b.tv_sec;
}


static inline void
timespec_less(struct timespec a, struct timespec b, struct timespec * result)
{
        if (a.tv_nsec < b.tv_nsec)
        {
                a.tv_sec--;
                a.tv_nsec += 1000000000;
        }

        result->tv_sec = a.tv_sec - b.tv_sec;
        result->tv_nsec = a.tv_nsec - b.tv_nsec;
}
# 89 "/opt/Wive/kernel/include/linux/time.h"
static inline unsigned long
mktime (unsigned int year, unsigned int mon,
        unsigned int day, unsigned int hour,
        unsigned int min, unsigned int sec)
{
        if (0 >= (int) (mon -= 2)) {
                mon += 12;
                year -= 1;
        }

        return (((
                (unsigned long) (year/4 - year/100 + year/400 + 367*mon/12 + day) +
                        year*365 - 719499
            )*24 + hour
          )*60 + min
        )*60 + sec;
}




struct timeval {
        time_t tv_sec;
        suseconds_t tv_usec;
};


static inline void
timeval_less(struct timeval a, struct timeval b, struct timeval * result)
{
        if (a.tv_usec < b.tv_usec)
        {
                a.tv_sec--;
                a.tv_usec += 1000000;
        }

        result->tv_sec = a.tv_sec - b.tv_sec;
        result->tv_usec = a.tv_usec - b.tv_usec;
}

static __inline__ void
timeval_to_timespec(struct timeval tv, struct timespec * ts)
{
        ts->tv_sec = tv.tv_sec;
        ts->tv_nsec = (long) tv.tv_usec * 1000;
}

struct timezone {
        int tz_minuteswest;
        int tz_dsttime;
};




extern void do_gettimeofday(struct timeval *tv);
extern void do_settimeofday(struct timeval *tv);
# 162 "/opt/Wive/kernel/include/linux/time.h"
struct itimerspec {
        struct timespec it_interval;
        struct timespec it_value;
};

struct itimerval {
        struct timeval it_interval;
        struct timeval it_value;
};
# 8 "/opt/Wive/kernel/include/linux/affs_fs_i.h" 2
# 17 "/opt/Wive/kernel/include/linux/affs_fs_i.h"
struct affs_ext_key {
        u32 ext;
        u32 key;
};




struct affs_inode_info {
        u32 i_opencnt;
        struct semaphore i_link_lock;
        struct semaphore i_ext_lock;

        u32 i_blkcnt;
        u32 i_extcnt;
        u32 *i_lc;
        u32 i_lc_size;
        u32 i_lc_shift;
        u32 i_lc_mask;
        struct affs_ext_key *i_ac;
        u32 i_ext_last;
        struct buffer_head *i_ext_bh;
        unsigned long mmu_private;
        u32 i_protect;
        u32 i_lastalloc;
        int i_pa_cnt;
# 51 "/opt/Wive/kernel/include/linux/affs_fs_i.h"
};
# 302 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/ufs_fs_i.h" 1
# 16 "/opt/Wive/kernel/include/linux/ufs_fs_i.h"
struct ufs_inode_info {
        union {
                __u32 i_data[15];
                __u8 i_symlink[4*15];
        } i_u1;
        __u32 i_flags;
        __u32 i_gen;
        __u32 i_shadow;
        __u32 i_unused1;
        __u32 i_unused2;
        __u32 i_oeftflag;
        __u16 i_osync;
        __u32 i_lastfrag;
};
# 303 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/efs_fs_i.h" 1
# 12 "/opt/Wive/kernel/include/linux/efs_fs_i.h"
typedef int32_t efs_block_t;
typedef uint32_t efs_ino_t;






typedef union extent_u {
        unsigned char raw[8];
        struct extent_s {
                unsigned int ex_magic:8;
                unsigned int ex_bn:24;
                unsigned int ex_length:8;
                unsigned int ex_offset:24;
        } cooked;
} efs_extent;

typedef struct edevs {
        short odev;
        unsigned int ndev;
} efs_devs;





struct efs_dinode {
        u_short di_mode;
        short di_nlink;
        u_short di_uid;
        u_short di_gid;
        int32_t di_size;
        int32_t di_atime;
        int32_t di_mtime;
        int32_t di_ctime;
        uint32_t di_gen;
        short di_numextents;
        u_char di_version;
        u_char di_spare;
        union di_addr {
                efs_extent di_extents[12];
                efs_devs di_dev;
        } di_u;
};


struct efs_inode_info {
        int numextents;
        int lastextent;

        efs_extent extents[12];
};
# 304 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/coda_fs_i.h" 1
# 14 "/opt/Wive/kernel/include/linux/coda_fs_i.h"
# 1 "/opt/Wive/kernel/include/linux/coda.h" 1
# 109 "/opt/Wive/kernel/include/linux/coda.h"
typedef unsigned long long u_quad_t;
# 164 "/opt/Wive/kernel/include/linux/coda.h"
struct venus_dirent {
        unsigned long d_fileno;
        unsigned short d_reclen;
        unsigned char d_type;
        unsigned char d_namlen;
        char d_name[255 + 1];
};
# 198 "/opt/Wive/kernel/include/linux/coda.h"
typedef u_long VolumeId;
typedef u_long VnodeId;
typedef u_long Unique_t;
typedef u_long FileVersion;




typedef struct ViceFid {
    VolumeId Volume;
    VnodeId Vnode;
    Unique_t Unique;
} ViceFid;




static __inline__ ino_t coda_f2i(struct ViceFid *fid)
{
        if ( ! fid )
                return 0;
        if (fid->Vnode == 0xfffffffe || fid->Vnode == 0xffffffff)
                return ((fid->Volume << 20) | (fid->Unique & 0xfffff));
        else
                return (fid->Unique + (fid->Vnode<<10) + (fid->Volume<<20));
}
# 233 "/opt/Wive/kernel/include/linux/coda.h"
typedef u_int32_t vuid_t;
typedef u_int32_t vgid_t;




struct coda_cred {
    vuid_t cr_uid, cr_euid, cr_suid, cr_fsuid;
    vgid_t cr_groupid, cr_egid, cr_sgid, cr_fsgid;
};







enum coda_vtype { C_VNON, C_VREG, C_VDIR, C_VBLK, C_VCHR, C_VLNK, C_VSOCK, C_VFIFO, C_VBAD };

struct coda_vattr {
        long va_type;
        u_short va_mode;
        short va_nlink;
        vuid_t va_uid;
        vgid_t va_gid;
        long va_fileid;
        u_quad_t va_size;
        long va_blocksize;
        struct timespec va_atime;
        struct timespec va_mtime;
        struct timespec va_ctime;
        u_long va_gen;
        u_long va_flags;
        u_quad_t va_rdev;
        u_quad_t va_bytes;
        u_quad_t va_filerev;
};




struct coda_statfs {
    int32_t f_blocks;
    int32_t f_bfree;
    int32_t f_bavail;
    int32_t f_files;
    int32_t f_ffree;
};
# 337 "/opt/Wive/kernel/include/linux/coda.h"
struct coda_in_hdr {
    unsigned long opcode;
    unsigned long unique;
    u_short pid;
    u_short pgid;
    u_short sid;
    struct coda_cred cred;
};


struct coda_out_hdr {
    unsigned long opcode;
    unsigned long unique;
    unsigned long result;
};


struct coda_root_out {
    struct coda_out_hdr oh;
    ViceFid VFid;
};

struct coda_root_in {
    struct coda_in_hdr in;
};


struct coda_open_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
    int flags;
};

struct coda_open_out {
    struct coda_out_hdr oh;
    u_quad_t dev;
    ino_t inode;
};



struct coda_store_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
    int flags;
};

struct coda_store_out {
    struct coda_out_hdr out;
};


struct coda_release_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
    int flags;
};

struct coda_release_out {
    struct coda_out_hdr out;
};


struct coda_close_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
    int flags;
};

struct coda_close_out {
    struct coda_out_hdr out;
};


struct coda_ioctl_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
    int cmd;
    int len;
    int rwflag;
    char *data;
};

struct coda_ioctl_out {
    struct coda_out_hdr oh;
    int len;
    caddr_t data;
};



struct coda_getattr_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
};

struct coda_getattr_out {
    struct coda_out_hdr oh;
    struct coda_vattr attr;
};



struct coda_setattr_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
    struct coda_vattr attr;
};

struct coda_setattr_out {
    struct coda_out_hdr out;
};


struct coda_access_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
    int flags;
};

struct coda_access_out {
    struct coda_out_hdr out;
};







struct coda_lookup_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
    int name;
    int flags;
};

struct coda_lookup_out {
    struct coda_out_hdr oh;
    ViceFid VFid;
    int vtype;
};



struct coda_create_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
    struct coda_vattr attr;
    int excl;
    int mode;
    int name;
};

struct coda_create_out {
    struct coda_out_hdr oh;
    ViceFid VFid;
    struct coda_vattr attr;
};



struct coda_remove_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
    int name;
};

struct coda_remove_out {
    struct coda_out_hdr out;
};


struct coda_link_in {
    struct coda_in_hdr ih;
    ViceFid sourceFid;
    ViceFid destFid;
    int tname;
};

struct coda_link_out {
    struct coda_out_hdr out;
};



struct coda_rename_in {
    struct coda_in_hdr ih;
    ViceFid sourceFid;
    int srcname;
    ViceFid destFid;
    int destname;
};

struct coda_rename_out {
    struct coda_out_hdr out;
};


struct coda_mkdir_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
    struct coda_vattr attr;
    int name;
};

struct coda_mkdir_out {
    struct coda_out_hdr oh;
    ViceFid VFid;
    struct coda_vattr attr;
};



struct coda_rmdir_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
    int name;
};

struct coda_rmdir_out {
    struct coda_out_hdr out;
};


struct coda_symlink_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
    int srcname;
    struct coda_vattr attr;
    int tname;
};

struct coda_symlink_out {
    struct coda_out_hdr out;
};


struct coda_readlink_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
};

struct coda_readlink_out {
    struct coda_out_hdr oh;
    int count;
    caddr_t data;
};



struct coda_fsync_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
};

struct coda_fsync_out {
    struct coda_out_hdr out;
};


struct coda_vget_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
};

struct coda_vget_out {
    struct coda_out_hdr oh;
    ViceFid VFid;
    int vtype;
};
# 616 "/opt/Wive/kernel/include/linux/coda.h"
struct coda_purgeuser_out {
    struct coda_out_hdr oh;
    struct coda_cred cred;
};



struct coda_zapfile_out {
    struct coda_out_hdr oh;
    ViceFid CodaFid;
};



struct coda_zapdir_out {
    struct coda_out_hdr oh;
    ViceFid CodaFid;
};



struct coda_zapvnode_out {
    struct coda_out_hdr oh;
    struct coda_cred cred;
    ViceFid VFid;
};



struct coda_purgefid_out {
    struct coda_out_hdr oh;
    ViceFid CodaFid;
};



struct coda_replace_out {
    struct coda_out_hdr oh;
    ViceFid NewFid;
    ViceFid OldFid;
};


struct coda_open_by_fd_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
    int flags;
};

struct coda_open_by_fd_out {
    struct coda_out_hdr oh;
    int fd;


    struct file *fh;

};


struct coda_open_by_path_in {
    struct coda_in_hdr ih;
    ViceFid VFid;
    int flags;
};

struct coda_open_by_path_out {
    struct coda_out_hdr oh;
        int path;
};


struct coda_statfs_in {
    struct coda_in_hdr in;
};

struct coda_statfs_out {
    struct coda_out_hdr oh;
    struct coda_statfs stat;
};
# 703 "/opt/Wive/kernel/include/linux/coda.h"
union inputArgs {
    struct coda_in_hdr ih;
    struct coda_open_in coda_open;
    struct coda_store_in coda_store;
    struct coda_release_in coda_release;
    struct coda_close_in coda_close;
    struct coda_ioctl_in coda_ioctl;
    struct coda_getattr_in coda_getattr;
    struct coda_setattr_in coda_setattr;
    struct coda_access_in coda_access;
    struct coda_lookup_in coda_lookup;
    struct coda_create_in coda_create;
    struct coda_remove_in coda_remove;
    struct coda_link_in coda_link;
    struct coda_rename_in coda_rename;
    struct coda_mkdir_in coda_mkdir;
    struct coda_rmdir_in coda_rmdir;
    struct coda_symlink_in coda_symlink;
    struct coda_readlink_in coda_readlink;
    struct coda_fsync_in coda_fsync;
    struct coda_vget_in coda_vget;
    struct coda_open_by_fd_in coda_open_by_fd;
    struct coda_open_by_path_in coda_open_by_path;
    struct coda_statfs_in coda_statfs;
};

union outputArgs {
    struct coda_out_hdr oh;
    struct coda_root_out coda_root;
    struct coda_open_out coda_open;
    struct coda_ioctl_out coda_ioctl;
    struct coda_getattr_out coda_getattr;
    struct coda_lookup_out coda_lookup;
    struct coda_create_out coda_create;
    struct coda_mkdir_out coda_mkdir;
    struct coda_readlink_out coda_readlink;
    struct coda_vget_out coda_vget;
    struct coda_purgeuser_out coda_purgeuser;
    struct coda_zapfile_out coda_zapfile;
    struct coda_zapdir_out coda_zapdir;
    struct coda_zapvnode_out coda_zapvnode;
    struct coda_purgefid_out coda_purgefid;
    struct coda_replace_out coda_replace;
    struct coda_open_by_fd_out coda_open_by_fd;
    struct coda_open_by_path_out coda_open_by_path;
    struct coda_statfs_out coda_statfs;
};

union coda_downcalls {


    struct coda_purgeuser_out purgeuser;
    struct coda_zapfile_out zapfile;
    struct coda_zapdir_out zapdir;
    struct coda_zapvnode_out zapvnode;
    struct coda_purgefid_out purgefid;
    struct coda_replace_out replace;
};







struct ViceIoctl {
        caddr_t in, out;
        short in_size;
        short out_size;
};

struct PioctlData {
        const char *path;
        int follow;
        struct ViceIoctl vi;
};
# 797 "/opt/Wive/kernel/include/linux/coda.h"
struct coda_mount_data {
        int version;
        int fd;
};
# 15 "/opt/Wive/kernel/include/linux/coda_fs_i.h" 2




struct coda_inode_info {
        struct ViceFid c_fid;
        u_short c_flags;
        struct list_head c_cilist;
        struct file *c_container;
        unsigned int c_contcount;
        struct coda_cred c_cached_cred;
        unsigned int c_cached_perm;
};







int coda_cnode_make(struct inode **, struct ViceFid *, struct super_block *);
struct inode *coda_iget(struct super_block *sb, struct ViceFid *fid, struct coda_vattr *attr);
int coda_cnode_makectl(struct inode **inode, struct super_block *sb);
struct inode *coda_fid_to_inode(ViceFid *fid, struct super_block *sb);
void coda_replace_fid(struct inode *, ViceFid *, ViceFid *);
# 305 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/romfs_fs_i.h" 1





struct romfs_inode_info {
        unsigned long i_metasize;
        unsigned long i_dataoffset;
};
# 306 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/shmem_fs.h" 1
# 16 "/opt/Wive/kernel/include/linux/shmem_fs.h"
typedef struct {
        unsigned long val;
} swp_entry_t;

extern atomic_t shmem_nrpages;

struct shmem_inode_info {
        spinlock_t lock;
        struct semaphore sem;
        unsigned long next_index;
        swp_entry_t i_direct[16];
        void **i_indirect;
        unsigned long swapped;
        int locked;
        struct list_head list;
        struct inode *inode;
};

struct shmem_sb_info {
        unsigned long max_blocks;
        unsigned long free_blocks;
        unsigned long max_inodes;
        unsigned long free_inodes;
        spinlock_t stat_lock;
};
# 307 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/smb_fs_i.h" 1
# 18 "/opt/Wive/kernel/include/linux/smb_fs_i.h"
struct smb_inode_info {





        unsigned int open;
        __u16 fileid;
        __u16 attr;

        __u16 access;
        __u16 flags;
        unsigned long oldmtime;
        unsigned long closed;
        unsigned openers;
};
# 308 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/hfs_fs_i.h" 1
# 19 "/opt/Wive/kernel/include/linux/hfs_fs_i.h"
struct hfs_inode_info {
        int magic;

        unsigned long mmu_private;
        struct hfs_cat_entry *entry;


        struct hfs_fork *fork;
        int convert;


        ino_t file_type;
        char dir_size;


        const struct hfs_hdr_layout *default_layout;
        struct hfs_hdr_layout *layout;


        int tz_secondswest;


        void (*d_drop_op)(struct dentry *, const ino_t);
};
# 309 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/adfs_fs_i.h" 1
# 13 "/opt/Wive/kernel/include/linux/adfs_fs_i.h"
struct adfs_inode_info {
        unsigned long mmu_private;
        unsigned long parent_id;
        __u32 loadaddr;
        __u32 execaddr;
        unsigned int filetype;
        unsigned int attr;
        int stamped:1;
};
# 310 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/qnx4_fs_i.h" 1
# 14 "/opt/Wive/kernel/include/linux/qnx4_fs_i.h"
# 1 "/opt/Wive/kernel/include/linux/qnxtypes.h" 1
# 15 "/opt/Wive/kernel/include/linux/qnxtypes.h"
typedef __u16 qnx4_nxtnt_t;
typedef __u8 qnx4_ftype_t;

typedef struct {
        __u32 xtnt_blk;
        __u32 xtnt_size;
} qnx4_xtnt_t;

typedef __u16 qnx4_mode_t;
typedef __u16 qnx4_muid_t;
typedef __u16 qnx4_mgid_t;
typedef __u32 qnx4_off_t;
typedef __u16 qnx4_nlink_t;
# 15 "/opt/Wive/kernel/include/linux/qnx4_fs_i.h" 2




struct qnx4_inode_info {
        char i_reserved[16];
        qnx4_off_t i_size;
        qnx4_xtnt_t i_first_xtnt;
        __u32 i_xblk;
        __s32 i_ftime;
        __s32 i_mtime;
        __s32 i_atime;
        __s32 i_ctime;
        qnx4_nxtnt_t i_num_xtnts;
        qnx4_mode_t i_mode;
        qnx4_muid_t i_uid;
        qnx4_mgid_t i_gid;
        qnx4_nlink_t i_nlink;
        __u8 i_zero[4];
        qnx4_ftype_t i_type;
        __u8 i_status;
        unsigned long mmu_private;
};
# 311 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/reiserfs_fs_i.h" 1






typedef enum {



    i_item_key_version_mask = 0x0001,


    i_stat_data_version_mask = 0x0002,

    i_pack_on_close_mask = 0x0004,

    i_nopack_mask = 0x0008,



    i_link_saved_unlink_mask = 0x0010,
    i_link_saved_truncate_mask = 0x0020
} reiserfs_inode_flags;


struct reiserfs_inode_info {
    __u32 i_key [4];



    __u32 i_flags;

    __u32 i_first_direct_byte;

    int i_prealloc_block;
    int i_prealloc_count;
    struct list_head i_prealloc_list;





    unsigned long i_trans_id ;
    unsigned long i_trans_index ;
};
# 312 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/bfs_fs_i.h" 1
# 12 "/opt/Wive/kernel/include/linux/bfs_fs_i.h"
struct bfs_inode_info {
        unsigned long i_dsk_ino;
        unsigned long i_sblock;
        unsigned long i_eblock;
};
# 313 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/udf_fs_i.h" 1
# 24 "/opt/Wive/kernel/include/linux/udf_fs_i.h"
typedef struct
{
        __u32 logicalBlockNum;
        __u16 partitionReferenceNum;
} lb_addr;


struct udf_inode_info
{
        long i_umtime;
        long i_uctime;
        long i_crtime;
        long i_ucrtime;

        lb_addr i_location;
        __u64 i_unique;
        __u32 i_lenEAttr;
        __u32 i_lenAlloc;
        __u64 i_lenExtents;
        __u32 i_next_alloc_block;
        __u32 i_next_alloc_goal;
        unsigned i_alloc_type : 3;
        unsigned i_extended_fe : 1;
        unsigned i_strat_4096 : 1;
        unsigned i_new_inode : 1;
        unsigned reserved : 26;
};
# 314 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/ncp_fs_i.h" 1
# 17 "/opt/Wive/kernel/include/linux/ncp_fs_i.h"
struct ncp_inode_info {
        __u32 dirEntNum __attribute__((packed));
        __u32 DosDirNum __attribute__((packed));
        __u32 volNumber __attribute__((packed));
        __u32 nwattr;
        struct semaphore open_sem;
        atomic_t opened;
        int access;
        __u32 server_file_handle __attribute__((packed));
        __u8 open_create_action __attribute__((packed));
        __u8 file_handle[6] __attribute__((packed));
};
# 315 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/proc_fs_i.h" 1
struct proc_inode_info {
        struct task_struct *task;
        int type;
        union {
                int (*proc_get_link)(struct inode *, struct dentry **, struct vfsmount **);
                int (*proc_read)(struct task_struct *task, char *page);
        } op;
        struct file *file;
};
# 316 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/usbdev_fs_i.h" 1
struct usb_device;
struct usb_bus;

struct usbdev_inode_info {
        struct list_head dlist;
        struct list_head slist;
        union {
                struct usb_device *dev;
                struct usb_bus *bus;
        } p;
};
# 317 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/jffs2_fs_i.h" 1
# 16 "/opt/Wive/kernel/include/linux/jffs2_fs_i.h"
struct jffs2_inode_info {
# 26 "/opt/Wive/kernel/include/linux/jffs2_fs_i.h"
        struct semaphore sem;


        __u32 highest_version;


        struct jffs2_node_frag *fraglist;






        struct jffs2_full_dnode *metadata;


        struct jffs2_full_dirent *dents;


        struct jffs2_inode_cache *inocache;





        __u16 flags;
        __u8 usercompr;
};
# 318 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/cramfs_fs_sb.h" 1






struct cramfs_sb_info {
                        unsigned long magic;
                        unsigned long size;
                        unsigned long blocks;
                        unsigned long files;
                        unsigned long flags;
};
# 319 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/squashfs_fs_i.h" 1
# 25 "/opt/Wive/kernel/include/linux/squashfs_fs_i.h"
typedef struct squashfs_inode_info {
        unsigned int start_block;
        unsigned int block_list_start;
        unsigned int offset;
        unsigned int fragment_start_block;
        unsigned int fragment_size;
        unsigned int fragment_offset;
        } squashfs_inode_info;
# 320 "/opt/Wive/kernel/include/linux/fs.h" 2
# 346 "/opt/Wive/kernel/include/linux/fs.h"
struct iattr {
        unsigned int ia_valid;
        umode_t ia_mode;
        uid_t ia_uid;
        gid_t ia_gid;
        loff_t ia_size;
        time_t ia_atime;
        time_t ia_mtime;
        time_t ia_ctime;
        unsigned int ia_attr_flags;
};
# 370 "/opt/Wive/kernel/include/linux/fs.h"
# 1 "/opt/Wive/kernel/include/linux/quota.h" 1
# 42 "/opt/Wive/kernel/include/linux/quota.h"
# 1 "/opt/Wive/kernel/include/linux/errno.h" 1



# 1 "/opt/Wive/kernel/include/asm/errno.h" 1
# 5 "/opt/Wive/kernel/include/linux/errno.h" 2
# 43 "/opt/Wive/kernel/include/linux/quota.h" 2
# 109 "/opt/Wive/kernel/include/linux/quota.h"
struct dqblk {
        __u32 dqb_bhardlimit;
        __u32 dqb_bsoftlimit;
        __u32 dqb_curblocks;
        __u32 dqb_ihardlimit;
        __u32 dqb_isoftlimit;
        __u32 dqb_curinodes;
        time_t dqb_btime;
        time_t dqb_itime;
};
# 134 "/opt/Wive/kernel/include/linux/quota.h"
struct dqstats {
        __u32 lookups;
        __u32 drops;
        __u32 reads;
        __u32 writes;
        __u32 cache_hits;
        __u32 allocated_dquots;
        __u32 free_dquots;
        __u32 syncs;
};



extern int nr_dquots, nr_free_dquots;
extern int dquot_root_squash;
# 159 "/opt/Wive/kernel/include/linux/quota.h"
struct dquot {
        struct list_head dq_hash;
        struct list_head dq_inuse;
        struct list_head dq_free;
        wait_queue_head_t dq_wait_lock;
        wait_queue_head_t dq_wait_free;
        int dq_count;


        struct super_block *dq_sb;
        unsigned int dq_id;
        kdev_t dq_dev;
        short dq_type;
        short dq_flags;
        unsigned long dq_referenced;

        struct dqblk dq_dqb;
};
# 371 "/opt/Wive/kernel/include/linux/fs.h" 2





struct page;
struct address_space;
struct kiobuf;

struct address_space_operations {
        int (*writepage)(struct page *);
        int (*readpage)(struct file *, struct page *);
        int (*sync_page)(struct page *);




        int (*prepare_write)(struct file *, struct page *, unsigned, unsigned);
        int (*commit_write)(struct file *, struct page *, unsigned, unsigned);

        int (*bmap)(struct address_space *, long);
        int (*flushpage) (struct page *, unsigned long);
        int (*releasepage) (struct page *, int);

        int (*direct_IO)(int, struct inode *, struct kiobuf *, unsigned long, int);
        void (*removepage)(struct page *);
};

struct address_space {
        struct list_head clean_pages;
        struct list_head dirty_pages;
        struct list_head locked_pages;
        unsigned long nrpages;
        struct address_space_operations *a_ops;
        struct inode *host;
        struct vm_area_struct *i_mmap;
        struct vm_area_struct *i_mmap_shared;
        spinlock_t i_shared_lock;
        int gfp_mask;
};

struct char_device {
        struct list_head hash;
        atomic_t count;
        dev_t dev;
        atomic_t openers;
        struct semaphore sem;
};

struct block_device {
        struct list_head bd_hash;
        atomic_t bd_count;
        struct inode * bd_inode;
        dev_t bd_dev;
        int bd_openers;
        const struct block_device_operations *bd_op;
        struct semaphore bd_sem;
        struct list_head bd_inodes;
};

struct inode {
        struct list_head i_hash;
        struct list_head i_list;
        struct list_head i_dentry;

        struct list_head i_dirty_buffers;
        struct list_head i_dirty_data_buffers;

        unsigned long i_ino;
        atomic_t i_count;
        kdev_t i_dev;
        umode_t i_mode;
        nlink_t i_nlink;
        uid_t i_uid;
        gid_t i_gid;
        kdev_t i_rdev;
        loff_t i_size;
        time_t i_atime;
        time_t i_mtime;
        time_t i_ctime;
        unsigned int i_blkbits;
        unsigned long i_blksize;
        unsigned long i_blocks;
        unsigned long i_version;
        struct semaphore i_sem;
        struct semaphore i_zombie;
        struct inode_operations *i_op;
        struct file_operations *i_fop;
        struct super_block *i_sb;
        wait_queue_head_t i_wait;
        struct file_lock *i_flock;
        struct address_space *i_mapping;
        struct address_space i_data;
        struct dquot *i_dquot[2];

        struct list_head i_devices;
        struct pipe_inode_info *i_pipe;
        struct block_device *i_bdev;
        struct char_device *i_cdev;

        unsigned long i_dnotify_mask;
        struct dnotify_struct *i_dnotify;

        unsigned long i_state;

        unsigned int i_flags;
        unsigned char i_sock;

        atomic_t i_writecount;
        unsigned int i_attr_flags;
        __u32 i_generation;
        union {
                struct minix_inode_info minix_i;
                struct ext2_inode_info ext2_i;
                struct ext3_inode_info ext3_i;
                struct hpfs_inode_info hpfs_i;
                struct ntfs_inode_info ntfs_i;
                struct msdos_inode_info msdos_i;
                struct umsdos_inode_info umsdos_i;
                struct iso_inode_info isofs_i;
                struct nfs_inode_info nfs_i;
                struct sysv_inode_info sysv_i;
                struct affs_inode_info affs_i;
                struct ufs_inode_info ufs_i;
                struct efs_inode_info efs_i;
                struct romfs_inode_info romfs_i;
                struct shmem_inode_info shmem_i;
                struct coda_inode_info coda_i;
                struct smb_inode_info smbfs_i;
                struct hfs_inode_info hfs_i;
                struct adfs_inode_info adfs_i;
                struct qnx4_inode_info qnx4_i;
                struct reiserfs_inode_info reiserfs_i;
                struct bfs_inode_info bfs_i;
                struct udf_inode_info udf_i;
                struct ncp_inode_info ncpfs_i;
                struct proc_inode_info proc_i;
                struct socket socket_i;
                struct usbdev_inode_info usbdev_i;
                struct jffs2_inode_info jffs2_i;
                struct squashfs_inode_info squashfs_i;
                void *generic_ip;
        } u;
};

struct fown_struct {
        int pid;
        uid_t uid, euid;
        int signum;
};

struct file {
        struct list_head f_list;
        struct dentry *f_dentry;
        struct vfsmount *f_vfsmnt;
        struct file_operations *f_op;
        atomic_t f_count;
        unsigned int f_flags;
        mode_t f_mode;
        loff_t f_pos;
        unsigned long f_reada, f_ramax, f_raend, f_ralen, f_rawin;
        struct fown_struct f_owner;
        unsigned int f_uid, f_gid;
        int f_error;

        unsigned long f_version;


        void *private_data;


        struct kiobuf *f_iobuf;
        long f_iobuf_lock;
};
extern spinlock_t files_lock;






extern int init_private_file(struct file *, struct dentry *, int);
# 570 "/opt/Wive/kernel/include/linux/fs.h"
typedef struct files_struct *fl_owner_t;

struct file_lock {
        struct file_lock *fl_next;
        struct list_head fl_link;
        struct list_head fl_block;
        fl_owner_t fl_owner;
        unsigned int fl_pid;
        wait_queue_head_t fl_wait;
        struct file *fl_file;
        unsigned char fl_flags;
        unsigned char fl_type;
        loff_t fl_start;
        loff_t fl_end;

        void (*fl_notify)(struct file_lock *);
        void (*fl_insert)(struct file_lock *);
        void (*fl_remove)(struct file_lock *);

        struct fasync_struct * fl_fasync;

        union {
                struct nfs_lock_info nfs_fl;
        } fl_u;
};
# 603 "/opt/Wive/kernel/include/linux/fs.h"
extern struct list_head file_lock_list;

# 1 "/opt/Wive/kernel/include/linux/fcntl.h" 1



# 1 "/opt/Wive/kernel/include/asm/fcntl.h" 1
# 84 "/opt/Wive/kernel/include/asm/fcntl.h"
typedef struct flock {
        short l_type;
        short l_whence;
        __kernel_off_t l_start;
        __kernel_off_t l_len;
        long l_sysid;
        __kernel_pid_t l_pid;
        long pad[4];
} flock_t;

typedef struct flock64 {
        short l_type;
        short l_whence;
        loff_t l_start;
        loff_t l_len;
        pid_t l_pid;
} flock64_t;
# 5 "/opt/Wive/kernel/include/linux/fcntl.h" 2
# 606 "/opt/Wive/kernel/include/linux/fs.h" 2

extern int fcntl_getlk(unsigned int, struct flock *);
extern int fcntl_setlk(unsigned int, unsigned int, struct flock *);

extern int fcntl_getlk64(unsigned int, struct flock64 *);
extern int fcntl_setlk64(unsigned int, unsigned int, struct flock64 *);


extern void locks_init_lock(struct file_lock *);
extern void locks_copy_lock(struct file_lock *, struct file_lock *);
extern void locks_remove_posix(struct file *, fl_owner_t);
extern void locks_remove_flock(struct file *);
extern struct file_lock *posix_test_lock(struct file *, struct file_lock *);
extern int posix_lock_file(struct file *, struct file_lock *, unsigned int);
extern void posix_block_lock(struct file_lock *, struct file_lock *);
extern void posix_unblock_lock(struct file_lock *);
extern int posix_locks_deadlock(struct file_lock *, struct file_lock *);
extern int __get_lease(struct inode *inode, unsigned int flags);
extern time_t lease_get_mtime(struct inode *);
extern int lock_may_read(struct inode *, loff_t start, unsigned long count);
extern int lock_may_write(struct inode *, loff_t start, unsigned long count);

struct fasync_struct {
        int magic;
        int fa_fd;
        struct fasync_struct *fa_next;
        struct file *fa_file;
};




extern int fasync_helper(int, struct file *, int, struct fasync_struct **);

extern void kill_fasync(struct fasync_struct **, int, int);

extern void __kill_fasync(struct fasync_struct *, int, int);

struct nameidata {
        struct dentry *dentry;
        struct vfsmount *mnt;
        struct qstr last;
        unsigned int flags;
        int last_type;
};




struct quota_mount_options
{
        unsigned int flags;
        struct semaphore dqio_sem;
        struct semaphore dqoff_sem;
        struct file *files[2];
        time_t inode_expire[2];
        time_t block_expire[2];
        char rsquash[2];
};
# 673 "/opt/Wive/kernel/include/linux/fs.h"
# 1 "/opt/Wive/kernel/include/linux/minix_fs_sb.h" 1






struct minix_sb_info {
                        unsigned long s_ninodes;
                        unsigned long s_nzones;
                        unsigned long s_imap_blocks;
                        unsigned long s_zmap_blocks;
                        unsigned long s_firstdatazone;
                        unsigned long s_log_zone_size;
                        unsigned long s_max_size;
                        int s_dirsize;
                        int s_namelen;
                        int s_link_max;
                        struct buffer_head ** s_imap;
                        struct buffer_head ** s_zmap;
                        struct buffer_head * s_sbh;
                        struct minix_super_block * s_ms;
                        unsigned short s_mount_state;
                        unsigned short s_version;
};
# 674 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/ext2_fs_sb.h" 1
# 30 "/opt/Wive/kernel/include/linux/ext2_fs_sb.h"
struct ext2_sb_info {
        unsigned long s_frag_size;
        unsigned long s_frags_per_block;
        unsigned long s_inodes_per_block;
        unsigned long s_frags_per_group;
        unsigned long s_blocks_per_group;
        unsigned long s_inodes_per_group;
        unsigned long s_itb_per_group;
        unsigned long s_gdb_count;
        unsigned long s_desc_per_block;
        unsigned long s_groups_count;
        struct buffer_head * s_sbh;
        struct ext2_super_block * s_es;
        struct buffer_head ** s_group_desc;
        unsigned short s_loaded_inode_bitmaps;
        unsigned short s_loaded_block_bitmaps;
        unsigned long s_inode_bitmap_number[8];
        struct buffer_head * s_inode_bitmap[8];
        unsigned long s_block_bitmap_number[8];
        struct buffer_head * s_block_bitmap[8];
        unsigned long s_mount_opt;
        uid_t s_resuid;
        gid_t s_resgid;
        unsigned short s_mount_state;
        unsigned short s_pad;
        int s_addr_per_block_bits;
        int s_desc_per_block_bits;
        int s_inode_size;
        int s_first_ino;
};
# 675 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/ext3_fs_sb.h" 1
# 20 "/opt/Wive/kernel/include/linux/ext3_fs_sb.h"
# 1 "/opt/Wive/kernel/include/linux/timer.h" 1
# 16 "/opt/Wive/kernel/include/linux/timer.h"
struct timer_list {
        struct list_head list;
        unsigned long expires;
        unsigned long data;
        void (*function)(unsigned long);
};

extern void add_timer(struct timer_list * timer);
extern int del_timer(struct timer_list * timer);
# 41 "/opt/Wive/kernel/include/linux/timer.h"
int mod_timer(struct timer_list *timer, unsigned long expires);

extern void it_real_fn(unsigned long);

static inline void init_timer(struct timer_list * timer)
{
        timer->list.next = timer->list.prev = ((void *)0);
}

static inline int timer_pending (const struct timer_list * timer)
{
        return timer->list.next != ((void *)0);
}
# 21 "/opt/Wive/kernel/include/linux/ext3_fs_sb.h" 2
# 35 "/opt/Wive/kernel/include/linux/ext3_fs_sb.h"
struct ext3_sb_info {
        unsigned long s_frag_size;
        unsigned long s_frags_per_block;
        unsigned long s_inodes_per_block;
        unsigned long s_frags_per_group;
        unsigned long s_blocks_per_group;
        unsigned long s_inodes_per_group;
        unsigned long s_itb_per_group;
        unsigned long s_gdb_count;
        unsigned long s_desc_per_block;
        unsigned long s_groups_count;
        struct buffer_head * s_sbh;
        struct ext3_super_block * s_es;
        struct buffer_head ** s_group_desc;
        unsigned short s_loaded_inode_bitmaps;
        unsigned short s_loaded_block_bitmaps;
        unsigned long s_inode_bitmap_number[8];
        struct buffer_head * s_inode_bitmap[8];
        unsigned long s_block_bitmap_number[8];
        struct buffer_head * s_block_bitmap[8];
        unsigned long s_mount_opt;
        uid_t s_resuid;
        gid_t s_resgid;
        unsigned short s_mount_state;
        unsigned short s_pad;
        int s_addr_per_block_bits;
        int s_desc_per_block_bits;
        int s_inode_size;
        int s_first_ino;
        u32 s_next_generation;


        struct inode * s_journal_inode;
        struct journal_s * s_journal;
        struct list_head s_orphan;
        unsigned long s_commit_interval;
        struct block_device *journal_bdev;




};
# 676 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/hpfs_fs_sb.h" 1



struct hpfs_sb_info {
        ino_t sb_root;
        unsigned sb_fs_size;
        unsigned sb_bitmaps;
        unsigned sb_dirband_start;
        unsigned sb_dirband_size;
        unsigned sb_dmap;
        unsigned sb_n_free;
        unsigned sb_n_free_dnodes;
        uid_t sb_uid;
        gid_t sb_gid;
        umode_t sb_mode;
        unsigned sb_conv : 2;
        unsigned sb_eas : 2;
        unsigned sb_err : 2;
        unsigned sb_chk : 2;
        unsigned sb_lowercase : 1;
        unsigned sb_was_error : 1;
        unsigned sb_chkdsk : 2;
        unsigned sb_rd_fnode : 2;
        unsigned sb_rd_inode : 2;


        wait_queue_head_t sb_iget_q;
        unsigned char *sb_cp_table;


        unsigned *sb_bmp_dir;
        unsigned sb_c_bitmap;
        wait_queue_head_t sb_creation_de;

        unsigned sb_creation_de_lock : 1;

        int sb_timeshift;
};
# 677 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/ntfs_fs_sb.h" 1





struct ntfs_sb_info{

        ntfs_uid_t uid;
        ntfs_gid_t gid;
        ntmode_t umask;
        void *nls_map;
        unsigned int ngt;
        char mft_zone_multiplier;
        unsigned long mft_data_pos;
        ntfs_cluster_t mft_zone_pos;
        ntfs_cluster_t mft_zone_start;
        ntfs_cluster_t mft_zone_end;
        ntfs_cluster_t data1_zone_pos;
        ntfs_cluster_t data2_zone_pos;


        ntfs_size_t partition_bias;

        ntfs_u32 at_standard_information;
        ntfs_u32 at_attribute_list;
        ntfs_u32 at_file_name;
        ntfs_u32 at_volume_version;
        ntfs_u32 at_security_descriptor;
        ntfs_u32 at_volume_name;
        ntfs_u32 at_volume_information;
        ntfs_u32 at_data;
        ntfs_u32 at_index_root;
        ntfs_u32 at_index_allocation;
        ntfs_u32 at_bitmap;
        ntfs_u32 at_symlink;

        int sector_size;
        int cluster_size;
        int cluster_size_bits;
        int mft_clusters_per_record;
        int mft_record_size;
        int mft_record_size_bits;
        int index_clusters_per_record;
        int index_record_size;
        int index_record_size_bits;
        ntfs_cluster_t nr_clusters;
        ntfs_cluster_t mft_lcn;
        ntfs_cluster_t mft_mirr_lcn;

        unsigned char *mft;
        unsigned short *upcase;
        unsigned int upcase_length;

        struct ntfs_inode_info *mft_ino;
        struct ntfs_inode_info *mftmirr;
        struct ntfs_inode_info *bitmap;
        struct super_block *sb;
        unsigned char ino_flags;
};
# 678 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/msdos_fs_sb.h" 1


# 1 "/opt/Wive/kernel/include/linux/fat_cvf.h" 1





struct cvf_format
{ int cvf_version;
  char* cvf_version_text;
  unsigned long flags;
  int (*detect_cvf) (struct super_block*sb);
  int (*mount_cvf) (struct super_block*sb,char*options);
  int (*unmount_cvf) (struct super_block*sb);
  struct buffer_head* (*cvf_bread) (struct super_block*sb,int block);
  struct buffer_head* (*cvf_getblk) (struct super_block*sb,int block);
  void (*cvf_brelse) (struct super_block *sb,struct buffer_head *bh);
  void (*cvf_mark_buffer_dirty) (struct super_block *sb,
                              struct buffer_head *bh);
  void (*cvf_set_uptodate) (struct super_block *sb,
                         struct buffer_head *bh,
                         int val);
  int (*cvf_is_uptodate) (struct super_block *sb,struct buffer_head *bh);
  void (*cvf_ll_rw_block) (struct super_block *sb,
                        int opr,
                        int nbreq,
                        struct buffer_head *bh[32]);
  int (*fat_access) (struct super_block *sb,int nr,int new_value);
  int (*cvf_statfs) (struct super_block *sb,struct statfs *buf, int bufsiz);
  int (*cvf_bmap) (struct inode *inode,int block);
  ssize_t (*cvf_file_read) ( struct file *, char *, size_t, loff_t *);
  ssize_t (*cvf_file_write) ( struct file *, const char *, size_t, loff_t *);
  int (*cvf_mmap) (struct file *, struct vm_area_struct *);
  int (*cvf_readpage) (struct inode *, struct page *);
  int (*cvf_writepage) (struct inode *, struct page *);
  int (*cvf_dir_ioctl) (struct inode * inode, struct file * filp,
                        unsigned int cmd, unsigned long arg);
  void (*zero_out_cluster) (struct inode*, int clusternr);
};

int register_cvf_format(struct cvf_format*cvf_format);
int unregister_cvf_format(struct cvf_format*cvf_format);
void dec_cvf_format_use_count_by_version(int version);
int detect_cvf(struct super_block*sb,char*force);

extern struct cvf_format *cvf_formats[];
extern int cvf_format_use_count[];
# 4 "/opt/Wive/kernel/include/linux/msdos_fs_sb.h" 2





struct fat_mount_options {
        uid_t fs_uid;
        gid_t fs_gid;
        unsigned short fs_umask;
        unsigned short codepage;
        char *iocharset;
        unsigned short shortname;
        unsigned char name_check;
        unsigned char conversion;
        unsigned quiet:1,
                 showexec:1,
                 sys_immutable:1,
                 dotsOK:1,
                 isvfat:1,
                 utf8:1,
                 unicode_xlate:1,
                 posixfs:1,
                 numtail:1,
                 atari:1,
                 fat32:1,
                 nocase:1;
};

struct msdos_sb_info {
        unsigned short cluster_size;
        unsigned short cluster_bits;
        unsigned char fats,fat_bits;
        unsigned short fat_start;
        unsigned long fat_length;
        unsigned long dir_start;
        unsigned short dir_entries;
        unsigned long data_start;
        unsigned long clusters;
        unsigned long root_cluster;
        unsigned long fsinfo_sector;
        struct semaphore fat_lock;
        int prev_free;
        int free_clusters;
        struct fat_mount_options options;
        struct nls_table *nls_disk;
        struct nls_table *nls_io;
        struct cvf_format* cvf_format;
        void *dir_ops;
        void *private_data;
        int dir_per_block;
        int dir_per_block_bits;
};
# 679 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/iso_fs_sb.h" 1






struct isofs_sb_info {
        unsigned long s_ninodes;
        unsigned long s_nzones;
        unsigned long s_firstdatazone;
        unsigned long s_log_zone_size;
        unsigned long s_max_size;

        unsigned char s_high_sierra;
        unsigned char s_mapping;
        int s_rock_offset;
        unsigned char s_rock;
        unsigned char s_joliet_level;
        unsigned char s_utf8;
        unsigned char s_cruft;


        unsigned char s_unhide;
        unsigned char s_nosuid;
        unsigned char s_nodev;
        unsigned char s_nocompress;

        mode_t s_mode;
        gid_t s_gid;
        uid_t s_uid;
        struct nls_table *s_nls_iocharset;
};
# 680 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/nfs_fs_sb.h" 1
# 9 "/opt/Wive/kernel/include/linux/nfs_fs_sb.h"
struct nfs_server {
        struct rpc_clnt * client;
        struct nfs_rpc_ops * rpc_ops;
        int flags;
        unsigned int caps;
        unsigned int rsize;
        unsigned int rpages;
        unsigned int wsize;
        unsigned int wpages;
        unsigned int dtsize;
        unsigned int bsize;
        unsigned int acregmin;
        unsigned int acregmax;
        unsigned int acdirmin;
        unsigned int acdirmax;
        unsigned int namelen;
        char * hostname;
        struct nfs_reqlist * rw_requests;
        struct list_head lru_read,
                                lru_dirty,
                                lru_commit,
                                lru_busy;
};




struct nfs_sb_info {
        struct nfs_server s_server;
};
# 681 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/sysv_fs_sb.h" 1
# 13 "/opt/Wive/kernel/include/linux/sysv_fs_sb.h"
struct sysv_sb_info {
        int s_type;
        char s_bytesex;
        char s_truncate;

        nlink_t s_link_max;
        unsigned int s_inodes_per_block;
        unsigned int s_inodes_per_block_1;
        unsigned int s_inodes_per_block_bits;
        unsigned int s_ind_per_block;
        unsigned int s_ind_per_block_bits;
        unsigned int s_ind_per_block_2;
        unsigned int s_toobig_block;
        unsigned int s_block_base;
        unsigned short s_fic_size;
        unsigned short s_flc_size;

        struct buffer_head *s_bh1;
        struct buffer_head *s_bh2;


        char * s_sbd1;
        char * s_sbd2;
        u16 *s_sb_fic_count;
        u16 *s_sb_fic_inodes;
        u16 *s_sb_total_free_inodes;
        u16 *s_bcache_count;
        u32 *s_bcache;
        u32 *s_free_blocks;
        u32 *s_sb_time;
        u32 *s_sb_state;


        u32 s_firstinodezone;
        u32 s_firstdatazone;
        u32 s_ninodes;
        u32 s_ndatazones;
        u32 s_nzones;
        u16 s_namelen;
};
# 682 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/affs_fs_sb.h" 1
# 11 "/opt/Wive/kernel/include/linux/affs_fs_sb.h"
struct affs_bm_info {
        u32 bm_key;
        u32 bm_free;
};

struct affs_sb_info {
        int s_partition_size;
        int s_reserved;

        u32 s_data_blksize;
        u32 s_root_block;
        int s_hashsize;
        unsigned long s_flags;
        uid_t s_uid;
        gid_t s_gid;
        umode_t s_mode;
        struct buffer_head *s_root_bh;
        struct semaphore s_bmlock;
        struct affs_bm_info *s_bitmap;
        u32 s_bmap_count;
        u32 s_bmap_bits;
        u32 s_last_bmap;
        struct buffer_head *s_bmap_bh;
        char *s_prefix;
        int s_prefix_len;
        char s_volume[32];
};
# 683 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/ufs_fs_sb.h" 1
# 17 "/opt/Wive/kernel/include/linux/ufs_fs_sb.h"
# 1 "/opt/Wive/kernel/include/linux/ufs_fs.h" 1
# 237 "/opt/Wive/kernel/include/linux/ufs_fs.h"
struct ufs_timeval {
        __s32 tv_sec;
        __s32 tv_usec;
};

struct ufs_dir_entry {
        __u32 d_ino;
        __u16 d_reclen;
        union {
                __u16 d_namlen;
                struct {
                        __u8 d_type;
                        __u8 d_namlen;
                } d_44;
        } d_u;
        __u8 d_name[255 + 1];
};

struct ufs_csum {
        __u32 cs_ndir;
        __u32 cs_nbfree;
        __u32 cs_nifree;
        __u32 cs_nffree;
};




struct ufs_super_block {
        __u32 fs_link;
        __u32 fs_rlink;
        __u32 fs_sblkno;
        __u32 fs_cblkno;
        __u32 fs_iblkno;
        __u32 fs_dblkno;
        __u32 fs_cgoffset;
        __u32 fs_cgmask;
        __u32 fs_time;
        __u32 fs_size;
        __u32 fs_dsize;
        __u32 fs_ncg;
        __u32 fs_bsize;
        __u32 fs_fsize;
        __u32 fs_frag;

        __u32 fs_minfree;
        __u32 fs_rotdelay;
        __u32 fs_rps;

        __u32 fs_bmask;
        __u32 fs_fmask;
        __u32 fs_bshift;
        __u32 fs_fshift;

        __u32 fs_maxcontig;
        __u32 fs_maxbpg;

        __u32 fs_fragshift;
        __u32 fs_fsbtodb;
        __u32 fs_sbsize;
        __u32 fs_csmask;
        __u32 fs_csshift;
        __u32 fs_nindir;
        __u32 fs_inopb;
        __u32 fs_nspf;

        __u32 fs_optim;

        union {
                struct {
                        __u32 fs_npsect;
                } fs_sun;
                struct {
                        __s32 fs_state;
                } fs_sunx86;
        } fs_u1;
        __u32 fs_interleave;
        __u32 fs_trackskew;




        __u32 fs_id[2];

        __u32 fs_csaddr;
        __u32 fs_cssize;
        __u32 fs_cgsize;

        __u32 fs_ntrak;
        __u32 fs_nsect;
        __u32 fs_spc;

        __u32 fs_ncyl;

        __u32 fs_cpg;
        __u32 fs_ipg;
        __u32 fs_fpg;

        struct ufs_csum fs_cstotal;

        __s8 fs_fmod;
        __s8 fs_clean;
        __s8 fs_ronly;
        __s8 fs_flags;
        __s8 fs_fsmnt[512];

        __u32 fs_cgrotor;
        __u32 fs_csp[31];
        __u32 fs_maxcluster;
        __u32 fs_cpc;
        __u16 fs_opostbl[16][8];
        union {
                struct {
                        __s32 fs_sparecon[53];
                        __s32 fs_reclaim;
                        __s32 fs_sparecon2[1];
                        __s32 fs_state;
                        __u32 fs_qbmask[2];
                        __u32 fs_qfmask[2];
                } fs_sun;
                struct {
                        __s32 fs_sparecon[53];
                        __s32 fs_reclaim;
                        __s32 fs_sparecon2[1];
                        __u32 fs_npsect;
                        __u32 fs_qbmask[2];
                        __u32 fs_qfmask[2];
                } fs_sunx86;
                struct {
                        __s32 fs_sparecon[50];
                        __s32 fs_contigsumsize;
                        __s32 fs_maxsymlinklen;
                        __s32 fs_inodefmt;
                        __u32 fs_maxfilesize[2];
                        __u32 fs_qbmask[2];
                        __u32 fs_qfmask[2];
                        __s32 fs_state;
                } fs_44;
        } fs_u2;
        __s32 fs_postblformat;
        __s32 fs_nrpos;
        __s32 fs_postbloff;
        __s32 fs_rotbloff;
        __s32 fs_magic;
        __u8 fs_space[1];
};
# 415 "/opt/Wive/kernel/include/linux/ufs_fs.h"
struct ufs_cylinder_group {
        __u32 cg_link;
        __u32 cg_magic;
        __u32 cg_time;
        __u32 cg_cgx;
        __u16 cg_ncyl;
        __u16 cg_niblk;
        __u32 cg_ndblk;
        struct ufs_csum cg_cs;
        __u32 cg_rotor;
        __u32 cg_frotor;
        __u32 cg_irotor;
        __u32 cg_frsum[(8192 / 1024)];
        __u32 cg_btotoff;
        __u32 cg_boff;
        __u32 cg_iusedoff;
        __u32 cg_freeoff;
        __u32 cg_nextfreeoff;
        union {
                struct {
                        __u32 cg_clustersumoff;
                        __u32 cg_clusteroff;
                        __u32 cg_nclusterblks;
                        __u32 cg_sparecon[13];
                } cg_44;
                __u32 cg_sparecon[16];
        } cg_u;
        __u8 cg_space[1];

};




struct ufs_inode {
        __u16 ui_mode;
        __u16 ui_nlink;
        union {
                struct {
                        __u16 ui_suid;
                        __u16 ui_sgid;
                } oldids;
                __u32 ui_inumber;
                __u32 ui_author;
        } ui_u1;
        __u64 ui_size;
        struct ufs_timeval ui_atime;
        struct ufs_timeval ui_mtime;
        struct ufs_timeval ui_ctime;
        union {
                struct {
                        __u32 ui_db[12];
                        __u32 ui_ib[3];
                } ui_addr;
                __u8 ui_symlink[4*(12 +3)];
        } ui_u2;
        __u32 ui_flags;
        __u32 ui_blocks;
        __u32 ui_gen;
        union {
                struct {
                        __u32 ui_shadow;
                        __u32 ui_uid;
                        __u32 ui_gid;
                        __u32 ui_oeftflag;
                } ui_sun;
                struct {
                        __u32 ui_uid;
                        __u32 ui_gid;
                        __s32 ui_spare[2];
                } ui_44;
                struct {
                        __u32 ui_uid;
                        __u32 ui_gid;
                        __u16 ui_modeh;
                        __u16 ui_spare;
                        __u32 ui_trans;
                } ui_hurd;
        } ui_u3;
};
# 514 "/opt/Wive/kernel/include/linux/ufs_fs.h"
extern void ufs_free_fragments (struct inode *, unsigned, unsigned);
extern void ufs_free_blocks (struct inode *, unsigned, unsigned);
extern unsigned ufs_new_fragments (struct inode *, u32 *, unsigned, unsigned, unsigned, int *);


extern struct ufs_cg_private_info * ufs_load_cylinder (struct super_block *, unsigned);
extern void ufs_put_cylinder (struct super_block *, unsigned);


extern struct inode_operations ufs_dir_inode_operations;
extern int ufs_check_dir_entry (const char *, struct inode *, struct ufs_dir_entry *, struct buffer_head *, unsigned long);
extern int ufs_add_link (struct dentry *, struct inode *);
extern ino_t ufs_inode_by_name(struct inode *, struct dentry *);
extern int ufs_make_empty(struct inode *, struct inode *);
extern struct ufs_dir_entry * ufs_find_entry (struct dentry *, struct buffer_head **);
extern int ufs_delete_entry (struct inode *, struct ufs_dir_entry *, struct buffer_head *);
extern int ufs_empty_dir (struct inode *);
extern struct ufs_dir_entry * ufs_dotdot (struct inode *, struct buffer_head **);
extern void ufs_set_link(struct inode *, struct ufs_dir_entry *, struct buffer_head *, struct inode *);


extern struct inode_operations ufs_file_inode_operations;
extern struct file_operations ufs_file_operations;

extern struct address_space_operations ufs_aops;


extern void ufs_free_inode (struct inode *inode);
extern struct inode * ufs_new_inode (const struct inode *, int);


extern int ufs_frag_map (struct inode *, int);
extern void ufs_read_inode (struct inode *);
extern void ufs_put_inode (struct inode *);
extern void ufs_write_inode (struct inode *, int);
extern int ufs_sync_inode (struct inode *);
extern void ufs_delete_inode (struct inode *);
extern struct buffer_head * ufs_getfrag (struct inode *, unsigned, int, int *);
extern struct buffer_head * ufs_bread (struct inode *, unsigned, int, int *);


extern struct file_operations ufs_dir_operations;


extern struct file_system_type ufs_fs_type;
extern void ufs_warning (struct super_block *, const char *, const char *, ...) __attribute__ ((format (printf, 3, 4)));
extern void ufs_error (struct super_block *, const char *, const char *, ...) __attribute__ ((format (printf, 3, 4)));
extern void ufs_panic (struct super_block *, const char *, const char *, ...) __attribute__ ((format (printf, 3, 4)));
extern void ufs_write_super (struct super_block *);


extern struct inode_operations ufs_fast_symlink_inode_operations;


extern void ufs_truncate (struct inode *);
# 18 "/opt/Wive/kernel/include/linux/ufs_fs_sb.h" 2





struct ufs_buffer_head {
        unsigned fragment;
        unsigned count;
        struct buffer_head * bh[(8192 / 1024)];
};

struct ufs_cg_private_info {
        struct ufs_cylinder_group ucg;
        __u32 c_cgx;
        __u16 c_ncyl;
        __u16 c_niblk;
        __u32 c_ndblk;
        __u32 c_rotor;
        __u32 c_frotor;
        __u32 c_irotor;
        __u32 c_btotoff;
        __u32 c_boff;
        __u32 c_iusedoff;
        __u32 c_freeoff;
        __u32 c_nextfreeoff;
        __u32 c_clustersumoff;
        __u32 c_clusteroff;
        __u32 c_nclusterblks;
};

struct ufs_sb_private_info {
        struct ufs_buffer_head s_ubh;
        __u32 s_sblkno;
        __u32 s_cblkno;
        __u32 s_iblkno;
        __u32 s_dblkno;
        __u32 s_cgoffset;
        __u32 s_cgmask;
        __u32 s_size;
        __u32 s_dsize;
        __u32 s_ncg;
        __u32 s_bsize;
        __u32 s_fsize;
        __u32 s_fpb;
        __u32 s_minfree;
        __u32 s_bmask;
        __u32 s_fmask;
        __u32 s_bshift;
        __u32 s_fshift;
        __u32 s_fpbshift;
        __u32 s_fsbtodb;
        __u32 s_sbsize;
        __u32 s_csmask;
        __u32 s_csshift;
        __u32 s_nindir;
        __u32 s_inopb;
        __u32 s_nspf;
        __u32 s_npsect;
        __u32 s_interleave;
        __u32 s_trackskew;
        __u32 s_csaddr;
        __u32 s_cssize;
        __u32 s_cgsize;
        __u32 s_ntrak;
        __u32 s_nsect;
        __u32 s_spc;
        __u32 s_ipg;
        __u32 s_fpg;
        __u32 s_cpc;
        __s32 s_contigsumsize;
        __s64 s_qbmask;
        __s64 s_qfmask;
        __s32 s_postblformat;
        __s32 s_nrpos;
        __s32 s_postbloff;
        __s32 s_rotbloff;

        __u32 s_fpbmask;
        __u32 s_apb;
        __u32 s_2apb;
        __u32 s_3apb;
        __u32 s_apbmask;
        __u32 s_apbshift;
        __u32 s_2apbshift;
        __u32 s_3apbshift;
        __u32 s_nspfshift;
        __u32 s_nspb;
        __u32 s_inopf;
        __u32 s_sbbase;
        __u32 s_bpf;
        __u32 s_bpfshift;
        __u32 s_bpfmask;

        __u32 s_maxsymlinklen;
};





struct ufs_sb_info {
        struct ufs_sb_private_info * s_uspi;
        struct ufs_csum * s_csp[31];
        unsigned s_bytesex;
        unsigned s_flags;
        struct buffer_head ** s_ucg;
        struct ufs_cg_private_info * s_ucpi[8];
        unsigned s_cgno[8];
        unsigned short s_cg_loaded;
        unsigned s_mount_opt;
};







struct ufs_super_block_first {
        __u32 fs_link;
        __u32 fs_rlink;
        __u32 fs_sblkno;
        __u32 fs_cblkno;
        __u32 fs_iblkno;
        __u32 fs_dblkno;
        __u32 fs_cgoffset;
        __u32 fs_cgmask;
        __u32 fs_time;
        __u32 fs_size;
        __u32 fs_dsize;
        __u32 fs_ncg;
        __u32 fs_bsize;
        __u32 fs_fsize;
        __u32 fs_frag;
        __u32 fs_minfree;
        __u32 fs_rotdelay;
        __u32 fs_rps;
        __u32 fs_bmask;
        __u32 fs_fmask;
        __u32 fs_bshift;
        __u32 fs_fshift;
        __u32 fs_maxcontig;
        __u32 fs_maxbpg;
        __u32 fs_fragshift;
        __u32 fs_fsbtodb;
        __u32 fs_sbsize;
        __u32 fs_csmask;
        __u32 fs_csshift;
        __u32 fs_nindir;
        __u32 fs_inopb;
        __u32 fs_nspf;
        __u32 fs_optim;
        union {
                struct {
                        __u32 fs_npsect;
                } fs_sun;
                struct {
                        __s32 fs_state;
                } fs_sunx86;
        } fs_u1;
        __u32 fs_interleave;
        __u32 fs_trackskew;
        __u32 fs_id[2];
        __u32 fs_csaddr;
        __u32 fs_cssize;
        __u32 fs_cgsize;
        __u32 fs_ntrak;
        __u32 fs_nsect;
        __u32 fs_spc;
        __u32 fs_ncyl;
        __u32 fs_cpg;
        __u32 fs_ipg;
        __u32 fs_fpg;
        struct ufs_csum fs_cstotal;
        __s8 fs_fmod;
        __s8 fs_clean;
        __s8 fs_ronly;
        __s8 fs_flags;
        __s8 fs_fsmnt[512 - 212];

};

struct ufs_super_block_second {
        __s8 fs_fsmnt[212];
        __u32 fs_cgrotor;
        __u32 fs_csp[31];
        __u32 fs_maxcluster;
        __u32 fs_cpc;
        __u16 fs_opostbl[82];
};

struct ufs_super_block_third {
        __u16 fs_opostbl[46];
        union {
                struct {
                        __s32 fs_sparecon[53];
                        __s32 fs_reclaim;
                        __s32 fs_sparecon2[1];
                        __s32 fs_state;
                        __u32 fs_qbmask[2];
                        __u32 fs_qfmask[2];
                } fs_sun;
                struct {
                        __s32 fs_sparecon[53];
                        __s32 fs_reclaim;
                        __s32 fs_sparecon2[1];
                        __u32 fs_npsect;
                        __u32 fs_qbmask[2];
                        __u32 fs_qfmask[2];
                } fs_sunx86;
                struct {
                        __s32 fs_sparecon[50];
                        __s32 fs_contigsumsize;
                        __s32 fs_maxsymlinklen;
                        __s32 fs_inodefmt;
                        __u32 fs_maxfilesize[2];
                        __u32 fs_qbmask[2];
                        __u32 fs_qfmask[2];
                        __s32 fs_state;
                } fs_44;
        } fs_u2;
        __s32 fs_postblformat;
        __s32 fs_nrpos;
        __s32 fs_postbloff;
        __s32 fs_rotbloff;
        __s32 fs_magic;
        __u8 fs_space[1];
};
# 684 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/efs_fs_sb.h" 1
# 25 "/opt/Wive/kernel/include/linux/efs_fs_sb.h"
struct efs_super {
        int32_t fs_size;
        int32_t fs_firstcg;
        int32_t fs_cgfsize;
        short fs_cgisize;
        short fs_sectors;
        short fs_heads;
        short fs_ncg;
        short fs_dirty;
        int32_t fs_time;
        int32_t fs_magic;
        char fs_fname[6];
        char fs_fpack[6];
        int32_t fs_bmsize;
        int32_t fs_tfree;
        int32_t fs_tinode;
        int32_t fs_bmblock;
        int32_t fs_replsb;
        int32_t fs_lastialloc;
        char fs_spare[20];
        int32_t fs_checksum;
};


struct efs_sb_info {
        int32_t fs_magic;
        int32_t fs_start;
        int32_t first_block;
        int32_t total_blocks;
        int32_t group_size;
        int32_t data_free;
        int32_t inode_free;
        short inode_blocks;
        short total_groups;
};
# 685 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/romfs_fs_sb.h" 1





struct romfs_sb_info {
        unsigned long s_maxsize;
};
# 686 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/smb_fs_sb.h" 1
# 15 "/opt/Wive/kernel/include/linux/smb_fs_sb.h"
# 1 "/opt/Wive/kernel/include/linux/smb.h" 1
# 14 "/opt/Wive/kernel/include/linux/smb.h"
enum smb_protocol {
        SMB_PROTOCOL_NONE,
        SMB_PROTOCOL_CORE,
        SMB_PROTOCOL_COREPLUS,
        SMB_PROTOCOL_LANMAN1,
        SMB_PROTOCOL_LANMAN2,
        SMB_PROTOCOL_NT1
};

enum smb_case_hndl {
        SMB_CASE_DEFAULT,
        SMB_CASE_LOWER,
        SMB_CASE_UPPER
};

struct smb_dskattr {
        __u16 total;
        __u16 allocblocks;
        __u16 blocksize;
        __u16 free;
};

struct smb_conn_opt {


        unsigned int fd;

        enum smb_protocol protocol;
        enum smb_case_hndl case_handling;



        __u32 max_xmit;
        __u16 server_uid;
        __u16 tid;


        __u16 secmode;
        __u16 maxmux;
        __u16 maxvcs;
        __u16 rawmode;
        __u32 sesskey;


        __u32 maxraw;
        __u32 capabilities;
        __s16 serverzone;
};




struct smb_nls_codepage {
        char local_name[20];
        char remote_name[20];
};
# 78 "/opt/Wive/kernel/include/linux/smb.h"
struct smb_fattr {

        __u16 attr;

        unsigned long f_ino;
        umode_t f_mode;
        nlink_t f_nlink;
        uid_t f_uid;
        gid_t f_gid;
        kdev_t f_rdev;
        off_t f_size;
        time_t f_atime;
        time_t f_mtime;
        time_t f_ctime;
        unsigned long f_blksize;
        unsigned long f_blocks;
};

enum smb_conn_state {
        CONN_VALID,
        CONN_INVALID,

        CONN_RETRIED,
        CONN_RETRYING
};
# 16 "/opt/Wive/kernel/include/linux/smb_fs_sb.h" 2







struct smb_sb_info {
        enum smb_conn_state state;
        struct file * sock_file;

        struct smb_mount_data_kernel *mnt;
        unsigned char *temp_buf;




        unsigned int generation;
        pid_t conn_pid;
        struct smb_conn_opt opt;

        struct semaphore sem;
        wait_queue_head_t wait;

        __u32 packet_size;
        unsigned char * packet;
        unsigned short rcls;
        unsigned short err;


        void *data_ready;


        struct nls_table *remote_nls;
        struct nls_table *local_nls;




        char *name_buf;

        int (*convert)(char *, int, const char *, int,
                       struct nls_table *, struct nls_table *);
};


static inline void
smb_lock_server(struct smb_sb_info *server)
{
        down(&(server->sem));
}

static inline void
smb_unlock_server(struct smb_sb_info *server)
{
        up(&(server->sem));
}
# 687 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/hfs_fs_sb.h" 1
# 15 "/opt/Wive/kernel/include/linux/hfs_fs_sb.h"
struct hfs_name;

typedef int (*hfs_namein_fn) (char *, const struct hfs_name *);
typedef void (*hfs_nameout_fn) (struct hfs_name *, const char *, int);
typedef void (*hfs_ifill_fn) (struct inode *, ino_t, const int);






struct hfs_sb_info {
        int magic;
        struct hfs_mdb *s_mdb;
        int s_quiet;

        int s_lowercase;
        int s_afpd;
        int s_version;
        hfs_namein_fn s_namein;


        hfs_nameout_fn s_nameout;


        hfs_ifill_fn s_ifill;

        const struct hfs_name *s_reserved1;
        const struct hfs_name *s_reserved2;
        __u32 s_type;
        __u32 s_creator;
        umode_t s_umask;

        uid_t s_uid;
        gid_t s_gid;
        char s_conv;
};
# 688 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/adfs_fs_sb.h" 1
# 13 "/opt/Wive/kernel/include/linux/adfs_fs_sb.h"
struct adfs_discmap;
struct adfs_dir_ops;




struct adfs_sb_info {
        struct adfs_discmap *s_map;
        struct adfs_dir_ops *s_dir;

        uid_t s_uid;
        gid_t s_gid;
        umode_t s_owner_mask;
        umode_t s_other_mask;

        __u32 s_ids_per_zone;
        __u32 s_idlen;
        __u32 s_map_size;
        unsigned long s_size;
        signed int s_map2blk;
        unsigned int s_log2sharesize;
        unsigned int s_version;
        unsigned int s_namelen;
};
# 689 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/qnx4_fs_sb.h" 1
# 14 "/opt/Wive/kernel/include/linux/qnx4_fs_sb.h"
# 1 "/opt/Wive/kernel/include/linux/qnx4_fs.h" 1
# 45 "/opt/Wive/kernel/include/linux/qnx4_fs.h"
struct qnx4_inode_entry {
        char di_fname[16];
        qnx4_off_t di_size;
        qnx4_xtnt_t di_first_xtnt;
        __u32 di_xblk;
        __s32 di_ftime;
        __s32 di_mtime;
        __s32 di_atime;
        __s32 di_ctime;
        qnx4_nxtnt_t di_num_xtnts;
        qnx4_mode_t di_mode;
        qnx4_muid_t di_uid;
        qnx4_mgid_t di_gid;
        qnx4_nlink_t di_nlink;
        __u8 di_zero[4];
        qnx4_ftype_t di_type;
        __u8 di_status;
};

struct qnx4_link_info {
        char dl_fname[48];
        __u32 dl_inode_blk;
        __u8 dl_inode_ndx;
        __u8 dl_spare[10];
        __u8 dl_status;
};

struct qnx4_xblk {
        __u32 xblk_next_xblk;
        __u32 xblk_prev_xblk;
        __u8 xblk_num_xtnts;
        __u8 xblk_spare[3];
        __s32 xblk_num_blocks;
        qnx4_xtnt_t xblk_xtnts[60];
        char xblk_signature[8];
        qnx4_xtnt_t xblk_first_xtnt;
};

struct qnx4_super_block {
        struct qnx4_inode_entry RootDir;
        struct qnx4_inode_entry Inode;
        struct qnx4_inode_entry Boot;
        struct qnx4_inode_entry AltBoot;
};
# 100 "/opt/Wive/kernel/include/linux/qnx4_fs.h"
extern struct dentry *qnx4_lookup(struct inode *dir, struct dentry *dentry);
extern unsigned long qnx4_count_free_blocks(struct super_block *sb);
extern unsigned long qnx4_block_map(struct inode *inode, long iblock);

extern struct buffer_head *qnx4_getblk(struct inode *, int, int);
extern struct buffer_head *qnx4_bread(struct inode *, int, int);

extern int qnx4_create(struct inode *dir, struct dentry *dentry, int mode);
extern struct inode_operations qnx4_file_inode_operations;
extern struct inode_operations qnx4_dir_inode_operations;
extern struct file_operations qnx4_file_operations;
extern struct file_operations qnx4_dir_operations;
extern int qnx4_is_free(struct super_block *sb, long block);
extern int qnx4_set_bitmap(struct super_block *sb, long block, int busy);
extern int qnx4_create(struct inode *inode, struct dentry *dentry, int mode);
extern void qnx4_truncate(struct inode *inode);
extern void qnx4_free_inode(struct inode *inode);
extern int qnx4_unlink(struct inode *dir, struct dentry *dentry);
extern int qnx4_rmdir(struct inode *dir, struct dentry *dentry);
extern int qnx4_sync_file(struct file *file, struct dentry *dentry, int);
extern int qnx4_sync_inode(struct inode *inode);
extern int qnx4_get_block(struct inode *inode, long iblock, struct buffer_head *bh, int create);
# 15 "/opt/Wive/kernel/include/linux/qnx4_fs_sb.h" 2





struct qnx4_sb_info {
        struct buffer_head *sb_buf;
        struct qnx4_super_block *sb;
        unsigned int Version;
        struct qnx4_inode_entry *BitMap;
};
# 690 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/reiserfs_fs_sb.h" 1







# 1 "/opt/Wive/kernel/include/linux/tqueue.h" 1
# 38 "/opt/Wive/kernel/include/linux/tqueue.h"
struct tq_struct {
        struct list_head list;
        unsigned long sync;
        void (*routine)(void *);
        void *data;
};
# 64 "/opt/Wive/kernel/include/linux/tqueue.h"
typedef struct list_head task_queue;




extern task_queue tq_timer, tq_immediate, tq_disk;
# 94 "/opt/Wive/kernel/include/linux/tqueue.h"
extern spinlock_t tqueue_lock;





extern void __run_task_queue(task_queue *list);

static inline void run_task_queue(task_queue *list)
{
        if ((!list_empty(&*list)))
                __run_task_queue(list);
}
# 9 "/opt/Wive/kernel/include/linux/reiserfs_fs_sb.h" 2
# 26 "/opt/Wive/kernel/include/linux/reiserfs_fs_sb.h"
struct reiserfs_super_block
{
  __u32 s_block_count;
  __u32 s_free_blocks;
  __u32 s_root_block;
  __u32 s_journal_block;
  __u32 s_journal_dev;
# 43 "/opt/Wive/kernel/include/linux/reiserfs_fs_sb.h"
  __u32 s_orig_journal_size;
  __u32 s_journal_trans_max ;
  __u32 s_journal_block_count ;
  __u32 s_journal_max_batch ;
  __u32 s_journal_max_commit_age ;
  __u32 s_journal_max_trans_age ;
  __u16 s_blocksize;
  __u16 s_oid_maxsize;
  __u16 s_oid_cursize;
  __u16 s_state;
  char s_magic[12];
  __u32 s_hash_function_code;
  __u16 s_tree_height;
  __u16 s_bmap_nr;
  __u16 s_version;




  __u16 s_reserved;
  __u32 s_inode_generation;
  char s_unused[124] ;
} __attribute__ ((__packed__));
# 121 "/opt/Wive/kernel/include/linux/reiserfs_fs_sb.h"
struct reiserfs_super_block_v1
{
  __u32 s_block_count;
  __u32 s_free_blocks;
  __u32 s_root_block;
  __u32 s_journal_block;
  __u32 s_journal_dev;
  __u32 s_orig_journal_size;
  __u32 s_journal_trans_max ;
  __u32 s_journal_block_count ;
  __u32 s_journal_max_batch ;
  __u32 s_journal_max_commit_age ;
  __u32 s_journal_max_trans_age ;
  __u16 s_blocksize;
  __u16 s_oid_maxsize;
  __u16 s_oid_cursize;
  __u16 s_state;
  char s_magic[16];
  __u16 s_tree_height;
  __u16 s_bmap_nr;
  __u32 s_reserved;
} __attribute__ ((__packed__));
# 200 "/opt/Wive/kernel/include/linux/reiserfs_fs_sb.h"
struct reiserfs_journal_cnode {
  struct buffer_head *bh ;
  kdev_t dev ;
  unsigned long blocknr ;
  long state ;
  struct reiserfs_journal_list *jlist ;
  struct reiserfs_journal_cnode *next ;
  struct reiserfs_journal_cnode *prev ;
  struct reiserfs_journal_cnode *hprev ;
  struct reiserfs_journal_cnode *hnext ;
};

struct reiserfs_bitmap_node {
  int id ;
  char *data ;
  struct list_head list ;
} ;

struct reiserfs_list_bitmap {
  struct reiserfs_journal_list *journal_list ;
  struct reiserfs_bitmap_node **bitmaps ;
} ;




struct reiserfs_transaction_handle {

  char *t_caller ;
  int t_blocks_logged ;
  int t_blocks_allocated ;
  unsigned long t_trans_id ;
  struct super_block *t_super ;

} ;







struct reiserfs_journal_list {
  unsigned long j_start ;
  unsigned long j_len ;
  atomic_t j_nonzerolen ;
  atomic_t j_commit_left ;
  atomic_t j_flushing ;
  atomic_t j_commit_flushing ;
  atomic_t j_older_commits_done ;
  unsigned long j_trans_id ;
  time_t j_timestamp ;
  struct reiserfs_list_bitmap *j_list_bitmap ;
  struct buffer_head *j_commit_bh ;
  struct reiserfs_journal_cnode *j_realblock ;
  struct reiserfs_journal_cnode *j_freedlist ;
  wait_queue_head_t j_commit_wait ;
  wait_queue_head_t j_flush_wait ;
} ;

struct reiserfs_page_list ;

struct reiserfs_journal {
  struct buffer_head ** j_ap_blocks ;
  struct reiserfs_journal_cnode *j_last ;
  struct reiserfs_journal_cnode *j_first ;

  long j_state ;
  unsigned long j_trans_id ;
  unsigned long j_mount_id ;
  unsigned long j_start ;
  unsigned long j_len ;
  unsigned long j_len_alloc ;
  atomic_t j_wcount ;
  unsigned long j_bcount ;
  unsigned long j_first_unflushed_offset ;
  unsigned long j_last_flush_trans_id ;
  struct buffer_head *j_header_bh ;




  struct reiserfs_page_list *j_flush_pages ;
  time_t j_trans_start_time ;
  wait_queue_head_t j_wait ;
  atomic_t j_wlock ;
  wait_queue_head_t j_join_wait ;
  atomic_t j_jlock ;
  int j_journal_list_index ;
  int j_list_bitmap_index ;
  int j_must_wait ;
  int j_next_full_flush ;
  int j_next_async_flush ;

  int j_cnode_used ;
  int j_cnode_free ;

  struct reiserfs_journal_cnode *j_cnode_free_list ;
  struct reiserfs_journal_cnode *j_cnode_free_orig ;

  int j_free_bitmap_nodes ;
  int j_used_bitmap_nodes ;
  struct list_head j_bitmap_nodes ;
  struct inode j_dummy_inode ;
  struct reiserfs_list_bitmap j_list_bitmap[5] ;
  struct reiserfs_journal_list j_journal_list[64] ;
  struct reiserfs_journal_cnode *j_hash_table[8192] ;
  struct reiserfs_journal_cnode *j_list_hash_table[8192] ;

  struct list_head j_prealloc_list;
};




typedef __u32 (*hashf_t) (const signed char *, int);

struct proc_dir_entry;
# 381 "/opt/Wive/kernel/include/linux/reiserfs_fs_sb.h"
typedef struct reiserfs_proc_info_data
{} reiserfs_proc_info_data_t;



struct reiserfs_sb_info
{
    struct buffer_head * s_sbh;


    struct reiserfs_super_block * s_rs;
    struct buffer_head ** s_ap_bitmap;
    struct reiserfs_journal *s_journal ;
    unsigned short s_mount_state;


    void (*end_io_handler)(struct buffer_head *, int);
    hashf_t s_hash_function;


    unsigned long s_mount_opt;




    wait_queue_head_t s_wait;

    atomic_t s_generation_counter;

    unsigned long s_properties;



    int s_kmallocs;
    int s_disk_reads;
    int s_disk_writes;
    int s_fix_nodes;
    int s_do_balance;
    int s_unneeded_left_neighbor;
    int s_good_search_by_key_reada;
    int s_bmaps;
    int s_bmaps_without_search;
    int s_direct2indirect;
    int s_indirect2direct;



    int s_is_unlinked_ok;
    reiserfs_proc_info_data_t s_proc_info_data;
    struct proc_dir_entry *procdir;
};
# 491 "/opt/Wive/kernel/include/linux/reiserfs_fs_sb.h"
void reiserfs_file_buffer (struct buffer_head * bh, int list);
int reiserfs_is_super(struct super_block *s) ;
int journal_mark_dirty(struct reiserfs_transaction_handle *, struct super_block *, struct buffer_head *bh) ;
int flush_old_commits(struct super_block *s, int) ;
int show_reiserfs_locks(void) ;
int reiserfs_resize(struct super_block *, unsigned long) ;
# 691 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/bfs_fs_sb.h" 1
# 12 "/opt/Wive/kernel/include/linux/bfs_fs_sb.h"
struct bfs_sb_info {
        unsigned long si_blocks;
        unsigned long si_freeb;
        unsigned long si_freei;
        unsigned long si_lf_ioff;
        unsigned long si_lf_sblk;
        unsigned long si_lf_eblk;
        unsigned long si_lasti;
        char * si_imap;
        struct buffer_head * si_sbh;
        struct bfs_super_block * si_bfs_sb;
};
# 692 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/udf_fs_sb.h" 1
# 21 "/opt/Wive/kernel/include/linux/udf_fs_sb.h"
#pragma pack(1)
# 30 "/opt/Wive/kernel/include/linux/udf_fs_sb.h"
struct udf_sparing_data
{
        __u16 s_packet_len;
        struct buffer_head *s_spar_map[4];
};

struct udf_virtual_data
{
        __u32 s_num_entries;
        __u16 s_start_offset;
};

struct udf_bitmap
{
        __u32 s_extLength;
        __u32 s_extPosition;
        __u16 s_nr_groups;
        struct buffer_head **s_block_bitmap;
};

struct udf_part_map
{
        union
        {
                struct udf_bitmap *s_bitmap;
                struct inode *s_table;
        } s_uspace;
        union
        {
                struct udf_bitmap *s_bitmap;
                struct inode *s_table;
        } s_fspace;
        __u32 s_partition_root;
        __u32 s_partition_len;
        __u16 s_partition_type;
        __u16 s_partition_num;
        union
        {
                struct udf_sparing_data s_sparing;
                struct udf_virtual_data s_virtual;
        } s_type_specific;
        __u32 (*s_partition_func)(struct super_block *, __u32, __u16, __u32);
        __u16 s_volumeseqnum;
        __u16 s_partition_flags;
};

#pragma pack()

struct udf_sb_info
{
        struct udf_part_map *s_partmaps;
        __u8 s_volident[32];


        __u16 s_partitions;
        __u16 s_partition;


        __s32 s_session;
        __u32 s_anchor[4];
        __u32 s_lastblock;

        struct buffer_head *s_lvidbh;


        mode_t s_umask;
        gid_t s_gid;
        uid_t s_uid;


        time_t s_recordtime;


        __u16 s_serialnum;


        __u16 s_udfrev;


        __u32 s_flags;


        struct nls_table *s_nls_map;


        struct inode *s_vat;
};
# 693 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/ncp_fs_sb.h" 1
# 12 "/opt/Wive/kernel/include/linux/ncp_fs_sb.h"
# 1 "/opt/Wive/kernel/include/linux/ncp_mount.h" 1
# 12 "/opt/Wive/kernel/include/linux/ncp_mount.h"
# 1 "/opt/Wive/kernel/include/linux/ncp.h" 1
# 22 "/opt/Wive/kernel/include/linux/ncp.h"
struct ncp_request_header {
        __u16 type __attribute__((packed));
        __u8 sequence __attribute__((packed));
        __u8 conn_low __attribute__((packed));
        __u8 task __attribute__((packed));
        __u8 conn_high __attribute__((packed));
        __u8 function __attribute__((packed));
        __u8 data[0] __attribute__((packed));
};




struct ncp_reply_header {
        __u16 type __attribute__((packed));
        __u8 sequence __attribute__((packed));
        __u8 conn_low __attribute__((packed));
        __u8 task __attribute__((packed));
        __u8 conn_high __attribute__((packed));
        __u8 completion_code __attribute__((packed));
        __u8 connection_state __attribute__((packed));
        __u8 data[0] __attribute__((packed));
};



struct ncp_volume_info {
        __u32 total_blocks;
        __u32 free_blocks;
        __u32 purgeable_blocks;
        __u32 not_yet_purgeable_blocks;
        __u32 total_dir_entries;
        __u32 available_dir_entries;
        __u8 sectors_per_block;
        char volume_name[(16) + 1];
};
# 112 "/opt/Wive/kernel/include/linux/ncp.h"
struct nw_info_struct {
        __u32 spaceAlloc __attribute__((packed));
        __u32 attributes __attribute__((packed));
        __u16 flags __attribute__((packed));
        __u32 dataStreamSize __attribute__((packed));
        __u32 totalStreamSize __attribute__((packed));
        __u16 numberOfStreams __attribute__((packed));
        __u16 creationTime __attribute__((packed));
        __u16 creationDate __attribute__((packed));
        __u32 creatorID __attribute__((packed));
        __u16 modifyTime __attribute__((packed));
        __u16 modifyDate __attribute__((packed));
        __u32 modifierID __attribute__((packed));
        __u16 lastAccessDate __attribute__((packed));
        __u16 archiveTime __attribute__((packed));
        __u16 archiveDate __attribute__((packed));
        __u32 archiverID __attribute__((packed));
        __u16 inheritedRightsMask __attribute__((packed));
        __u32 dirEntNum __attribute__((packed));
        __u32 DosDirNum __attribute__((packed));
        __u32 volNumber __attribute__((packed));
        __u32 EADataSize __attribute__((packed));
        __u32 EAKeyCount __attribute__((packed));
        __u32 EAKeySize __attribute__((packed));
        __u32 NSCreator __attribute__((packed));
        __u8 nameLen __attribute__((packed));
        __u8 entryName[256] __attribute__((packed));
};
# 156 "/opt/Wive/kernel/include/linux/ncp.h"
struct nw_modify_dos_info {
        __u32 attributes __attribute__((packed));
        __u16 creationDate __attribute__((packed));
        __u16 creationTime __attribute__((packed));
        __u32 creatorID __attribute__((packed));
        __u16 modifyDate __attribute__((packed));
        __u16 modifyTime __attribute__((packed));
        __u32 modifierID __attribute__((packed));
        __u16 archiveDate __attribute__((packed));
        __u16 archiveTime __attribute__((packed));
        __u32 archiverID __attribute__((packed));
        __u16 lastAccessDate __attribute__((packed));
        __u16 inheritanceGrantMask __attribute__((packed));
        __u16 inheritanceRevokeMask __attribute__((packed));
        __u32 maximumSpace __attribute__((packed));
};

struct nw_search_sequence {
        __u8 volNumber __attribute__((packed));
        __u32 dirBase __attribute__((packed));
        __u32 sequence __attribute__((packed));
};
# 13 "/opt/Wive/kernel/include/linux/ncp_mount.h" 2
# 25 "/opt/Wive/kernel/include/linux/ncp_mount.h"
struct ncp_mount_data {
        int version;
        unsigned int ncp_fd;
        __kernel_uid_t mounted_uid;
        __kernel_pid_t wdog_pid;

        unsigned char mounted_vol[(16) + 1];
        unsigned int time_out;

        unsigned int retry_count;
        unsigned int flags;

        __kernel_uid_t uid;
        __kernel_gid_t gid;
        __kernel_mode_t file_mode;
        __kernel_mode_t dir_mode;
};



struct ncp_mount_data_v4 {
        int version;
        unsigned long flags;


        unsigned long mounted_uid;

        long wdog_pid;

        unsigned int ncp_fd;
        unsigned int time_out;

        unsigned int retry_count;



        unsigned long uid;
        unsigned long gid;

        unsigned long file_mode;
        unsigned long dir_mode;
};



struct ncp_mount_data_kernel {
        unsigned long flags;
        unsigned int int_flags;

        __kernel_uid32_t mounted_uid;
        __kernel_pid_t wdog_pid;
        unsigned int ncp_fd;
        unsigned int time_out;

        unsigned int retry_count;
        unsigned char mounted_vol[(16) + 1];
        __kernel_uid32_t uid;
        __kernel_gid32_t gid;
        __kernel_mode_t file_mode;
        __kernel_mode_t dir_mode;
};
# 13 "/opt/Wive/kernel/include/linux/ncp_fs_sb.h" 2





struct ncp_server {

        struct ncp_mount_data_kernel m;



        __u8 name_space[(64) + 2];

        struct file *ncp_filp;

        u8 sequence;
        u8 task;
        u16 connection;

        u8 completion;
        u8 conn_status;



        int buffer_size;

        int reply_size;

        int packet_size;
        unsigned char *packet;


        int lock;
        struct semaphore sem;

        int current_size;
        int has_subfunction;
        int ncp_reply_size;

        int root_setuped;


        int sign_wanted;
        int sign_active;
        char sign_root[8];
        char sign_last[16];


        struct {
                int auth_type;
                size_t object_name_len;
                void* object_name;
                int object_type;
        } auth;

        struct {
                size_t len;
                void* data;
        } priv;


        struct nls_table *nls_vol;
        struct nls_table *nls_io;


        int dentry_ttl;


        unsigned int flags;
};
# 92 "/opt/Wive/kernel/include/linux/ncp_fs_sb.h"
static inline int ncp_conn_valid(struct ncp_server *server)
{
        return ((server->conn_status & 0x11) == 0);
}

static inline void ncp_invalidate_conn(struct ncp_server *server)
{
        server->conn_status |= 0x01;
}
# 694 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/usbdev_fs_sb.h" 1
struct usbdev_sb_info {
        struct list_head slist;
        struct list_head ilist;
        uid_t devuid;
        gid_t devgid;
        umode_t devmode;
        uid_t busuid;
        gid_t busgid;
        umode_t busmode;
        uid_t listuid;
        gid_t listgid;
        umode_t listmode;
};
# 695 "/opt/Wive/kernel/include/linux/fs.h" 2

# 1 "/opt/Wive/kernel/include/linux/jffs2_fs_sb.h" 1







# 1 "/opt/Wive/kernel/include/linux/completion.h" 1
# 13 "/opt/Wive/kernel/include/linux/completion.h"
struct completion {
        unsigned int done;
        wait_queue_head_t wait;
};







static inline void init_completion(struct completion *x)
{
        x->done = 0;
        init_waitqueue_head(&x->wait);
}

extern void wait_for_completion(struct completion *);
extern void complete(struct completion *);
# 9 "/opt/Wive/kernel/include/linux/jffs2_fs_sb.h" 2
# 20 "/opt/Wive/kernel/include/linux/jffs2_fs_sb.h"
struct jffs2_sb_info {
        struct mtd_info *mtd;

        __u32 highest_ino;
        unsigned int flags;
        spinlock_t nodelist_lock;


        struct task_struct *gc_task;
        struct semaphore gc_thread_start;
        struct completion gc_thread_exit;



        struct semaphore alloc_sem;




        __u32 flash_size;
        __u32 used_size;
        __u32 dirty_size;
        __u32 free_size;
        __u32 erasing_size;
        __u32 bad_size;
        __u32 sector_size;



        __u32 nr_free_blocks;
        __u32 nr_erasing_blocks;

        __u32 nr_blocks;
        struct jffs2_eraseblock *blocks;

        struct jffs2_eraseblock *nextblock;

        struct jffs2_eraseblock *gcblock;

        struct list_head clean_list;
        struct list_head dirty_list;
        struct list_head erasing_list;
        struct list_head erase_pending_list;
        struct list_head erase_complete_list;
        struct list_head free_list;
        struct list_head bad_list;
        struct list_head bad_used_list;

        spinlock_t erase_completion_lock;

        wait_queue_head_t erase_wait;
        struct jffs2_inode_cache *inocache_list[1];
        spinlock_t inocache_lock;
};
# 697 "/opt/Wive/kernel/include/linux/fs.h" 2
# 1 "/opt/Wive/kernel/include/linux/squashfs_fs_sb.h" 1
# 25 "/opt/Wive/kernel/include/linux/squashfs_fs_sb.h"
# 1 "/opt/Wive/kernel/include/linux/squashfs_fs.h" 1
# 112 "/opt/Wive/kernel/include/linux/squashfs_fs.h"
typedef unsigned int squashfs_fragment_index;
# 132 "/opt/Wive/kernel/include/linux/squashfs_fs.h"
typedef unsigned int squashfs_block;
typedef long long squashfs_inode;

typedef unsigned int squashfs_uid;

typedef struct squashfs_super_block {
        unsigned int s_magic;
        unsigned int inodes;
        unsigned int bytes_used;
        unsigned int uid_start;
        unsigned int guid_start;
        unsigned int inode_table_start;
        unsigned int directory_table_start;
        unsigned int s_major:16;
        unsigned int s_minor:16;
        unsigned int block_size_1:16;
        unsigned int block_log:16;
        unsigned int flags:8;
        unsigned int no_uids:8;
        unsigned int no_guids:8;
        unsigned int mkfs_time ;
        squashfs_inode root_inode;
        unsigned int block_size;
        unsigned int fragments;
        unsigned int fragment_table_start;
} __attribute__ ((packed)) squashfs_super_block;

typedef struct {
        unsigned int inode_type:4;
        unsigned int mode:12;
        unsigned int uid:8;
        unsigned int guid:8;
} __attribute__ ((packed)) squashfs_base_inode_header;

typedef squashfs_base_inode_header squashfs_ipc_inode_header;

typedef struct {
        unsigned int inode_type:4;
        unsigned int mode:12;
        unsigned int uid:8;
        unsigned int guid:8;
        unsigned short rdev;
} __attribute__ ((packed)) squashfs_dev_inode_header;

typedef struct {
        unsigned int inode_type:4;
        unsigned int mode:12;
        unsigned int uid:8;
        unsigned int guid:8;
        unsigned short symlink_size;
        char symlink[0];
} __attribute__ ((packed)) squashfs_symlink_inode_header;

typedef struct {
        unsigned int inode_type:4;
        unsigned int mode:12;
        unsigned int uid:8;
        unsigned int guid:8;
        unsigned int mtime;
        squashfs_block start_block;
        unsigned int fragment;
        unsigned int offset;
        unsigned int file_size:32;
        unsigned short block_list[0];
} __attribute__ ((packed)) squashfs_reg_inode_header;

typedef struct {
        unsigned int inode_type:4;
        unsigned int mode:12;
        unsigned int uid:8;
        unsigned int guid:8;
        unsigned int file_size:19;
        unsigned int offset:13;
        unsigned int mtime;
        unsigned int start_block:24;
} __attribute__ ((packed)) squashfs_dir_inode_header;

typedef union {
        squashfs_base_inode_header base;
        squashfs_dev_inode_header dev;
        squashfs_symlink_inode_header symlink;
        squashfs_reg_inode_header reg;
        squashfs_dir_inode_header dir;
        squashfs_ipc_inode_header ipc;
} squashfs_inode_header;

typedef struct {
        unsigned int offset:13;
        unsigned int type:3;
        unsigned int size:8;
        char name[0];
} __attribute__ ((packed)) squashfs_dir_entry;

typedef struct {
        unsigned int count:8;
        unsigned int start_block:24;
} __attribute__ ((packed)) squashfs_dir_header;


typedef struct {
        unsigned int start_block;
        unsigned int size;
} __attribute__ ((packed)) squashfs_fragment_entry;

extern int squashfs_uncompress_block(void *d, int dstlen, void *s, int srclen);
extern int squashfs_uncompress_init(void);
extern int squashfs_uncompress_exit(void);
# 26 "/opt/Wive/kernel/include/linux/squashfs_fs_sb.h" 2

typedef struct {
        unsigned int block;
        int length;
        unsigned int next_index;
        char *data;
        } squashfs_cache;

struct squashfs_fragment_cache {
        unsigned int block;
        int length;
        unsigned int locked;
        char *data;
        };

typedef struct squashfs_sb_info {
        squashfs_super_block sBlk;
        int devblksize;
        int devblksize_log2;
        int swap;
        squashfs_cache *block_cache;
        struct squashfs_fragment_cache *fragment;
        int next_cache;
        int next_fragment;
        squashfs_uid *uid;
        squashfs_uid *guid;
        squashfs_fragment_index *fragment_index;
        unsigned int read_size;
        char *read_data;
        char *read_page;
        struct semaphore read_page_mutex;
        struct semaphore block_cache_mutex;
        struct semaphore fragment_mutex;
        wait_queue_head_t waitq;
        wait_queue_head_t fragment_wait_queue;
        struct inode *(*iget)(struct super_block *s, squashfs_inode inode);
        unsigned int (*read_blocklist)(struct inode *inode, int index, int readahead_blks,
                                        char *block_list, char **block_p, unsigned int *bsize);
        } squashfs_sb_info;
# 698 "/opt/Wive/kernel/include/linux/fs.h" 2

extern struct list_head super_blocks;
extern spinlock_t sb_lock;



struct super_block {
        struct list_head s_list;
        kdev_t s_dev;
        unsigned long s_blocksize;
        unsigned char s_blocksize_bits;
        unsigned char s_dirt;
        unsigned long long s_maxbytes;
        struct file_system_type *s_type;
        struct super_operations *s_op;
        struct dquot_operations *dq_op;
        unsigned long s_flags;
        unsigned long s_magic;
        struct dentry *s_root;
        struct rw_semaphore s_umount;
        struct semaphore s_lock;
        int s_count;
        atomic_t s_active;

        struct list_head s_dirty;
        struct list_head s_locked_inodes;
        struct list_head s_files;

        struct block_device *s_bdev;
        struct list_head s_instances;
        struct quota_mount_options s_dquot;

        union {
                struct minix_sb_info minix_sb;
                struct ext2_sb_info ext2_sb;
                struct ext3_sb_info ext3_sb;
                struct hpfs_sb_info hpfs_sb;
                struct ntfs_sb_info ntfs_sb;
                struct msdos_sb_info msdos_sb;
                struct isofs_sb_info isofs_sb;
                struct nfs_sb_info nfs_sb;
                struct sysv_sb_info sysv_sb;
                struct affs_sb_info affs_sb;
                struct ufs_sb_info ufs_sb;
                struct efs_sb_info efs_sb;
                struct shmem_sb_info shmem_sb;
                struct romfs_sb_info romfs_sb;
                struct smb_sb_info smbfs_sb;
                struct hfs_sb_info hfs_sb;
                struct adfs_sb_info adfs_sb;
                struct qnx4_sb_info qnx4_sb;
                struct reiserfs_sb_info reiserfs_sb;
                struct bfs_sb_info bfs_sb;
                struct udf_sb_info udf_sb;
                struct ncp_server ncpfs_sb;
                struct usbdev_sb_info usbdevfs_sb;
                struct jffs2_sb_info jffs2_sb;
                struct cramfs_sb_info cramfs_sb;
                struct squashfs_sb_info squashfs_sb;
                void *generic_sbp;
        } u;




        struct semaphore s_vfs_rename_sem;
# 772 "/opt/Wive/kernel/include/linux/fs.h"
        struct semaphore s_nfsd_free_path_sem;
};




extern int vfs_create(struct inode *, struct dentry *, int);
extern int vfs_mkdir(struct inode *, struct dentry *, int);
extern int vfs_mknod(struct inode *, struct dentry *, int, dev_t);
extern int vfs_symlink(struct inode *, struct dentry *, const char *);
extern int vfs_link(struct dentry *, struct inode *, struct dentry *);
extern int vfs_rmdir(struct inode *, struct dentry *);
extern int vfs_unlink(struct inode *, struct dentry *);
extern int vfs_rename(struct inode *, struct dentry *, struct inode *, struct dentry *);
# 806 "/opt/Wive/kernel/include/linux/fs.h"
typedef int (*filldir_t)(void *, const char *, int, loff_t, ino_t, unsigned);

struct block_device_operations {
        int (*open) (struct inode *, struct file *);
        int (*release) (struct inode *, struct file *);
        int (*ioctl) (struct inode *, struct file *, unsigned, unsigned long);
        int (*check_media_change) (kdev_t);
        int (*revalidate) (kdev_t);
        struct module *owner;
};






struct file_operations {
        struct module *owner;
        loff_t (*llseek) (struct file *, loff_t, int);
        ssize_t (*read) (struct file *, char *, size_t, loff_t *);
        ssize_t (*write) (struct file *, const char *, size_t, loff_t *);
        int (*readdir) (struct file *, void *, filldir_t);
        unsigned int (*poll) (struct file *, struct poll_table_struct *);
        int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);
        int (*mmap) (struct file *, struct vm_area_struct *);
        int (*open) (struct inode *, struct file *);
        int (*flush) (struct file *);
        int (*release) (struct inode *, struct file *);
        int (*fsync) (struct file *, struct dentry *, int datasync);
        int (*fasync) (int, struct file *, int);
        int (*lock) (struct file *, int, struct file_lock *);
        ssize_t (*readv) (struct file *, const struct iovec *, unsigned long, loff_t *);
        ssize_t (*writev) (struct file *, const struct iovec *, unsigned long, loff_t *);
        ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
        unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
};

struct inode_operations {
        int (*create) (struct inode *,struct dentry *,int);
        struct dentry * (*lookup) (struct inode *,struct dentry *);
        int (*link) (struct dentry *,struct inode *,struct dentry *);
        int (*unlink) (struct inode *,struct dentry *);
        int (*symlink) (struct inode *,struct dentry *,const char *);
        int (*mkdir) (struct inode *,struct dentry *,int);
        int (*rmdir) (struct inode *,struct dentry *);
        int (*mknod) (struct inode *,struct dentry *,int,int);
        int (*rename) (struct inode *, struct dentry *,
                        struct inode *, struct dentry *);
        int (*readlink) (struct dentry *, char *,int);
        int (*follow_link) (struct dentry *, struct nameidata *);
        void (*truncate) (struct inode *);
        int (*permission) (struct inode *, int);
        int (*revalidate) (struct dentry *);
        int (*setattr) (struct dentry *, struct iattr *);
        int (*getattr) (struct dentry *, struct iattr *);
};

struct seq_file;





struct super_operations {
        void (*read_inode) (struct inode *);







        void (*read_inode2) (struct inode *, void *) ;
        void (*dirty_inode) (struct inode *);
        void (*write_inode) (struct inode *, int);
        void (*put_inode) (struct inode *);
        void (*delete_inode) (struct inode *);
        void (*put_super) (struct super_block *);
        void (*write_super) (struct super_block *);
        void (*write_super_lockfs) (struct super_block *);
        void (*unlockfs) (struct super_block *);
        int (*statfs) (struct super_block *, struct statfs *);
        int (*remount_fs) (struct super_block *, int *, char *);
        void (*clear_inode) (struct inode *);
        void (*umount_begin) (struct super_block *);
# 914 "/opt/Wive/kernel/include/linux/fs.h"
        struct dentry * (*fh_to_dentry)(struct super_block *sb, __u32 *fh, int len, int fhtype, int parent);
        int (*dentry_to_fh)(struct dentry *, __u32 *fh, int *lenp, int need_parent);
        int (*show_options)(struct seq_file *, struct vfsmount *);
};
# 929 "/opt/Wive/kernel/include/linux/fs.h"
extern void __mark_inode_dirty(struct inode *, int);
static inline void mark_inode_dirty(struct inode *inode)
{
        __mark_inode_dirty(inode, (1 | 2 | 4));
}

static inline void mark_inode_dirty_sync(struct inode *inode)
{
        __mark_inode_dirty(inode, 1);
}

static inline void mark_inode_dirty_pages(struct inode *inode)
{
        __mark_inode_dirty(inode, 4);
}

struct dquot_operations {
        void (*initialize) (struct inode *, short);
        void (*drop) (struct inode *);
        int (*alloc_block) (struct inode *, unsigned long, char);
        int (*alloc_inode) (const struct inode *, unsigned long);
        void (*free_block) (struct inode *, unsigned long);
        void (*free_inode) (const struct inode *, unsigned long);
        int (*transfer) (struct inode *, struct iattr *);
};

struct file_system_type {
        const char *name;
        int fs_flags;
        struct super_block *(*read_super) (struct super_block *, void *, int);
        struct module *owner;
        struct file_system_type * next;
        struct list_head fs_supers;
};
# 987 "/opt/Wive/kernel/include/linux/fs.h"
extern int register_filesystem(struct file_system_type *);
extern int unregister_filesystem(struct file_system_type *);
extern struct vfsmount *kern_mount(struct file_system_type *);
extern int may_umount(struct vfsmount *);
extern long do_mount(char *, char *, char *, unsigned long, void *);



extern int vfs_statfs(struct super_block *, struct statfs *);
# 1005 "/opt/Wive/kernel/include/linux/fs.h"
extern int locks_mandatory_locked(struct inode *);
extern int locks_mandatory_area(int, struct inode *, struct file *, loff_t, size_t);
# 1015 "/opt/Wive/kernel/include/linux/fs.h"
static inline int locks_verify_locked(struct inode *inode)
{
        if ((((inode)->i_sb->s_flags & (64)) && ((inode)->i_mode & (0002000 | 00010)) == 0002000))
                return locks_mandatory_locked(inode);
        return 0;
}

static inline int locks_verify_area(int read_write, struct inode *inode,
                                    struct file *filp, loff_t offset,
                                    size_t count)
{
        if (inode->i_flock && (((inode)->i_sb->s_flags & (64)) && ((inode)->i_mode & (0002000 | 00010)) == 0002000))
                return locks_mandatory_area(read_write, inode, filp, offset, count);
        return 0;
}

static inline int locks_verify_truncate(struct inode *inode,
                                    struct file *filp,
                                    loff_t size)
{
        if (inode->i_flock && (((inode)->i_sb->s_flags & (64)) && ((inode)->i_mode & (0002000 | 00010)) == 0002000))
                return locks_mandatory_area(
                        2, inode, filp,
                        size < inode->i_size ? size : inode->i_size,
                        (size < inode->i_size ? inode->i_size - size
                         : size - inode->i_size)
                );
        return 0;
}

static inline int get_lease(struct inode *inode, unsigned int mode)
{
        if (inode->i_flock && (inode->i_flock->fl_flags & 32))
                return __get_lease(inode, mode);
        return 0;
}



 long sys_open(const char *, int, int);
 long sys_close(unsigned int);
extern int do_truncate(struct dentry *, loff_t start);

extern struct file *filp_open(const char *, int, int);
extern struct file * dentry_open(struct dentry *, struct vfsmount *, int);
extern int filp_close(struct file *, fl_owner_t id);
extern char * getname(const char *);


extern void vfs_caches_init(unsigned long);




enum {BDEV_FILE, BDEV_SWAP, BDEV_FS, BDEV_RAW};
extern int register_blkdev(unsigned int, const char *, struct block_device_operations *);
extern int unregister_blkdev(unsigned int, const char *);
extern struct block_device *bdget(dev_t);
extern int bd_acquire(struct inode *inode);
extern void bd_forget(struct inode *inode);
extern void bdput(struct block_device *);
extern struct char_device *cdget(dev_t);
extern void cdput(struct char_device *);
extern int blkdev_open(struct inode *, struct file *);
extern int blkdev_close(struct inode *, struct file *);
extern struct file_operations def_blk_fops;
extern struct address_space_operations def_blk_aops;
extern struct file_operations def_fifo_fops;
extern int ioctl_by_bdev(struct block_device *, unsigned, unsigned long);
extern int blkdev_get(struct block_device *, mode_t, unsigned, int);
extern int blkdev_put(struct block_device *, int);


extern const struct block_device_operations *get_blkfops(unsigned int);
extern int register_chrdev(unsigned int, const char *, struct file_operations *);
extern int unregister_chrdev(unsigned int, const char *);
extern int chrdev_open(struct inode *, struct file *);
extern const char * bdevname(kdev_t);
extern const char * cdevname(kdev_t);
extern const char * kdevname(kdev_t);
extern void init_special_inode(struct inode *, umode_t, int);


extern void make_bad_inode(struct inode *);
extern int is_bad_inode(struct inode *);

extern struct file_operations read_fifo_fops;
extern struct file_operations write_fifo_fops;
extern struct file_operations rdwr_fifo_fops;
extern struct file_operations read_pipe_fops;
extern struct file_operations write_pipe_fops;
extern struct file_operations rdwr_pipe_fops;

extern int fs_may_remount_ro(struct super_block *);

extern int try_to_free_buffers(struct page *, unsigned int);
extern void refile_buffer(struct buffer_head * buf);
extern void create_empty_buffers(struct page *, kdev_t, unsigned long);
extern void end_buffer_io_sync(struct buffer_head *bh, int uptodate);


extern void set_buffer_async_io(struct buffer_head *bh) ;






static inline void get_bh(struct buffer_head * bh)
{
        atomic_add(1,(&(bh)->b_count));
}

static inline void put_bh(struct buffer_head *bh)
{
        __asm__ __volatile__("": : :"memory");
        atomic_sub(1,(&bh->b_count));
}




static inline void mark_buffer_uptodate(struct buffer_head * bh, int on)
{
        if (on)
                set_bit(BH_Uptodate, &bh->b_state);
        else
                clear_bit(BH_Uptodate, &bh->b_state);
}



static inline void __mark_buffer_clean(struct buffer_head *bh)
{
        refile_buffer(bh);
}

static inline void mark_buffer_clean(struct buffer_head * bh)
{
        if (test_and_clear_bit(BH_Dirty, &(bh)->b_state))
                __mark_buffer_clean(bh);
}

extern void __mark_dirty(struct buffer_head *bh);
extern void __mark_buffer_dirty(struct buffer_head *bh);
extern void mark_buffer_dirty(struct buffer_head *bh);
extern void buffer_insert_inode_data_queue(struct buffer_head *, struct inode *);



static inline void mark_buffer_async(struct buffer_head * bh, int on)
{
        if (on)
                set_bit(BH_Async, &bh->b_state);
        else
                clear_bit(BH_Async, &bh->b_state);
}







static inline void buffer_IO_error(struct buffer_head * bh)
{
        mark_buffer_clean(bh);



        bh->b_end_io(bh, 0);
}

extern void buffer_insert_inode_queue(struct buffer_head *, struct inode *);
static inline void mark_buffer_dirty_inode(struct buffer_head *bh, struct inode *inode)
{
        mark_buffer_dirty(bh);
        buffer_insert_inode_queue(bh, inode);
}

extern void set_buffer_flushtime(struct buffer_head *);
extern void balance_dirty(void);
extern int check_disk_change(kdev_t);
extern int invalidate_inodes(struct super_block *);
extern int invalidate_device(kdev_t, int);
extern void invalidate_inode_pages(struct inode *);
extern void invalidate_inode_pages2(struct address_space *);
extern void invalidate_inode_buffers(struct inode *);


extern void invalidate_bdev(struct block_device *, int);
extern void __invalidate_buffers(kdev_t dev, int);
extern void sync_inodes(kdev_t);
extern void sync_unlocked_inodes(void);
extern void write_inode_now(struct inode *, int);
extern int sync_buffers(kdev_t, int);
extern void sync_dev(kdev_t);
extern int fsync_dev(kdev_t);
extern int fsync_super(struct super_block *);
extern int fsync_no_super(kdev_t);
extern void sync_inodes_sb(struct super_block *);
extern int osync_inode_buffers(struct inode *);
extern int osync_inode_data_buffers(struct inode *);
extern int fsync_inode_buffers(struct inode *);
extern int fsync_inode_data_buffers(struct inode *);
extern int inode_has_buffers(struct inode *);
extern int filemap_fdatasync(struct address_space *);
extern int filemap_fdatawait(struct address_space *);
extern void sync_supers(kdev_t);
extern int bmap(struct inode *, int);
extern int notify_change(struct dentry *, struct iattr *);
extern int permission(struct inode *, int);
extern int vfs_permission(struct inode *, int);
extern int get_write_access(struct inode *);
extern int deny_write_access(struct file *);
static inline void put_write_access(struct inode * inode)
{
        atomic_sub(1,(&inode->i_writecount));
}
static inline void allow_write_access(struct file *file)
{
        if (file)
                atomic_add(1,(&file->f_dentry->d_inode->i_writecount));
}
extern int do_pipe(int *);

extern int open_namei(const char *, int, int, struct nameidata *);

extern int kernel_read(struct file *, unsigned long, char *, unsigned long);
extern struct file * open_exec(const char *);


extern int is_subdir(struct dentry *, struct dentry *);
extern ino_t find_inode_number(struct dentry *, struct qstr *);
# 1258 "/opt/Wive/kernel/include/linux/fs.h"
static inline void *ERR_PTR(long error)
{
        return (void *) error;
}

static inline long PTR_ERR(const void *ptr)
{
        return (long) ptr;
}

static inline long IS_ERR(const void *ptr)
{
        return (unsigned long)ptr > (unsigned long)-1000L;
}
# 1289 "/opt/Wive/kernel/include/linux/fs.h"
enum {LAST_NORM, LAST_ROOT, LAST_DOT, LAST_DOTDOT, LAST_BIND};
# 1300 "/opt/Wive/kernel/include/linux/fs.h"
typedef struct {
        size_t written;
        size_t count;
        char * buf;
        int error;
} read_descriptor_t;

typedef int (*read_actor_t)(read_descriptor_t *, struct page *, unsigned long, unsigned long);


extern loff_t default_llseek(struct file *file, loff_t offset, int origin);

extern int __user_walk(const char *, unsigned, struct nameidata *);
extern int path_init(const char *, unsigned, struct nameidata *);
extern int path_walk(const char *, struct nameidata *);
extern int link_path_walk(const char *, struct nameidata *);
extern void path_release(struct nameidata *);
extern int follow_down(struct vfsmount **, struct dentry **);
extern int follow_up(struct vfsmount **, struct dentry **);
extern struct dentry * lookup_one_len(const char *, struct dentry *, int);
extern struct dentry * lookup_hash(struct qstr *, struct dentry *);



extern void iput(struct inode *);
extern void refile_inode(struct inode *inode);
extern void force_delete(struct inode *);
extern struct inode * igrab(struct inode *);
extern ino_t iunique(struct super_block *, ino_t);

typedef int (*find_inode_t)(struct inode *, unsigned long, void *);
extern struct inode * iget4(struct super_block *, unsigned long, find_inode_t, void *);
static inline struct inode *iget(struct super_block *sb, unsigned long ino)
{
        return iget4(sb, ino, ((void *)0), ((void *)0));
}

extern void clear_inode(struct inode *);
extern struct inode * get_empty_inode(void);

static inline struct inode * new_inode(struct super_block *sb)
{
        struct inode *inode = get_empty_inode();
        if (inode) {
                inode->i_sb = sb;
                inode->i_dev = sb->s_dev;
                inode->i_blkbits = sb->s_blocksize_bits;
        }
        return inode;
}
extern void remove_suid(struct inode *inode);

extern void insert_inode_hash(struct inode *);
extern void remove_inode_hash(struct inode *);
extern struct file * get_empty_filp(void);
extern void file_move(struct file *f, struct list_head *list);
extern struct buffer_head * get_hash_table(kdev_t, int, int);
extern struct buffer_head * getblk(kdev_t, int, int);
extern void ll_rw_block(int, int, struct buffer_head * bh[]);
extern void submit_bh(int, struct buffer_head *);
extern int is_read_only(kdev_t);
extern void __brelse(struct buffer_head *);
static inline void brelse(struct buffer_head *buf)
{
        if (buf)
                __brelse(buf);
}
extern void __bforget(struct buffer_head *);
static inline void bforget(struct buffer_head *buf)
{
        if (buf)
                __bforget(buf);
}
extern int set_blocksize(kdev_t, int);
extern struct buffer_head * bread(kdev_t, int, int);
static inline struct buffer_head * sb_bread(struct super_block *sb, int block)
{
        return bread(sb->s_dev, block, sb->s_blocksize);
}
static inline struct buffer_head * sb_getblk(struct super_block *sb, int block)
{
        return getblk(sb->s_dev, block, sb->s_blocksize);
}
static inline struct buffer_head * sb_get_hash_table(struct super_block *sb, int block)
{
        return get_hash_table(sb->s_dev, block, sb->s_blocksize);
}
extern void wakeup_bdflush(void);
extern void put_unused_buffer_head(struct buffer_head * bh);
extern struct buffer_head * get_unused_buffer_head(int async);

extern int brw_page(int, struct page *, kdev_t, int [], int);

typedef int (get_block_t)(struct inode*,long,struct buffer_head*,int);


extern int try_to_release_page(struct page * page, int gfp_mask);
extern int discard_bh_page(struct page *, unsigned long, int);


extern int block_symlink(struct inode *, const char *, int);
extern int block_write_full_page(struct page*, get_block_t*);
extern int block_read_full_page(struct page*, get_block_t*);
extern int block_prepare_write(struct page*, unsigned, unsigned, get_block_t*);
extern int cont_prepare_write(struct page*, unsigned, unsigned, get_block_t*,
                                unsigned long *);
extern int generic_cont_expand(struct inode *inode, loff_t size) ;
extern int block_commit_write(struct page *page, unsigned from, unsigned to);
extern int block_sync_page(struct page *);

int generic_block_bmap(struct address_space *, long, get_block_t *);
int generic_commit_write(struct file *, struct page *, unsigned, unsigned);
int block_truncate_page(struct address_space *, loff_t, get_block_t *);
extern int generic_direct_IO(int, struct inode *, struct kiobuf *, unsigned long, int, get_block_t *);
extern int waitfor_one_page(struct page *);
extern int writeout_one_page(struct page *);

extern int generic_file_mmap(struct file *, struct vm_area_struct *);
extern int file_read_actor(read_descriptor_t * desc, struct page *page, unsigned long offset, unsigned long size);
extern ssize_t generic_file_read(struct file *, char *, size_t, loff_t *);
extern ssize_t generic_file_write(struct file *, const char *, size_t, loff_t *);
extern void do_generic_file_read(struct file *, loff_t *, read_descriptor_t *, read_actor_t);
extern loff_t no_llseek(struct file *file, loff_t offset, int origin);
extern loff_t generic_file_llseek(struct file *file, loff_t offset, int origin);
extern ssize_t generic_read_dir(struct file *, char *, size_t, loff_t *);
extern int generic_file_open(struct inode * inode, struct file * filp);

extern struct file_operations generic_ro_fops;

extern int vfs_readlink(struct dentry *, char *, int, const char *);
extern int vfs_follow_link(struct nameidata *, const char *);
extern int page_readlink(struct dentry *, char *, int);
extern int page_follow_link(struct dentry *, struct nameidata *);
extern struct inode_operations page_symlink_inode_operations;

extern int vfs_readdir(struct file *, filldir_t, void *);
extern int dcache_readdir(struct file *, void *, filldir_t);

extern struct file_system_type *get_fs_type(const char *name);
extern struct super_block *get_super(kdev_t);
extern void drop_super(struct super_block *sb);
static inline int is_mounted(kdev_t dev)
{
        struct super_block *sb = get_super(dev);
        if (sb) {
                drop_super(sb);
                return 1;
        }
        return 0;
}
unsigned long generate_cluster(kdev_t, int b[], int);
unsigned long generate_cluster_swab32(kdev_t, int b[], int);
extern kdev_t ROOT_DEV;
extern char root_device_name[];


extern void show_buffers(void);
extern void mount_root(void);


extern unsigned int real_root_dev;
extern int change_root(kdev_t, const char *);


extern ssize_t char_read(struct file *, char *, size_t, loff_t *);
extern ssize_t block_read(struct file *, char *, size_t, loff_t *);
extern int read_ahead[];

extern ssize_t char_write(struct file *, const char *, size_t, loff_t *);
extern ssize_t block_write(struct file *, const char *, size_t, loff_t *);

extern int file_fsync(struct file *, struct dentry *, int);
extern int generic_buffer_fdatasync(struct inode *inode, unsigned long start_idx, unsigned long end_idx);
extern int generic_osync_inode(struct inode *, int);




extern int inode_change_ok(struct inode *, struct iattr *);
extern int inode_setattr(struct inode *, struct iattr *);
# 1500 "/opt/Wive/kernel/include/linux/fs.h"
static inline struct dentry *lock_parent(struct dentry *dentry)
{
        struct dentry *dir = dget(dentry->d_parent);

        down(&dir->d_inode->i_sem);
        return dir;
}

static inline struct dentry *get_parent(struct dentry *dentry)
{
        return dget(dentry->d_parent);
}

static inline void unlock_dir(struct dentry *dir)
{
        up(&dir->d_inode->i_sem);
        dput(dir);
}





static inline void double_down(struct semaphore *s1, struct semaphore *s2)
{
        if (s1 != s2) {
                if ((unsigned long) s1 < (unsigned long) s2) {
                        struct semaphore *tmp = s2;
                        s2 = s1; s1 = tmp;
                }
                down(s1);
        }
        down(s2);
}
# 1544 "/opt/Wive/kernel/include/linux/fs.h"
static inline void triple_down(struct semaphore *s1,
                               struct semaphore *s2,
                               struct semaphore *s3)
{
        if (s1 != s2) {
                if ((unsigned long) s1 < (unsigned long) s2) {
                        if ((unsigned long) s1 < (unsigned long) s3) {
                                struct semaphore *tmp = s3;
                                s3 = s1; s1 = tmp;
                        }
                        if ((unsigned long) s1 < (unsigned long) s2) {
                                struct semaphore *tmp = s2;
                                s2 = s1; s1 = tmp;
                        }
                } else {
                        if ((unsigned long) s1 < (unsigned long) s3) {
                                struct semaphore *tmp = s3;
                                s3 = s1; s1 = tmp;
                        }
                        if ((unsigned long) s2 < (unsigned long) s3) {
                                struct semaphore *tmp = s3;
                                s3 = s2; s2 = tmp;
                        }
                }
                down(s1);
        } else if ((unsigned long) s2 < (unsigned long) s3) {
                struct semaphore *tmp = s3;
                s3 = s2; s2 = tmp;
        }
        down(s2);
        down(s3);
}

static inline void double_up(struct semaphore *s1, struct semaphore *s2)
{
        up(s1);
        if (s1 != s2)
                up(s2);
}

static inline void triple_up(struct semaphore *s1,
                             struct semaphore *s2,
                             struct semaphore *s3)
{
        up(s1);
        if (s1 != s2)
                up(s2);
        up(s3);
}

static inline void double_lock(struct dentry *d1, struct dentry *d2)
{
        double_down(&d1->d_inode->i_sem, &d2->d_inode->i_sem);
}

static inline void double_unlock(struct dentry *d1, struct dentry *d2)
{
        double_up(&d1->d_inode->i_sem,&d2->d_inode->i_sem);
        dput(d1);
        dput(d2);
}
# 16 "balloc.c" 2
# 1 "/opt/Wive/kernel/include/linux/ext2_fs.h" 1
# 126 "/opt/Wive/kernel/include/linux/ext2_fs.h"
struct ext2_acl_header
{
        __u32 aclh_size;
        __u32 aclh_file_count;
        __u32 aclh_acle_count;
        __u32 aclh_first_acle;
};

struct ext2_acl_entry
{
        __u32 acle_size;
        __u16 acle_perms;
        __u16 acle_type;
        __u16 acle_tag;
        __u16 acle_pad1;
        __u32 acle_next;

};




struct ext2_group_desc
{
        __u32 bg_block_bitmap;
        __u32 bg_inode_bitmap;
        __u32 bg_inode_table;
        __u16 bg_free_blocks_count;
        __u16 bg_free_inodes_count;
        __u16 bg_used_dirs_count;
        __u16 bg_pad;
        __u32 bg_reserved[3];
};
# 217 "/opt/Wive/kernel/include/linux/ext2_fs.h"
struct ext2_inode {
        __u16 i_mode;
        __u16 i_uid;
        __u32 i_size;
        __u32 i_atime;
        __u32 i_ctime;
        __u32 i_mtime;
        __u32 i_dtime;
        __u16 i_gid;
        __u16 i_links_count;
        __u32 i_blocks;
        __u32 i_flags;
        union {
                struct {
                        __u32 l_i_reserved1;
                } linux1;
                struct {
                        __u32 h_i_translator;
                } hurd1;
                struct {
                        __u32 m_i_reserved1;
                } masix1;
        } osd1;
        __u32 i_block[(((12 + 1) + 1) + 1)];
        __u32 i_generation;
        __u32 i_file_acl;
        __u32 i_dir_acl;
        __u32 i_faddr;
        union {
                struct {
                        __u8 l_i_frag;
                        __u8 l_i_fsize;
                        __u16 i_pad1;
                        __u16 l_i_uid_high;
                        __u16 l_i_gid_high;
                        __u32 l_i_reserved2;
                } linux2;
                struct {
                        __u8 h_i_frag;
                        __u8 h_i_fsize;
                        __u16 h_i_mode_high;
                        __u16 h_i_uid_high;
                        __u16 h_i_gid_high;
                        __u32 h_i_author;
                } hurd2;
                struct {
                        __u8 m_i_frag;
                        __u8 m_i_fsize;
                        __u16 m_pad1;
                        __u32 m_i_reserved2[2];
                } masix2;
        } osd2;
};
# 339 "/opt/Wive/kernel/include/linux/ext2_fs.h"
struct ext2_super_block {
        __u32 s_inodes_count;
        __u32 s_blocks_count;
        __u32 s_r_blocks_count;
        __u32 s_free_blocks_count;
        __u32 s_free_inodes_count;
        __u32 s_first_data_block;
        __u32 s_log_block_size;
        __s32 s_log_frag_size;
        __u32 s_blocks_per_group;
        __u32 s_frags_per_group;
        __u32 s_inodes_per_group;
        __u32 s_mtime;
        __u32 s_wtime;
        __u16 s_mnt_count;
        __s16 s_max_mnt_count;
        __u16 s_magic;
        __u16 s_state;
        __u16 s_errors;
        __u16 s_minor_rev_level;
        __u32 s_lastcheck;
        __u32 s_checkinterval;
        __u32 s_creator_os;
        __u32 s_rev_level;
        __u16 s_def_resuid;
        __u16 s_def_resgid;
# 378 "/opt/Wive/kernel/include/linux/ext2_fs.h"
        __u32 s_first_ino;
        __u16 s_inode_size;
        __u16 s_block_group_nr;
        __u32 s_feature_compat;
        __u32 s_feature_incompat;
        __u32 s_feature_ro_compat;
        __u8 s_uuid[16];
        char s_volume_name[16];
        char s_last_mounted[64];
        __u32 s_algorithm_usage_bitmap;




        __u8 s_prealloc_blocks;
        __u8 s_prealloc_dir_blocks;
        __u16 s_padding1;
        __u32 s_reserved[204];
};
# 488 "/opt/Wive/kernel/include/linux/ext2_fs.h"
struct ext2_dir_entry {
        __u32 inode;
        __u16 rec_len;
        __u16 name_len;
        char name[255];
};







struct ext2_dir_entry_2 {
        __u32 inode;
        __u16 rec_len;
        __u8 name_len;
        __u8 file_type;
        char name[255];
};





enum {
        EXT2_FT_UNKNOWN,
        EXT2_FT_REG_FILE,
        EXT2_FT_DIR,
        EXT2_FT_CHRDEV,
        EXT2_FT_BLKDEV,
        EXT2_FT_FIFO,
        EXT2_FT_SOCK,
        EXT2_FT_SYMLINK,
        EXT2_FT_MAX
};
# 549 "/opt/Wive/kernel/include/linux/ext2_fs.h"
extern int ext2_bg_has_super(struct super_block *sb, int group);
extern unsigned long ext2_bg_num_gdb(struct super_block *sb, int group);
extern int ext2_new_block (struct inode *, unsigned long,
                           __u32 *, __u32 *, int *);
extern void ext2_free_blocks (struct inode *, unsigned long,
                              unsigned long);
extern unsigned long ext2_count_free_blocks (struct super_block *);
extern void ext2_check_blocks_bitmap (struct super_block *);
extern struct ext2_group_desc * ext2_get_group_desc(struct super_block * sb,
                                                    unsigned int block_group,
                                                    struct buffer_head ** bh);


extern int ext2_add_link (struct dentry *, struct inode *);
extern ino_t ext2_inode_by_name(struct inode *, struct dentry *);
extern int ext2_make_empty(struct inode *, struct inode *);
extern struct ext2_dir_entry_2 * ext2_find_entry (struct inode *,struct dentry *, struct page **);
extern int ext2_delete_entry (struct ext2_dir_entry_2 *, struct page *);
extern int ext2_empty_dir (struct inode *);
extern struct ext2_dir_entry_2 * ext2_dotdot (struct inode *, struct page **);
extern void ext2_set_link(struct inode *, struct ext2_dir_entry_2 *, struct page *, struct inode *);


extern int ext2_sync_file (struct file *, struct dentry *, int);
extern int ext2_fsync_inode (struct inode *, int);


extern struct inode * ext2_new_inode (const struct inode *, int);
extern void ext2_free_inode (struct inode *);
extern unsigned long ext2_count_free_inodes (struct super_block *);
extern void ext2_check_inodes_bitmap (struct super_block *);
extern unsigned long ext2_count_free (struct buffer_head *, unsigned);


extern void ext2_read_inode (struct inode *);
extern void ext2_write_inode (struct inode *, int);
extern void ext2_put_inode (struct inode *);
extern void ext2_delete_inode (struct inode *);
extern int ext2_sync_inode (struct inode *);
extern void ext2_discard_prealloc (struct inode *);
extern void ext2_truncate (struct inode *);


extern int ext2_ioctl (struct inode *, struct file *, unsigned int,
                       unsigned long);


extern void ext2_error (struct super_block *, const char *, const char *, ...)
        __attribute__ ((format (printf, 3, 4)));
extern void ext2_panic (struct super_block *, const char *,
                                   const char *, ...)
        __attribute__ ((noreturn, format (printf, 3, 4)));
extern void ext2_warning (struct super_block *, const char *, const char *, ...)
        __attribute__ ((format (printf, 3, 4)));
extern void ext2_update_dynamic_rev (struct super_block *sb);
extern void ext2_put_super (struct super_block *);
extern void ext2_write_super (struct super_block *);
extern int ext2_remount (struct super_block *, int *, char *);
extern struct super_block * ext2_read_super (struct super_block *,void *,int);
extern int ext2_statfs (struct super_block *, struct statfs *);






extern struct file_operations ext2_dir_operations;


extern struct inode_operations ext2_file_inode_operations;
extern struct file_operations ext2_file_operations;


extern struct address_space_operations ext2_aops;


extern struct inode_operations ext2_dir_inode_operations;


extern struct inode_operations ext2_fast_symlink_inode_operations;
# 17 "balloc.c" 2
# 1 "/opt/Wive/kernel/include/linux/locks.h" 1




# 1 "/opt/Wive/kernel/include/linux/mm.h" 1



# 1 "/opt/Wive/kernel/include/linux/sched.h" 1





extern unsigned long event;


# 1 "/opt/Wive/kernel/include/linux/binfmts.h" 1



# 1 "/opt/Wive/kernel/include/linux/ptrace.h" 1
# 5 "/opt/Wive/kernel/include/linux/binfmts.h" 2
# 1 "/opt/Wive/kernel/include/linux/capability.h" 1
# 32 "/opt/Wive/kernel/include/linux/capability.h"
typedef struct __user_cap_header_struct {
        __u32 version;
        int pid;
} *cap_user_header_t;

typedef struct __user_cap_data_struct {
        __u32 effective;
        __u32 permitted;
        __u32 inheritable;
} *cap_user_data_t;
# 55 "/opt/Wive/kernel/include/linux/capability.h"
typedef __u32 kernel_cap_t;
# 286 "/opt/Wive/kernel/include/linux/capability.h"
extern kernel_cap_t cap_bset;
# 314 "/opt/Wive/kernel/include/linux/capability.h"
static inline kernel_cap_t cap_combine(kernel_cap_t a, kernel_cap_t b)
{
     kernel_cap_t dest;
     (dest) = (a) | (b);
     return dest;
}

static inline kernel_cap_t cap_intersect(kernel_cap_t a, kernel_cap_t b)
{
     kernel_cap_t dest;
     (dest) = (a) & (b);
     return dest;
}

static inline kernel_cap_t cap_drop(kernel_cap_t a, kernel_cap_t drop)
{
     kernel_cap_t dest;
     (dest) = (a) & ~(drop);
     return dest;
}

static inline kernel_cap_t cap_invert(kernel_cap_t c)
{
     kernel_cap_t dest;
     (dest) = ~(c);
     return dest;
}
# 6 "/opt/Wive/kernel/include/linux/binfmts.h" 2
# 22 "/opt/Wive/kernel/include/linux/binfmts.h"
struct linux_binprm{
        char buf[128];
        struct page *page[32];
        unsigned long p;
        int sh_bang;
        struct file * file;
        int e_uid, e_gid;
        kernel_cap_t cap_inheritable, cap_permitted, cap_effective;
        int argc, envc;
        char * filename;
        unsigned long loader, exec;
};





struct linux_binfmt {
        struct linux_binfmt * next;
        struct module *module;
        int (*load_binary)(struct linux_binprm *, struct pt_regs * regs);
        int (*load_shlib)(struct file *);
        int (*core_dump)(long signr, struct pt_regs * regs, struct file * file);
        unsigned long min_coredump;
};

extern int register_binfmt(struct linux_binfmt *);
extern int unregister_binfmt(struct linux_binfmt *);

extern int prepare_binprm(struct linux_binprm *);
extern void remove_arg_zero(struct linux_binprm *);
extern int search_binary_handler(struct linux_binprm *,struct pt_regs *);
extern int flush_old_exec(struct linux_binprm * bprm);
extern int setup_arg_pages(struct linux_binprm * bprm);
extern int copy_strings(int argc,char ** argv,struct linux_binprm *bprm);
extern int copy_strings_kernel(int argc,char ** argv,struct linux_binprm *bprm);
extern void compute_creds(struct linux_binprm *binprm);
extern int do_coredump(long signr, struct pt_regs * regs);
extern void set_binfmt(struct linux_binfmt *new);
# 10 "/opt/Wive/kernel/include/linux/sched.h" 2



# 1 "/opt/Wive/kernel/include/linux/times.h" 1



struct tms {
        clock_t tms_utime;
        clock_t tms_stime;
        clock_t tms_cutime;
        clock_t tms_cstime;
};
# 14 "/opt/Wive/kernel/include/linux/sched.h" 2
# 1 "/opt/Wive/kernel/include/linux/timex.h" 1
# 152 "/opt/Wive/kernel/include/linux/timex.h"
# 1 "/opt/Wive/kernel/include/asm/timex.h" 1
# 26 "/opt/Wive/kernel/include/asm/timex.h"
typedef unsigned int cycles_t;
extern cycles_t cacheflush_time;

static inline cycles_t get_cycles (void)
{
        return ({ int __res; __asm__ __volatile__( ".set\tpush\n\t" ".set\treorder\n\t" "mfc0\t%0,""$9""\n\t" ".set\tpop" : "=r" (__res)); __res;});

}
# 153 "/opt/Wive/kernel/include/linux/timex.h" 2
# 161 "/opt/Wive/kernel/include/linux/timex.h"
struct timex {
        unsigned int modes;
        long offset;
        long freq;
        long maxerror;
        long esterror;
        int status;
        long constant;
        long precision;
        long tolerance;


        struct timeval time;
        long tick;

        long ppsfreq;
        long jitter;
        int shift;
        long stabil;
        long jitcnt;
        long calcnt;
        long errcnt;
        long stbcnt;

        int :32; int :32; int :32; int :32;
        int :32; int :32; int :32; int :32;
        int :32; int :32; int :32; int :32;
};
# 253 "/opt/Wive/kernel/include/linux/timex.h"
extern long tick;
extern int tickadj;




extern int time_state;
extern int time_status;
extern long time_offset;
extern long time_constant;
extern long time_tolerance;
extern long time_precision;
extern long time_maxerror;
extern long time_esterror;

extern long time_phase;
extern long time_freq;
extern long time_adj;
extern long time_reftime;

extern long time_adjust;


extern long pps_offset;
extern long pps_jitter;
extern long pps_freq;
extern long pps_stabil;
extern long pps_valid;


extern int pps_shift;
extern long pps_jitcnt;
extern long pps_calcnt;
extern long pps_errcnt;
extern long pps_stbcnt;
# 15 "/opt/Wive/kernel/include/linux/sched.h" 2
# 1 "/opt/Wive/kernel/include/linux/rbtree.h" 1
# 100 "/opt/Wive/kernel/include/linux/rbtree.h"
typedef struct rb_node_s
{
        struct rb_node_s * rb_parent;
        int rb_color;


        struct rb_node_s * rb_right;
        struct rb_node_s * rb_left;
}
rb_node_t;

typedef struct rb_root_s
{
        struct rb_node_s * rb_node;
}
rb_root_t;





extern void rb_insert_color(rb_node_t *, rb_root_t *);
extern void rb_erase(rb_node_t *, rb_root_t *);


extern rb_node_t *rb_next(rb_node_t *);
extern rb_node_t *rb_prev(rb_node_t *);
extern rb_node_t *rb_first(rb_root_t *);
extern rb_node_t *rb_last(rb_root_t *);

static inline void rb_link_node(rb_node_t * node, rb_node_t * parent, rb_node_t ** rb_link)
{
        node->rb_parent = parent;
        node->rb_color = 0;
        node->rb_left = node->rb_right = ((void *)0);

        *rb_link = node;
}
# 16 "/opt/Wive/kernel/include/linux/sched.h" 2





# 1 "/opt/Wive/kernel/include/asm/mmu.h" 1




typedef unsigned long mm_context_t;
# 22 "/opt/Wive/kernel/include/linux/sched.h" 2

# 1 "/opt/Wive/kernel/include/linux/smp.h" 1
# 24 "/opt/Wive/kernel/include/linux/sched.h" 2
# 1 "/opt/Wive/kernel/include/linux/tty.h" 1
# 26 "/opt/Wive/kernel/include/linux/tty.h"
# 1 "/opt/Wive/kernel/include/linux/major.h" 1
# 182 "/opt/Wive/kernel/include/linux/major.h"
static __inline__ int scsi_blk_major(int m) {
        return (((m) == 8 || ((m) >= 65 && (m) <= 71)) || (m) == 11);
}
# 195 "/opt/Wive/kernel/include/linux/major.h"
static __inline__ int ide_blk_major(int m)
{
        return ((m) == 3 || (m) == 22 || (m) == 33 || (m) == 34 || (m) == 56 || (m) == 57 || (m) == 88 || (m) == 89 || (m) == 90 || (m) == 91);
}
# 27 "/opt/Wive/kernel/include/linux/tty.h" 2
# 1 "/opt/Wive/kernel/include/linux/termios.h" 1




# 1 "/opt/Wive/kernel/include/asm/termios.h" 1
# 11 "/opt/Wive/kernel/include/asm/termios.h"
# 1 "/opt/Wive/kernel/include/asm/termbits.h" 1
# 16 "/opt/Wive/kernel/include/asm/termbits.h"
typedef unsigned char cc_t;
typedef unsigned long speed_t;
typedef unsigned long tcflag_t;






struct termios {
        tcflag_t c_iflag;
        tcflag_t c_oflag;
        tcflag_t c_cflag;
        tcflag_t c_lflag;



        cc_t c_line;
        cc_t c_cc[23];
};
# 12 "/opt/Wive/kernel/include/asm/termios.h" 2
# 1 "/opt/Wive/kernel/include/asm/ioctls.h" 1
# 13 "/opt/Wive/kernel/include/asm/termios.h" 2

struct sgttyb {
        char sg_ispeed;
        char sg_ospeed;
        char sg_erase;
        char sg_kill;
        int sg_flags;
};

struct tchars {
        char t_intrc;
        char t_quitc;
        char t_startc;
        char t_stopc;
        char t_eofc;
        char t_brkc;
};

struct ltchars {
        char t_suspc;
        char t_dsuspc;
        char t_rprntc;
        char t_flushc;
        char t_werasc;
        char t_lnextc;
};




struct winsize {
        unsigned short ws_row;
        unsigned short ws_col;
        unsigned short ws_xpixel;
        unsigned short ws_ypixel;
};


struct termio {
        unsigned short c_iflag;
        unsigned short c_oflag;
        unsigned short c_cflag;
        unsigned short c_lflag;
        char c_line;
        unsigned char c_cc[23];
};
# 6 "/opt/Wive/kernel/include/linux/termios.h" 2
# 28 "/opt/Wive/kernel/include/linux/tty.h" 2
# 1 "/opt/Wive/kernel/include/linux/tqueue.h" 1
# 29 "/opt/Wive/kernel/include/linux/tty.h" 2
# 1 "/opt/Wive/kernel/include/linux/tty_driver.h" 1
# 120 "/opt/Wive/kernel/include/linux/tty_driver.h"
struct tty_driver {
        int magic;
        const char *driver_name;
        const char *name;
        int name_base;
        short major;
        short minor_start;
        short num;
        short type;
        short subtype;
        struct termios init_termios;
        int flags;
        int *refcount;
        struct proc_dir_entry *proc_entry;
        struct tty_driver *other;




        struct tty_struct **table;
        struct termios **termios;
        struct termios **termios_locked;
        void *driver_state;





        int (*open)(struct tty_struct * tty, struct file * filp);
        void (*close)(struct tty_struct * tty, struct file * filp);
        int (*write)(struct tty_struct * tty, int from_user,
                      const unsigned char *buf, int count);
        void (*put_char)(struct tty_struct *tty, unsigned char ch);
        void (*flush_chars)(struct tty_struct *tty);
        int (*write_room)(struct tty_struct *tty);
        int (*chars_in_buffer)(struct tty_struct *tty);
        int (*ioctl)(struct tty_struct *tty, struct file * file,
                    unsigned int cmd, unsigned long arg);
        void (*set_termios)(struct tty_struct *tty, struct termios * old);
        void (*throttle)(struct tty_struct * tty);
        void (*unthrottle)(struct tty_struct * tty);
        void (*stop)(struct tty_struct *tty);
        void (*start)(struct tty_struct *tty);
        void (*hangup)(struct tty_struct *tty);
        void (*break_ctl)(struct tty_struct *tty, int state);
        void (*flush_buffer)(struct tty_struct *tty);
        void (*set_ldisc)(struct tty_struct *tty);
        void (*wait_until_sent)(struct tty_struct *tty, int timeout);
        void (*send_xchar)(struct tty_struct *tty, char ch);
        int (*read_proc)(char *page, char **start, off_t off,
                          int count, int *eof, void *data);
        int (*write_proc)(struct file *file, const char *buffer,
                          unsigned long count, void *data);




        struct tty_driver *next;
        struct tty_driver *prev;
};
# 30 "/opt/Wive/kernel/include/linux/tty.h" 2
# 1 "/opt/Wive/kernel/include/linux/tty_ldisc.h" 1
# 103 "/opt/Wive/kernel/include/linux/tty_ldisc.h"
struct tty_ldisc {
        int magic;
        char *name;
        int num;
        int flags;



        int (*open)(struct tty_struct *);
        void (*close)(struct tty_struct *);
        void (*flush_buffer)(struct tty_struct *tty);
        ssize_t (*chars_in_buffer)(struct tty_struct *tty);
        ssize_t (*read)(struct tty_struct * tty, struct file * file,
                        unsigned char * buf, size_t nr);
        ssize_t (*write)(struct tty_struct * tty, struct file * file,
                         const unsigned char * buf, size_t nr);
        int (*ioctl)(struct tty_struct * tty, struct file * file,
                         unsigned int cmd, unsigned long arg);
        void (*set_termios)(struct tty_struct *tty, struct termios * old);
        unsigned int (*poll)(struct tty_struct *, struct file *,
                             struct poll_table_struct *);




        void (*receive_buf)(struct tty_struct *, const unsigned char *cp,
                               char *fp, int count);
        int (*receive_room)(struct tty_struct *);
        void (*write_wakeup)(struct tty_struct *);
};
# 31 "/opt/Wive/kernel/include/linux/tty.h" 2
# 68 "/opt/Wive/kernel/include/linux/tty.h"
struct screen_info {
        unsigned char orig_x;
        unsigned char orig_y;
        unsigned short dontuse1;
        unsigned short orig_video_page;
        unsigned char orig_video_mode;
        unsigned char orig_video_cols;
        unsigned short unused2;
        unsigned short orig_video_ega_bx;
        unsigned short unused3;
        unsigned char orig_video_lines;
        unsigned char orig_video_isVGA;
        unsigned short orig_video_points;


        unsigned short lfb_width;
        unsigned short lfb_height;
        unsigned short lfb_depth;
        unsigned long lfb_base;
        unsigned long lfb_size;
        unsigned short dontuse2, dontuse3;
        unsigned short lfb_linelength;
        unsigned char red_size;
        unsigned char red_pos;
        unsigned char green_size;
        unsigned char green_pos;
        unsigned char blue_size;
        unsigned char blue_pos;
        unsigned char rsvd_size;
        unsigned char rsvd_pos;
        unsigned short vesapm_seg;
        unsigned short vesapm_off;
        unsigned short pages;

};

extern struct screen_info screen_info;
# 148 "/opt/Wive/kernel/include/linux/tty.h"
struct tty_flip_buffer {
        struct tq_struct tqueue;
        struct semaphore pty_sem;
        char *char_buf_ptr;
        unsigned char *flag_buf_ptr;
        int count;
        int buf_num;
        unsigned char char_buf[2*512];
        char flag_buf[2*512];
        unsigned char slop[4];
};
# 268 "/opt/Wive/kernel/include/linux/tty.h"
struct tty_struct {
        int magic;
        struct tty_driver driver;
        struct tty_ldisc ldisc;
        struct termios *termios, *termios_locked;
        int pgrp;
        int session;
        kdev_t device;
        unsigned long flags;
        int count;
        struct winsize winsize;
        unsigned char stopped:1, hw_stopped:1, flow_stopped:1, packet:1;
        unsigned char low_latency:1, warned:1;
        unsigned char ctrl_status;

        struct tty_struct *link;
        struct fasync_struct *fasync;
        struct tty_flip_buffer flip;
        int max_flip_cnt;
        int alt_speed;
        wait_queue_head_t write_wait;
        wait_queue_head_t read_wait;
        struct tq_struct tq_hangup;
        void *disc_data;
        void *driver_data;
        struct list_head tty_files;







        unsigned int column;
        unsigned char lnext:1, erasing:1, raw:1, real_raw:1, icanon:1;
        unsigned char closing:1;
        unsigned short minimum_to_wake;
        unsigned overrun_time;
        int num_overrun;
        unsigned long process_char_map[256/(8*sizeof(unsigned long))];
        char *read_buf;
        int read_head;
        int read_tail;
        int read_cnt;
        unsigned long read_flags[4096/(8*sizeof(unsigned long))];
        int canon_data;
        unsigned long canon_head;
        unsigned int canon_column;
        struct semaphore atomic_read;
        struct semaphore atomic_write;
        spinlock_t read_lock;

        struct tq_struct SAK_tq;
};
# 350 "/opt/Wive/kernel/include/linux/tty.h"
extern void tty_write_flush(struct tty_struct *);

extern struct termios tty_std_termios;
extern struct tty_struct * redirect;
extern struct tty_ldisc ldiscs[];
extern int fg_console, last_console, want_console;

extern int kmsg_redirect;

extern void con_init(void);
extern void console_init(void);

extern int lp_init(void);
extern int pty_init(void);
extern void tty_init(void);
extern int mxser_init(void);
extern int moxa_init(void);
extern int ip2_init(void);
extern int pcxe_init(void);
extern int pc_init(void);
extern int vcs_init(void);
extern int rp_init(void);
extern int cy_init(void);
extern int stl_init(void);
extern int stli_init(void);
extern int specialix_init(void);
extern int espserial_init(void);
extern int macserial_init(void);
extern int a2232board_init(void);

extern int tty_paranoia_check(struct tty_struct *tty, kdev_t device,
                              const char *routine);
extern char *tty_name(struct tty_struct *tty, char *buf);
extern void tty_wait_until_sent(struct tty_struct * tty, long timeout);
extern int tty_check_change(struct tty_struct * tty);
extern void stop_tty(struct tty_struct * tty);
extern void start_tty(struct tty_struct * tty);
extern int tty_register_ldisc(int disc, struct tty_ldisc *new_ldisc);
extern int tty_register_driver(struct tty_driver *driver);
extern int tty_unregister_driver(struct tty_driver *driver);
extern void tty_register_devfs (struct tty_driver *driver, unsigned int flags,
                                unsigned minor);
extern void tty_unregister_devfs (struct tty_driver *driver, unsigned minor);
extern int tty_read_raw_data(struct tty_struct *tty, unsigned char *bufp,
                             int buflen);
extern void tty_write_message(struct tty_struct *tty, char *msg);

extern int is_orphaned_pgrp(int pgrp);
extern int is_ignored(int sig);
extern int tty_signal(int sig, struct tty_struct *tty);
extern void tty_hangup(struct tty_struct * tty);
extern void tty_vhangup(struct tty_struct * tty);
extern void tty_unhangup(struct file *filp);
extern int tty_hung_up_p(struct file * filp);
extern void do_SAK(struct tty_struct *tty);
extern void disassociate_ctty(int priv);
extern void tty_flip_buffer_push(struct tty_struct *tty);
extern int tty_get_baud_rate(struct tty_struct *tty);

extern void tty_wakeup(struct tty_struct *tty);
extern void tty_ldisc_flush(struct tty_struct *tty);


extern struct tty_ldisc tty_ldisc_N_TTY;


extern int n_tty_ioctl(struct tty_struct * tty, struct file * file,
                       unsigned int cmd, unsigned long arg);



extern void serial_console_init(void);



extern int pcxe_open(struct tty_struct *tty, struct file *filp);



extern void console_print(const char *);



extern int vt_ioctl(struct tty_struct *tty, struct file * file,
                    unsigned int cmd, unsigned long arg);
# 25 "/opt/Wive/kernel/include/linux/sched.h" 2
# 1 "/opt/Wive/kernel/include/linux/sem.h" 1



# 1 "/opt/Wive/kernel/include/linux/ipc.h" 1
# 9 "/opt/Wive/kernel/include/linux/ipc.h"
struct ipc_perm
{
        __kernel_key_t key;
        __kernel_uid_t uid;
        __kernel_gid_t gid;
        __kernel_uid_t cuid;
        __kernel_gid_t cgid;
        __kernel_mode_t mode;
        unsigned short seq;
};


# 1 "/opt/Wive/kernel/include/asm/ipcbuf.h" 1
# 14 "/opt/Wive/kernel/include/asm/ipcbuf.h"
struct ipc64_perm
{
        __kernel_key_t key;
        __kernel_uid_t uid;
        __kernel_gid_t gid;
        __kernel_uid_t cuid;
        __kernel_gid_t cgid;
        __kernel_mode_t mode;
        unsigned short seq;
        unsigned short __pad1;
        unsigned long __unused1;
        unsigned long __unused2;
};
# 22 "/opt/Wive/kernel/include/linux/ipc.h" 2
# 57 "/opt/Wive/kernel/include/linux/ipc.h"
struct kern_ipc_perm
{
        key_t key;
        uid_t uid;
        gid_t gid;
        uid_t cuid;
        gid_t cgid;
        mode_t mode;
        unsigned long seq;
};
# 5 "/opt/Wive/kernel/include/linux/sem.h" 2
# 23 "/opt/Wive/kernel/include/linux/sem.h"
struct semid_ds {
        struct ipc_perm sem_perm;
        __kernel_time_t sem_otime;
        __kernel_time_t sem_ctime;
        struct sem *sem_base;
        struct sem_queue *sem_pending;
        struct sem_queue **sem_pending_last;
        struct sem_undo *undo;
        unsigned short sem_nsems;
};


# 1 "/opt/Wive/kernel/include/asm/sembuf.h" 1
# 13 "/opt/Wive/kernel/include/asm/sembuf.h"
struct semid64_ds {
        struct ipc64_perm sem_perm;
        __kernel_time_t sem_otime;
        __kernel_time_t sem_ctime;
        unsigned long sem_nsems;
        unsigned long __unused1;
        unsigned long __unused2;
};
# 36 "/opt/Wive/kernel/include/linux/sem.h" 2


struct sembuf {
        unsigned short sem_num;
        short sem_op;
        short sem_flg;
};


union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
        void *__pad;
};

struct seminfo {
        int semmap;
        int semmni;
        int semmns;
        int semmnu;
        int semmsl;
        int semopm;
        int semume;
        int semusz;
        int semvmx;
        int semaem;
};
# 82 "/opt/Wive/kernel/include/linux/sem.h"
struct sem {
        int semval;
        int sempid;
};


struct sem_array {
        struct kern_ipc_perm sem_perm;
        time_t sem_otime;
        time_t sem_ctime;
        struct sem *sem_base;
        struct sem_queue *sem_pending;
        struct sem_queue **sem_pending_last;
        struct sem_undo *undo;
        unsigned long sem_nsems;
};


struct sem_queue {
        struct sem_queue * next;
        struct sem_queue ** prev;
        struct task_struct* sleeper;
        struct sem_undo * undo;
        int pid;
        int status;
        struct sem_array * sma;
        int id;
        struct sembuf * sops;
        int nsops;
        int alter;
};




struct sem_undo {
        struct sem_undo * proc_next;
        struct sem_undo * id_next;
        int semid;
        short * semadj;
};

 long sys_semget (key_t key, int nsems, int semflg);
 long sys_semop (int semid, struct sembuf *sops, unsigned nsops);
 long sys_semctl (int semid, int semnum, int cmd, union semun arg);
# 26 "/opt/Wive/kernel/include/linux/sched.h" 2
# 1 "/opt/Wive/kernel/include/linux/signal.h" 1



# 1 "/opt/Wive/kernel/include/asm/signal.h" 1
# 19 "/opt/Wive/kernel/include/asm/signal.h"
typedef struct {
        unsigned long sig[(128 / 32)];
} sigset_t;

typedef unsigned long old_sigset_t;
# 129 "/opt/Wive/kernel/include/asm/signal.h"
typedef void (*__sighandler_t)(int);






struct sigaction {
        unsigned int sa_flags;
        __sighandler_t sa_handler;
        sigset_t sa_mask;
        void (*sa_restorer)(void);
        int sa_resv[1];
};

struct k_sigaction {
        struct sigaction sa;



};


typedef struct sigaltstack {
        void *ss_sp;
        size_t ss_size;
        int ss_flags;
} stack_t;


# 1 "/opt/Wive/kernel/include/asm/sigcontext.h" 1
# 15 "/opt/Wive/kernel/include/asm/sigcontext.h"
struct sigcontext {
        unsigned int sc_regmask;
        unsigned int sc_status;
        unsigned long long sc_pc;
        unsigned long long sc_regs[32];
        unsigned long long sc_fpregs[32];
        unsigned int sc_ownedfp;
        unsigned int sc_fpc_csr;
        unsigned int sc_fpc_eir;
        unsigned int sc_used_math;
        unsigned int sc_ssflags;
        unsigned long long sc_mdhi;
        unsigned long long sc_mdlo;

        unsigned int sc_cause;
        unsigned int sc_badvaddr;

        unsigned long sc_sigset[4];
};
# 160 "/opt/Wive/kernel/include/asm/signal.h" 2
# 5 "/opt/Wive/kernel/include/linux/signal.h" 2
# 1 "/opt/Wive/kernel/include/asm/siginfo.h" 1
# 17 "/opt/Wive/kernel/include/asm/siginfo.h"
typedef union sigval {
        int sival_int;
        void *sival_ptr;
} sigval_t;
# 33 "/opt/Wive/kernel/include/asm/siginfo.h"
typedef struct siginfo {
        int si_signo;
        int si_code;
        int si_errno;
        int __pad0[128 / sizeof(int) - ((128/sizeof(int)) - 3) - 3];

        union {
                int _pad[((128/sizeof(int)) - 3)];


                struct {
                        pid_t _pid;
                        uid_t _uid;
                } _kill;


                struct {
                        pid_t _pid;
                        uid_t _uid;
                        int _status;
                        clock_t _utime;
                        clock_t _stime;
                } _sigchld;


                struct {
                        pid_t _pid;
                        clock_t _utime;
                        int _status;
                        clock_t _stime;
                } _irix_sigchld;


                struct {
                        void *_addr;
                } _sigfault;


                struct {
                        long _band;
                        int _fd;
                } _sigpoll;


                struct {
                        unsigned int _timer1;
                        unsigned int _timer2;
                } _timer;


                struct {
                        pid_t _pid;
                        uid_t _uid;
                        sigval_t _sigval;
                } _rt;

        } _sifields;
} siginfo_t;
# 228 "/opt/Wive/kernel/include/asm/siginfo.h"
typedef struct sigevent {
        sigval_t sigev_value;
        int sigev_signo;
        int sigev_notify;
        union {
                int _pad[((64 -(sizeof(long) + 2*sizeof(int))) / sizeof(int))];

                struct {
                        void (*_function)(sigval_t);
                        void *_attribute;
                } _sigev_thread;
        } _sigev_un;
} sigevent_t;







static inline void copy_siginfo(siginfo_t *to, siginfo_t *from)
{
        if (from->si_code < 0)
                memcpy(to, from, sizeof(siginfo_t));
        else

                memcpy(to, from, 3*sizeof(int) + sizeof(from->_sifields._sigchld));
}

extern int copy_siginfo_to_user(siginfo_t *to, siginfo_t *from);
# 6 "/opt/Wive/kernel/include/linux/signal.h" 2






struct sigqueue {
        struct sigqueue *next;
        siginfo_t info;
};

struct sigpending {
        struct sigqueue *head, **tail;
        sigset_t signal;
};
# 31 "/opt/Wive/kernel/include/linux/signal.h"
static inline void sigaddset(sigset_t *set, int _sig)
{
        unsigned long sig = _sig - 1;
        if ((128 / 32) == 1)
                set->sig[0] |= 1UL << sig;
        else
                set->sig[sig / 32] |= 1UL << (sig % 32);
}

static inline void sigdelset(sigset_t *set, int _sig)
{
        unsigned long sig = _sig - 1;
        if ((128 / 32) == 1)
                set->sig[0] &= ~(1UL << sig);
        else
                set->sig[sig / 32] &= ~(1UL << (sig % 32));
}

static inline int sigismember(sigset_t *set, int _sig)
{
        unsigned long sig = _sig - 1;
        if ((128 / 32) == 1)
                return 1 & (set->sig[0] >> sig);
        else
                return 1 & (set->sig[sig / 32] >> (sig % 32));
}

static inline int sigfindinword(unsigned long word)
{
        return ffz(~word);
}
# 108 "/opt/Wive/kernel/include/linux/signal.h"
static inline void sigorsets(sigset_t *r, const sigset_t *a, const sigset_t *b) { unsigned long a0, a1, a2, a3, b0, b1, b2, b3; unsigned long i; for (i = 0; i < (128 / 32)/4; ++i) { a0 = a->sig[4*i+0]; a1 = a->sig[4*i+1]; a2 = a->sig[4*i+2]; a3 = a->sig[4*i+3]; b0 = b->sig[4*i+0]; b1 = b->sig[4*i+1]; b2 = b->sig[4*i+2]; b3 = b->sig[4*i+3]; r->sig[4*i+0] = ((a0) | (b0)); r->sig[4*i+1] = ((a1) | (b1)); r->sig[4*i+2] = ((a2) | (b2)); r->sig[4*i+3] = ((a3) | (b3)); } switch ((128 / 32) % 4) { case 3: a0 = a->sig[4*i+0]; a1 = a->sig[4*i+1]; a2 = a->sig[4*i+2]; b0 = b->sig[4*i+0]; b1 = b->sig[4*i+1]; b2 = b->sig[4*i+2]; r->sig[4*i+0] = ((a0) | (b0)); r->sig[4*i+1] = ((a1) | (b1)); r->sig[4*i+2] = ((a2) | (b2)); break; case 2: a0 = a->sig[4*i+0]; a1 = a->sig[4*i+1]; b0 = b->sig[4*i+0]; b1 = b->sig[4*i+1]; r->sig[4*i+0] = ((a0) | (b0)); r->sig[4*i+1] = ((a1) | (b1)); break; case 1: a0 = a->sig[4*i+0]; b0 = b->sig[4*i+0]; r->sig[4*i+0] = ((a0) | (b0)); break; } }


static inline void sigandsets(sigset_t *r, const sigset_t *a, const sigset_t *b) { unsigned long a0, a1, a2, a3, b0, b1, b2, b3; unsigned long i; for (i = 0; i < (128 / 32)/4; ++i) { a0 = a->sig[4*i+0]; a1 = a->sig[4*i+1]; a2 = a->sig[4*i+2]; a3 = a->sig[4*i+3]; b0 = b->sig[4*i+0]; b1 = b->sig[4*i+1]; b2 = b->sig[4*i+2]; b3 = b->sig[4*i+3]; r->sig[4*i+0] = ((a0) & (b0)); r->sig[4*i+1] = ((a1) & (b1)); r->sig[4*i+2] = ((a2) & (b2)); r->sig[4*i+3] = ((a3) & (b3)); } switch ((128 / 32) % 4) { case 3: a0 = a->sig[4*i+0]; a1 = a->sig[4*i+1]; a2 = a->sig[4*i+2]; b0 = b->sig[4*i+0]; b1 = b->sig[4*i+1]; b2 = b->sig[4*i+2]; r->sig[4*i+0] = ((a0) & (b0)); r->sig[4*i+1] = ((a1) & (b1)); r->sig[4*i+2] = ((a2) & (b2)); break; case 2: a0 = a->sig[4*i+0]; a1 = a->sig[4*i+1]; b0 = b->sig[4*i+0]; b1 = b->sig[4*i+1]; r->sig[4*i+0] = ((a0) & (b0)); r->sig[4*i+1] = ((a1) & (b1)); break; case 1: a0 = a->sig[4*i+0]; b0 = b->sig[4*i+0]; r->sig[4*i+0] = ((a0) & (b0)); break; } }


static inline void signandsets(sigset_t *r, const sigset_t *a, const sigset_t *b) { unsigned long a0, a1, a2, a3, b0, b1, b2, b3; unsigned long i; for (i = 0; i < (128 / 32)/4; ++i) { a0 = a->sig[4*i+0]; a1 = a->sig[4*i+1]; a2 = a->sig[4*i+2]; a3 = a->sig[4*i+3]; b0 = b->sig[4*i+0]; b1 = b->sig[4*i+1]; b2 = b->sig[4*i+2]; b3 = b->sig[4*i+3]; r->sig[4*i+0] = ((a0) & ~(b0)); r->sig[4*i+1] = ((a1) & ~(b1)); r->sig[4*i+2] = ((a2) & ~(b2)); r->sig[4*i+3] = ((a3) & ~(b3)); } switch ((128 / 32) % 4) { case 3: a0 = a->sig[4*i+0]; a1 = a->sig[4*i+1]; a2 = a->sig[4*i+2]; b0 = b->sig[4*i+0]; b1 = b->sig[4*i+1]; b2 = b->sig[4*i+2]; r->sig[4*i+0] = ((a0) & ~(b0)); r->sig[4*i+1] = ((a1) & ~(b1)); r->sig[4*i+2] = ((a2) & ~(b2)); break; case 2: a0 = a->sig[4*i+0]; a1 = a->sig[4*i+1]; b0 = b->sig[4*i+0]; b1 = b->sig[4*i+1]; r->sig[4*i+0] = ((a0) & ~(b0)); r->sig[4*i+1] = ((a1) & ~(b1)); break; case 1: a0 = a->sig[4*i+0]; b0 = b->sig[4*i+0]; r->sig[4*i+0] = ((a0) & ~(b0)); break; } }
# 140 "/opt/Wive/kernel/include/linux/signal.h"
static inline void signotset(sigset_t *set) { unsigned long i; for (i = 0; i < (128 / 32)/4; ++i) { set->sig[4*i+0] = (~(set->sig[4*i+0])); set->sig[4*i+1] = (~(set->sig[4*i+1])); set->sig[4*i+2] = (~(set->sig[4*i+2])); set->sig[4*i+3] = (~(set->sig[4*i+3])); } switch ((128 / 32) % 4) { case 3: set->sig[4*i+2] = (~(set->sig[4*i+2])); case 2: set->sig[4*i+1] = (~(set->sig[4*i+1])); case 1: set->sig[4*i+0] = (~(set->sig[4*i+0])); } }




static inline void sigemptyset(sigset_t *set)
{
        switch ((128 / 32)) {
        default:
                memset(set, 0, sizeof(sigset_t));
                break;
        case 2: set->sig[1] = 0;
        case 1: set->sig[0] = 0;
                break;
        }
}

static inline void sigfillset(sigset_t *set)
{
        switch ((128 / 32)) {
        default:
                memset(set, -1, sizeof(sigset_t));
                break;
        case 2: set->sig[1] = -1;
        case 1: set->sig[0] = -1;
                break;
        }
}

extern char * render_sigset_t(sigset_t *set, char *buffer);



static inline void sigaddsetmask(sigset_t *set, unsigned long mask)
{
        set->sig[0] |= mask;
}

static inline void sigdelsetmask(sigset_t *set, unsigned long mask)
{
        set->sig[0] &= ~mask;
}

static inline int sigtestsetmask(sigset_t *set, unsigned long mask)
{
        return (set->sig[0] & mask) != 0;
}

static inline void siginitset(sigset_t *set, unsigned long mask)
{
        set->sig[0] = mask;
        switch ((128 / 32)) {
        default:
                memset(&set->sig[1], 0, sizeof(long)*((128 / 32)-1));
                break;
        case 2: set->sig[1] = 0;
        case 1: ;
        }
}

static inline void siginitsetinv(sigset_t *set, unsigned long mask)
{
        set->sig[0] = ~mask;
        switch ((128 / 32)) {
        default:
                memset(&set->sig[1], -1, sizeof(long)*((128 / 32)-1));
                break;
        case 2: set->sig[1] = -1;
        case 1: ;
        }
}



static inline void init_sigpending(struct sigpending *sig)
{
        sigemptyset(&sig->signal);
        sig->head = ((void *)0);
        sig->tail = &sig->head;
}

extern long do_sigpending(void *, unsigned long);
# 27 "/opt/Wive/kernel/include/linux/sched.h" 2
# 1 "/opt/Wive/kernel/include/linux/securebits.h" 1





extern unsigned securebits;
# 28 "/opt/Wive/kernel/include/linux/sched.h" 2
# 1 "/opt/Wive/kernel/include/linux/fs_struct.h" 1




struct fs_struct {
        atomic_t count;
        rwlock_t lock;
        int umask;
        struct dentry * root, * pwd, * altroot;
        struct vfsmount * rootmnt, * pwdmnt, * altrootmnt;
};
# 20 "/opt/Wive/kernel/include/linux/fs_struct.h"
extern void exit_fs(struct task_struct *);
extern void set_fs_altroot(void);

struct fs_struct *copy_fs_struct(struct fs_struct *old);
void put_fs_struct(struct fs_struct *fs);
# 29 "/opt/Wive/kernel/include/linux/sched.h" 2
# 1 "/opt/Wive/kernel/include/linux/lock_break.h" 1
# 30 "/opt/Wive/kernel/include/linux/sched.h" 2

struct exec_domain;
# 59 "/opt/Wive/kernel/include/linux/sched.h"
extern unsigned long avenrun[];
# 76 "/opt/Wive/kernel/include/linux/sched.h"
extern int nr_running, nr_threads;
extern int last_pid;



# 1 "/opt/Wive/kernel/include/linux/param.h" 1
# 82 "/opt/Wive/kernel/include/linux/sched.h" 2
# 1 "/opt/Wive/kernel/include/linux/resource.h" 1
# 21 "/opt/Wive/kernel/include/linux/resource.h"
struct rusage {
        struct timeval ru_utime;
        struct timeval ru_stime;
        long ru_maxrss;
        long ru_ixrss;
        long ru_idrss;
        long ru_isrss;
        long ru_minflt;
        long ru_majflt;
        long ru_nswap;
        long ru_inblock;
        long ru_oublock;
        long ru_msgsnd;
        long ru_msgrcv;
        long ru_nsignals;
        long ru_nvcsw;
        long ru_nivcsw;
};

struct rlimit {
        unsigned long rlim_cur;
        unsigned long rlim_max;
};
# 56 "/opt/Wive/kernel/include/linux/resource.h"
# 1 "/opt/Wive/kernel/include/asm/resource.h" 1
# 57 "/opt/Wive/kernel/include/linux/resource.h" 2
# 83 "/opt/Wive/kernel/include/linux/sched.h" 2
# 127 "/opt/Wive/kernel/include/linux/sched.h"
struct sched_param {
        int sched_priority;
};

struct completion;
# 143 "/opt/Wive/kernel/include/linux/sched.h"
extern rwlock_t tasklist_lock;
extern spinlock_t runqueue_lock;
extern spinlock_t mmlist_lock;

extern void sched_init(void);
extern void init_idle(void);
extern void show_state(void);
extern void cpu_init (void);
extern void trap_init(void);
extern void update_process_times(int user);
extern void update_one_process(struct task_struct *p, unsigned long user,
                               unsigned long system, int cpu);


extern signed long schedule_timeout(signed long timeout);
 void schedule(void);

extern int schedule_task(struct tq_struct *task);
extern void flush_scheduled_tasks(void);
extern int start_context_thread(void);
extern int current_is_keventd(void);
# 174 "/opt/Wive/kernel/include/linux/sched.h"
struct files_struct {
        atomic_t count;
        rwlock_t file_lock;
        int max_fds;
        int max_fdset;
        int next_fd;
        struct file ** fd;
        fd_set *close_on_exec;
        fd_set *open_fds;
        fd_set close_on_exec_init;
        fd_set open_fds_init;
        struct file * fd_array[32];
};
# 206 "/opt/Wive/kernel/include/linux/sched.h"
struct mm_struct {
        struct vm_area_struct * mmap;
        rb_root_t mm_rb;
        struct vm_area_struct * mmap_cache;
        pgd_t * pgd;
        atomic_t mm_users;
        atomic_t mm_count;
        int map_count;
        struct rw_semaphore mmap_sem;
        spinlock_t page_table_lock;

        struct list_head mmlist;




        unsigned long start_code, end_code, start_data, end_data;
        unsigned long start_brk, brk, start_stack;
        unsigned long arg_start, arg_end, env_start, env_end;
        unsigned long rss, total_vm, locked_vm;
        unsigned long def_flags;
        unsigned long cpu_vm_mask;
        unsigned long swap_address;

        unsigned dumpable:1;


        mm_context_t context;
};

extern int mmlist_nr;
# 249 "/opt/Wive/kernel/include/linux/sched.h"
struct signal_struct {
        atomic_t count;
        struct k_sigaction action[128];
        spinlock_t siglock;
};
# 265 "/opt/Wive/kernel/include/linux/sched.h"
struct user_struct {
        atomic_t __count;
        atomic_t processes;
        atomic_t files;


        struct user_struct *next, **pprev;
        uid_t uid;
};






extern struct user_struct root_user;


struct task_struct {



        volatile long state;
        int preempt_count;
        int sigpending;
        mm_segment_t addr_limit;



        struct exec_domain *exec_domain;
        volatile long need_resched;
        unsigned long ptrace;

        int lock_depth;






        long counter;
        long nice;
        unsigned long policy;
        struct mm_struct *mm;
        int processor;
# 318 "/opt/Wive/kernel/include/linux/sched.h"
        unsigned long cpus_runnable, cpus_allowed;




        struct list_head run_list;
        unsigned long sleep_time;

        struct task_struct *next_task, *prev_task;
        struct mm_struct *active_mm;
        struct list_head local_pages;
        unsigned int allocation_order, nr_local_pages;
        unsigned long flags;


        struct linux_binfmt *binfmt;
        int exit_code, exit_signal;
        int pdeath_signal;

        unsigned long personality;
        int did_exec:1;
        pid_t pid;
        pid_t pgrp;
        pid_t tty_old_pgrp;
        pid_t session;
        pid_t tgid;

        int leader;





        struct task_struct *p_opptr, *p_pptr, *p_cptr, *p_ysptr, *p_osptr;
        struct list_head thread_group;


        struct task_struct *pidhash_next;
        struct task_struct **pidhash_pprev;

        wait_queue_head_t wait_chldexit;
        struct completion *vfork_done;
        unsigned long rt_priority;
        unsigned long it_real_value, it_prof_value, it_virt_value;
        unsigned long it_real_incr, it_prof_incr, it_virt_incr;
        struct timer_list real_timer;
        struct tms times;
        unsigned long start_time;
        long per_cpu_utime[1], per_cpu_stime[1];

        unsigned long min_flt, maj_flt, nswap, cmin_flt, cmaj_flt, cnswap;
        int swappable:1;

        uid_t uid,euid,suid,fsuid;
        gid_t gid,egid,sgid,fsgid;
        int ngroups;
        gid_t groups[32];
        kernel_cap_t cap_effective, cap_inheritable, cap_permitted;
        int keep_capabilities:1;
        struct user_struct *user;

        struct rlimit rlim[11];
        unsigned short used_math;
        char comm[16];

        int link_count, total_link_count;
        struct tty_struct *tty;
        unsigned int locks;

        struct sem_undo *semundo;
        struct sem_queue *semsleeping;

        struct thread_struct thread;

        struct fs_struct *fs;

        struct files_struct *files;

        spinlock_t sigmask_lock;
        struct signal_struct *sig;

        sigset_t blocked;
        struct sigpending pending;

        unsigned long sas_ss_sp;
        size_t sas_ss_size;
        int (*notifier)(void *priv);
        void *notifier_data;
        sigset_t *notifier_mask;


        u32 parent_exec_id;
        u32 self_exec_id;

        spinlock_t alloc_lock;


        void *journal_info;



};
# 468 "/opt/Wive/kernel/include/linux/sched.h"
extern struct exec_domain default_exec_domain;
# 522 "/opt/Wive/kernel/include/linux/sched.h"
union task_union {
        struct task_struct task;
        unsigned long stack[2048*sizeof(long)/sizeof(long)];
};

extern union task_union init_task_union;

extern struct mm_struct init_mm;
extern struct task_struct *init_tasks[1];



extern struct task_struct *pidhash[(4096 >> 2)];



static inline void hash_pid(struct task_struct *p)
{
        struct task_struct **htable = &pidhash[((((p->pid) >> 8) ^ (p->pid)) & ((4096 >> 2) - 1))];

        if((p->pidhash_next = *htable) != ((void *)0))
                (*htable)->pidhash_pprev = &p->pidhash_next;
        *htable = p;
        p->pidhash_pprev = htable;
}

static inline void unhash_pid(struct task_struct *p)
{
        if(p->pidhash_next)
                p->pidhash_next->pidhash_pprev = p->pidhash_pprev;
        *p->pidhash_pprev = p->pidhash_next;
}

static inline struct task_struct *find_task_by_pid(int pid)
{
        struct task_struct *p, **htable = &pidhash[((((pid) >> 8) ^ (pid)) & ((4096 >> 2) - 1))];

        for(p = *htable; p && p->pid != pid; p = p->pidhash_next)
                ;

        return p;
}



static inline void task_set_cpu(struct task_struct *tsk, unsigned int cpu)
{
        tsk->processor = cpu;
        tsk->cpus_runnable = 1UL << cpu;
}

static inline void task_release_cpu(struct task_struct *tsk)
{
        tsk->cpus_runnable = ~0UL;
}


extern struct user_struct * alloc_uid(uid_t);
extern void free_uid(struct user_struct *);

# 1 "/opt/Wive/kernel/include/asm/current.h" 1
# 15 "/opt/Wive/kernel/include/asm/current.h"
register struct task_struct *current asm("$28");
# 583 "/opt/Wive/kernel/include/linux/sched.h" 2

extern unsigned long volatile jiffies;
extern unsigned long itimer_ticks;
extern unsigned long itimer_next;
extern struct timeval xtime;
extern void do_timer(struct pt_regs *);

extern unsigned int * prof_buffer;
extern unsigned long prof_len;
extern unsigned long prof_shift;



extern void __wake_up(wait_queue_head_t *q, unsigned int mode, int nr);
extern void __wake_up_sync(wait_queue_head_t *q, unsigned int mode, int nr);
extern void sleep_on(wait_queue_head_t *q);
extern long sleep_on_timeout(wait_queue_head_t *q, signed long timeout);

extern void interruptible_sleep_on(wait_queue_head_t *q);
extern long interruptible_sleep_on_timeout(wait_queue_head_t *q, signed long timeout);

extern int wake_up_process(struct task_struct * tsk);
# 616 "/opt/Wive/kernel/include/linux/sched.h"
 long sys_wait4(pid_t pid,unsigned int * stat_addr, int options, struct rusage * ru);

extern int in_group_p(gid_t);
extern int in_egroup_p(gid_t);

extern __attribute__((noreturn)) void cpu_idle(void);

extern void release_task(struct task_struct * p);

extern void proc_caches_init(void);
extern void flush_signals(struct task_struct *);
extern void flush_signal_handlers(struct task_struct *);
extern int dequeue_signal(sigset_t *, siginfo_t *);
extern void block_all_signals(int (*notifier)(void *priv), void *priv,
                              sigset_t *mask);
extern void unblock_all_signals(void);
extern int send_sig_info(int, struct siginfo *, struct task_struct *);
extern int force_sig_info(int, struct siginfo *, struct task_struct *);
extern int kill_pg_info(int, struct siginfo *, pid_t);
extern int kill_sl_info(int, struct siginfo *, pid_t);
extern int kill_proc_info(int, struct siginfo *, pid_t);
extern void notify_parent(struct task_struct *, int);
extern void do_notify_parent(struct task_struct *, int);
extern void force_sig(int, struct task_struct *);
extern int send_sig(int, struct task_struct *, int);
extern int kill_pg(pid_t, int, int);
extern int kill_sl(pid_t, int, int);
extern int kill_proc(pid_t, int, int);
extern int do_sigaction(int, const struct k_sigaction *, struct k_sigaction *);
extern int do_sigaltstack(const stack_t *, stack_t *, unsigned long);

static inline int signal_pending(struct task_struct *p)
{
        return (p->sigpending != 0);
}





static inline int has_pending_signals(sigset_t *signal, sigset_t *blocked)
{
        unsigned long ready;
        long i;

        switch ((128 / 32)) {
        default:
                for (i = (128 / 32), ready = 0; --i >= 0 ;)
                        ready |= signal->sig[i] &~ blocked->sig[i];
                break;

        case 4: ready = signal->sig[3] &~ blocked->sig[3];
                ready |= signal->sig[2] &~ blocked->sig[2];
                ready |= signal->sig[1] &~ blocked->sig[1];
                ready |= signal->sig[0] &~ blocked->sig[0];
                break;

        case 2: ready = signal->sig[1] &~ blocked->sig[1];
                ready |= signal->sig[0] &~ blocked->sig[0];
                break;

        case 1: ready = signal->sig[0] &~ blocked->sig[0];
        }
        return ready != 0;
}





static inline void recalc_sigpending(struct task_struct *t)
{
        t->sigpending = has_pending_signals(&t->pending.signal, &t->blocked);
}



static inline int on_sig_stack(unsigned long sp)
{
        return (sp - current->sas_ss_sp < current->sas_ss_size);
}

static inline int sas_ss_flags(unsigned long sp)
{
        return (current->sas_ss_size == 0 ? 2
                : on_sig_stack(sp) ? 1 : 0);
}

extern int request_irq(unsigned int,
                       void (*handler)(int, void *, struct pt_regs *),
                       unsigned long, const char *, void *);
extern void free_irq(unsigned int, void *);
# 723 "/opt/Wive/kernel/include/linux/sched.h"
static inline int suser(void)
{
        if (!( (1 << (0 +1)) & 0x00000000 ? (1 << (0)) & 0x00000000 : (1 << (0)) & securebits ) && current->euid == 0) {
                current->flags |= 0x00000100;
                return 1;
        }
        return 0;
}

static inline int fsuser(void)
{
        if (!( (1 << (0 +1)) & 0x00000000 ? (1 << (0)) & 0x00000000 : (1 << (0)) & securebits ) && current->fsuid == 0) {
                current->flags |= 0x00000100;
                return 1;
        }
        return 0;
}







static inline int capable(int cap)
{

        if (((current->cap_effective) & (1 << (cap))))



        {
                current->flags |= 0x00000100;
                return 1;
        }
        return 0;
}




extern struct mm_struct * mm_alloc(void);

extern struct mm_struct * start_lazy_tlb(void);
extern void end_lazy_tlb(struct mm_struct *mm);


extern inline void __mmdrop(struct mm_struct *);
static inline void mmdrop(struct mm_struct * mm)
{
        if ((atomic_sub_return(1, (&mm->mm_count)) == 0))
                __mmdrop(mm);
}


extern void mmput(struct mm_struct *);

extern void mm_release(void);




extern struct file ** alloc_fd_array(int);
extern int expand_fd_array(struct files_struct *, int nr);
extern void free_fd_array(struct file **, int);

extern fd_set *alloc_fdset(int);
extern int expand_fdset(struct files_struct *, int nr);
extern void free_fdset(fd_set *, int);

extern int copy_thread(int, unsigned long, unsigned long, unsigned long, struct task_struct *, struct pt_regs *);
extern void flush_thread(void);
extern void exit_thread(void);

extern void exit_mm(struct task_struct *);
extern void exit_files(struct task_struct *);
extern void exit_sighand(struct task_struct *);

extern void reparent_to_init(void);
extern void daemonize(void);

extern int do_execve(char *, char **, char **, struct pt_regs *);
extern int do_fork(unsigned long, unsigned long, struct pt_regs *, unsigned long);

extern void add_wait_queue(wait_queue_head_t *q, wait_queue_t * wait);
extern void add_wait_queue_exclusive(wait_queue_head_t *q, wait_queue_t * wait);
extern void remove_wait_queue(wait_queue_head_t *q, wait_queue_t * wait);
# 891 "/opt/Wive/kernel/include/linux/sched.h"
static inline void del_from_runqueue(struct task_struct * p)
{
        nr_running--;
        p->sleep_time = jiffies;
        list_del(&p->run_list);
        p->run_list.next = ((void *)0);
}

static inline int task_on_runqueue(struct task_struct *p)
{
        return (p->run_list.next != ((void *)0));
}

static inline void unhash_process(struct task_struct *p)
{
        if (task_on_runqueue(p)) __out_of_line_bug(906);
        do { __cli(); (void)(&tasklist_lock); } while (0);
        nr_threads--;
        unhash_pid(p);
        do { (p)->next_task->prev_task = (p)->prev_task; (p)->prev_task->next_task = (p)->next_task; if ((p)->p_osptr) (p)->p_osptr->p_ysptr = (p)->p_ysptr; if ((p)->p_ysptr) (p)->p_ysptr->p_osptr = (p)->p_osptr; else (p)->p_pptr->p_cptr = (p)->p_osptr; } while (0);
        list_del(&p->thread_group);
        do { do { } while(0); __sti(); } while (0);
}


static inline void task_lock(struct task_struct *p)
{
        (void)(&p->alloc_lock);
}

static inline void task_unlock(struct task_struct *p)
{
        do { } while(0);
}


static inline char * d_path(struct dentry *dentry, struct vfsmount *vfsmnt,
                                char *buf, int buflen)
{
        char *res;
        struct vfsmount *rootmnt;
        struct dentry *root;
        (void)(&current->fs->lock);
        rootmnt = mntget(current->fs->rootmnt);
        root = dget(current->fs->root);
        do { } while(0);
        (void)(&dcache_lock);
        res = __d_path(dentry, vfsmnt, root, rootmnt, buf, buflen);
        do { } while(0);
        dput(root);
        mntput(rootmnt);
        return res;
}


# 1 "/opt/Wive/kernel/include/linux/dcache.h" 1
# 276 "/opt/Wive/kernel/include/linux/dcache.h"
static __inline__ void d_drop(struct dentry * dentry)
{
        (void)(&dcache_lock);
        list_del(&dentry->d_hash);
        do { (&dentry->d_hash)->next = (&dentry->d_hash); (&dentry->d_hash)->prev = (&dentry->d_hash); } while (0);
        do { } while(0);
}
# 947 "/opt/Wive/kernel/include/linux/sched.h" 2
# 1 "/opt/Wive/kernel/include/linux/tqueue.h" 1
# 116 "/opt/Wive/kernel/include/linux/tqueue.h"
static inline int queue_task(struct tq_struct *bh_pointer, task_queue *bh_list)
{
        int ret = 0;
        if (!test_and_set_bit(0,&bh_pointer->sync)) {
                unsigned long flags;
                do { __asm__ __volatile__( "__save_and_cli\t%0" : "=r" (flags) : : "memory"); (void)(&tqueue_lock); } while (0);
                list_add_tail(&bh_pointer->list, bh_list);
                do { do { } while(0); do { unsigned long __tmp1; __asm__ __volatile__( "__restore_flags\t%0" : "=r" (__tmp1) : "0" (flags) : "memory"); } while(0); } while (0);
                ret = 1;
        }
        return ret;
}
# 948 "/opt/Wive/kernel/include/linux/sched.h" 2
# 1 "/opt/Wive/kernel/include/linux/fs_struct.h" 1
# 37 "/opt/Wive/kernel/include/linux/fs_struct.h"
static inline void set_fs_root(struct fs_struct *fs,
        struct vfsmount *mnt,
        struct dentry *dentry)
{
        struct dentry *old_root;
        struct vfsmount *old_rootmnt;
        (void)(&fs->lock);
        old_root = fs->root;
        old_rootmnt = fs->rootmnt;
        fs->rootmnt = mntget(mnt);
        fs->root = dget(dentry);
        do { } while(0);
        if (old_root) {
                dput(old_root);
                mntput(old_rootmnt);
        }
}






static inline void set_fs_pwd(struct fs_struct *fs,
        struct vfsmount *mnt,
        struct dentry *dentry)
{
        struct dentry *old_pwd;
        struct vfsmount *old_pwdmnt;
        (void)(&fs->lock);
        old_pwd = fs->pwd;
        old_pwdmnt = fs->pwdmnt;
        fs->pwdmnt = mntget(mnt);
        fs->pwd = dget(dentry);
        do { } while(0);
        if (old_pwd) {
                dput(old_pwd);
                mntput(old_pwdmnt);
        }
}
# 949 "/opt/Wive/kernel/include/linux/sched.h" 2
# 5 "/opt/Wive/kernel/include/linux/mm.h" 2







# 1 "/opt/Wive/kernel/include/linux/mmzone.h" 1
# 21 "/opt/Wive/kernel/include/linux/mmzone.h"
typedef struct free_area_struct {
        struct list_head free_list;
        unsigned long *map;
} free_area_t;

struct pglist_data;
# 36 "/opt/Wive/kernel/include/linux/mmzone.h"
typedef struct zone_struct {



        spinlock_t lock;
        unsigned long free_pages;
        unsigned long pages_min, pages_low, pages_high;
        int need_balance;




        free_area_t free_area[10];




        struct pglist_data *zone_pgdat;
        struct page *zone_mem_map;
        unsigned long zone_start_paddr;
        unsigned long zone_start_mapnr;




        char *name;
        unsigned long size;
} zone_t;
# 81 "/opt/Wive/kernel/include/linux/mmzone.h"
typedef struct zonelist_struct {
        zone_t * zones [3 +1];
} zonelist_t;
# 98 "/opt/Wive/kernel/include/linux/mmzone.h"
struct bootmem_data;
typedef struct pglist_data {
        zone_t node_zones[3];
        zonelist_t node_zonelists[0x0f +1];
        int nr_zones;
        struct page *node_mem_map;
        unsigned long *valid_addr_bitmap;
        struct bootmem_data *bdata;
        unsigned long node_start_paddr;
        unsigned long node_start_mapnr;
        unsigned long node_size;
        int node_id;
        struct pglist_data *node_next;
} pg_data_t;

extern int numnodes;
extern pg_data_t *pgdat_list;
# 123 "/opt/Wive/kernel/include/linux/mmzone.h"
struct page;
extern void show_free_areas_core(pg_data_t *pgdat);
extern void free_area_init_core(int nid, pg_data_t *pgdat, struct page **gmap,
  unsigned long *zones_size, unsigned long paddr, unsigned long *zholes_size,
  struct page *pmap);

extern pg_data_t contig_page_data;
# 13 "/opt/Wive/kernel/include/linux/mm.h" 2
# 1 "/opt/Wive/kernel/include/linux/swap.h" 1
# 25 "/opt/Wive/kernel/include/linux/swap.h"
union swap_header {
        struct
        {
                char reserved[(1L << 12) - 10];
                char magic[10];
        } magic;
        struct
        {
                char bootbits[1024];
                unsigned int version;
                unsigned int last_page;
                unsigned int nr_badpages;
                unsigned int padding[125];
                unsigned int badpages[1];
        } info;
};
# 64 "/opt/Wive/kernel/include/linux/swap.h"
struct swap_info_struct {
        unsigned int flags;
        kdev_t swap_device;
        spinlock_t sdev_lock;
        struct dentry * swap_file;
        struct vfsmount *swap_vfsmnt;
        unsigned short * swap_map;
        unsigned int lowest_bit;
        unsigned int highest_bit;
        unsigned int cluster_next;
        unsigned int cluster_nr;
        int prio;
        int pages;
        unsigned long max;
        int next;
};

extern int nr_swap_pages;




extern unsigned int nr_free_pages(void);
extern unsigned int nr_free_buffer_pages(void);
extern int nr_active_pages;
extern int nr_inactive_pages;
extern atomic_t nr_async_pages;
extern atomic_t page_cache_size;
extern atomic_t buffermem_pages;
extern spinlock_t pagecache_lock;
extern void __remove_inode_page(struct page *);


struct task_struct;
struct vm_area_struct;
struct sysinfo;

struct zone_t;


extern void lru_cache_add(struct page *);
extern void __lru_cache_del(struct page *);
extern void lru_cache_del(struct page *);

extern void activate_page(struct page *);

extern void swap_setup(void);


extern wait_queue_head_t kswapd_wait;
extern int try_to_free_pages(zone_t *, unsigned int, unsigned int);


extern void rw_swap_page(int, struct page *);
extern void rw_swap_page_nolock(int, swp_entry_t, char *);






extern void show_swap_cache_info(void);

extern int add_to_swap_cache(struct page *, swp_entry_t);
extern void __delete_from_swap_cache(struct page *page);
extern void delete_from_swap_cache(struct page *page);
extern void free_page_and_swap_cache(struct page *page);
extern struct page * lookup_swap_cache(swp_entry_t);
extern struct page * read_swap_cache_async(swp_entry_t);


extern void out_of_memory(void);


extern int total_swap_pages;
extern unsigned int nr_swapfiles;
extern struct swap_info_struct swap_info[];
extern int is_swap_partition(kdev_t);
extern void si_swapinfo(struct sysinfo *);
extern swp_entry_t get_swap_page(void);
extern void get_swaphandle_info(swp_entry_t, unsigned long *, kdev_t *,
                                        struct inode **);
extern int swap_duplicate(swp_entry_t);
extern int swap_count(struct page *);
extern int valid_swaphandles(swp_entry_t, unsigned long *);
extern void swap_free(swp_entry_t);
extern void free_swap_and_cache(swp_entry_t);
struct swap_list_t {
        int head;
        int next;
};
extern struct swap_list_t swap_list;
 long sys_swapoff(const char *);
 long sys_swapon(const char *, int);

extern spinlock_t pagemap_lru_lock;

extern void mark_page_accessed(struct page *);
# 203 "/opt/Wive/kernel/include/linux/swap.h"
extern spinlock_t swaplock;






extern void shmem_unuse(swp_entry_t entry, struct page *page);
# 14 "/opt/Wive/kernel/include/linux/mm.h" 2


extern unsigned long max_mapnr;
extern unsigned long num_physpages;
extern void * high_memory;
extern int page_cluster;

extern struct list_head active_list;
extern struct list_head inactive_list;


# 1 "/opt/Wive/kernel/include/asm/pgtable.h" 1
# 13 "/opt/Wive/kernel/include/asm/pgtable.h"
# 1 "/opt/Wive/kernel/include/asm/addrspace.h" 1
# 14 "/opt/Wive/kernel/include/asm/pgtable.h" 2






# 1 "/opt/Wive/kernel/include/asm/fixmap.h" 1
# 49 "/opt/Wive/kernel/include/asm/fixmap.h"
enum fixed_addresses {




        __end_of_fixed_addresses
};

extern void __set_fixmap (enum fixed_addresses idx,
                                        unsigned long phys, pgprot_t flags);
# 80 "/opt/Wive/kernel/include/asm/fixmap.h"
extern void __this_fixmap_does_not_exist(void);






static inline unsigned long fix_to_virt(const unsigned int idx)
{
# 98 "/opt/Wive/kernel/include/asm/fixmap.h"
        if (idx >= __end_of_fixed_addresses)
                __this_fixmap_does_not_exist();

        return ((0xffffe000UL) - ((idx) << 12));
}
# 21 "/opt/Wive/kernel/include/asm/pgtable.h" 2
# 34 "/opt/Wive/kernel/include/asm/pgtable.h"
extern void (*_flush_cache_all)(void);
extern void (*___flush_cache_all)(void);
extern void (*_flush_cache_mm)(struct mm_struct *mm);
extern void (*_flush_cache_range)(struct mm_struct *mm, unsigned long start,
        unsigned long end);
extern void (*_flush_cache_page)(struct vm_area_struct *vma,
        unsigned long page);
extern void (*_flush_page_to_ram)(struct page * page);
extern void (*_flush_icache_range)(unsigned long start, unsigned long end);
extern void (*_flush_icache_page)(struct vm_area_struct *vma,
        struct page *page);
extern void (*_flush_cache_sigtramp)(unsigned long addr);
extern void (*_flush_icache_all)(void);
# 70 "/opt/Wive/kernel/include/asm/pgtable.h"
extern void add_wired_entry(unsigned long entrylo0, unsigned long entrylo1,
                               unsigned long entryhi, unsigned long pagemask);
# 80 "/opt/Wive/kernel/include/asm/pgtable.h"
extern int add_temporary_entry(unsigned long entrylo0, unsigned long entrylo1,
                               unsigned long entryhi, unsigned long pagemask);
# 268 "/opt/Wive/kernel/include/asm/pgtable.h"
extern unsigned long empty_zero_page;
extern unsigned long zero_page_mask;




extern void load_pgd(unsigned long pg_dir);

extern pmd_t invalid_pte_table[(1L << 12)/sizeof(pmd_t)];





static inline unsigned long pmd_page(pmd_t pmd)
{
        return ((pmd).pmd);
}

static inline void pmd_set(pmd_t * pmdp, pte_t * ptep)
{
        ((*pmdp).pmd) = (((unsigned long) ptep) & (~((1L << 12)-1)));
}

static inline int pte_none(pte_t pte) { return !((pte).pte); }
static inline int pte_present(pte_t pte) { return ((pte).pte) & (1<<0); }





static inline void set_pte(pte_t *ptep, pte_t pteval)
{
        *ptep = pteval;
}

static inline void pte_clear(pte_t *ptep)
{
        set_pte(ptep, ((pte_t) { (0) } ));
}
# 319 "/opt/Wive/kernel/include/asm/pgtable.h"
static inline int pmd_none(pmd_t pmd)
{
        return ((pmd).pmd) == (unsigned long) invalid_pte_table;
}

static inline int pmd_bad(pmd_t pmd)
{
        return ((pmd_page(pmd) > (unsigned long) high_memory) ||
                (pmd_page(pmd) < 0x80000000UL));
}

static inline int pmd_present(pmd_t pmd)
{
        return (((pmd).pmd) != (unsigned long) invalid_pte_table);
}

static inline void pmd_clear(pmd_t *pmdp)
{
        ((*pmdp).pmd) = ((unsigned long) invalid_pte_table);
}






static inline int pgd_none(pgd_t pgd) { return 0; }
static inline int pgd_bad(pgd_t pgd) { return 0; }
static inline int pgd_present(pgd_t pgd) { return 1; }
static inline void pgd_clear(pgd_t *pgdp) { }
# 365 "/opt/Wive/kernel/include/asm/pgtable.h"
static inline int pte_read(pte_t pte) { return ((pte).pte) & (1<<1); }
static inline int pte_write(pte_t pte) { return ((pte).pte) & (1<<2); }
static inline int pte_dirty(pte_t pte) { return ((pte).pte) & (1<<4); }
static inline int pte_young(pte_t pte) { return ((pte).pte) & (1<<3); }

static inline pte_t pte_wrprotect(pte_t pte)
{
        ((pte).pte) &= ~((1<<2) | (1<<10));
        return pte;
}

static inline pte_t pte_rdprotect(pte_t pte)
{
        ((pte).pte) &= ~((1<<1) | (1<<9));
        return pte;
}

static inline pte_t pte_mkclean(pte_t pte)
{
        ((pte).pte) &= ~((1<<4)|(1<<10));
        return pte;
}

static inline pte_t pte_mkold(pte_t pte)
{
        ((pte).pte) &= ~((1<<3)|(1<<9));
        return pte;
}

static inline pte_t pte_mkwrite(pte_t pte)
{
        ((pte).pte) |= (1<<2);
        if (((pte).pte) & (1<<4))
                ((pte).pte) |= (1<<10);
        return pte;
}

static inline pte_t pte_mkread(pte_t pte)
{
        ((pte).pte) |= (1<<1);
        if (((pte).pte) & (1<<3))
                ((pte).pte) |= (1<<9);
        return pte;
}

static inline pte_t pte_mkdirty(pte_t pte)
{
        ((pte).pte) |= (1<<4);
        if (((pte).pte) & (1<<2))
                ((pte).pte) |= (1<<10);
        return pte;
}
# 426 "/opt/Wive/kernel/include/asm/pgtable.h"
static inline pgprot_t pgprot_noncached(pgprot_t _prot)
{
        unsigned long prot = ((_prot).pgprot);

        prot = (prot & ~(1<<11)) | (1<<11);

        return ((pgprot_t) { (prot) } );
}

static inline pte_t pte_mkyoung(pte_t pte)
{
        ((pte).pte) |= (1<<3);
        if (((pte).pte) & (1<<1))
                ((pte).pte) |= (1<<9);
        return pte;
}
# 470 "/opt/Wive/kernel/include/asm/pgtable.h"
static inline pte_t mk_pte_phys(phys_t physpage, pgprot_t pgprot)
{



        return ((pte_t) { (physpage | ((pgprot).pgprot)) } );

}

static inline pte_t pte_modify(pte_t pte, pgprot_t newprot)
{
        return ((pte_t) { ((((pte).pte) & ((~((1L << 12)-1)) | (1<<3) | (1<<4) | (1<<11))) | ((newprot).pgprot)) } );
}
# 496 "/opt/Wive/kernel/include/asm/pgtable.h"
static inline pgd_t *pgd_offset(struct mm_struct *mm, unsigned long address)
{
        return mm->pgd + ((address) >> 22);
}


static inline pmd_t *pmd_offset(pgd_t *dir, unsigned long address)
{
        return (pmd_t *) dir;
}


static inline pte_t *pte_offset(pmd_t * dir, unsigned long address)
{
        return (pte_t *) (pmd_page(*dir)) +
               ((address >> 12) & (1024 - 1));
}

extern int do_check_pgt_cache(int, int);

extern pgd_t swapper_pg_dir[1024];
extern void paging_init(void);

extern void update_mmu_cache(struct vm_area_struct *vma,
                                unsigned long address, pte_t pte);
# 533 "/opt/Wive/kernel/include/asm/pgtable.h"
# 1 "/opt/Wive/kernel/include/asm-generic/pgtable.h" 1



static inline int ptep_test_and_clear_young(pte_t *ptep)
{
        pte_t pte = *ptep;
        if (!pte_young(pte))
                return 0;
        set_pte(ptep, pte_mkold(pte));
        return 1;
}

static inline int ptep_test_and_clear_dirty(pte_t *ptep)
{
        pte_t pte = *ptep;
        if (!pte_dirty(pte))
                return 0;
        set_pte(ptep, pte_mkclean(pte));
        return 1;
}

static inline pte_t ptep_get_and_clear(pte_t *ptep)
{
        pte_t pte = *ptep;
        pte_clear(ptep);
        return pte;
}

static inline void ptep_set_wrprotect(pte_t *ptep)
{
        pte_t old_pte = *ptep;
        set_pte(ptep, pte_wrprotect(old_pte));
}

static inline void ptep_mkdirty(pte_t *ptep)
{
        pte_t old_pte = *ptep;
        set_pte(ptep, pte_mkdirty(old_pte));
}
# 534 "/opt/Wive/kernel/include/asm/pgtable.h" 2
# 26 "/opt/Wive/kernel/include/linux/mm.h" 2
# 43 "/opt/Wive/kernel/include/linux/mm.h"
struct vm_area_struct {
        struct mm_struct * vm_mm;
        unsigned long vm_start;
        unsigned long vm_end;



        struct vm_area_struct *vm_next;

        pgprot_t vm_page_prot;
        unsigned long vm_flags;

        rb_node_t vm_rb;






        struct vm_area_struct *vm_next_share;
        struct vm_area_struct **vm_pprev_share;


        struct vm_operations_struct * vm_ops;


        unsigned long vm_pgoff;

        struct file * vm_file;
        unsigned long vm_raend;
        void * vm_private_data;
};
# 115 "/opt/Wive/kernel/include/linux/mm.h"
extern int vm_min_readahead;
extern int vm_max_readahead;





extern pgprot_t protection_map[16];
# 133 "/opt/Wive/kernel/include/linux/mm.h"
struct vm_operations_struct {
        void (*open)(struct vm_area_struct * area);
        void (*close)(struct vm_area_struct * area);
        struct page * (*nopage)(struct vm_area_struct * area, unsigned long address, int unused);
};
# 154 "/opt/Wive/kernel/include/linux/mm.h"
typedef struct page {
        struct list_head list;
        struct address_space *mapping;
        unsigned long index;
        struct page *next_hash;

        atomic_t count;
        unsigned long flags;

        struct list_head lru;

        wait_queue_head_t wait;
        struct page **pprev_hash;
        struct buffer_head * buffers;
        void *virtual;

        struct zone_struct *zone;
} mem_map_t;
# 308 "/opt/Wive/kernel/include/linux/mm.h"
extern void set_page_dirty(struct page *);
# 352 "/opt/Wive/kernel/include/linux/mm.h"
extern mem_map_t * mem_map;







extern struct page * _alloc_pages(unsigned int gfp_mask, unsigned int order);
extern struct page * __alloc_pages(unsigned int gfp_mask, unsigned int order, zonelist_t *zonelist);
extern struct page * alloc_pages_node(int nid, unsigned int gfp_mask, unsigned int order);

static inline struct page * alloc_pages(unsigned int gfp_mask, unsigned int order)
{



        if (order >= 10)
                return ((void *)0);
        return _alloc_pages(gfp_mask, order);
}



extern unsigned long __get_free_pages(unsigned int gfp_mask, unsigned int order);
extern unsigned long get_zeroed_page(unsigned int gfp_mask);
# 393 "/opt/Wive/kernel/include/linux/mm.h"
extern void __free_pages(struct page *page, unsigned int order);
extern void free_pages(unsigned long addr, unsigned int order);




extern void show_free_areas(void);
extern void show_free_areas_node(pg_data_t *pgdat);

extern void clear_page_tables(struct mm_struct *, unsigned long, int);

extern int fail_writepage(struct page *);
struct page * shmem_nopage(struct vm_area_struct * vma, unsigned long address, int unused);
struct file *shmem_file_setup(char * name, loff_t size);
extern void shmem_lock(struct file * file, int lock);
extern int shmem_zero_setup(struct vm_area_struct *);

extern void zap_page_range(struct mm_struct *mm, unsigned long address, unsigned long size, int actions);
extern int copy_page_range(struct mm_struct *dst, struct mm_struct *src, struct vm_area_struct *vma);
extern int remap_page_range(unsigned long from, unsigned long to, unsigned long size, pgprot_t prot);
extern int zeromap_page_range(unsigned long from, unsigned long size, pgprot_t prot);

extern int vmtruncate(struct inode * inode, loff_t offset);
extern pmd_t *__pmd_alloc(struct mm_struct *mm, pgd_t *pgd, unsigned long address);
extern pte_t *pte_alloc(struct mm_struct *mm, pmd_t *pmd, unsigned long address);
extern int handle_mm_fault(struct mm_struct *mm,struct vm_area_struct *vma, unsigned long address, int write_access);
extern int make_pages_present(unsigned long addr, unsigned long end);
extern int access_process_vm(struct task_struct *tsk, unsigned long addr, void *buf, int len, int write);
extern int ptrace_readdata(struct task_struct *tsk, unsigned long src, char *dst, int len);
extern int ptrace_writedata(struct task_struct *tsk, char * src, unsigned long dst, int len);
extern int ptrace_attach(struct task_struct *tsk);
extern int ptrace_detach(struct task_struct *, unsigned int);
extern void ptrace_disable(struct task_struct *);
extern int ptrace_check_attach(struct task_struct *task, int kill);

int get_user_pages(struct task_struct *tsk, struct mm_struct *mm, unsigned long start,
                int len, int write, int force, struct page **pages, struct vm_area_struct **vmas);






static inline pmd_t *pmd_alloc(struct mm_struct *mm, pgd_t *pgd, unsigned long address)
{
        if (pgd_none(*pgd))
                return __pmd_alloc(mm, pgd, address);
        return pmd_offset(pgd, address);
}

extern int pgt_cache_water[2];
extern int check_pgt_cache(void);

extern void free_area_init(unsigned long * zones_size);
extern void free_area_init_node(int nid, pg_data_t *pgdat, struct page *pmap,
        unsigned long * zones_size, unsigned long zone_start_paddr,
        unsigned long *zholes_size);
extern void mem_init(void);
extern void show_mem(void);
extern void si_meminfo(struct sysinfo * val);
extern void swapin_readahead(swp_entry_t);

extern struct address_space swapper_space;


static inline int is_page_cache_freeable(struct page * page)
{
        return ((&(page)->count)->counter) - !!page->buffers == 1;
}

extern int can_share_swap_page(struct page *);
extern int remove_exclusive_swap_page(struct page *);

extern void __free_pte(pte_t);


extern void lock_vma_mappings(struct vm_area_struct *);
extern void unlock_vma_mappings(struct vm_area_struct *);
extern void insert_vm_struct(struct mm_struct *, struct vm_area_struct *);
extern void __insert_vm_struct(struct mm_struct *, struct vm_area_struct *);
extern void build_mmap_rb(struct mm_struct *);
extern void exit_mmap(struct mm_struct *);

extern unsigned long get_unmapped_area(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);

extern unsigned long do_mmap_pgoff(struct file *file, unsigned long addr,
        unsigned long len, unsigned long prot,
        unsigned long flag, unsigned long pgoff);

static inline unsigned long do_mmap(struct file *file, unsigned long addr,
        unsigned long len, unsigned long prot,
        unsigned long flag, unsigned long offset)
{
        unsigned long ret = -22;
        if ((offset + (((len) + (1L << 12) - 1) & (~((1L << 12)-1)))) < offset)
                goto out;
        if (!(offset & ~(~((1L << 12)-1))))
                ret = do_mmap_pgoff(file, addr, len, prot, flag, offset >> 12);
out:
        return ret;
}

extern int do_munmap(struct mm_struct *, unsigned long, size_t);

extern unsigned long do_brk(unsigned long, unsigned long);

static inline void __vma_unlink(struct mm_struct * mm, struct vm_area_struct * vma, struct vm_area_struct * prev)
{
        prev->vm_next = vma->vm_next;
        rb_erase(&vma->vm_rb, &mm->mm_rb);
        if (mm->mmap_cache == vma)
                mm->mmap_cache = prev;
}

static inline int can_vma_merge(struct vm_area_struct * vma, unsigned long vm_flags)
{
        if (!vma->vm_file && vma->vm_flags == vm_flags)
                return 1;
        else
                return 0;
}

struct zone_t;

extern void remove_inode_page(struct page *);
extern unsigned long page_unuse(struct page *);
extern void truncate_inode_pages(struct address_space *, loff_t);


extern int filemap_sync(struct vm_area_struct *, unsigned long, size_t, unsigned int);
extern struct page *filemap_nopage(struct vm_area_struct *, unsigned long, int);
# 554 "/opt/Wive/kernel/include/linux/mm.h"
static inline unsigned int pf_gfp_mask(unsigned int gfp_mask)
{

        if (current->flags & 0x00004000)
                gfp_mask &= ~(0x40 | 0x80 | 0x100);

        return gfp_mask;
}



static inline int expand_stack(struct vm_area_struct * vma, unsigned long address)
{
        unsigned long grow;






        address &= (~((1L << 12)-1));
        (void)(&vma->vm_mm->page_table_lock);
        grow = (vma->vm_start - address) >> 12;
        if (vma->vm_end - address > current->rlim[3].rlim_cur ||
            ((vma->vm_mm->total_vm + grow) << 12) > current->rlim[6].rlim_cur) {
                do { } while(0);
                return -12;
        }
        vma->vm_start = address;
        vma->vm_pgoff -= grow;
        vma->vm_mm->total_vm += grow;
        if (vma->vm_flags & 0x00002000)
                vma->vm_mm->locked_vm += grow;
        do { } while(0);
        return 0;
}


extern struct vm_area_struct * find_vma(struct mm_struct * mm, unsigned long addr);
extern struct vm_area_struct * find_vma_prev(struct mm_struct * mm, unsigned long addr,
                                             struct vm_area_struct **pprev);



static inline struct vm_area_struct * find_vma_intersection(struct mm_struct * mm, unsigned long start_addr, unsigned long end_addr)
{
        struct vm_area_struct * vma = find_vma(mm,start_addr);

        if (vma && end_addr <= vma->vm_start)
                vma = ((void *)0);
        return vma;
}

extern struct vm_area_struct *find_extend_vma(struct mm_struct *mm, unsigned long addr);
# 6 "/opt/Wive/kernel/include/linux/locks.h" 2


# 1 "/opt/Wive/kernel/include/linux/pagemap.h" 1
# 16 "/opt/Wive/kernel/include/linux/pagemap.h"
# 1 "/opt/Wive/kernel/include/linux/highmem.h" 1




# 1 "/opt/Wive/kernel/include/asm/pgalloc.h" 1
# 25 "/opt/Wive/kernel/include/asm/pgalloc.h"
extern void local_flush_tlb_all(void);
extern void local_flush_tlb_mm(struct mm_struct *mm);
extern void local_flush_tlb_range(struct mm_struct *mm, unsigned long start,
                               unsigned long end);
extern void local_flush_tlb_page(struct vm_area_struct *vma,
                                 unsigned long page);
extern void local_flush_tlb_one(unsigned long page);
# 49 "/opt/Wive/kernel/include/asm/pgalloc.h"
static inline void flush_tlb_pgtables(struct mm_struct *mm,
                                      unsigned long start, unsigned long end)
{

}
# 70 "/opt/Wive/kernel/include/asm/pgalloc.h"
extern void pgd_init(unsigned long page);

static __inline__ pgd_t *get_pgd_slow(void)
{
        pgd_t *ret = (pgd_t *)__get_free_pages((0x20 | 0x10 | 0x40 | 0x80 | 0x100), 0), *init;

        if (ret) {
                init = pgd_offset(&init_mm, 0);
                pgd_init((unsigned long)ret);
                memcpy (ret + (0x80000000UL/(1UL << 22)), init + (0x80000000UL/(1UL << 22)),
                        (1024 - (0x80000000UL/(1UL << 22))) * sizeof(pgd_t));
        }
        return ret;
}

static __inline__ pgd_t *get_pgd_fast(void)
{
        unsigned long *ret;

        if((ret = (cpu_data[0].pgd_quick)) != ((void *)0)) {
                (cpu_data[0].pgd_quick) = (unsigned long *)(*ret);
                ret[0] = ret[1];
                (cpu_data[0].pgtable_cache_sz)--;
        } else
                ret = (unsigned long *)get_pgd_slow();
        return (pgd_t *)ret;
}

static __inline__ void free_pgd_fast(pgd_t *pgd)
{
        *(unsigned long *)pgd = (unsigned long) (cpu_data[0].pgd_quick);
        (cpu_data[0].pgd_quick) = (unsigned long *) pgd;
        (cpu_data[0].pgtable_cache_sz)++;
}

static __inline__ void free_pgd_slow(pgd_t *pgd)
{
        free_pages((unsigned long)pgd, 0);
}

static __inline__ pte_t *get_pte_fast(void)
{
        unsigned long *ret;

        if((ret = (unsigned long *)(cpu_data[0].pte_quick)) != ((void *)0)) {
                (cpu_data[0].pte_quick) = (unsigned long *)(*ret);
                ret[0] = ret[1];
                (cpu_data[0].pgtable_cache_sz)--;
        }
        return (pte_t *)ret;
}

static __inline__ void free_pte_fast(pte_t *pte)
{
        *(unsigned long *)pte = (unsigned long) (cpu_data[0].pte_quick);
        (cpu_data[0].pte_quick) = (unsigned long *) pte;
        (cpu_data[0].pgtable_cache_sz)++;
}

static __inline__ void free_pte_slow(pte_t *pte)
{
        free_pages(((unsigned long)pte),0);
}


static __inline__ pmd_t *get_pmd_fast(void)
{
        return (pmd_t *)0;
}

static __inline__ void free_pmd_fast(pmd_t *pmd)
{
}

static __inline__ void free_pmd_slow(pmd_t *pmd)
{
}

extern void __bad_pte(pmd_t *pmd);

static inline pte_t *pte_alloc_one(struct mm_struct *mm, unsigned long address)
{
        pte_t *pte;

        pte = (pte_t *) __get_free_pages(((0x20 | 0x10 | 0x40 | 0x80 | 0x100)),0);
        if (pte)
                _clear_page(pte);
        return pte;
}

static inline pte_t *pte_alloc_one_fast(struct mm_struct *mm, unsigned long address)
{
        unsigned long *ret;

        if ((ret = (unsigned long *)(cpu_data[0].pte_quick)) != ((void *)0)) {
                (cpu_data[0].pte_quick) = (unsigned long *)(*ret);
                ret[0] = ret[1];
                (cpu_data[0].pgtable_cache_sz)--;
        }
        return (pte_t *)ret;
}

static __inline__ void pte_free_fast(pte_t *pte)
{
        *(unsigned long *)pte = (unsigned long) (cpu_data[0].pte_quick);
        (cpu_data[0].pte_quick) = (unsigned long *) pte;
        (cpu_data[0].pgtable_cache_sz)++;
}

static __inline__ void pte_free_slow(pte_t *pte)
{
        free_pages(((unsigned long)pte),0);
}
# 197 "/opt/Wive/kernel/include/asm/pgalloc.h"
extern int do_check_pgt_cache(int, int);
# 6 "/opt/Wive/kernel/include/linux/highmem.h" 2
# 31 "/opt/Wive/kernel/include/linux/highmem.h"
static inline unsigned int nr_free_highpages(void) { return 0; }

static inline void *kmap(struct page *page) { return ((page)->virtual); }
# 46 "/opt/Wive/kernel/include/linux/highmem.h"
static inline void clear_user_highpage(struct page *page, unsigned long vaddr)
{
        void *addr = kmap(page);
        _clear_page(addr);
        do { } while (0);
}

static inline void clear_highpage(struct page *page)
{
        _clear_page(kmap(page));
        do { } while (0);
}




static inline void memclear_highpage_flush(struct page *page, unsigned int offset, unsigned int size)
{
        char *kaddr;

        if (offset + size > (1L << 12))
                __out_of_line_bug(67);
        kaddr = kmap(page);
        memset(kaddr + offset, 0, size);
        do { } while (0);
        _flush_page_to_ram(page);
        do { } while (0);
}

static inline void copy_user_highpage(struct page *to, struct page *from, unsigned long vaddr)
{
        char *vfrom, *vto;

        vfrom = kmap(from);
        vto = kmap(to);
        _copy_page(vto, vfrom);
        do { } while (0);
        do { } while (0);
}
# 17 "/opt/Wive/kernel/include/linux/pagemap.h" 2
# 34 "/opt/Wive/kernel/include/linux/pagemap.h"
static inline struct page *page_cache_alloc(struct address_space *x)
{
        return alloc_pages(x->gfp_mask, 0);
}






extern unsigned int page_hash_bits;



extern atomic_t page_cache_size;
extern struct page **page_hash_table;

extern void page_cache_init(unsigned long);
# 62 "/opt/Wive/kernel/include/linux/pagemap.h"
static inline unsigned long _page_hashfn(struct address_space * mapping, unsigned long index)
{


        return (((((unsigned long) mapping)/(sizeof(struct inode) & ~ (sizeof(struct inode) - 1)))+index)+(((((unsigned long) mapping)/(sizeof(struct inode) & ~ (sizeof(struct inode) - 1)))+index)>>(page_hash_bits))) & ((1 << (page_hash_bits))-1);


}



extern struct page * __find_get_page(struct address_space *mapping,
                                unsigned long index, struct page **hash);


extern struct page * __find_lock_page (struct address_space * mapping,
                                unsigned long index, struct page **hash);
extern struct page * find_or_create_page(struct address_space *mapping,
                                unsigned long index, unsigned int gfp_mask);

extern void lock_page(struct page *page);
extern void unlock_page(struct page *page);


extern struct page *find_trylock_page(struct address_space *, unsigned long);

extern void add_to_page_cache(struct page * page, struct address_space *mapping, unsigned long index);
extern void add_to_page_cache_locked(struct page * page, struct address_space *mapping, unsigned long index);
extern int add_to_page_cache_unique(struct page * page, struct address_space *mapping, unsigned long index, struct page **hash);

extern void ___wait_on_page(struct page *);

static inline void wait_on_page(struct page * page)
{
        if (test_bit(0, &(page)->flags))
                ___wait_on_page(page);
}

extern struct page * grab_cache_page (struct address_space *, unsigned long);
extern struct page * grab_cache_page_nowait (struct address_space *, unsigned long);

typedef int filler_t(void *, struct page*);

extern struct page *read_cache_page(struct address_space *, unsigned long,
                                filler_t *, void *);
# 9 "/opt/Wive/kernel/include/linux/locks.h" 2






extern void __wait_on_buffer(struct buffer_head *);

static inline void wait_on_buffer(struct buffer_head * bh)
{
        if (test_bit(BH_Lock, &bh->b_state))
                __wait_on_buffer(bh);
}

static inline void lock_buffer(struct buffer_head * bh)
{
        while (test_and_set_bit(BH_Lock, &bh->b_state))
                __wait_on_buffer(bh);
}

extern void unlock_buffer(struct buffer_head *bh);







static inline void lock_super(struct super_block * sb)
{
        down(&sb->s_lock);
}

static inline void unlock_super(struct super_block * sb)
{
        up(&sb->s_lock);
}
# 18 "balloc.c" 2
# 1 "/opt/Wive/kernel/include/linux/quotaops.h" 1
# 14 "/opt/Wive/kernel/include/linux/quotaops.h"
# 1 "/opt/Wive/kernel/include/linux/smp_lock.h" 1
# 15 "/opt/Wive/kernel/include/linux/quotaops.h" 2
# 177 "/opt/Wive/kernel/include/linux/quotaops.h"
extern __inline__ int DQUOT_PREALLOC_BLOCK_NODIRTY(struct inode *inode, int nr)
{
        do { } while(0);
        inode->i_blocks += nr << (inode->i_sb->s_blocksize_bits - 9);
        do { } while(0);
        return 0;
}

extern __inline__ int DQUOT_PREALLOC_BLOCK(struct inode *inode, int nr)
{
        DQUOT_PREALLOC_BLOCK_NODIRTY(inode, nr);
        mark_inode_dirty(inode);
        return 0;
}

extern __inline__ int DQUOT_ALLOC_BLOCK_NODIRTY(struct inode *inode, int nr)
{
        do { } while(0);
        inode->i_blocks += nr << (inode->i_sb->s_blocksize_bits - 9);
        do { } while(0);
        return 0;
}

extern __inline__ int DQUOT_ALLOC_BLOCK(struct inode *inode, int nr)
{
        DQUOT_ALLOC_BLOCK_NODIRTY(inode, nr);
        mark_inode_dirty(inode);
        return 0;
}

extern __inline__ void DQUOT_FREE_BLOCK_NODIRTY(struct inode *inode, int nr)
{
        do { } while(0);
        inode->i_blocks -= nr << (inode->i_sb->s_blocksize_bits - 9);
        do { } while(0);
}

extern __inline__ void DQUOT_FREE_BLOCK(struct inode *inode, int nr)
{
        DQUOT_FREE_BLOCK_NODIRTY(inode, nr);
        mark_inode_dirty(inode);
}
# 19 "balloc.c" 2
# 38 "balloc.c"
struct ext2_group_desc * ext2_get_group_desc(struct super_block * sb,
                                             unsigned int block_group,
                                             struct buffer_head ** bh)
{
        unsigned long group_desc;
        unsigned long desc;
        struct ext2_group_desc * gdp;

        if (block_group >= sb->u.ext2_sb.s_groups_count) {
                ext2_error (sb, "ext2_get_group_desc",
                            "block_group >= groups_count - "
                            "block_group = %d, groups_count = %lu",
                            block_group, sb->u.ext2_sb.s_groups_count);

                return ((void *)0);
        }

        group_desc = block_group / ((sb)->u.ext2_sb.s_desc_per_block);
        desc = block_group % ((sb)->u.ext2_sb.s_desc_per_block);
        if (!sb->u.ext2_sb.s_group_desc[group_desc]) {
                ext2_error (sb, "ext2_get_group_desc",
                            "Group descriptor not loaded - "
                            "block_group = %d, group_desc = %lu, desc = %lu",
                             block_group, group_desc, desc);
                return ((void *)0);
        }

        gdp = (struct ext2_group_desc *)
              sb->u.ext2_sb.s_group_desc[group_desc]->b_data;
        if (bh)
                *bh = sb->u.ext2_sb.s_group_desc[group_desc];
        return gdp + desc;
}
# 79 "balloc.c"
static int read_block_bitmap (struct super_block * sb,
                               unsigned int block_group,
                               unsigned long bitmap_nr)
{
        struct ext2_group_desc * gdp;
        struct buffer_head * bh = ((void *)0);
        int retval = -5;

        gdp = ext2_get_group_desc (sb, block_group, ((void *)0));
        if (!gdp)
                goto error_out;
        retval = 0;
        bh = sb_bread(sb, (__builtin_constant_p((__u32)((gdp->bg_block_bitmap))) ? ({ __u32 __x = (((gdp->bg_block_bitmap))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((gdp->bg_block_bitmap)))));
        if (!bh) {
                ext2_error (sb, "read_block_bitmap",
                            "Cannot read block bitmap - "
                            "block_group = %d, block_bitmap = %lu",
                            block_group, (unsigned long) gdp->bg_block_bitmap);
                retval = -5;
        }




error_out:
        sb->u.ext2_sb.s_block_bitmap_number[bitmap_nr] = block_group;
        sb->u.ext2_sb.s_block_bitmap[bitmap_nr] = bh;
        return retval;
}
# 122 "balloc.c"
static int __load_block_bitmap (struct super_block * sb,
                                unsigned int block_group)
{
        int i, j, retval = 0;
        unsigned long block_bitmap_number;
        struct buffer_head * block_bitmap;

        if (block_group >= sb->u.ext2_sb.s_groups_count)
                ext2_panic (sb, "load_block_bitmap",
                            "block_group >= groups_count - "
                            "block_group = %d, groups_count = %lu",
                            block_group, sb->u.ext2_sb.s_groups_count);

        if (sb->u.ext2_sb.s_groups_count <= 8) {
                if (sb->u.ext2_sb.s_block_bitmap[block_group]) {
                        if (sb->u.ext2_sb.s_block_bitmap_number[block_group] ==
                            block_group)
                                return block_group;
                        ext2_error (sb, "__load_block_bitmap",
                                    "block_group != block_bitmap_number");
                }
                retval = read_block_bitmap (sb, block_group, block_group);
                if (retval < 0)
                        return retval;
                return block_group;
        }

        for (i = 0; i < sb->u.ext2_sb.s_loaded_block_bitmaps &&
                    sb->u.ext2_sb.s_block_bitmap_number[i] != block_group; i++)
                ;
        if (i < sb->u.ext2_sb.s_loaded_block_bitmaps &&
            sb->u.ext2_sb.s_block_bitmap_number[i] == block_group) {
                block_bitmap_number = sb->u.ext2_sb.s_block_bitmap_number[i];
                block_bitmap = sb->u.ext2_sb.s_block_bitmap[i];
                for (j = i; j > 0; j--) {
                        sb->u.ext2_sb.s_block_bitmap_number[j] =
                                sb->u.ext2_sb.s_block_bitmap_number[j - 1];
                        sb->u.ext2_sb.s_block_bitmap[j] =
                                sb->u.ext2_sb.s_block_bitmap[j - 1];
                }
                sb->u.ext2_sb.s_block_bitmap_number[0] = block_bitmap_number;
                sb->u.ext2_sb.s_block_bitmap[0] = block_bitmap;






                if (!block_bitmap)
                        retval = read_block_bitmap (sb, block_group, 0);
        } else {
                if (sb->u.ext2_sb.s_loaded_block_bitmaps < 8)
                        sb->u.ext2_sb.s_loaded_block_bitmaps++;
                else
                        brelse (sb->u.ext2_sb.s_block_bitmap[8 - 1]);
                for (j = sb->u.ext2_sb.s_loaded_block_bitmaps - 1; j > 0; j--) {
                        sb->u.ext2_sb.s_block_bitmap_number[j] =
                                sb->u.ext2_sb.s_block_bitmap_number[j - 1];
                        sb->u.ext2_sb.s_block_bitmap[j] =
                                sb->u.ext2_sb.s_block_bitmap[j - 1];
                }
                retval = read_block_bitmap (sb, block_group, 0);
        }
        return retval;
}
# 201 "balloc.c"
static inline int load_block_bitmap (struct super_block * sb,
                                     unsigned int block_group)
{
        int slot;





        if (sb->u.ext2_sb.s_loaded_block_bitmaps > 0 &&
            sb->u.ext2_sb.s_block_bitmap_number[0] == block_group &&
            sb->u.ext2_sb.s_block_bitmap[0]) {
                return 0;
        }




        else if (sb->u.ext2_sb.s_groups_count <= 8 &&
                 sb->u.ext2_sb.s_block_bitmap_number[block_group] == block_group &&
                 sb->u.ext2_sb.s_block_bitmap[block_group]) {
                slot = block_group;
        }



        else {
                slot = __load_block_bitmap (sb, block_group);
        }




        if (slot < 0)
                return slot;





        if (!sb->u.ext2_sb.s_block_bitmap[slot])
                return -5;




        return slot;
}


void ext2_free_blocks (struct inode * inode, unsigned long block,
                       unsigned long count)
{
        struct buffer_head * bh;
        struct buffer_head * bh2;
        unsigned long block_group;
        unsigned long bit;
        unsigned long i;
        int bitmap_nr;
        unsigned long overflow;
        struct super_block * sb;
        struct ext2_group_desc * gdp;
        struct ext2_super_block * es;

        sb = inode->i_sb;
        if (!sb) {
                printk ("ext2_free_blocks: nonexistent device");
                return;
        }
        lock_super (sb);
        es = sb->u.ext2_sb.s_es;
        if (block < (__builtin_constant_p((__u32)((es->s_first_data_block))) ? ({ __u32 __x = (((es->s_first_data_block))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_first_data_block)))) ||
            (block + count) > (__builtin_constant_p((__u32)((es->s_blocks_count))) ? ({ __u32 __x = (((es->s_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_blocks_count))))) {
                ext2_error (sb, "ext2_free_blocks",
                            "Freeing blocks not in datazone - "
                            "block = %lu, count = %lu", block, count);
                goto error_return;
        }

        ;

do_more:
        overflow = 0;
        block_group = (block - (__builtin_constant_p((__u32)((es->s_first_data_block))) ? ({ __u32 __x = (((es->s_first_data_block))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_first_data_block))))) /
                      ((sb)->u.ext2_sb.s_blocks_per_group);
        bit = (block - (__builtin_constant_p((__u32)((es->s_first_data_block))) ? ({ __u32 __x = (((es->s_first_data_block))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_first_data_block))))) %
                      ((sb)->u.ext2_sb.s_blocks_per_group);




        if (bit + count > ((sb)->u.ext2_sb.s_blocks_per_group)) {
                overflow = bit + count - ((sb)->u.ext2_sb.s_blocks_per_group);
                count -= overflow;
        }
        bitmap_nr = load_block_bitmap (sb, block_group);
        if (bitmap_nr < 0)
                goto error_return;

        bh = sb->u.ext2_sb.s_block_bitmap[bitmap_nr];
        gdp = ext2_get_group_desc (sb, block_group, &bh2);
        if (!gdp)
                goto error_return;

        if ((((__builtin_constant_p((__u32)((gdp->bg_block_bitmap))) ? ({ __u32 __x = (((gdp->bg_block_bitmap))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((gdp->bg_block_bitmap))))) >= (block) && ((__builtin_constant_p((__u32)((gdp->bg_block_bitmap))) ? ({ __u32 __x = (((gdp->bg_block_bitmap))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((gdp->bg_block_bitmap))))) <= (block) + (count) - 1) ||
            (((__builtin_constant_p((__u32)((gdp->bg_inode_bitmap))) ? ({ __u32 __x = (((gdp->bg_inode_bitmap))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((gdp->bg_inode_bitmap))))) >= (block) && ((__builtin_constant_p((__u32)((gdp->bg_inode_bitmap))) ? ({ __u32 __x = (((gdp->bg_inode_bitmap))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((gdp->bg_inode_bitmap))))) <= (block) + (count) - 1) ||
            ((block) >= ((__builtin_constant_p((__u32)((gdp->bg_inode_table))) ? ({ __u32 __x = (((gdp->bg_inode_table))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((gdp->bg_inode_table))))) && (block) <= ((__builtin_constant_p((__u32)((gdp->bg_inode_table))) ? ({ __u32 __x = (((gdp->bg_inode_table))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((gdp->bg_inode_table))))) + (sb->u.ext2_sb.s_itb_per_group) - 1) ||

            ((block + count - 1) >= ((__builtin_constant_p((__u32)((gdp->bg_inode_table))) ? ({ __u32 __x = (((gdp->bg_inode_table))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((gdp->bg_inode_table))))) && (block + count - 1) <= ((__builtin_constant_p((__u32)((gdp->bg_inode_table))) ? ({ __u32 __x = (((gdp->bg_inode_table))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((gdp->bg_inode_table))))) + (sb->u.ext2_sb.s_itb_per_group) - 1))

                ext2_error (sb, "ext2_free_blocks",
                            "Freeing blocks in system zones - "
                            "Block = %lu, count = %lu",
                            block, count);

        for (i = 0; i < count; i++) {
                if (!ext2_clear_bit (bit + i, bh->b_data))
                        ext2_error (sb, "ext2_free_blocks",
                                      "bit already cleared for block %lu",
                                      block + i);
                else {
                        DQUOT_FREE_BLOCK(inode, 1);
                        gdp->bg_free_blocks_count =
                                (__builtin_constant_p((__u16)(((__builtin_constant_p((__u16)((gdp->bg_free_blocks_count))) ? ({ __u16 __x = (((gdp->bg_free_blocks_count))); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }) : __fswab16(((gdp->bg_free_blocks_count))))+1))) ? ({ __u16 __x = ((((__builtin_constant_p((__u16)((gdp->bg_free_blocks_count))) ? ({ __u16 __x = (((gdp->bg_free_blocks_count))); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }) : __fswab16(((gdp->bg_free_blocks_count))))+1))); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }) : __fswab16((((__builtin_constant_p((__u16)((gdp->bg_free_blocks_count))) ? ({ __u16 __x = (((gdp->bg_free_blocks_count))); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }) : __fswab16(((gdp->bg_free_blocks_count))))+1))));
                        es->s_free_blocks_count =
                                (__builtin_constant_p((__u32)(((__builtin_constant_p((__u32)((es->s_free_blocks_count))) ? ({ __u32 __x = (((es->s_free_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_free_blocks_count))))+1))) ? ({ __u32 __x = ((((__builtin_constant_p((__u32)((es->s_free_blocks_count))) ? ({ __u32 __x = (((es->s_free_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_free_blocks_count))))+1))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32((((__builtin_constant_p((__u32)((es->s_free_blocks_count))) ? ({ __u32 __x = (((es->s_free_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_free_blocks_count))))+1))));
                }
        }

        mark_buffer_dirty(bh2);
        mark_buffer_dirty(sb->u.ext2_sb.s_sbh);

        mark_buffer_dirty(bh);
        if (sb->s_flags & 16) {
                ll_rw_block (1, 1, &bh);
                wait_on_buffer (bh);
        }
        if (overflow) {
                block += count;
                count = overflow;
                goto do_more;
        }
        sb->s_dirt = 1;
error_return:
        unlock_super (sb);
        return;
}
# 357 "balloc.c"
int ext2_new_block (struct inode * inode, unsigned long goal,
    u32 * prealloc_count, u32 * prealloc_block, int * err)
{
        struct buffer_head * bh;
        struct buffer_head * bh2;
        char * p, * r;
        int i, j, k, tmp;
        int bitmap_nr;
        struct super_block * sb;
        struct ext2_group_desc * gdp;
        struct ext2_super_block * es;



        *err = -28;
        sb = inode->i_sb;
        if (!sb) {
                printk ("ext2_new_block: nonexistent device");
                return 0;
        }

        lock_super (sb);
        es = sb->u.ext2_sb.s_es;
        if ((__builtin_constant_p((__u32)((es->s_free_blocks_count))) ? ({ __u32 __x = (((es->s_free_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_free_blocks_count)))) <= (__builtin_constant_p((__u32)((es->s_r_blocks_count))) ? ({ __u32 __x = (((es->s_r_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_r_blocks_count)))) &&
            ((sb->u.ext2_sb.s_resuid != current->fsuid) &&
             (sb->u.ext2_sb.s_resgid == 0 ||
              !in_group_p (sb->u.ext2_sb.s_resgid)) &&
             !capable(24)))
                goto out;

        ;

repeat:



        if (goal < (__builtin_constant_p((__u32)((es->s_first_data_block))) ? ({ __u32 __x = (((es->s_first_data_block))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_first_data_block)))) ||
            goal >= (__builtin_constant_p((__u32)((es->s_blocks_count))) ? ({ __u32 __x = (((es->s_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_blocks_count)))))
                goal = (__builtin_constant_p((__u32)((es->s_first_data_block))) ? ({ __u32 __x = (((es->s_first_data_block))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_first_data_block))));
        i = (goal - (__builtin_constant_p((__u32)((es->s_first_data_block))) ? ({ __u32 __x = (((es->s_first_data_block))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_first_data_block))))) / ((sb)->u.ext2_sb.s_blocks_per_group);
        gdp = ext2_get_group_desc (sb, i, &bh2);
        if (!gdp)
                goto io_error;

        if ((__builtin_constant_p((__u16)((gdp->bg_free_blocks_count))) ? ({ __u16 __x = (((gdp->bg_free_blocks_count))); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }) : __fswab16(((gdp->bg_free_blocks_count)))) > 0) {
                j = ((goal - (__builtin_constant_p((__u32)((es->s_first_data_block))) ? ({ __u32 __x = (((es->s_first_data_block))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_first_data_block))))) % ((sb)->u.ext2_sb.s_blocks_per_group));




                bitmap_nr = load_block_bitmap (sb, i);
                if (bitmap_nr < 0)
                        goto io_error;

                bh = sb->u.ext2_sb.s_block_bitmap[bitmap_nr];

                ;

                if (!ext2_test_bit(j, bh->b_data)) {
                        ;
                        goto got_block;
                }
                if (j) {
# 428 "balloc.c"
                        int end_goal = (j + 63) & ~63;
                        j = ext2_find_next_zero_bit(bh->b_data, end_goal, j);
                        if (j < end_goal)
                                goto got_block;
                }

                ;
# 445 "balloc.c"
                p = ((char *) bh->b_data) + (j >> 3);
                r = memscan(p, 0, (((sb)->u.ext2_sb.s_blocks_per_group) - j + 7) >> 3);
                k = (r - ((char *) bh->b_data)) << 3;
                if (k < ((sb)->u.ext2_sb.s_blocks_per_group)) {
                        j = k;
                        goto search_back;
                }

                k = ext2_find_next_zero_bit ((unsigned long *) bh->b_data,
                                        ((sb)->u.ext2_sb.s_blocks_per_group),
                                        j);
                if (k < ((sb)->u.ext2_sb.s_blocks_per_group)) {
                        j = k;
                        goto got_block;
                }
        }

        ;





        for (k = 0; k < sb->u.ext2_sb.s_groups_count; k++) {
                i++;
                if (i >= sb->u.ext2_sb.s_groups_count)
                        i = 0;
                gdp = ext2_get_group_desc (sb, i, &bh2);
                if (!gdp)
                        goto io_error;
                if ((__builtin_constant_p((__u16)((gdp->bg_free_blocks_count))) ? ({ __u16 __x = (((gdp->bg_free_blocks_count))); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }) : __fswab16(((gdp->bg_free_blocks_count)))) > 0)
                        break;
        }
        if (k >= sb->u.ext2_sb.s_groups_count)
                goto out;
        bitmap_nr = load_block_bitmap (sb, i);
        if (bitmap_nr < 0)
                goto io_error;

        bh = sb->u.ext2_sb.s_block_bitmap[bitmap_nr];
        r = memscan(bh->b_data, 0, ((sb)->u.ext2_sb.s_blocks_per_group) >> 3);
        j = (r - bh->b_data) << 3;
        if (j < ((sb)->u.ext2_sb.s_blocks_per_group))
                goto search_back;
        else
                j = ext2_find_next_zero_bit(((unsigned long *) bh->b_data), (((sb)->u.ext2_sb.s_blocks_per_group)), 0);

        if (j >= ((sb)->u.ext2_sb.s_blocks_per_group)) {
                ext2_error (sb, "ext2_new_block",
                            "Free blocks count corrupted for block group %d", i);
                goto out;
        }

search_back:





        for (k = 0; k < 7 && j > 0 && !ext2_test_bit (j - 1, bh->b_data); k++, j--);

got_block:

        ;




        if(DQUOT_ALLOC_BLOCK(inode, 1)) {
                *err = -1133;
                goto out;
        }

        tmp = j + i * ((sb)->u.ext2_sb.s_blocks_per_group) + (__builtin_constant_p((__u32)((es->s_first_data_block))) ? ({ __u32 __x = (((es->s_first_data_block))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_first_data_block))));

        if (tmp == (__builtin_constant_p((__u32)((gdp->bg_block_bitmap))) ? ({ __u32 __x = (((gdp->bg_block_bitmap))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((gdp->bg_block_bitmap)))) ||
            tmp == (__builtin_constant_p((__u32)((gdp->bg_inode_bitmap))) ? ({ __u32 __x = (((gdp->bg_inode_bitmap))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((gdp->bg_inode_bitmap)))) ||
            ((tmp) >= ((__builtin_constant_p((__u32)((gdp->bg_inode_table))) ? ({ __u32 __x = (((gdp->bg_inode_table))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((gdp->bg_inode_table))))) && (tmp) <= ((__builtin_constant_p((__u32)((gdp->bg_inode_table))) ? ({ __u32 __x = (((gdp->bg_inode_table))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((gdp->bg_inode_table))))) + (sb->u.ext2_sb.s_itb_per_group) - 1))

                ext2_error (sb, "ext2_new_block",
                            "Allocating block in system zone - "
                            "block = %u", tmp);

        if (ext2_set_bit (j, bh->b_data)) {
                ext2_warning (sb, "ext2_new_block",
                              "bit already set for block %d", j);
                DQUOT_FREE_BLOCK(inode, 1);
                goto repeat;
        }

        ;






        if (prealloc_count && !*prealloc_count) {
                int prealloc_goal;
                unsigned long next_block = tmp + 1;

                prealloc_goal = es->s_prealloc_blocks ?
                        es->s_prealloc_blocks : 8;

                *prealloc_block = next_block;

                for (k = 1;
                     k < prealloc_goal && (j + k) < ((sb)->u.ext2_sb.s_blocks_per_group);
                     k++, next_block++) {
                        if (DQUOT_PREALLOC_BLOCK(inode, 1))
                                break;

                        if (*prealloc_block + *prealloc_count != next_block ||
                            ext2_set_bit (j + k, bh->b_data)) {

                                DQUOT_FREE_BLOCK(inode, 1);
                                break;
                        }
                        (*prealloc_count)++;

                }




                gdp->bg_free_blocks_count =
                        (__builtin_constant_p((__u16)(((__builtin_constant_p((__u16)((gdp->bg_free_blocks_count))) ? ({ __u16 __x = (((gdp->bg_free_blocks_count))); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }) : __fswab16(((gdp->bg_free_blocks_count)))) - (k - 1)))) ? ({ __u16 __x = ((((__builtin_constant_p((__u16)((gdp->bg_free_blocks_count))) ? ({ __u16 __x = (((gdp->bg_free_blocks_count))); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }) : __fswab16(((gdp->bg_free_blocks_count)))) - (k - 1)))); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }) : __fswab16((((__builtin_constant_p((__u16)((gdp->bg_free_blocks_count))) ? ({ __u16 __x = (((gdp->bg_free_blocks_count))); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }) : __fswab16(((gdp->bg_free_blocks_count)))) - (k - 1)))));

                es->s_free_blocks_count =
                        (__builtin_constant_p((__u32)(((__builtin_constant_p((__u32)((es->s_free_blocks_count))) ? ({ __u32 __x = (((es->s_free_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_free_blocks_count)))) - (k - 1)))) ? ({ __u32 __x = ((((__builtin_constant_p((__u32)((es->s_free_blocks_count))) ? ({ __u32 __x = (((es->s_free_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_free_blocks_count)))) - (k - 1)))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32((((__builtin_constant_p((__u32)((es->s_free_blocks_count))) ? ({ __u32 __x = (((es->s_free_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_free_blocks_count)))) - (k - 1)))));

                ;

        }


        j = tmp;

        mark_buffer_dirty(bh);
        if (sb->s_flags & 16) {
                ll_rw_block (1, 1, &bh);
                wait_on_buffer (bh);
        }

        if (j >= (__builtin_constant_p((__u32)((es->s_blocks_count))) ? ({ __u32 __x = (((es->s_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_blocks_count))))) {
                ext2_error (sb, "ext2_new_block",
                            "block(%d) >= blocks count(%d) - "
                            "block_group = %d, es == %p ",j,
                        (__builtin_constant_p((__u32)((es->s_blocks_count))) ? ({ __u32 __x = (((es->s_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_blocks_count)))), i, es);
                goto out;
        }

        ;


        gdp->bg_free_blocks_count = (__builtin_constant_p((__u16)(((__builtin_constant_p((__u16)((gdp->bg_free_blocks_count))) ? ({ __u16 __x = (((gdp->bg_free_blocks_count))); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }) : __fswab16(((gdp->bg_free_blocks_count)))) - 1))) ? ({ __u16 __x = ((((__builtin_constant_p((__u16)((gdp->bg_free_blocks_count))) ? ({ __u16 __x = (((gdp->bg_free_blocks_count))); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }) : __fswab16(((gdp->bg_free_blocks_count)))) - 1))); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }) : __fswab16((((__builtin_constant_p((__u16)((gdp->bg_free_blocks_count))) ? ({ __u16 __x = (((gdp->bg_free_blocks_count))); ((__u16)( (((__u16)(__x) & (__u16)0x00ffU) << 8) | (((__u16)(__x) & (__u16)0xff00U) >> 8) )); }) : __fswab16(((gdp->bg_free_blocks_count)))) - 1))));
        mark_buffer_dirty(bh2);
        es->s_free_blocks_count = (__builtin_constant_p((__u32)(((__builtin_constant_p((__u32)((es->s_free_blocks_count))) ? ({ __u32 __x = (((es->s_free_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_free_blocks_count)))) - 1))) ? ({ __u32 __x = ((((__builtin_constant_p((__u32)((es->s_free_blocks_count))) ? ({ __u32 __x = (((es->s_free_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_free_blocks_count)))) - 1))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32((((__builtin_constant_p((__u32)((es->s_free_blocks_count))) ? ({ __u32 __x = (((es->s_free_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((es->s_free_blocks_count)))) - 1))));
        mark_buffer_dirty(sb->u.ext2_sb.s_sbh);
        sb->s_dirt = 1;
        unlock_super (sb);
        *err = 0;
        return j;

io_error:
        *err = -5;
out:
        unlock_super (sb);
        return 0;

}

unsigned long ext2_count_free_blocks (struct super_block * sb)
{
# 651 "balloc.c"
        return (__builtin_constant_p((__u32)((sb->u.ext2_sb.s_es->s_free_blocks_count))) ? ({ __u32 __x = (((sb->u.ext2_sb.s_es->s_free_blocks_count))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((sb->u.ext2_sb.s_es->s_free_blocks_count))));

}

static inline int block_in_use (unsigned long block,
                                struct super_block * sb,
                                unsigned char * map)
{
        return ext2_test_bit ((block - (__builtin_constant_p((__u32)((sb->u.ext2_sb.s_es->s_first_data_block))) ? ({ __u32 __x = (((sb->u.ext2_sb.s_es->s_first_data_block))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((sb->u.ext2_sb.s_es->s_first_data_block))))) %
                         ((sb)->u.ext2_sb.s_blocks_per_group), map);
}

static inline int test_root(int a, int b)
{
        if (a == 0)
                return 1;
        while (1) {
                if (a == 1)
                        return 1;
                if (a % b)
                        return 0;
                a = a / b;
        }
}

int ext2_group_sparse(int group)
{
        return (test_root(group, 3) || test_root(group, 5) ||
                test_root(group, 7));
}
# 690 "balloc.c"
int ext2_bg_has_super(struct super_block *sb, int group)
{
        if (( (&((sb)->u.ext2_sb))->s_es->s_feature_ro_compat & (__builtin_constant_p((__u32)((0x0001))) ? ({ __u32 __x = (((0x0001))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((0x0001)))) )&&
            !ext2_group_sparse(group))
                return 0;
        return 1;
}
# 707 "balloc.c"
unsigned long ext2_bg_num_gdb(struct super_block *sb, int group)
{
        if (( (&((sb)->u.ext2_sb))->s_es->s_feature_ro_compat & (__builtin_constant_p((__u32)((0x0001))) ? ({ __u32 __x = (((0x0001))); ((__u32)( (((__u32)(__x) & (__u32)0x000000ffUL) << 24) | (((__u32)(__x) & (__u32)0x0000ff00UL) << 8) | (((__u32)(__x) & (__u32)0x00ff0000UL) >> 8) | (((__u32)(__x) & (__u32)0xff000000UL) >> 24) )); }) : __fswab32(((0x0001)))) )&&
            !ext2_group_sparse(group))
                return 0;
        return (&((sb)->u.ext2_sb))->s_gdb_count;
}
