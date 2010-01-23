# 1 "evrpc.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "evrpc.c"
# 28 "evrpc.c"
# 1 "config.h" 1
# 29 "evrpc.c" 2
# 38 "evrpc.c"
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

# 39 "evrpc.c" 2

# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/socket.h" 1 3 4
# 25 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/socket.h" 3 4


# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/uio.h" 1 3 4
# 26 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/uio.h" 3 4



# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uio.h" 1 3 4
# 42 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uio.h" 3 4
struct iovec
  {
    void *iov_base;
    size_t iov_len;
  };
# 30 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/uio.h" 2 3 4







extern ssize_t readv (int __fd, __const struct iovec *__vector, int __count)
     ;






extern ssize_t writev (int __fd, __const struct iovec *__vector, int __count)
     ;


# 28 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/socket.h" 2 3 4

# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 1 3 4
# 30 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/socket.h" 2 3 4





# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/socket.h" 1 3 4
# 29 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/socket.h" 3 4
# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 1 3 4
# 30 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/socket.h" 2 3 4

# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/limits.h" 1 3 4
# 11 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/limits.h" 3 4
# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/syslimits.h" 1 3 4






# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/limits.h" 1 3 4
# 122 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/limits.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/limits.h" 1 3 4
# 144 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/limits.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/posix1_lim.h" 1 3 4
# 130 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/posix1_lim.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/local_lim.h" 1 3 4
# 36 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/local_lim.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/linux/limits.h" 1 3 4
# 37 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/local_lim.h" 2 3 4
# 131 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/posix1_lim.h" 2 3 4
# 145 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/limits.h" 2 3 4



# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/posix2_lim.h" 1 3 4
# 149 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/limits.h" 2 3 4
# 123 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/limits.h" 2 3 4
# 8 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/syslimits.h" 2 3 4
# 12 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/limits.h" 2 3 4
# 32 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/socket.h" 2 3 4




typedef __socklen_t socklen_t;




enum __socket_type
{
  SOCK_DGRAM = 1,


  SOCK_STREAM = 2,


  SOCK_RAW = 3,

  SOCK_RDM = 4,

  SOCK_SEQPACKET = 5,


  SOCK_PACKET = 10



};
# 142 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/socket.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sockaddr.h" 1 3 4
# 29 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sockaddr.h" 3 4
typedef unsigned short int sa_family_t;
# 143 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/socket.h" 2 3 4


struct sockaddr
  {
    sa_family_t sa_family;
    char sa_data[14];
  };
# 162 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/socket.h" 3 4
struct sockaddr_storage
  {
    sa_family_t ss_family;
    __uint32_t __ss_align;
    char __ss_padding[(128 - (2 * sizeof (__uint32_t)))];
  };



enum
  {
    MSG_OOB = 0x01,

    MSG_PEEK = 0x02,

    MSG_DONTROUTE = 0x04,






    MSG_CTRUNC = 0x08,

    MSG_PROXY = 0x10,

    MSG_TRUNC = 0x20,

    MSG_DONTWAIT = 0x40,

    MSG_EOR = 0x80,

    MSG_WAITALL = 0x100,

    MSG_FIN = 0x200,

    MSG_SYN = 0x400,

    MSG_CONFIRM = 0x800,

    MSG_RST = 0x1000,

    MSG_ERRQUEUE = 0x2000,

    MSG_NOSIGNAL = 0x4000,

    MSG_MORE = 0x8000

  };




struct msghdr
  {
    void *msg_name;
    socklen_t msg_namelen;

    struct iovec *msg_iov;
    int msg_iovlen;

    void *msg_control;
    socklen_t msg_controllen;

    int msg_flags;
  };


struct cmsghdr
  {
    size_t cmsg_len;

    int cmsg_level;
    int cmsg_type;

    __extension__ unsigned char __cmsg_data [];

  };
# 257 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/socket.h" 3 4
extern struct cmsghdr *__cmsg_nxthdr (struct msghdr *__mhdr,
                                      struct cmsghdr *__cmsg) ;
# 284 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/socket.h" 3 4
enum
  {
    SCM_RIGHTS = 0x01,


    SCM_CREDENTIALS = 0x02,


    __SCM_CONNECT = 0x03
  };



struct ucred
{
  pid_t pid;
  uid_t uid;
  gid_t gid;
};


# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/asm/socket.h" 1 3 4



# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/asm/sockios.h" 1 3 4
# 13 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/asm/sockios.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/asm/ioctl.h" 1 3 4
# 14 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/asm/sockios.h" 2 3 4
# 5 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/asm/socket.h" 2 3 4
# 306 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/socket.h" 2 3 4



struct linger
  {
    int l_onoff;
    int l_linger;
  };
# 36 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/socket.h" 2 3 4




struct osockaddr
  {
    unsigned short int sa_family;
    unsigned char sa_data[14];
  };




enum
{
  SHUT_RD = 0,

  SHUT_WR,

  SHUT_RDWR

};
# 100 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/socket.h" 3 4
extern int socket (int __domain, int __type, int __protocol) ;





extern int socketpair (int __domain, int __type, int __protocol,
                       int __fds[2]) ;


extern int bind (int __fd, __const struct sockaddr * __addr, socklen_t __len)
     ;


extern int getsockname (int __fd, struct sockaddr *__restrict __addr,
                        socklen_t *__restrict __len) ;





extern int connect (int __fd, __const struct sockaddr * __addr, socklen_t __len)
     ;



extern int getpeername (int __fd, struct sockaddr *__restrict __addr,
                        socklen_t *__restrict __len) ;



extern ssize_t send (int __fd, __const void *__buf, size_t __n, int __flags)
     ;



extern ssize_t recv (int __fd, void *__buf, size_t __n, int __flags)
     ;



extern ssize_t sendto (int __fd, __const void *__buf, size_t __n,
                       int __flags, __const struct sockaddr * __addr,
                       socklen_t __addr_len) ;





extern ssize_t recvfrom (int __fd, void *__restrict __buf, size_t __n, int __flags,
                         struct sockaddr *__restrict __addr, socklen_t *__restrict __addr_len)
     ;




extern ssize_t sendmsg (int __fd, __const struct msghdr *__message, int __flags)
     ;



extern ssize_t recvmsg (int __fd, struct msghdr *__message, int __flags)
     ;





extern int getsockopt (int __fd, int __level, int __optname,
                       void *__restrict __optval,
                       socklen_t *__restrict __optlen) ;




extern int setsockopt (int __fd, int __level, int __optname,
                       __const void *__optval, socklen_t __optlen) ;





extern int listen (int __fd, int __n) ;






extern int accept (int __fd, struct sockaddr *__restrict __addr,
                   socklen_t *__restrict __addr_len)
     ;







extern int shutdown (int __fd, int __how) ;
# 212 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/socket.h" 3 4
extern int isfdtype (int __fd, int __fdtype) ;



# 41 "evrpc.c" 2


# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/time.h" 1 3 4
# 26 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/time.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/time.h" 1 3 4
# 27 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/time.h" 2 3 4

# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/time.h" 1 3 4
# 29 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/time.h" 2 3 4




typedef __suseconds_t suseconds_t;





# 56 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/time.h" 3 4
struct timezone
  {
    int tz_minuteswest;
    int tz_dsttime;
  };

typedef struct timezone *__restrict __timezone_ptr_t;
# 72 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/time.h" 3 4
extern int gettimeofday (struct timeval *__restrict __tv,
                         __timezone_ptr_t __tz) ;




extern int settimeofday (__const struct timeval *__tv,
                         __const struct timezone *__tz) ;





extern int adjtime (__const struct timeval *__delta,
                    struct timeval *__olddelta) ;




enum __itimer_which
  {

    ITIMER_REAL = 0,


    ITIMER_VIRTUAL = 1,



    ITIMER_PROF = 2

  };



struct itimerval
  {

    struct timeval it_interval;

    struct timeval it_value;
  };






typedef int __itimer_which_t;




extern int getitimer (__itimer_which_t __which,
                      struct itimerval *__value) ;




extern int setitimer (__itimer_which_t __which,
                      __const struct itimerval *__restrict __new,
                      struct itimerval *__restrict __old) ;



extern int utimes (__const char *__file, __const struct timeval __tvp[2])
     ;
# 171 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/time.h" 3 4

# 44 "evrpc.c" 2

# 1 "compat/sys/queue.h" 1
# 46 "evrpc.c" 2
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 1 3 4
# 30 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4




# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 1 3 4
# 35 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 2 3 4
# 45 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
typedef struct _UC_FILE FILE;
# 55 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
typedef struct _UC_FILE __FILE;
# 67 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uClibc_stdio.h" 1 3 4
# 149 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uClibc_stdio.h" 3 4
# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 1 3 4
# 325 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 3 4
typedef int wchar_t;
# 150 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uClibc_stdio.h" 2 3 4




# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/wchar.h" 1 3 4
# 52 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/wchar.h" 3 4
# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 1 3 4
# 354 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 3 4
typedef unsigned int wint_t;
# 53 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/wchar.h" 2 3 4

# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/wchar.h" 1 3 4
# 55 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/wchar.h" 2 3 4
# 81 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/wchar.h" 3 4
typedef struct
{
        wchar_t mask;
        wchar_t wc;
} __mbstate_t;
# 155 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uClibc_stdio.h" 2 3 4
# 203 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uClibc_stdio.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/pthreadtypes.h" 1 3 4
# 204 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uClibc_stdio.h" 2 3 4
# 241 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uClibc_stdio.h" 3 4
typedef struct {
        __off_t __pos;

        __mbstate_t __mbstate;


        int mblen_pending;

} __stdio_fpos_t;


typedef struct {
        __off64_t __pos;

        __mbstate_t __mbstate;


        int mblen_pending;

} __stdio_fpos64_t;





typedef __off64_t __offmax_t;
# 275 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uClibc_stdio.h" 3 4
typedef __ssize_t __io_read_fn(void *cookie,
                                                           char *buf, size_t bufsize);
typedef __ssize_t __io_write_fn(void *cookie,
                                                                const char *buf, size_t bufsize);




typedef int __io_seek_fn(void *cookie,
                                                 __offmax_t *pos, int whence);
typedef int __io_close_fn(void *cookie);

typedef struct {
        __io_read_fn *read;
        __io_write_fn *write;
        __io_seek_fn *seek;
        __io_close_fn *close;
} _IO_cookie_io_functions_t;
# 318 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uClibc_stdio.h" 3 4
struct _UC_FILE {
        unsigned short modeflags;



        unsigned char ungot_width[2];
        wchar_t ungot[2];



        int filedes;

        struct _UC_FILE *nextopen;


        unsigned char *bufstart;
        unsigned char *bufend;
        unsigned char *bufpos;
        unsigned char *bufread;

        unsigned char *bufgetc;


        unsigned char *bufputc;



        void *cookie;
        _IO_cookie_io_functions_t gcs;


        __mbstate_t state;


        int user_locking;
        pthread_mutex_t lock;





};
# 391 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uClibc_stdio.h" 3 4
extern __ssize_t _cs_read(void *cookie, char *buf, size_t bufsize);
extern __ssize_t _cs_write(void *cookie, const char *buf, size_t bufsize);
extern int _cs_seek(void *cookie, __offmax_t *pos, int whence);
extern int _cs_close(void *cookie);
# 464 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uClibc_stdio.h" 3 4
extern FILE *_stdio_openlist;


extern pthread_mutex_t _stdio_openlist_lock;
extern void __stdio_init_mutex(pthread_mutex_t *m);


extern int _stdio_adjpos(FILE * __restrict stream, __offmax_t * pos);
extern int _stdio_lseek(FILE *stream, __offmax_t *pos, int whence);

extern size_t _stdio_fwrite(const unsigned char *buffer, size_t bytes,
                                                          FILE *stream);
extern size_t _stdio_fread(unsigned char *buffer, size_t bytes,
                                                         FILE *stream);

