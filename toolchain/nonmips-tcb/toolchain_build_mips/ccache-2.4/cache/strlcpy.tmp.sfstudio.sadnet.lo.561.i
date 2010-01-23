# 1 "strlcpy.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "strlcpy.c"
# 34 "strlcpy.c"
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 1 3 4
# 26 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/features.h" 1 3 4
# 303 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/features.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/cdefs.h" 1 3 4
# 304 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/features.h" 2 3 4
# 338 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/features.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uClibc_config.h" 1 3 4
# 339 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/features.h" 2 3 4
# 27 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 2 3 4



# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/types.h" 1 3 4
# 29 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/types.h" 3 4
# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 1 3 4
# 213 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 3 4
typedef unsigned int size_t;
# 30 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/types.h" 2 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/kernel_types.h" 1 3 4
# 36 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/kernel_types.h" 3 4
typedef unsigned int __kernel_dev_t;
typedef unsigned long __kernel_ino_t;
typedef unsigned int __kernel_mode_t;
# 47 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/kernel_types.h" 3 4
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
# 31 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/types.h" 2 3 4


typedef unsigned char __u_char;
typedef unsigned short __u_short;
typedef unsigned int __u_int;
typedef unsigned long __u_long;

__extension__ typedef unsigned long long int __u_quad_t;
__extension__ typedef long long int __quad_t;
# 50 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/types.h" 3 4
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;

__extension__ typedef signed long long int __int64_t;
__extension__ typedef unsigned long long int __uint64_t;

typedef __quad_t *__qaddr_t;

typedef __u_quad_t __dev_t;
typedef __u_int __uid_t;
typedef __u_int __gid_t;
typedef __u_long __ino_t;
typedef __u_int __mode_t;
typedef __u_int __nlink_t;
typedef long int __off_t;
typedef __quad_t __loff_t;
typedef int __pid_t;
typedef int __ssize_t;
typedef __u_long __rlim_t;
typedef __u_quad_t __rlim64_t;
typedef __u_int __id_t;

typedef struct
  {
    int __val[2];
  } __fsid_t;


typedef int __daddr_t;
typedef char *__caddr_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;
typedef long int __swblk_t;

typedef long int __clock_t;


typedef int __clockid_t;


typedef int __timer_t;





typedef int __key_t;


typedef long int __ipc_pid_t;



typedef long int __blksize_t;




typedef long int __blkcnt_t;
typedef __quad_t __blkcnt64_t;


typedef __u_long __fsblkcnt_t;
typedef __u_quad_t __fsblkcnt64_t;


typedef __u_long __fsfilcnt_t;
typedef __u_quad_t __fsfilcnt64_t;


typedef __u_quad_t __ino64_t;


typedef __loff_t __off64_t;


typedef long int __t_scalar_t;
typedef unsigned long int __t_uscalar_t;


typedef int __intptr_t;


typedef unsigned int __socklen_t;




# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/pthreadtypes.h" 1 3 4
# 23 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/pthreadtypes.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sched.h" 1 3 4
# 68 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sched.h" 3 4
struct __sched_param
  {
    int __sched_priority;
  };
# 24 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/pthreadtypes.h" 2 3 4


struct _pthread_fastlock
{
  long int __status;
  int __spinlock;

};



typedef struct _pthread_descr_struct *_pthread_descr;





typedef struct __pthread_attr_s
{
  int __detachstate;
  int __schedpolicy;
  struct __sched_param __schedparam;
  int __inheritsched;
  int __scope;
  size_t __guardsize;
  int __stackaddr_set;
  void *__stackaddr;
  size_t __stacksize;
} pthread_attr_t;



typedef struct
{
  struct _pthread_fastlock __c_lock;
  _pthread_descr __c_waiting;
} pthread_cond_t;



typedef struct
{
  int __dummy;
} pthread_condattr_t;


typedef unsigned int pthread_key_t;