extern FILE *_stdio_fopen(const char * __restrict filename,
                                                        const char * __restrict mode,
                                                        FILE * __restrict stream, int filedes);

extern FILE *_stdio_fsfopen(const char * __restrict filename,
                                                        const char * __restrict mode,
                                                        register FILE * __restrict stream);

extern void _stdio_init(void);
extern void _stdio_term(void);


extern void __stdio_validate_FILE(FILE *stream);





extern size_t _wstdio_fwrite(const wchar_t *__restrict ws, size_t n,
                                                         register FILE *__restrict stream);
# 592 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uClibc_stdio.h" 3 4
typedef __stdio_fpos_t _UC_fpos_t;

typedef __stdio_fpos64_t _UC_fpos64_t;
# 68 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 2 3 4



# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stdarg.h" 1 3 4
# 43 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 72 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 2 3 4



typedef _UC_fpos_t fpos_t;
# 124 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/stdio_lim.h" 1 3 4
# 125 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 2 3 4



extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
# 139 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern int remove (__const char *__filename) ;

extern int rename (__const char *__old, __const char *__new) ;




extern FILE *tmpfile (void) ;
# 158 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern char *tmpnam (char *__s) ;




extern char *tmpnam_r (char *__s) ;
# 175 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern char *tempnam (__const char *__dir, __const char *__pfx)
     __attribute__ ((__malloc__));




extern int fclose (FILE *__stream) ;

extern int fflush (FILE *__stream) ;



extern int fflush_unlocked (FILE *__stream) ;
# 198 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern FILE *fopen (__const char *__restrict __filename,
                    __const char *__restrict __modes) ;

extern FILE *freopen (__const char *__restrict __filename,
                      __const char *__restrict __modes,
                      FILE *__restrict __stream) ;
# 228 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern FILE *fdopen (int __fd, __const char *__modes) ;
# 253 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern void setbuf (FILE *__restrict __stream, char *__restrict __buf) ;



extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
                    int __modes, size_t __n) ;




extern void setbuffer (FILE *__restrict __stream, char *__restrict __buf,
                       size_t __size) ;


extern void setlinebuf (FILE *__stream) ;




extern int fprintf (FILE *__restrict __stream,
                    __const char *__restrict __format, ...) ;

extern int printf (__const char *__restrict __format, ...) ;

extern int sprintf (char *__restrict __s,
                    __const char *__restrict __format, ...) ;


extern int vfprintf (FILE *__restrict __s, __const char *__restrict __format,
                     __gnuc_va_list __arg) ;

extern int vprintf (__const char *__restrict __format, __gnuc_va_list __arg)
     ;

extern int vsprintf (char *__restrict __s, __const char *__restrict __format,
                     __gnuc_va_list __arg) ;



extern int snprintf (char *__restrict __s, size_t __maxlen,
                     __const char *__restrict __format, ...)
     __attribute__ ((__format__ (__printf__, 3, 4)));

extern int vsnprintf (char *__restrict __s, size_t __maxlen,
                      __const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__format__ (__printf__, 3, 0)));
# 324 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern int fscanf (FILE *__restrict __stream,
                   __const char *__restrict __format, ...) ;

extern int scanf (__const char *__restrict __format, ...) ;

extern int sscanf (__const char *__restrict __s,
                   __const char *__restrict __format, ...) ;
# 350 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern int fgetc (FILE *__stream) ;
extern int getc (FILE *__stream) ;


extern int getchar (void) ;
# 366 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern int getc_unlocked (FILE *__stream) ;
extern int getchar_unlocked (void) ;







extern int fgetc_unlocked (FILE *__stream) ;




extern int fputc (int __c, FILE *__stream) ;
extern int putc (int __c, FILE *__stream) ;


extern int putchar (int __c) ;
# 396 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern int fputc_unlocked (int __c, FILE *__stream) ;




extern int putc_unlocked (int __c, FILE *__stream) ;
extern int putchar_unlocked (int __c) ;
# 411 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern int getw (FILE *__stream) ;


extern int putw (int __w, FILE *__stream) ;




extern char *fgets (char *__restrict __s, int __n, FILE *__restrict __stream)
     ;



extern char *fgets_unlocked (char *__restrict __s, int __n,
                             FILE *__restrict __stream) ;




extern char *gets (char *__s) ;
# 454 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern int fputs (__const char *__restrict __s, FILE *__restrict __stream)
     ;
# 464 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern int puts (__const char *__s) ;



extern int ungetc (int __c, FILE *__stream) ;



extern size_t fread (void *__restrict __ptr, size_t __size,
                     size_t __n, FILE *__restrict __stream) ;

extern size_t fwrite (__const void *__restrict __ptr, size_t __size,
                      size_t __n, FILE *__restrict __s) ;



extern size_t fread_unlocked (void *__restrict __ptr, size_t __size,
                              size_t __n, FILE *__restrict __stream) ;
extern size_t fwrite_unlocked (__const void *__restrict __ptr, size_t __size,
                               size_t __n, FILE *__restrict __stream) ;




extern int fseek (FILE *__stream, long int __off, int __whence) ;

extern long int ftell (FILE *__stream) ;

extern void rewind (FILE *__stream) ;
# 508 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern int fgetpos (FILE *__restrict __stream, fpos_t *__restrict __pos)
     ;

extern int fsetpos (FILE *__stream, __const fpos_t *__pos) ;
# 544 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern void clearerr (FILE *__stream) ;

extern int feof (FILE *__stream) ;

extern int ferror (FILE *__stream) ;



extern void clearerr_unlocked (FILE *__stream) ;
extern int feof_unlocked (FILE *__stream) ;
extern int ferror_unlocked (FILE *__stream) ;




extern void perror (__const char *__s) ;
# 578 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern int fileno (FILE *__stream) ;




extern int fileno_unlocked (FILE *__stream) ;






extern FILE *popen (__const char *__command, __const char *__modes) ;


extern int pclose (FILE *__stream) ;





extern char *ctermid (char *__s) ;
# 625 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
extern void flockfile (FILE *__stream) ;



extern int ftrylockfile (FILE *__stream) ;


extern void funlockfile (FILE *__stream) ;
# 650 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4

# 47 "evrpc.c" 2
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 1 3 4
# 35 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4
# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 1 3 4
# 36 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 2 3 4


# 95 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4


typedef struct
  {
    int quot;
    int rem;
  } div_t;



typedef struct
  {
    long int quot;
    long int rem;
  } ldiv_t;



# 143 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4
extern size_t _stdlib_mb_cur_max (void) ;






extern double atof (__const char *__nptr) __attribute__ ((__pure__));


extern int atoi (__const char *__nptr) __attribute__ ((__pure__));

extern long int atol (__const char *__nptr) __attribute__ ((__pure__));

# 167 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4


extern double strtod (__const char *__restrict __nptr,
                      char **__restrict __endptr) ;

# 185 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4


extern long int strtol (__const char *__restrict __nptr,
                        char **__restrict __endptr, int __base) ;

extern unsigned long int strtoul (__const char *__restrict __nptr,
                                  char **__restrict __endptr, int __base)
     ;

# 455 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4
extern char *l64a (long int __n) ;


extern long int a64l (__const char *__s) __attribute__ ((__pure__));
# 470 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4
extern long int random (void) ;


extern void srandom (unsigned int __seed) ;





extern char *initstate (unsigned int __seed, char *__statebuf,
                        size_t __statelen) ;



extern char *setstate (char *__statebuf) ;







struct random_data
  {
    int32_t *fptr;
    int32_t *rptr;
    int32_t *state;
    int rand_type;
    int rand_deg;
    int rand_sep;
    int32_t *end_ptr;
  };

extern int random_r (struct random_data *__restrict __buf,
                     int32_t *__restrict __result) ;

extern int srandom_r (unsigned int __seed, struct random_data *__buf) ;

extern int initstate_r (unsigned int __seed, char *__restrict __statebuf,
                        size_t __statelen,
                        struct random_data *__restrict __buf) ;

extern int setstate_r (char *__restrict __statebuf,
                       struct random_data *__restrict __buf) ;






extern int rand (void) ;

extern void srand (unsigned int __seed) ;




extern int rand_r (unsigned int *__seed) ;
# 536 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4
extern double drand48 (void) ;
extern double erand48 (unsigned short int __xsubi[3]) ;



extern long int lrand48 (void) ;
extern long int nrand48 (unsigned short int __xsubi[3]) ;


extern long int mrand48 (void) ;
extern long int jrand48 (unsigned short int __xsubi[3]) ;


extern void srand48 (long int __seedval) ;
extern unsigned short int *seed48 (unsigned short int __seed16v[3]) ;
extern void lcong48 (unsigned short int __param[7]) ;





struct drand48_data
  {
    unsigned short int __x[3];
    unsigned short int __old_x[3];
    unsigned short int __c;
    unsigned short int __init;
    unsigned long long int __a;
  };



extern int drand48_r (struct drand48_data *__restrict __buffer,
                      double *__restrict __result) ;
extern int erand48_r (unsigned short int __xsubi[3],
                      struct drand48_data *__restrict __buffer,
                      double *__restrict __result) ;



extern int lrand48_r (struct drand48_data *__restrict __buffer,
                      long int *__restrict __result) ;
extern int nrand48_r (unsigned short int __xsubi[3],
                      struct drand48_data *__restrict __buffer,
                      long int *__restrict __result) ;


extern int mrand48_r (struct drand48_data *__restrict __buffer,
                      long int *__restrict __result) ;
extern int jrand48_r (unsigned short int __xsubi[3],
                      struct drand48_data *__restrict __buffer,
                      long int *__restrict __result) ;


extern int srand48_r (long int __seedval, struct drand48_data *__buffer)
     ;

extern int seed48_r (unsigned short int __seed16v[3],
                     struct drand48_data *__buffer) ;

extern int lcong48_r (unsigned short int __param[7],
                      struct drand48_data *__buffer) ;









extern void *malloc (size_t __size) __attribute__ ((__malloc__));

extern void *calloc (size_t __nmemb, size_t __size)
     __attribute__ ((__malloc__));







extern void *realloc (void *__ptr, size_t __size) __attribute__ ((__malloc__));

extern void free (void *__ptr) ;




extern void cfree (void *__ptr) ;



# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/alloca.h" 1 3 4
# 25 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/alloca.h" 3 4
# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 1 3 4
# 26 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/alloca.h" 2 3 4







extern void *alloca (size_t __size) ;






# 630 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 2 3 4




extern void *valloc (size_t __size) __attribute__ ((__malloc__));
# 657 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4


extern void abort (void) __attribute__ ((__noreturn__));



extern int atexit (void (*__func) (void)) ;





extern int on_exit (void (*__func) (int __status, void *__arg), void *__arg)
     ;






extern void exit (int __status) __attribute__ ((__noreturn__));

# 689 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4


extern char *getenv (__const char *__name) ;




extern char *__secure_getenv (__const char *__name) ;





extern int putenv (char *__string) ;





extern int setenv (__const char *__name, __const char *__value, int __replace)
     ;


extern int unsetenv (__const char *__name) ;
# 728 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4
extern int clearenv (void) ;
# 737 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4
extern char *mktemp (char *__template) ;
# 748 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4
extern int mkstemp (char *__template);
# 767 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4
extern char *mkdtemp (char *__template) ;








extern int system (__const char *__command);

# 795 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4
extern char *realpath (__const char *__restrict __name,
                       char *__restrict __resolved) ;






typedef int (*__compar_fn_t) (__const void *, __const void *);









extern void *bsearch (__const void *__key, __const void *__base,
                      size_t __nmemb, size_t __size, __compar_fn_t __compar);



extern void qsort (void *__base, size_t __nmemb, size_t __size,
                   __compar_fn_t __compar);



extern int abs (int __x) __attribute__ ((__const__));
extern long int labs (long int __x) __attribute__ ((__const__));












extern div_t div (int __numer, int __denom)
     __attribute__ ((__const__));
extern ldiv_t ldiv (long int __numer, long int __denom)
     __attribute__ ((__const__));

# 907 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4



extern int mblen (__const char *__s, size_t __n) ;


extern int mbtowc (wchar_t *__restrict __pwc,
                   __const char *__restrict __s, size_t __n) ;


extern int wctomb (char *__s, wchar_t __wchar) ;



extern size_t mbstowcs (wchar_t *__restrict __pwcs,
                        __const char *__restrict __s, size_t __n) ;

extern size_t wcstombs (char *__restrict __s,
                        __const wchar_t *__restrict __pwcs, size_t __n)
     ;

# 936 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4
extern int rpmatch (__const char *__response) ;
# 999 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdlib.h" 3 4
extern int getloadavg (double __loadavg[], int __nelem) ;






# 48 "evrpc.c" 2

# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 1 3 4
# 28 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4

# 175 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/posix_opt.h" 1 3 4
# 176 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 2 3 4
# 199 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 1 3 4
# 200 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 2 3 4
# 240 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
typedef __intptr_t intptr_t;
# 260 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern int access (__const char *__name, int __type) ;
# 291 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern __off_t lseek (int __fd, __off_t __offset, int __whence) ;
# 307 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern int close (int __fd) ;



extern ssize_t read (int __fd, void *__buf, size_t __nbytes) ;


extern ssize_t write (int __fd, __const void *__buf, size_t __n) ;
# 354 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern int pipe (int __pipedes[2]) ;
# 363 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern unsigned int alarm (unsigned int __seconds) ;
# 372 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern unsigned int sleep (unsigned int __seconds) ;






extern __useconds_t ualarm (__useconds_t __value, __useconds_t __interval)
     ;



extern int usleep (__useconds_t __useconds) ;





extern int pause (void) ;



extern int chown (__const char *__file, __uid_t __owner, __gid_t __group)
     ;



extern int fchown (int __fd, __uid_t __owner, __gid_t __group) ;




extern int lchown (__const char *__file, __uid_t __owner, __gid_t __group)
     ;




extern int chdir (__const char *__path) ;



extern int fchdir (int __fd) ;
# 424 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern char *getcwd (char *__buf, size_t __size) ;




extern char *get_current_dir_name (void) ;





extern char *getwd (char *__buf) ;




extern int dup (int __fd) ;


extern int dup2 (int __fd, int __fd2) ;


extern char **__environ;







extern int execve (__const char *__path, char *__const __argv[],
                   char *__const __envp[]) ;
# 467 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern int execv (__const char *__path, char *__const __argv[]) ;



extern int execle (__const char *__path, __const char *__arg, ...) ;



extern int execl (__const char *__path, __const char *__arg, ...) ;



extern int execvp (__const char *__file, char *__const __argv[]) ;




extern int execlp (__const char *__file, __const char *__arg, ...) ;




extern int nice (int __inc) ;




extern void _exit (int __status) __attribute__ ((__noreturn__));





# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/confname.h" 1 3 4
# 25 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/confname.h" 3 4
enum
  {
    _PC_LINK_MAX,

    _PC_MAX_CANON,

    _PC_MAX_INPUT,

    _PC_NAME_MAX,

    _PC_PATH_MAX,

    _PC_PIPE_BUF,

    _PC_CHOWN_RESTRICTED,

    _PC_NO_TRUNC,

    _PC_VDISABLE,

    _PC_SYNC_IO,

    _PC_ASYNC_IO,

    _PC_PRIO_IO,

    _PC_SOCK_MAXBUF,

    _PC_FILESIZEBITS,

    _PC_REC_INCR_XFER_SIZE,

    _PC_REC_MAX_XFER_SIZE,

    _PC_REC_MIN_XFER_SIZE,

    _PC_REC_XFER_ALIGN,

    _PC_ALLOC_SIZE_MIN,

    _PC_SYMLINK_MAX

  };


enum
  {
    _SC_ARG_MAX,

    _SC_CHILD_MAX,

    _SC_CLK_TCK,

    _SC_NGROUPS_MAX,

    _SC_OPEN_MAX,

    _SC_STREAM_MAX,

    _SC_TZNAME_MAX,

    _SC_JOB_CONTROL,

    _SC_SAVED_IDS,

    _SC_REALTIME_SIGNALS,

    _SC_PRIORITY_SCHEDULING,

    _SC_TIMERS,

    _SC_ASYNCHRONOUS_IO,

    _SC_PRIORITIZED_IO,

    _SC_SYNCHRONIZED_IO,

    _SC_FSYNC,

    _SC_MAPPED_FILES,

    _SC_MEMLOCK,

    _SC_MEMLOCK_RANGE,

    _SC_MEMORY_PROTECTION,

    _SC_MESSAGE_PASSING,

    _SC_SEMAPHORES,

    _SC_SHARED_MEMORY_OBJECTS,

    _SC_AIO_LISTIO_MAX,

    _SC_AIO_MAX,

    _SC_AIO_PRIO_DELTA_MAX,

    _SC_DELAYTIMER_MAX,

    _SC_MQ_OPEN_MAX,

    _SC_MQ_PRIO_MAX,

    _SC_VERSION,

    _SC_PAGESIZE,


    _SC_RTSIG_MAX,

    _SC_SEM_NSEMS_MAX,

    _SC_SEM_VALUE_MAX,

    _SC_SIGQUEUE_MAX,

    _SC_TIMER_MAX,




    _SC_BC_BASE_MAX,

    _SC_BC_DIM_MAX,

    _SC_BC_SCALE_MAX,

    _SC_BC_STRING_MAX,

    _SC_COLL_WEIGHTS_MAX,

    _SC_EQUIV_CLASS_MAX,

    _SC_EXPR_NEST_MAX,

    _SC_LINE_MAX,

    _SC_RE_DUP_MAX,

    _SC_CHARCLASS_NAME_MAX,


    _SC_2_VERSION,

    _SC_2_C_BIND,

    _SC_2_C_DEV,

    _SC_2_FORT_DEV,

    _SC_2_FORT_RUN,

    _SC_2_SW_DEV,

    _SC_2_LOCALEDEF,


    _SC_PII,

    _SC_PII_XTI,

    _SC_PII_SOCKET,

    _SC_PII_INTERNET,

    _SC_PII_OSI,

    _SC_POLL,

    _SC_SELECT,

    _SC_UIO_MAXIOV,

    _SC_IOV_MAX = _SC_UIO_MAXIOV,

    _SC_PII_INTERNET_STREAM,

    _SC_PII_INTERNET_DGRAM,

    _SC_PII_OSI_COTS,

    _SC_PII_OSI_CLTS,

    _SC_PII_OSI_M,

    _SC_T_IOV_MAX,



    _SC_THREADS,

    _SC_THREAD_SAFE_FUNCTIONS,

    _SC_GETGR_R_SIZE_MAX,

    _SC_GETPW_R_SIZE_MAX,

    _SC_LOGIN_NAME_MAX,

    _SC_TTY_NAME_MAX,

    _SC_THREAD_DESTRUCTOR_ITERATIONS,

    _SC_THREAD_KEYS_MAX,

    _SC_THREAD_STACK_MIN,

    _SC_THREAD_THREADS_MAX,

    _SC_THREAD_ATTR_STACKADDR,

    _SC_THREAD_ATTR_STACKSIZE,

    _SC_THREAD_PRIORITY_SCHEDULING,

    _SC_THREAD_PRIO_INHERIT,

    _SC_THREAD_PRIO_PROTECT,

    _SC_THREAD_PROCESS_SHARED,


    _SC_NPROCESSORS_CONF,

    _SC_NPROCESSORS_ONLN,

    _SC_PHYS_PAGES,

    _SC_AVPHYS_PAGES,

    _SC_ATEXIT_MAX,

    _SC_PASS_MAX,


    _SC_XOPEN_VERSION,

    _SC_XOPEN_XCU_VERSION,

    _SC_XOPEN_UNIX,

    _SC_XOPEN_CRYPT,

    _SC_XOPEN_ENH_I18N,

    _SC_XOPEN_SHM,


    _SC_2_CHAR_TERM,

    _SC_2_C_VERSION,

    _SC_2_UPE,


    _SC_XOPEN_XPG2,

    _SC_XOPEN_XPG3,

    _SC_XOPEN_XPG4,


    _SC_CHAR_BIT,

    _SC_CHAR_MAX,

    _SC_CHAR_MIN,

    _SC_INT_MAX,

    _SC_INT_MIN,

    _SC_LONG_BIT,

    _SC_WORD_BIT,

    _SC_MB_LEN_MAX,

    _SC_NZERO,

    _SC_SSIZE_MAX,

    _SC_SCHAR_MAX,

    _SC_SCHAR_MIN,

    _SC_SHRT_MAX,

    _SC_SHRT_MIN,

    _SC_UCHAR_MAX,

    _SC_UINT_MAX,

    _SC_ULONG_MAX,

    _SC_USHRT_MAX,


    _SC_NL_ARGMAX,

    _SC_NL_LANGMAX,

    _SC_NL_MSGMAX,

    _SC_NL_NMAX,

    _SC_NL_SETMAX,

    _SC_NL_TEXTMAX,


    _SC_XBS5_ILP32_OFF32,

    _SC_XBS5_ILP32_OFFBIG,

    _SC_XBS5_LP64_OFF64,

    _SC_XBS5_LPBIG_OFFBIG,


    _SC_XOPEN_LEGACY,

    _SC_XOPEN_REALTIME,

    _SC_XOPEN_REALTIME_THREADS,


    _SC_ADVISORY_INFO,

    _SC_BARRIERS,

    _SC_BASE,

    _SC_C_LANG_SUPPORT,

    _SC_C_LANG_SUPPORT_R,

    _SC_CLOCK_SELECTION,

    _SC_CPUTIME,

    _SC_THREAD_CPUTIME,

    _SC_DEVICE_IO,

    _SC_DEVICE_SPECIFIC,

    _SC_DEVICE_SPECIFIC_R,

    _SC_FD_MGMT,

    _SC_FIFO,

    _SC_PIPE,

    _SC_FILE_ATTRIBUTES,

    _SC_FILE_LOCKING,

    _SC_FILE_SYSTEM,

    _SC_MONOTONIC_CLOCK,

    _SC_MULTI_PROCESS,

    _SC_SINGLE_PROCESS,

    _SC_NETWORKING,

    _SC_READER_WRITER_LOCKS,

    _SC_SPIN_LOCKS,

    _SC_REGEXP,

    _SC_REGEX_VERSION,

    _SC_SHELL,

    _SC_SIGNALS,

    _SC_SPAWN,

    _SC_SPORADIC_SERVER,

    _SC_THREAD_SPORADIC_SERVER,

    _SC_SYSTEM_DATABASE,

    _SC_SYSTEM_DATABASE_R,

    _SC_TIMEOUTS,

    _SC_TYPED_MEMORY_OBJECTS,

    _SC_USER_GROUPS,

    _SC_USER_GROUPS_R,

    _SC_2_PBS,

    _SC_2_PBS_ACCOUNTING,

    _SC_2_PBS_LOCATE,

    _SC_2_PBS_MESSAGE,

    _SC_2_PBS_TRACK,

    _SC_SYMLOOP_MAX,

    _SC_STREAMS,

    _SC_2_PBS_CHECKPOINT,


    _SC_V6_ILP32_OFF32,

    _SC_V6_ILP32_OFFBIG,

    _SC_V6_LP64_OFF64,

    _SC_V6_LPBIG_OFFBIG,


    _SC_HOST_NAME_MAX,

    _SC_TRACE,

    _SC_TRACE_EVENT_FILTER,

    _SC_TRACE_INHERIT,

    _SC_TRACE_LOG

  };





enum
  {
    _CS_PATH,
# 563 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/confname.h" 3 4
    _CS_V6_WIDTH_RESTRICTED_ENVS

  };
# 501 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 2 3 4


extern long int pathconf (__const char *__path, int __name) ;


extern long int fpathconf (int __fd, int __name) ;