typedef struct
{
  int __m_reserved;
  int __m_count;
  _pthread_descr __m_owner;
  int __m_kind;
  struct _pthread_fastlock __m_lock;
} pthread_mutex_t;



typedef struct
{
  int __mutexkind;
} pthread_mutexattr_t;



typedef int pthread_once_t;
# 140 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/pthreadtypes.h" 3 4
typedef unsigned long int pthread_t;
# 144 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/types.h" 2 3 4
# 31 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 2 3 4



typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;




typedef __loff_t loff_t;



typedef __ino_t ino_t;
# 61 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 3 4
typedef __dev_t dev_t;




typedef __gid_t gid_t;




typedef __mode_t mode_t;




typedef __nlink_t nlink_t;




typedef __uid_t uid_t;





typedef __off_t off_t;
# 99 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 3 4
typedef __pid_t pid_t;




typedef __id_t id_t;




typedef __ssize_t ssize_t;





typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;





typedef __key_t key_t;
# 132 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/time.h" 1 3 4
# 74 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/time.h" 3 4


typedef __time_t time_t;



# 92 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/time.h" 3 4
typedef __clockid_t clockid_t;
# 104 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/time.h" 3 4
typedef __timer_t timer_t;
# 133 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 2 3 4
# 146 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 3 4
# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 1 3 4
# 147 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 2 3 4



typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;
# 190 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 3 4
typedef int int8_t __attribute__ ((__mode__ (__QI__)));
typedef int int16_t __attribute__ ((__mode__ (__HI__)));
typedef int int32_t __attribute__ ((__mode__ (__SI__)));
typedef int int64_t __attribute__ ((__mode__ (__DI__)));


typedef unsigned int u_int8_t __attribute__ ((__mode__ (__QI__)));
typedef unsigned int u_int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int u_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int u_int64_t __attribute__ ((__mode__ (__DI__)));

typedef int register_t __attribute__ ((__mode__ (__word__)));
# 212 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/endian.h" 1 3 4
# 37 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/endian.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/endian.h" 1 3 4
# 38 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/endian.h" 2 3 4
# 213 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 2 3 4


# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/select.h" 1 3 4
# 31 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/select.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/select.h" 1 3 4
# 32 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/select.h" 2 3 4


# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sigset.h" 1 3 4
# 23 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sigset.h" 3 4
typedef int __sig_atomic_t;




typedef struct
  {
    unsigned long int __val[(1024 / (8 * sizeof (unsigned long int)))];
  } __sigset_t;
# 35 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/select.h" 2 3 4



typedef __sigset_t sigset_t;




# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/time.h" 1 3 4
# 118 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/time.h" 3 4
struct timespec
  {
    __time_t tv_sec;
    long int tv_nsec;
  };
# 44 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/select.h" 2 3 4

# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/time.h" 1 3 4
# 71 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/time.h" 3 4
struct timeval
  {
    __time_t tv_sec;
    __suseconds_t tv_usec;
  };
# 46 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/select.h" 2 3 4



typedef long int __fd_mask;







typedef struct
  {






    __fd_mask __fds_bits[1024 / (8 * sizeof (__fd_mask))];


  } fd_set;






typedef __fd_mask fd_mask;
# 89 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/select.h" 3 4







extern int select (int __nfds, fd_set *__restrict __readfds,
                   fd_set *__restrict __writefds,
                   fd_set *__restrict __exceptfds,
                   struct timeval *__restrict __timeout) ;
# 112 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/select.h" 3 4

# 216 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 2 3 4


# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/sysmacros.h" 1 3 4
# 219 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 2 3 4
# 230 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 3 4
typedef __blkcnt_t blkcnt_t;



typedef __fsblkcnt_t fsblkcnt_t;



typedef __fsfilcnt_t fsfilcnt_t;
# 262 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 3 4

# 35 "strlcpy.c" 2


# 1 "config.h" 1
# 38 "strlcpy.c" 2