extern long int sysconf (int __name) __attribute__ ((__const__));



extern size_t confstr (int __name, char *__buf, size_t __len) ;




extern __pid_t getpid (void) ;


extern __pid_t getppid (void) ;




extern __pid_t getpgrp (void) ;
# 536 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern __pid_t __getpgid (__pid_t __pid) ;
# 545 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern int setpgid (__pid_t __pid, __pid_t __pgid) ;
# 562 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern int setpgrp (void) ;
# 580 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern __pid_t setsid (void) ;







extern __uid_t getuid (void) ;


extern __uid_t geteuid (void) ;


extern __gid_t getgid (void) ;


extern __gid_t getegid (void) ;




extern int getgroups (int __size, __gid_t __list[]) ;
# 614 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern int setuid (__uid_t __uid) ;




extern int setreuid (__uid_t __ruid, __uid_t __euid) ;




extern int seteuid (__uid_t __uid) ;






extern int setgid (__gid_t __gid) ;




extern int setregid (__gid_t __rgid, __gid_t __egid) ;




extern int setegid (__gid_t __gid) ;
# 666 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern __pid_t fork (void) ;






extern __pid_t vfork (void) ;





extern char *ttyname (int __fd) ;



extern int ttyname_r (int __fd, char *__buf, size_t __buflen) ;



extern int isatty (int __fd) ;





extern int ttyslot (void) ;




extern int link (__const char *__from, __const char *__to) ;



extern int symlink (__const char *__from, __const char *__to) ;




extern int readlink (__const char *__restrict __path, char *__restrict __buf,
                     size_t __len) ;



extern int unlink (__const char *__name) ;


extern int rmdir (__const char *__path) ;



extern __pid_t tcgetpgrp (int __fd) ;


extern int tcsetpgrp (int __fd, __pid_t __pgrp_id) ;



extern char *getlogin (void) ;
# 736 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern int setlogin (__const char *__name) ;
# 745 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/getopt.h" 1 3 4
# 47 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/getopt.h" 3 4
extern char *optarg;
# 61 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/getopt.h" 3 4
extern int optind;




extern int opterr;



extern int optopt;
# 139 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/getopt.h" 3 4
extern int getopt (int __argc, char *const *__argv, const char *__shortopts);
# 746 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 2 3 4







extern int gethostname (char *__name, size_t __len) ;






extern int sethostname (__const char *__name, size_t __len) ;



extern int sethostid (long int __id) ;





extern int getdomainname (char *__name, size_t __len) ;
extern int setdomainname (__const char *__name, size_t __len) ;





extern int vhangup (void) ;


extern int revoke (__const char *__file) ;







extern int profil (unsigned short int *__sample_buffer, size_t __size,
                   size_t __offset, unsigned int __scale) ;





extern int acct (__const char *__name) ;



extern char *getusershell (void) ;
extern void endusershell (void) ;
extern void setusershell (void) ;





extern int daemon (int __nochdir, int __noclose) ;






extern int chroot (__const char *__path) ;



extern char *getpass (__const char *__prompt) ;





extern int fsync (int __fd) ;






extern long int gethostid (void) ;


extern void sync (void) ;




extern int getpagesize (void) __attribute__ ((__const__));




extern int truncate (__const char *__file, __off_t __length) ;
# 860 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern int ftruncate (int __fd, __off_t __length) ;
# 876 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern int getdtablesize (void) ;
# 885 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern int brk (void *__addr) ;





extern void *sbrk (intptr_t __delta) ;
# 906 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern long int syscall (long int __sysno, ...) ;
# 926 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern int lockf (int __fd, int __cmd, __off_t __len) ;
# 957 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern int fdatasync (int __fildes) ;
# 1002 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/unistd.h" 3 4
extern int pthread_atfork (void (*__prepare) (void),
                           void (*__parent) (void),
                           void (*__child) (void)) ;



# 50 "evrpc.c" 2

# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/errno.h" 1 3 4
# 32 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/errno.h" 3 4




# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/errno.h" 1 3 4
# 22 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/errno.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/errno_values.h" 1 3 4
# 23 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/errno.h" 2 3 4
# 35 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/errno.h" 3 4
extern int errno;


extern int *__errno_location (void) __attribute__ ((__const__));
# 37 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/errno.h" 2 3 4
# 61 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/errno.h" 3 4

# 52 "evrpc.c" 2
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 1 3 4
# 31 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 3 4


# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sigset.h" 1 3 4
# 103 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sigset.h" 3 4
extern int __sigismember (__const __sigset_t *, int);
extern int __sigaddset (__sigset_t *, int);
extern int __sigdelset (__sigset_t *, int);
# 34 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 2 3 4






typedef __sig_atomic_t sig_atomic_t;
# 56 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/signum.h" 1 3 4
# 57 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 2 3 4
# 71 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 3 4
typedef void (*__sighandler_t) (int);




extern __sighandler_t __sysv_signal (int __sig, __sighandler_t __handler)
     ;
# 87 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 3 4
extern __sighandler_t signal (int __sig, __sighandler_t __handler) ;
# 110 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 3 4
extern int kill (__pid_t __pid, int __sig) ;






extern int killpg (__pid_t __pgrp, int __sig) ;



extern int raise (int __sig) ;



extern __sighandler_t ssignal (int __sig, __sighandler_t __handler) ;
extern int gsignal (int __sig) ;




extern void psignal (int __sig, __const char *__s) ;
# 140 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 3 4
extern int __sigpause (int __sig_or_mask, int __is_sig) ;




extern int sigpause (int __mask) ;
# 168 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 3 4
extern int sigblock (int __mask) ;


extern int sigsetmask (int __mask) ;


extern int siggetmask (void) ;
# 188 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 3 4
typedef __sighandler_t sig_t;







# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/time.h" 1 3 4
# 197 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 2 3 4


# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/siginfo.h" 1 3 4
# 31 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/siginfo.h" 3 4
typedef union sigval
  {
    int sival_int;
    void *sival_ptr;
  } sigval_t;
# 45 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/siginfo.h" 3 4
typedef struct siginfo
  {
    int si_signo;
    int si_code;
    int si_errno;


    union
      {
        int _pad[((128 / sizeof (int)) - 3)];


        struct
          {
            __pid_t si_pid;
            __uid_t si_uid;
          } _kill;


        struct
          {
            __pid_t si_pid;
            __uid_t si_uid;
            int si_status;
            __clock_t si_utime;
            __clock_t si_stime;
          } _sigchld;


        struct
          {
            void *si_addr;
          } _sigfault;


        struct
          {
            int si_band;
            int si_fd;
          } _sigpoll;


        struct
          {
            unsigned int _timer1;
            unsigned int _timer2;
          } _timer;


        struct
          {
            __pid_t si_pid;
            __uid_t si_uid;
            sigval_t si_sigval;
          } _rt;
      } _sifields;
  } siginfo_t;
# 120 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/siginfo.h" 3 4
enum
{
  SI_ASYNCNL = -6,

  SI_SIGIO,

  SI_MESGQ,

  SI_TIMER,

  SI_ASYNCIO,

  SI_QUEUE,

  SI_USER,

  SI_KERNEL = 0x80

};



enum
{
  ILL_ILLOPC = 1,

  ILL_ILLOPN,

  ILL_ILLADR,

  ILL_ILLTRP,

  ILL_PRVOPC,

  ILL_PRVREG,

  ILL_COPROC,

  ILL_BADSTK

};


enum
{
  FPE_INTDIV = 1,

  FPE_INTOVF,

  FPE_FLTDIV,

  FPE_FLTOVF,

  FPE_FLTUND,

  FPE_FLTRES,

  FPE_FLTINV,

  FPE_FLTSUB

};


enum
{
  SEGV_MAPERR = 1,

  SEGV_ACCERR

};


enum
{
  BUS_ADRALN = 1,

  BUS_ADRERR,

  BUS_OBJERR

};


enum
{
  TRAP_BRKPT = 1,

  TRAP_TRACE

};


enum
{
  CLD_EXITED = 1,

  CLD_KILLED,

  CLD_DUMPED,

  CLD_TRAPPED,

  CLD_STOPPED,

  CLD_CONTINUED

};


enum
{
  POLL_IN = 1,

  POLL_OUT,

  POLL_MSG,

  POLL_ERR,

  POLL_PRI,

  POLL_HUP

};
# 259 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/siginfo.h" 3 4
struct __pthread_attr_s;


typedef struct sigevent
  {
    sigval_t sigev_value;
    int sigev_signo;
    int sigev_notify;

    union
      {
        int _pad[((64 / sizeof (int)) - 3)];

        struct
          {
            void (*_function) (sigval_t);
            struct __pthread_attr_s *_attribute;
          } _sigev_thread;
      } _sigev_un;
  } sigevent_t;






enum
{
  SIGEV_SIGNAL = 0,

  SIGEV_NONE,

  SIGEV_CALLBACK,

  SIGEV_THREAD

};
# 200 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 2 3 4



extern int sigemptyset (sigset_t *__set) ;


extern int sigfillset (sigset_t *__set) ;


extern int sigaddset (sigset_t *__set, int __signo) ;


extern int sigdelset (sigset_t *__set, int __signo) ;


extern int sigismember (__const sigset_t *__set, int __signo) ;
# 232 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sigaction.h" 1 3 4
# 25 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sigaction.h" 3 4
struct sigaction
  {

    unsigned int sa_flags;



    union
      {

        __sighandler_t sa_handler;

        void (*sa_sigaction) (int, siginfo_t *, void *);
      }
    __sigaction_handler;






    __sigset_t sa_mask;
  };
# 233 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 2 3 4


extern int sigprocmask (int __how, __const sigset_t *__restrict __set,
                        sigset_t *__restrict __oset) ;



extern int sigsuspend (__const sigset_t *__set) ;


extern int sigaction (int __sig, __const struct sigaction *__restrict __act,
                      struct sigaction *__restrict __oact) ;


extern int sigpending (sigset_t *__set) ;



extern int sigwait (__const sigset_t *__restrict __set, int *__restrict __sig)
     ;



extern int sigwaitinfo (__const sigset_t *__restrict __set,
                        siginfo_t *__restrict __info) ;



extern int sigtimedwait (__const sigset_t *__restrict __set,
                         siginfo_t *__restrict __info,
                         __const struct timespec *__restrict __timeout)
     ;



extern int sigqueue (__pid_t __pid, int __sig, __const union sigval __val)
     ;
# 284 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 3 4
struct sigvec
  {
    __sighandler_t sv_handler;
    int sv_mask;

    int sv_flags;

  };
# 304 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 3 4
extern int sigvec (int __sig, __const struct sigvec *__vec,
                   struct sigvec *__ovec) ;



# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sigcontext.h" 1 3 4
# 28 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sigcontext.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/asm/sigcontext.h" 1 3 4
# 15 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/asm/sigcontext.h" 3 4
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
# 29 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sigcontext.h" 2 3 4
# 310 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 2 3 4


extern int sigreturn (struct sigcontext *__scp) ;
# 322 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 3 4
extern int siginterrupt (int __sig, int __interrupt) ;

# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sigstack.h" 1 3 4
# 26 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sigstack.h" 3 4
struct sigstack
  {
    void *ss_sp;
    int ss_onstack;
  };



enum
{
  SS_ONSTACK = 1,

  SS_DISABLE

};
# 50 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sigstack.h" 3 4
typedef struct sigaltstack
  {
    void *ss_sp;
    size_t ss_size;
    int ss_flags;
  } stack_t;
# 325 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 2 3 4







extern int sigstack (struct sigstack *__ss, struct sigstack *__oss) ;



extern int sigaltstack (__const struct sigaltstack *__restrict __ss,
                        struct sigaltstack *__restrict __oss) ;
# 360 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sigthread.h" 1 3 4
# 31 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/sigthread.h" 3 4
extern int pthread_sigmask (int __how,
                            __const __sigset_t *__restrict __newmask,
                            __sigset_t *__restrict __oldmask);


extern int pthread_kill (pthread_t __thread_id, int __signo) ;
# 361 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/signal.h" 2 3 4






extern int __libc_current_sigrtmin (void) ;

extern int __libc_current_sigrtmax (void) ;




# 53 "evrpc.c" 2
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/string.h" 1 3 4
# 28 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/string.h" 3 4





# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 1 3 4
# 34 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/string.h" 2 3 4




extern void *memcpy (void *__restrict __dest,
                     __const void *__restrict __src, size_t __n) ;


extern void *memmove (void *__dest, __const void *__src, size_t __n)
     ;






extern void *memccpy (void *__restrict __dest, __const void *__restrict __src,
                      int __c, size_t __n)
     ;





extern void *memset (void *__s, int __c, size_t __n) ;


extern int memcmp (__const void *__s1, __const void *__s2, size_t __n)
     __attribute__ ((__pure__));


extern void *memchr (__const void *__s, int __c, size_t __n)
      __attribute__ ((__pure__));

# 80 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/string.h" 3 4


extern char *strcpy (char *__restrict __dest, __const char *__restrict __src)
     ;

extern char *strncpy (char *__restrict __dest,
                      __const char *__restrict __src, size_t __n) ;


extern char *strcat (char *__restrict __dest, __const char *__restrict __src)
     ;

extern char *strncat (char *__restrict __dest, __const char *__restrict __src,
                      size_t __n) ;


extern int strcmp (__const char *__s1, __const char *__s2)
     __attribute__ ((__pure__));

extern int strncmp (__const char *__s1, __const char *__s2, size_t __n)
     __attribute__ ((__pure__));


extern int strcoll (__const char *__s1, __const char *__s2)
     __attribute__ ((__pure__));

extern size_t strxfrm (char *__restrict __dest,
                       __const char *__restrict __src, size_t __n) ;

# 134 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/string.h" 3 4
extern char *strdup (__const char *__s) __attribute__ ((__malloc__));
# 168 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/string.h" 3 4


extern char *strchr (__const char *__s, int __c) __attribute__ ((__pure__));

extern char *strrchr (__const char *__s, int __c) __attribute__ ((__pure__));











extern size_t strcspn (__const char *__s, __const char *__reject)
     __attribute__ ((__pure__));


extern size_t strspn (__const char *__s, __const char *__accept)
     __attribute__ ((__pure__));

extern char *strpbrk (__const char *__s, __const char *__accept)
     __attribute__ ((__pure__));

extern char *strstr (__const char *__haystack, __const char *__needle)
     __attribute__ ((__pure__));



extern char *strtok (char *__restrict __s, __const char *__restrict __delim)
     ;




extern char *__strtok_r (char *__restrict __s,
                         __const char *__restrict __delim,
                         char **__restrict __save_ptr) ;

extern char *strtok_r (char *__restrict __s, __const char *__restrict __delim,
                       char **__restrict __save_ptr) ;
# 236 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/string.h" 3 4


extern size_t strlen (__const char *__s) __attribute__ ((__pure__));

# 249 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/string.h" 3 4


extern char *strerror (int __errnum) ;








extern char *_glibc_strerror_r (int __errnum, char *__buf, size_t __buflen) ;
extern int _susv3_strerror_r (int __errnum, char *__buf, size_t buflen) ;
# 273 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/string.h" 3 4
extern char *strerror_r (int __errnum, char *__buf, size_t buflen) __asm__ ("" "_glibc_strerror_r");
# 284 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/string.h" 3 4
extern void __bzero (void *__s, size_t __n) ;



extern void bcopy (__const void *__src, void *__dest, size_t __n) ;


extern void bzero (void *__s, size_t __n) ;


extern int bcmp (__const void *__s1, __const void *__s2, size_t __n)
     __attribute__ ((__pure__));


extern char *index (__const char *__s, int __c) __attribute__ ((__pure__));


extern char *rindex (__const char *__s, int __c) __attribute__ ((__pure__));



extern int ffs (int __i) __attribute__ ((__const__));
# 318 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/string.h" 3 4
extern int strcasecmp (__const char *__s1, __const char *__s2)
     __attribute__ ((__pure__));


extern int strncasecmp (__const char *__s1, __const char *__s2, size_t __n)
     __attribute__ ((__pure__));
# 347 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/string.h" 3 4
extern char *strsep (char **__restrict __stringp,
                     __const char *__restrict __delim) ;
# 425 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/string.h" 3 4
extern size_t strlcat(char *__restrict dst, const char *__restrict src,
                      size_t n) ;
extern size_t strlcpy(char *__restrict dst, const char *__restrict src,
                      size_t n) ;



# 54 "evrpc.c" 2
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/assert.h" 1 3 4
# 36 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/assert.h" 3 4

extern void __assert (const char *, const char *, int, const char *) ;

# 55 "evrpc.c" 2

# 1 "event.h" 1
# 162 "event.h"
# 1 "event-config.h" 1
# 163 "event.h" 2







# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdint.h" 1 3 4
# 30 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdint.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/wordsize.h" 1 3 4
# 31 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdint.h" 2 3 4
# 51 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdint.h" 3 4
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;

typedef unsigned int uint32_t;





__extension__
typedef unsigned long long int uint64_t;






typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;



__extension__
typedef long long int int_least64_t;



typedef unsigned char uint_least8_t;
typedef unsigned short int uint_least16_t;
typedef unsigned int uint_least32_t;



__extension__
typedef unsigned long long int uint_least64_t;






typedef signed char int_fast8_t;





typedef int int_fast16_t;
typedef int int_fast32_t;
__extension__
typedef long long int int_fast64_t;



typedef unsigned char uint_fast8_t;





typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
__extension__
typedef unsigned long long int uint_fast64_t;
# 131 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdint.h" 3 4
typedef unsigned int uintptr_t;
# 140 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdint.h" 3 4
__extension__
typedef long long int intmax_t;
__extension__
typedef unsigned long long int uintmax_t;
# 171 "event.h" 2

# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stdarg.h" 1 3 4
# 105 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stdarg.h" 3 4
typedef __gnuc_va_list va_list;
# 173 "event.h" 2


# 1 "evutil.h" 1
# 101 "evutil.h"
int evutil_socketpair(int d, int type, int protocol, int sv[2]);
int evutil_make_socket_nonblocking(int sock);
# 165 "evutil.h"
int64_t evutil_strtoll(const char *s, char **endptr, int base);
# 175 "evutil.h"
int evutil_snprintf(char *buf, size_t buflen, const char *format, ...)

        __attribute__((format(printf, 3, 4)))

        ;
int evutil_vsnprintf(char *buf, size_t buflen, const char *format, va_list ap);
# 176 "event.h" 2
# 211 "event.h"
struct event_base;

struct event {
        struct { struct event *tqe_next; struct event **tqe_prev; } ev_next;
        struct { struct event *tqe_next; struct event **tqe_prev; } ev_active_next;
        struct { struct event *tqe_next; struct event **tqe_prev; } ev_signal_next;
        unsigned int min_heap_idx;

        struct event_base *ev_base;

        int ev_fd;
        short ev_events;
        short ev_ncalls;
        short *ev_pncalls;

        struct timeval ev_timeout;

        int ev_pri;

        void (*ev_callback)(int, short, void *arg);
        void *ev_arg;

        int ev_res;
        int ev_flags;
};
# 247 "event.h"
struct evkeyval {
        struct { struct evkeyval *tqe_next; struct evkeyval **tqe_prev; } next;

        char *key;
        char *value;
};







struct event_list { struct event *tqh_first; struct event **tqh_last; };
struct evkeyvalq { struct evkeyval *tqh_first; struct evkeyval **tqh_last; };
# 273 "event.h"
struct event_base *event_base_new(void);
# 284 "event.h"
struct event_base *event_init(void);
# 296 "event.h"
int event_reinit(struct event_base *base);
# 307 "event.h"
int event_dispatch(void);
# 316 "event.h"
int event_base_dispatch(struct event_base *);
# 325 "event.h"
const char *event_base_get_method(struct event_base *);
# 336 "event.h"
void event_base_free(struct event_base *);






typedef void (*event_log_cb)(int severity, const char *msg);







void event_set_log_callback(event_log_cb cb);







int event_base_set(struct event_base *, struct event *);
# 379 "event.h"
int event_loop(int);
# 392 "event.h"
int event_base_loop(struct event_base *, int);
# 407 "event.h"
int event_loopexit(const struct timeval *);
# 424 "event.h"
int event_base_loopexit(struct event_base *, const struct timeval *);
# 438 "event.h"
int event_loopbreak(void);
# 453 "event.h"
int event_base_loopbreak(struct event_base *);
# 546 "event.h"
void event_set(struct event *, int, short, void (*)(int, short, void *), void *);
# 566 "event.h"
int event_once(int, short, void (*)(int, short, void *), void *,
    const struct timeval *);
# 588 "event.h"
int event_base_once(struct event_base *base, int fd, short events,
    void (*callback)(int, short, void *), void *arg,
    const struct timeval *timeout);
# 611 "event.h"
int event_add(struct event *ev, const struct timeval *timeout);
# 625 "event.h"
int event_del(struct event *);

void event_active(struct event *, int, short);
# 641 "event.h"
int event_pending(struct event *ev, short event, struct timeval *tv);
# 666 "event.h"
const char *event_get_version(void);







const char *event_get_method(void);
# 697 "event.h"
int event_priority_init(int);
# 710 "event.h"
int event_base_priority_init(struct event_base *, int);
# 721 "event.h"
int event_priority_set(struct event *, int);




struct evbuffer {
        u_char *buffer;
        u_char *orig_buffer;

        size_t misalign;
        size_t totallen;
        size_t off;

        void (*cb)(struct evbuffer *, size_t, size_t, void *);
        void *cbarg;
};
# 745 "event.h"
struct bufferevent;
typedef void (*evbuffercb)(struct bufferevent *, void *);
typedef void (*everrorcb)(struct bufferevent *, short what, void *);

struct event_watermark {
        size_t low;
        size_t high;
};


struct bufferevent {
        struct event_base *ev_base;

        struct event ev_read;
        struct event ev_write;

        struct evbuffer *input;
        struct evbuffer *output;

        struct event_watermark wm_read;
        struct event_watermark wm_write;

        evbuffercb readcb;
        evbuffercb writecb;
        everrorcb errorcb;
        void *cbarg;

        int timeout_read;
        int timeout_write;

        short enabled;
};
# 813 "event.h"
struct bufferevent *bufferevent_new(int fd,
    evbuffercb readcb, evbuffercb writecb, everrorcb errorcb, void *cbarg);
# 825 "event.h"
int bufferevent_base_set(struct event_base *base, struct bufferevent *bufev);
# 835 "event.h"
int bufferevent_priority_set(struct bufferevent *bufev, int pri);







void bufferevent_free(struct bufferevent *bufev);
# 860 "event.h"
void bufferevent_setcb(struct bufferevent *bufev,
    evbuffercb readcb, evbuffercb writecb, everrorcb errorcb, void *cbarg);







void bufferevent_setfd(struct bufferevent *bufev, int fd);
# 884 "event.h"
int bufferevent_write(struct bufferevent *bufev,
    const void *data, size_t size);
# 897 "event.h"
int bufferevent_write_buffer(struct bufferevent *bufev, struct evbuffer *buf);
# 910 "event.h"
size_t bufferevent_read(struct bufferevent *bufev, void *data, size_t size);
# 920 "event.h"
int bufferevent_enable(struct bufferevent *bufev, short event);
# 931 "event.h"
int bufferevent_disable(struct bufferevent *bufev, short event);
# 941 "event.h"
void bufferevent_settimeout(struct bufferevent *bufev,
    int timeout_read, int timeout_write);
# 961 "event.h"
void bufferevent_setwatermark(struct bufferevent *bufev, short events,
    size_t lowmark, size_t highmark);
# 976 "event.h"
struct evbuffer *evbuffer_new(void);







void evbuffer_free(struct evbuffer *);
# 996 "event.h"
int evbuffer_expand(struct evbuffer *, size_t);
# 1006 "event.h"
int evbuffer_add(struct evbuffer *, const void *, size_t);
# 1018 "event.h"
int evbuffer_remove(struct evbuffer *, void *, size_t);
# 1030 "event.h"
char *evbuffer_readline(struct evbuffer *);
# 1043 "event.h"
int evbuffer_add_buffer(struct evbuffer *, struct evbuffer *);
# 1054 "event.h"
int evbuffer_add_printf(struct evbuffer *, const char *fmt, ...)

  __attribute__((format(printf, 2, 3)))

;
# 1069 "event.h"
int evbuffer_add_vprintf(struct evbuffer *, const char *fmt, va_list ap);
# 1078 "event.h"
void evbuffer_drain(struct evbuffer *, size_t);
# 1091 "event.h"
int evbuffer_write(struct evbuffer *, int);
# 1103 "event.h"
int evbuffer_read(struct evbuffer *, int, int);
# 1114 "event.h"
u_char *evbuffer_find(struct evbuffer *, const u_char *, size_t);
# 1123 "event.h"
void evbuffer_setcb(struct evbuffer *, void (*)(struct evbuffer *, size_t, size_t, void *), void *);







void evtag_init(void);

void evtag_marshal(struct evbuffer *evbuf, uint32_t tag, const void *data,
    uint32_t len);
# 1146 "event.h"
void encode_int(struct evbuffer *evbuf, uint32_t number);

void evtag_marshal_int(struct evbuffer *evbuf, uint32_t tag,
    uint32_t integer);

void evtag_marshal_string(struct evbuffer *buf, uint32_t tag,
    const char *string);

void evtag_marshal_timeval(struct evbuffer *evbuf, uint32_t tag,
    struct timeval *tv);

int evtag_unmarshal(struct evbuffer *src, uint32_t *ptag,
    struct evbuffer *dst);
int evtag_peek(struct evbuffer *evbuf, uint32_t *ptag);
int evtag_peek_length(struct evbuffer *evbuf, uint32_t *plength);
int evtag_payload_length(struct evbuffer *evbuf, uint32_t *plength);
int evtag_consume(struct evbuffer *evbuf);

int evtag_unmarshal_int(struct evbuffer *evbuf, uint32_t need_tag,
    uint32_t *pinteger);

int evtag_unmarshal_fixed(struct evbuffer *src, uint32_t need_tag,
    void *data, size_t len);

int evtag_unmarshal_string(struct evbuffer *evbuf, uint32_t need_tag,
    char **pstring);

int evtag_unmarshal_timeval(struct evbuffer *evbuf, uint32_t need_tag,
    struct timeval *ptv);
# 57 "evrpc.c" 2
# 1 "evrpc.h" 1
# 69 "evrpc.h"
struct evbuffer;
struct event_base;
struct evrpc_req_generic;


struct evrpc {
        struct { struct evrpc *tqe_next; struct evrpc **tqe_prev; } next;


        const char* uri;


        void *(*request_new)(void);


        void (*request_free)(void *);


        int (*request_unmarshal)(void *, struct evbuffer *);


        void *(*reply_new)(void);


        void (*reply_free)(void *);


        int (*reply_complete)(void *);


        void (*reply_marshal)(struct evbuffer*, void *);


        void (*cb)(struct evrpc_req_generic *, void *);
        void *cb_arg;


        struct evrpc_base *base;
};







struct evhttp_request;
struct evrpc_status;


struct evrpc_req_generic {

        void *request;


        void *reply;





        struct evrpc *rpc;




        struct evhttp_request* http_req;




        void (*done)(struct evrpc_req_generic* rpc);
};
# 256 "evrpc.h"
struct evrpc_base;
struct evhttp;
# 267 "evrpc.h"
struct evrpc_base *evrpc_init(struct evhttp *server);
# 277 "evrpc.h"
void evrpc_free(struct evrpc_base *base);
# 303 "evrpc.h"
int evrpc_register_rpc(struct evrpc_base *, struct evrpc *,
    void (*)(struct evrpc_req_generic*, void *), void *);
# 316 "evrpc.h"
int evrpc_unregister_rpc(struct evrpc_base *base, const char *name);





struct evrpc_pool;
struct evhttp_connection;




struct evrpc_status {





        int error;


        struct evhttp_request *http_req;
};

struct evrpc_request_wrapper {
        struct { struct evrpc_request_wrapper *tqe_next; struct evrpc_request_wrapper **tqe_prev; } next;


        struct evrpc_pool *pool;


        struct evhttp_connection *evcon;


        struct event ev_timeout;


        char *name;


        void (*cb)(struct evrpc_status*, void *request, void *reply, void *arg);
        void *cb_arg;

        void *request;
        void *reply;


        void (*request_marshal)(struct evbuffer *, void *);


        void (*reply_clear)(void *);


        int (*reply_unmarshal)(void *, struct evbuffer*);
};
# 390 "evrpc.h"
int evrpc_make_request(struct evrpc_request_wrapper *);
# 402 "evrpc.h"
struct evrpc_pool *evrpc_pool_new(struct event_base *base);





void evrpc_pool_free(struct evrpc_pool *pool);




void evrpc_pool_add_connection(struct evrpc_pool *,
    struct evhttp_connection *);
# 431 "evrpc.h"
void evrpc_pool_set_timeout(struct evrpc_pool *pool, int timeout_in_secs);






enum EVRPC_HOOK_TYPE {
        EVRPC_INPUT,
        EVRPC_OUTPUT
};
# 465 "evrpc.h"
void *evrpc_add_hook(void *vbase,
    enum EVRPC_HOOK_TYPE hook_type,
    int (*cb)(struct evhttp_request *, struct evbuffer *, void *),
    void *cb_arg);
# 478 "evrpc.h"
int evrpc_remove_hook(void *vbase,
    enum EVRPC_HOOK_TYPE hook_type,
    void *handle);
# 58 "evrpc.c" 2
# 1 "evrpc-internal.h" 1
# 30 "evrpc-internal.h"
# 1 "http-internal.h" 1
# 20 "http-internal.h"
enum message_read_status {
        ALL_DATA_READ = 1,
        MORE_DATA_EXPECTED = 0,
        DATA_CORRUPTED = -1,
        REQUEST_CANCELED = -2
};

enum evhttp_connection_error {
        EVCON_HTTP_TIMEOUT,
        EVCON_HTTP_EOF,
        EVCON_HTTP_INVALID_HEADER
};

struct evbuffer;
struct addrinfo;
struct evhttp_request;



enum evhttp_connection_state {
        EVCON_DISCONNECTED,
        EVCON_CONNECTING,
        EVCON_IDLE,
        EVCON_READING_FIRSTLINE,

        EVCON_READING_HEADERS,
        EVCON_READING_BODY,
        EVCON_READING_TRAILER,
        EVCON_WRITING
};

struct event_base;

struct evhttp_connection {

        struct { struct evhttp_connection *tqe_next; struct evhttp_connection **tqe_prev; } (next);

        int fd;
        struct event ev;
        struct event close_ev;
        struct evbuffer *input_buffer;
        struct evbuffer *output_buffer;

        char *bind_address;
        u_short bind_port;

        char *address;
        u_short port;

        int flags;




        int timeout;
        int retry_cnt;
        int retry_max;

        enum evhttp_connection_state state;


        struct evhttp *http_server;

        struct evcon_requestq { struct evhttp_request *tqh_first; struct evhttp_request **tqh_last; } requests;

                                                   void (*cb)(struct evhttp_connection *, void *);
        void *cb_arg;

        void (*closecb)(struct evhttp_connection *, void *);
        void *closecb_arg;

        struct event_base *base;
};

struct evhttp_cb {
        struct { struct evhttp_cb *tqe_next; struct evhttp_cb **tqe_prev; } next;

        char *what;

        void (*cb)(struct evhttp_request *req, void *);
        void *cbarg;
};


struct evconq { struct evhttp_connection *tqh_first; struct evhttp_connection **tqh_last; };


struct evhttp_bound_socket {
        struct { struct evhttp_bound_socket *tqe_next; struct evhttp_bound_socket **tqe_prev; } (next);

        struct event bind_ev;
};

struct evhttp {
        struct boundq { struct evhttp_bound_socket *tqh_first; struct evhttp_bound_socket **tqh_last; } sockets;

        struct httpcbq { struct evhttp_cb *tqh_first; struct evhttp_cb **tqh_last; } callbacks;
        struct evconq connections;

        int timeout;

        void (*gencb)(struct evhttp_request *req, void *);
        void *gencbarg;

        struct event_base *base;
};


void evhttp_connection_reset(struct evhttp_connection *);


int evhttp_connection_connect(struct evhttp_connection *);


void evhttp_connection_fail(struct evhttp_connection *,
    enum evhttp_connection_error error);

void evhttp_get_request(struct evhttp *, int, struct sockaddr *, socklen_t);

int evhttp_hostportfile(char *, char **, u_short *, char **);

int evhttp_parse_firstline(struct evhttp_request *, struct evbuffer*);
int evhttp_parse_headers(struct evhttp_request *, struct evbuffer*);

void evhttp_start_read(struct evhttp_connection *);
void evhttp_make_header(struct evhttp_connection *, struct evhttp_request *);

void evhttp_write_buffer(struct evhttp_connection *,
    void (*)(struct evhttp_connection *, void *), void *);


void evhttp_response_code(struct evhttp_request *, int, const char *);
void evhttp_send_page(struct evhttp_request *, struct evbuffer *);
# 31 "evrpc-internal.h" 2

struct evrpc;



struct evrpc_hook {
        struct { struct evrpc_hook *tqe_next; struct evrpc_hook **tqe_prev; } (next);


        int (*process)(struct evhttp_request *, struct evbuffer *, void *);
        void *process_arg;
};

struct evrpc_hook_list { struct evrpc_hook *tqh_first; struct evrpc_hook **tqh_last; };






struct _evrpc_hooks {

        struct evrpc_hook_list in_hooks;
        struct evrpc_hook_list out_hooks;
};




struct evrpc_base {
        struct _evrpc_hooks common;


        struct evhttp* http_server;


        struct evrpc_list { struct evrpc *tqh_first; struct evrpc **tqh_last; } registered_rpcs;
};

struct evrpc_req_generic;
void evrpc_reqstate_free(struct evrpc_req_generic* rpc_state);


struct evrpc_pool {
        struct _evrpc_hooks common;

        struct event_base *base;

        struct evconq connections;

        int timeout;

        struct evrpc_requestq { struct evrpc_request_wrapper *tqh_first; struct evrpc_request_wrapper **tqh_last; } requests;
};
# 59 "evrpc.c" 2
# 1 "evhttp.h" 1
# 64 "evhttp.h"
struct evhttp;
struct evhttp_request;
struct evkeyvalq;






struct evhttp *evhttp_new(struct event_base *base);
# 87 "evhttp.h"
int evhttp_bind_socket(struct evhttp *http, const char *address, u_short port);
# 105 "evhttp.h"
int evhttp_accept_socket(struct evhttp *http, int fd);
# 115 "evhttp.h"
void evhttp_free(struct evhttp* http);


void evhttp_set_cb(struct evhttp *, const char *,
    void (*)(struct evhttp_request *, void *), void *);


int evhttp_del_cb(struct evhttp *, const char *);



void evhttp_set_gencb(struct evhttp *,
    void (*)(struct evhttp_request *, void *), void *);







void evhttp_set_timeout(struct evhttp *, int timeout_in_secs);
# 146 "evhttp.h"
void evhttp_send_error(struct evhttp_request *req, int error,
    const char *reason);
# 157 "evhttp.h"
void evhttp_send_reply(struct evhttp_request *req, int code,
    const char *reason, struct evbuffer *databuf);


void evhttp_send_reply_start(struct evhttp_request *, int, const char *);
void evhttp_send_reply_chunk(struct evhttp_request *, struct evbuffer *);
void evhttp_send_reply_end(struct evhttp_request *);
# 174 "evhttp.h"
struct evhttp *evhttp_start(const char *address, u_short port);




enum evhttp_cmd_type { EVHTTP_REQ_GET, EVHTTP_REQ_POST, EVHTTP_REQ_HEAD };

enum evhttp_request_kind { EVHTTP_REQUEST, EVHTTP_RESPONSE };






struct evhttp_request {

        struct { struct evhttp_request *tqe_next; struct evhttp_request **tqe_prev; } next;
# 199 "evhttp.h"
        struct evhttp_connection *evcon;
        int flags;



        struct evkeyvalq *input_headers;
        struct evkeyvalq *output_headers;


        char *remote_host;
        u_short remote_port;

        enum evhttp_request_kind kind;
        enum evhttp_cmd_type type;

        char *uri;

        char major;
        char minor;

        int response_code;
        char *response_code_line;

        struct evbuffer *input_buffer;
        int64_t ntoread;
        int chunked;

        struct evbuffer *output_buffer;


        void (*cb)(struct evhttp_request *, void *);
        void *cb_arg;






        void (*chunk_cb)(struct evhttp_request *, void *);
};






struct evhttp_request *evhttp_request_new(
        void (*cb)(struct evhttp_request *, void *), void *arg);


void evhttp_request_set_chunked_cb(struct evhttp_request *,
    void (*cb)(struct evhttp_request *, void *));


void evhttp_request_free(struct evhttp_request *req);






struct evhttp_connection *evhttp_connection_new(
        const char *address, unsigned short port);


void evhttp_connection_free(struct evhttp_connection *evcon);


void evhttp_connection_set_local_address(struct evhttp_connection *evcon,
    const char *address);


void evhttp_connection_set_local_port(struct evhttp_connection *evcon,
    unsigned short port);


void evhttp_connection_set_timeout(struct evhttp_connection *evcon,
    int timeout_in_secs);


void evhttp_connection_set_retries(struct evhttp_connection *evcon,
    int retry_max);


void evhttp_connection_set_closecb(struct evhttp_connection *evcon,
    void (*)(struct evhttp_connection *, void *), void *);





void evhttp_connection_set_base(struct evhttp_connection *evcon,
    struct event_base *base);


void evhttp_connection_get_peer(struct evhttp_connection *evcon,
    char **address, u_short *port);


int evhttp_make_request(struct evhttp_connection *evcon,
    struct evhttp_request *req,
    enum evhttp_cmd_type type, const char *uri);

const char *evhttp_request_uri(struct evhttp_request *req);



const char *evhttp_find_header(const struct evkeyvalq *, const char *);
int evhttp_remove_header(struct evkeyvalq *, const char *);
int evhttp_add_header(struct evkeyvalq *, const char *, const char *);
void evhttp_clear_headers(struct evkeyvalq *);
# 322 "evhttp.h"
char *evhttp_encode_uri(const char *uri);
# 333 "evhttp.h"
char *evhttp_decode_uri(const char *uri);
# 351 "evhttp.h"
void evhttp_parse_query(const char *uri, struct evkeyvalq *headers);
# 365 "evhttp.h"
char *evhttp_htmlescape(const char *html);
# 60 "evrpc.c" 2

# 1 "log.h" 1
# 36 "log.h"
void event_err(int eval, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
void event_warn(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void event_errx(int eval, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
void event_warnx(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void event_msgx(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void _event_debugx(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
# 62 "evrpc.c" 2

struct evrpc_base *
evrpc_init(struct evhttp *http_server)
{
        struct evrpc_base* base = calloc(1, sizeof(struct evrpc_base));
        if (base == ((void *)0))
                return (((void *)0));


        evtag_init();

        do { (&base->registered_rpcs)->tqh_first = ((void *)0); (&base->registered_rpcs)->tqh_last = &(&base->registered_rpcs)->tqh_first; } while (0);
        do { (&base->common.in_hooks)->tqh_first = ((void *)0); (&base->common.in_hooks)->tqh_last = &(&base->common.in_hooks)->tqh_first; } while (0);
        do { (&base->common.out_hooks)->tqh_first = ((void *)0); (&base->common.out_hooks)->tqh_last = &(&base->common.out_hooks)->tqh_first; } while (0);
        base->http_server = http_server;

        return (base);
}

void
evrpc_free(struct evrpc_base *base)
{
        struct evrpc *rpc;
        struct evrpc_hook *hook;

        while ((rpc = ((&base->registered_rpcs)->tqh_first)) != ((void *)0)) {
                ((void) ((evrpc_unregister_rpc(base, rpc->uri)) || (__assert ("evrpc_unregister_rpc(base, rpc->uri)", "evrpc.c", 88, __PRETTY_FUNCTION__), 0)));
        }
        while ((hook = ((&base->common.in_hooks)->tqh_first)) != ((void *)0)) {
                ((void) ((evrpc_remove_hook(base, EVRPC_INPUT, hook)) || (__assert ("evrpc_remove_hook(base, EVRPC_INPUT, hook)", "evrpc.c", 91, __PRETTY_FUNCTION__), 0)));
        }
        while ((hook = ((&base->common.out_hooks)->tqh_first)) != ((void *)0)) {
                ((void) ((evrpc_remove_hook(base, EVRPC_OUTPUT, hook)) || (__assert ("evrpc_remove_hook(base, EVRPC_OUTPUT, hook)", "evrpc.c", 94, __PRETTY_FUNCTION__), 0)));
        }
        free(base);
}

void *
evrpc_add_hook(void *vbase,
    enum EVRPC_HOOK_TYPE hook_type,
    int (*cb)(struct evhttp_request *, struct evbuffer *, void *),
    void *cb_arg)
{
        struct _evrpc_hooks *base = vbase;
        struct evrpc_hook_list *head = ((void *)0);
        struct evrpc_hook *hook = ((void *)0);
        switch (hook_type) {
        case EVRPC_INPUT:
                head = &base->in_hooks;
                break;
        case EVRPC_OUTPUT:
                head = &base->out_hooks;
                break;
        default:
                ((void) ((hook_type == EVRPC_INPUT || hook_type == EVRPC_OUTPUT) || (__assert ("hook_type == EVRPC_INPUT || hook_type == EVRPC_OUTPUT", "evrpc.c", 116, __PRETTY_FUNCTION__), 0)));
        }

        hook = calloc(1, sizeof(struct evrpc_hook));
        ((void) ((hook != ((void *)0)) || (__assert ("hook != ((void *)0)", "evrpc.c", 120, __PRETTY_FUNCTION__), 0)));

        hook->process = cb;
        hook->process_arg = cb_arg;
        do { (hook)->next.tqe_next = ((void *)0); (hook)->next.tqe_prev = (head)->tqh_last; *(head)->tqh_last = (hook); (head)->tqh_last = &(hook)->next.tqe_next; } while (0);

        return (hook);
}

static int
evrpc_remove_hook_internal(struct evrpc_hook_list *head, void *handle)
{
        struct evrpc_hook *hook = ((void *)0);
        for((hook) = ((head)->tqh_first); (hook) != ((void *)0); (hook) = ((hook)->next.tqe_next)) {
                if (hook == handle) {
                        do { if (((hook)->next.tqe_next) != ((void *)0)) (hook)->next.tqe_next->next.tqe_prev = (hook)->next.tqe_prev; else (head)->tqh_last = (hook)->next.tqe_prev; *(hook)->next.tqe_prev = (hook)->next.tqe_next; } while (0);
                        free(hook);
                        return (1);
                }
        }

        return (0);
}





int
evrpc_remove_hook(void *vbase, enum EVRPC_HOOK_TYPE hook_type, void *handle)
{
        struct _evrpc_hooks *base = vbase;
        struct evrpc_hook_list *head = ((void *)0);
        switch (hook_type) {
        case EVRPC_INPUT:
                head = &base->in_hooks;
                break;
        case EVRPC_OUTPUT:
                head = &base->out_hooks;
                break;
        default:
                ((void) ((hook_type == EVRPC_INPUT || hook_type == EVRPC_OUTPUT) || (__assert ("hook_type == EVRPC_INPUT || hook_type == EVRPC_OUTPUT", "evrpc.c", 161, __PRETTY_FUNCTION__), 0)));
        }

        return (evrpc_remove_hook_internal(head, handle));
}

static int
evrpc_process_hooks(struct evrpc_hook_list *head,
    struct evhttp_request *req, struct evbuffer *evbuf)
{
        struct evrpc_hook *hook;
        for((hook) = ((head)->tqh_first); (hook) != ((void *)0); (hook) = ((hook)->next.tqe_next)) {
                if (hook->process(req, evbuf, hook->process_arg) == -1)
                        return (-1);
        }

        return (0);
}

static void evrpc_pool_schedule(struct evrpc_pool *pool);
static void evrpc_request_cb(struct evhttp_request *, void *);
void evrpc_request_done(struct evrpc_req_generic*);







static char *
evrpc_construct_uri(const char *uri)
{
        char *constructed_uri;
        int constructed_uri_len;

        constructed_uri_len = strlen("/.rpc.") + strlen(uri) + 1;
        if ((constructed_uri = malloc(constructed_uri_len)) == ((void *)0))
                event_err(1, "%s: failed to register rpc at %s",
                    __func__, uri);
        memcpy(constructed_uri, "/.rpc.", strlen("/.rpc."));
        memcpy(constructed_uri + strlen("/.rpc."), uri, strlen(uri));
        constructed_uri[constructed_uri_len - 1] = '\0';

        return (constructed_uri);
}

int
evrpc_register_rpc(struct evrpc_base *base, struct evrpc *rpc,
    void (*cb)(struct evrpc_req_generic *, void *), void *cb_arg)
{
        char *constructed_uri = evrpc_construct_uri(rpc->uri);

        rpc->base = base;
        rpc->cb = cb;
        rpc->cb_arg = cb_arg;

        do { (rpc)->next.tqe_next = ((void *)0); (rpc)->next.tqe_prev = (&base->registered_rpcs)->tqh_last; *(&base->registered_rpcs)->tqh_last = (rpc); (&base->registered_rpcs)->tqh_last = &(rpc)->next.tqe_next; } while (0);

        evhttp_set_cb(base->http_server,
            constructed_uri,
            evrpc_request_cb,
            rpc);

        free(constructed_uri);

        return (0);
}

int
evrpc_unregister_rpc(struct evrpc_base *base, const char *name)
{
        char *registered_uri = ((void *)0);
        struct evrpc *rpc;


        for((rpc) = ((&base->registered_rpcs)->tqh_first); (rpc) != ((void *)0); (rpc) = ((rpc)->next.tqe_next)) {
                if (strcmp(rpc->uri, name) == 0)
                        break;
        }
        if (rpc == ((void *)0)) {

                return (-1);
        }
        do { if (((rpc)->next.tqe_next) != ((void *)0)) (rpc)->next.tqe_next->next.tqe_prev = (rpc)->next.tqe_prev; else (&base->registered_rpcs)->tqh_last = (rpc)->next.tqe_prev; *(rpc)->next.tqe_prev = (rpc)->next.tqe_next; } while (0);

        free((char *)rpc->uri);
        free(rpc);

        registered_uri = evrpc_construct_uri(name);


        ((void) ((evhttp_del_cb(base->http_server, registered_uri) == 0) || (__assert ("evhttp_del_cb(base->http_server, registered_uri) == 0", "evrpc.c", 252, __PRETTY_FUNCTION__), 0)));

        free(registered_uri);
        return (0);
}

static void
evrpc_request_cb(struct evhttp_request *req, void *arg)
{
        struct evrpc *rpc = arg;
        struct evrpc_req_generic *rpc_state = ((void *)0);


        if (req->type != EVHTTP_REQ_POST ||
            (req->input_buffer)->off <= 0)
                goto error;






        if (evrpc_process_hooks(&rpc->base->common.in_hooks,
                req, req->input_buffer) == -1)
                goto error;

        rpc_state = calloc(1, sizeof(struct evrpc_req_generic));
        if (rpc_state == ((void *)0))
                goto error;


        rpc_state->request = rpc->request_new();
        if (rpc_state->request == ((void *)0))
                goto error;

        rpc_state->rpc = rpc;

        if (rpc->request_unmarshal(
                    rpc_state->request, req->input_buffer) == -1) {

                goto error;
        }



        rpc_state->reply = rpc->reply_new();
        if (rpc_state->reply == ((void *)0))
                goto error;

        rpc_state->http_req = req;
        rpc_state->done = evrpc_request_done;


        rpc->cb(rpc_state, rpc->cb_arg);

        return;

error:
        evrpc_reqstate_free(rpc_state);
        evhttp_send_error(req, 503, "Service Error");
        return;
}

void
evrpc_reqstate_free(struct evrpc_req_generic* rpc_state)
{

        if (rpc_state != ((void *)0)) {
                struct evrpc *rpc = rpc_state->rpc;

                if (rpc_state->request != ((void *)0))
                        rpc->request_free(rpc_state->request);
                if (rpc_state->reply != ((void *)0))
                        rpc->reply_free(rpc_state->reply);
                free(rpc_state);
        }
}

void
evrpc_request_done(struct evrpc_req_generic* rpc_state)
{
        struct evhttp_request *req = rpc_state->http_req;
        struct evrpc *rpc = rpc_state->rpc;
        struct evbuffer* data = ((void *)0);

        if (rpc->reply_complete(rpc_state->reply) == -1) {

                goto error;
        }

        if ((data = evbuffer_new()) == ((void *)0)) {

                goto error;
        }


        rpc->reply_marshal(data, rpc_state->reply);


        if (evrpc_process_hooks(&rpc->base->common.out_hooks,
                req, data) == -1)
                goto error;


        if (evhttp_find_header(req->output_headers, "Content-Type") == ((void *)0)) {
                evhttp_add_header(req->output_headers,
                    "Content-Type", "application/octet-stream");
        }

        evhttp_send_reply(req, 200, "OK", data);

        evbuffer_free(data);

        evrpc_reqstate_free(rpc_state);

        return;

error:
        if (data != ((void *)0))
                evbuffer_free(data);
        evrpc_reqstate_free(rpc_state);
        evhttp_send_error(req, 503, "Service Error");
        return;
}



static int evrpc_schedule_request(struct evhttp_connection *connection,
    struct evrpc_request_wrapper *ctx);

struct evrpc_pool *
evrpc_pool_new(struct event_base *base)
{
        struct evrpc_pool *pool = calloc(1, sizeof(struct evrpc_pool));
        if (pool == ((void *)0))
                return (((void *)0));

        do { (&pool->connections)->tqh_first = ((void *)0); (&pool->connections)->tqh_last = &(&pool->connections)->tqh_first; } while (0);
        do { (&pool->requests)->tqh_first = ((void *)0); (&pool->requests)->tqh_last = &(&pool->requests)->tqh_first; } while (0);

        do { (&pool->common.in_hooks)->tqh_first = ((void *)0); (&pool->common.in_hooks)->tqh_last = &(&pool->common.in_hooks)->tqh_first; } while (0);
        do { (&pool->common.out_hooks)->tqh_first = ((void *)0); (&pool->common.out_hooks)->tqh_last = &(&pool->common.out_hooks)->tqh_first; } while (0);

        pool->base = base;
        pool->timeout = -1;

        return (pool);
}

static void
evrpc_request_wrapper_free(struct evrpc_request_wrapper *request)
{
        free(request->name);
        free(request);
}

void
evrpc_pool_free(struct evrpc_pool *pool)
{
        struct evhttp_connection *connection;
        struct evrpc_request_wrapper *request;
        struct evrpc_hook *hook;

        while ((request = ((&pool->requests)->tqh_first)) != ((void *)0)) {
                do { if (((request)->next.tqe_next) != ((void *)0)) (request)->next.tqe_next->next.tqe_prev = (request)->next.tqe_prev; else (&pool->requests)->tqh_last = (request)->next.tqe_prev; *(request)->next.tqe_prev = (request)->next.tqe_next; } while (0);

                evrpc_request_wrapper_free(request);
        }

        while ((connection = ((&pool->connections)->tqh_first)) != ((void *)0)) {
                do { if (((connection)->next.tqe_next) != ((void *)0)) (connection)->next.tqe_next->next.tqe_prev = (connection)->next.tqe_prev; else (&pool->connections)->tqh_last = (connection)->next.tqe_prev; *(connection)->next.tqe_prev = (connection)->next.tqe_next; } while (0);
                evhttp_connection_free(connection);
        }

        while ((hook = ((&pool->common.in_hooks)->tqh_first)) != ((void *)0)) {
                ((void) ((evrpc_remove_hook(pool, EVRPC_INPUT, hook)) || (__assert ("evrpc_remove_hook(pool, EVRPC_INPUT, hook)", "evrpc.c", 427, __PRETTY_FUNCTION__), 0)));
        }

        while ((hook = ((&pool->common.out_hooks)->tqh_first)) != ((void *)0)) {
                ((void) ((evrpc_remove_hook(pool, EVRPC_OUTPUT, hook)) || (__assert ("evrpc_remove_hook(pool, EVRPC_OUTPUT, hook)", "evrpc.c", 431, __PRETTY_FUNCTION__), 0)));
        }

        free(pool);
}






void
evrpc_pool_add_connection(struct evrpc_pool *pool,
    struct evhttp_connection *connection) {
        ((void) ((connection->http_server == ((void *)0)) || (__assert ("connection->http_server == ((void *)0)", "evrpc.c", 445, __PRETTY_FUNCTION__), 0)));
        do { (connection)->next.tqe_next = ((void *)0); (connection)->next.tqe_prev = (&pool->connections)->tqh_last; *(&pool->connections)->tqh_last = (connection); (&pool->connections)->tqh_last = &(connection)->next.tqe_next; } while (0);




        if (pool->base != ((void *)0))
                evhttp_connection_set_base(connection, pool->base);





        if (connection->timeout == -1)
                connection->timeout = pool->timeout;






        if (((&pool->requests)->tqh_first) != ((void *)0)) {
                struct evrpc_request_wrapper *request =
                    ((&pool->requests)->tqh_first);
                do { if (((request)->next.tqe_next) != ((void *)0)) (request)->next.tqe_next->next.tqe_prev = (request)->next.tqe_prev; else (&pool->requests)->tqh_last = (request)->next.tqe_prev; *(request)->next.tqe_prev = (request)->next.tqe_next; } while (0);
                evrpc_schedule_request(connection, request);
        }
}

void
evrpc_pool_set_timeout(struct evrpc_pool *pool, int timeout_in_secs)
{
        struct evhttp_connection *evcon;
        for((evcon) = ((&pool->connections)->tqh_first); (evcon) != ((void *)0); (evcon) = ((evcon)->next.tqe_next)) {
                evcon->timeout = timeout_in_secs;
        }
        pool->timeout = timeout_in_secs;
}


static void evrpc_reply_done(struct evhttp_request *, void *);
static void evrpc_request_timeout(int, short, void *);





static struct evhttp_connection *
evrpc_pool_find_connection(struct evrpc_pool *pool)
{
        struct evhttp_connection *connection;
        for((connection) = ((&pool->connections)->tqh_first); (connection) != ((void *)0); (connection) = ((connection)->next.tqe_next)) {
                if (((&connection->requests)->tqh_first) == ((void *)0))
                        return (connection);
        }

        return (((void *)0));
}




static int
evrpc_schedule_request(struct evhttp_connection *connection,
    struct evrpc_request_wrapper *ctx)
{
        struct evhttp_request *req = ((void *)0);
        struct evrpc_pool *pool = ctx->pool;
        struct evrpc_status status;
        char *uri = ((void *)0);
        int res = 0;

        if ((req = evhttp_request_new(evrpc_reply_done, ctx)) == ((void *)0))
                goto error;


        ctx->request_marshal(req->output_buffer, ctx->request);

        uri = evrpc_construct_uri(ctx->name);
        if (uri == ((void *)0))
                goto error;


        ctx->evcon = connection;


        if (evrpc_process_hooks(&pool->common.out_hooks,
                req, req->output_buffer) == -1)
                goto error;

        if (pool->timeout > 0) {



                struct timeval tv;
                ((&tv)->tv_sec = (&tv)->tv_usec = 0);
                tv.tv_sec = pool->timeout;
                event_add(&ctx->ev_timeout, &tv);
        }


        res = evhttp_make_request(connection, req, EVHTTP_REQ_POST, uri);
        free(uri);

        if (res == -1)
                goto error;

        return (0);

error:
        memset(&status, 0, sizeof(status));
        status.error = 3;
        (*ctx->cb)(&status, ctx->request, ctx->reply, ctx->cb_arg);
        evrpc_request_wrapper_free(ctx);
        return (-1);
}

int
evrpc_make_request(struct evrpc_request_wrapper *ctx)
{
        struct evrpc_pool *pool = ctx->pool;


        event_set(&ctx->ev_timeout, -1, 0, evrpc_request_timeout, ctx);
        if (pool->base != ((void *)0))
                event_base_set(pool->base, &ctx->ev_timeout);


        ((void) ((((&pool->connections)->tqh_first) != ((void *)0)) || (__assert ("((&pool->connections)->tqh_first) != ((void *)0)", "evrpc.c", 573, __PRETTY_FUNCTION__), 0)));





        do { (ctx)->next.tqe_next = ((void *)0); (ctx)->next.tqe_prev = (&pool->requests)->tqh_last; *(&pool->requests)->tqh_last = (ctx); (&pool->requests)->tqh_last = &(ctx)->next.tqe_next; } while (0);

        evrpc_pool_schedule(pool);

        return (0);
}

static void
evrpc_reply_done(struct evhttp_request *req, void *arg)
{
        struct evrpc_request_wrapper *ctx = arg;
        struct evrpc_pool *pool = ctx->pool;
        struct evrpc_status status;
        int res = -1;


        event_del(&ctx->ev_timeout);

        memset(&status, 0, sizeof(status));
        status.http_req = req;


        if (req != ((void *)0)) {

                if (evrpc_process_hooks(&pool->common.in_hooks,
                        req, req->input_buffer) == -1) {
                        status.error = 4;
                        res = -1;
                } else {
                        res = ctx->reply_unmarshal(ctx->reply,
                            req->input_buffer);
                        if (res == -1) {
                                status.error = 2;
                        }
                }
        } else {
                status.error = 1;
        }

        if (res == -1) {

                ctx->reply_clear(ctx->reply);
        }

        (*ctx->cb)(&status, ctx->request, ctx->reply, ctx->cb_arg);

        evrpc_request_wrapper_free(ctx);




        evrpc_pool_schedule(pool);
}

static void
evrpc_pool_schedule(struct evrpc_pool *pool)
{
        struct evrpc_request_wrapper *ctx = ((&pool->requests)->tqh_first);
        struct evhttp_connection *evcon;


        if (ctx == ((void *)0))
                return;

        if ((evcon = evrpc_pool_find_connection(pool)) != ((void *)0)) {
                do { if (((ctx)->next.tqe_next) != ((void *)0)) (ctx)->next.tqe_next->next.tqe_prev = (ctx)->next.tqe_prev; else (&pool->requests)->tqh_last = (ctx)->next.tqe_prev; *(ctx)->next.tqe_prev = (ctx)->next.tqe_next; } while (0);
                evrpc_schedule_request(evcon, ctx);
        }
}

static void
evrpc_request_timeout(int fd, short what, void *arg)
{
        struct evrpc_request_wrapper *ctx = arg;
        struct evhttp_connection *evcon = ctx->evcon;
        ((void) ((evcon != ((void *)0)) || (__assert ("evcon != ((void *)0)", "evrpc.c", 654, __PRETTY_FUNCTION__), 0)));

        evhttp_connection_fail(evcon, EVCON_HTTP_TIMEOUT);
}
