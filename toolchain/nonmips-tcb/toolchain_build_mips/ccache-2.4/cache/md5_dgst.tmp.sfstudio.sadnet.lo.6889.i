# 1 "md5_dgst.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "md5_dgst.c"
# 59 "md5_dgst.c"
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 1 3 4
# 28 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/features.h" 1 3 4
# 303 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/features.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/cdefs.h" 1 3 4
# 304 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/features.h" 2 3 4
# 338 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/features.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/uClibc_config.h" 1 3 4
# 339 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/features.h" 2 3 4
# 29 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 2 3 4





# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 1 3 4
# 213 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 3 4
typedef unsigned int size_t;
# 35 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 2 3 4

# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/types.h" 1 3 4
# 29 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/types.h" 3 4
# 1 "/opt/Wive/toolchain/lib/gcc-lib/mips-linux-uclibc/3.3.3/include/stddef.h" 1 3 4
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
# 37 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 2 3 4
# 45 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
typedef struct _UC_FILE FILE;
# 55 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
typedef struct _UC_FILE __FILE;
# 65 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 3 4
# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 1 3 4
# 28 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/types.h" 3 4






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

# 66 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/stdio.h" 2 3 4

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

# 60 "md5_dgst.c" 2
# 1 "md5_locl.h" 1
# 59 "md5_locl.h"
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






# 60 "md5_locl.h" 2
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



# 61 "md5_locl.h" 2
# 1 "../../include/openssl/e_os2.h" 1
# 56 "../../include/openssl/e_os2.h"
# 1 "../../include/openssl/opensslconf.h" 1
# 57 "../../include/openssl/e_os2.h" 2
# 62 "md5_locl.h" 2
# 1 "../../include/openssl/md5.h" 1
# 62 "../../include/openssl/md5.h"
# 1 "../../include/openssl/e_os2.h" 1
# 56 "../../include/openssl/e_os2.h"
# 1 "../../include/openssl/opensslconf.h" 1
# 57 "../../include/openssl/e_os2.h" 2
# 63 "../../include/openssl/md5.h" 2
# 99 "../../include/openssl/md5.h"
typedef struct MD5state_st
        {
        unsigned int A,B,C,D;
        unsigned int Nl,Nh;
        unsigned int data[(64/4)];
        unsigned int num;
        } MD5_CTX;

int MD5_Init(MD5_CTX *c);
int MD5_Update(MD5_CTX *c, const void *data, size_t len);
int MD5_Final(unsigned char *md, MD5_CTX *c);
unsigned char *MD5(const unsigned char *d, size_t n, unsigned char *md);
void MD5_Transform(MD5_CTX *c, const unsigned char *b);
# 63 "md5_locl.h" 2
# 79 "md5_locl.h"
void md5_block_host_order (MD5_CTX *c, const void *p,size_t num);
void md5_block_data_order (MD5_CTX *c, const void *p,size_t num);
# 142 "md5_locl.h"
# 1 "../md32_common.h" 1
# 399 "../md32_common.h"
int MD5_Update (MD5_CTX *c, const void *data_, size_t len)
        {
        const unsigned char *data=data_;
        register unsigned int * p;
        register unsigned int l;
        size_t sw,sc,ew,ec;

        if (len==0) return 1;

        l=(c->Nl+(((unsigned int)len)<<3))&0xffffffffUL;


        if (l < c->Nl)
                c->Nh++;
        c->Nh+=(len>>29);
        c->Nl=l;

        if (c->num != 0)
                {
                p=c->data;
                sw=c->num>>2;
                sc=c->num&0x03;

                if ((c->num+len) >= 64)
                        {
                        l=p[sw]; { switch (sc) { case 0: l =((unsigned long)(*((data)++))); case 1: l|=((unsigned long)(*((data)++)))<< 8; case 2: l|=((unsigned long)(*((data)++)))<<16; case 3: l|=((unsigned long)(*((data)++)))<<24; } }; p[sw++]=l;
                        for (; sw<(64/4); sw++)
                                {
                                (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); p[sw]=l;
                                }
                        md5_block_host_order (c,p,1);
                        len-=(64 -c->num);
                        c->num=0;

                        }
                else
                        {
                        c->num+=(unsigned int)len;
                        if ((sc+len) < 4)
                                {
                                l=p[sw]; { switch (sc) { case 0: l =((unsigned long)(*((data)++))); if (--len == 0) break; case 1: l|=((unsigned long)(*((data)++)))<< 8; if (--len == 0) break; case 2: l|=((unsigned long)(*((data)++)))<<16; } }; p[sw]=l;
                                }
                        else
                                {
                                ew=(c->num>>2);
                                ec=(c->num&0x03);
                                if (sc)
                                        l=p[sw];
                                { switch (sc) { case 0: l =((unsigned long)(*((data)++))); case 1: l|=((unsigned long)(*((data)++)))<< 8; case 2: l|=((unsigned long)(*((data)++)))<<16; case 3: l|=((unsigned long)(*((data)++)))<<24; } };
                                p[sw++]=l;
                                for (; sw < ew; sw++)
                                        {
                                        (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); p[sw]=l;
                                        }
                                if (ec)
                                        {
                                        { l=0; (data)+=ec; switch (ec) { case 3: l =((unsigned long)(*(--(data))))<<16; case 2: l|=((unsigned long)(*(--(data))))<< 8; case 1: l|=((unsigned long)(*(--(data)))); } }; p[sw]=l;
                                        }
                                }
                        return 1;
                        }
                }

        sw=len/64;
        if (sw > 0)
                {
# 490 "../md32_common.h"
                        {
                        md5_block_data_order(c,data,sw);
                        sw*=64;
                        data+=sw;
                        len-=sw;
                        }

                }

        if (len!=0)
                {
                p = c->data;
                c->num = len;
                ew=len>>2;
                ec=len&0x03;
                for (; ew; ew--,p++)
                        {
                        (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); *p=l;
                        }
                { l=0; (data)+=ec; switch (ec) { case 3: l =((unsigned long)(*(--(data))))<<16; case 2: l|=((unsigned long)(*(--(data))))<< 8; case 1: l|=((unsigned long)(*(--(data)))); } };
                *p=l;
                }
        return 1;
        }


void MD5_Transform (MD5_CTX *c, const unsigned char *data)
        {
# 531 "../md32_common.h"
        md5_block_data_order (c,data,1);

        }


int MD5_Final (unsigned char *md, MD5_CTX *c)
        {
        register unsigned int *p;
        register unsigned long l;
        register int i,j;
        static const unsigned char end[4]={0x80,0x00,0x00,0x00};
        const unsigned char *cp=end;


        p=c->data;
        i=c->num>>2;
        j=c->num&0x03;
# 564 "../md32_common.h"
        l = (j==0) ? 0 : p[i];

        { switch (j) { case 0: l =((unsigned long)(*((cp)++))); case 1: l|=((unsigned long)(*((cp)++)))<< 8; case 2: l|=((unsigned long)(*((cp)++)))<<16; case 3: l|=((unsigned long)(*((cp)++)))<<24; } }; p[i++]=l;

        if (i>((64/4)-2))
                {
                if (i<(64/4)) p[i]=0;
                md5_block_host_order (c,p,1);
                i=0;
                }
        for (; i<((64/4)-2); i++)
                p[i]=0;





        p[(64/4)-2]=c->Nl;
        p[(64/4)-1]=c->Nh;

        md5_block_host_order (c,p,1);




        do { unsigned long ll; ll=(c)->A; (*(((md))++)=(unsigned char)(((ll) )&0xff), *(((md))++)=(unsigned char)(((ll)>> 8)&0xff), *(((md))++)=(unsigned char)(((ll)>>16)&0xff), *(((md))++)=(unsigned char)(((ll)>>24)&0xff), ll); ll=(c)->B; (*(((md))++)=(unsigned char)(((ll) )&0xff), *(((md))++)=(unsigned char)(((ll)>> 8)&0xff), *(((md))++)=(unsigned char)(((ll)>>16)&0xff), *(((md))++)=(unsigned char)(((ll)>>24)&0xff), ll); ll=(c)->C; (*(((md))++)=(unsigned char)(((ll) )&0xff), *(((md))++)=(unsigned char)(((ll)>> 8)&0xff), *(((md))++)=(unsigned char)(((ll)>>16)&0xff), *(((md))++)=(unsigned char)(((ll)>>24)&0xff), ll); ll=(c)->D; (*(((md))++)=(unsigned char)(((ll) )&0xff), *(((md))++)=(unsigned char)(((ll)>> 8)&0xff), *(((md))++)=(unsigned char)(((ll)>>16)&0xff), *(((md))++)=(unsigned char)(((ll)>>24)&0xff), ll); } while (0);


        c->num=0;




        return 1;
        }
# 143 "md5_locl.h" 2
# 61 "md5_dgst.c" 2
# 1 "../../include/openssl/opensslv.h" 1
# 62 "md5_dgst.c" 2

const char *MD5_version="MD5" " part of " "OpenSSL 0.9.8b 04 May 2006";
# 73 "md5_dgst.c"
int MD5_Init(MD5_CTX *c)
        {
        c->A=(unsigned long)0x67452301L;
        c->B=(unsigned long)0xefcdab89L;
        c->C=(unsigned long)0x98badcfeL;
        c->D=(unsigned long)0x10325476L;
        c->Nl=0;
        c->Nh=0;
        c->num=0;
        return 1;
        }


void md5_block_host_order (MD5_CTX *c, const void *data, size_t num)
        {
        const unsigned int *X=data;
        register unsigned long A,B,C,D;

        A=c->A;
        B=c->B;
        C=c->C;
        D=c->D;

        for (;num--;X+=(64/4))
                {

        { A+=((X[ 0])+(0xd76aa478L)+(((((C)) ^ ((D))) & ((B))) ^ ((D)))); A=(((A)<<(7))|(((A)&0xffffffff)>>(32-(7)))); A+=B; };;
        { D+=((X[ 1])+(0xe8c7b756L)+(((((B)) ^ ((C))) & ((A))) ^ ((C)))); D=(((D)<<(12))|(((D)&0xffffffff)>>(32-(12)))); D+=A; };;
        { C+=((X[ 2])+(0x242070dbL)+(((((A)) ^ ((B))) & ((D))) ^ ((B)))); C=(((C)<<(17))|(((C)&0xffffffff)>>(32-(17)))); C+=D; };;
        { B+=((X[ 3])+(0xc1bdceeeL)+(((((D)) ^ ((A))) & ((C))) ^ ((A)))); B=(((B)<<(22))|(((B)&0xffffffff)>>(32-(22)))); B+=C; };;
        { A+=((X[ 4])+(0xf57c0fafL)+(((((C)) ^ ((D))) & ((B))) ^ ((D)))); A=(((A)<<(7))|(((A)&0xffffffff)>>(32-(7)))); A+=B; };;
        { D+=((X[ 5])+(0x4787c62aL)+(((((B)) ^ ((C))) & ((A))) ^ ((C)))); D=(((D)<<(12))|(((D)&0xffffffff)>>(32-(12)))); D+=A; };;
        { C+=((X[ 6])+(0xa8304613L)+(((((A)) ^ ((B))) & ((D))) ^ ((B)))); C=(((C)<<(17))|(((C)&0xffffffff)>>(32-(17)))); C+=D; };;
        { B+=((X[ 7])+(0xfd469501L)+(((((D)) ^ ((A))) & ((C))) ^ ((A)))); B=(((B)<<(22))|(((B)&0xffffffff)>>(32-(22)))); B+=C; };;
        { A+=((X[ 8])+(0x698098d8L)+(((((C)) ^ ((D))) & ((B))) ^ ((D)))); A=(((A)<<(7))|(((A)&0xffffffff)>>(32-(7)))); A+=B; };;
        { D+=((X[ 9])+(0x8b44f7afL)+(((((B)) ^ ((C))) & ((A))) ^ ((C)))); D=(((D)<<(12))|(((D)&0xffffffff)>>(32-(12)))); D+=A; };;
        { C+=((X[10])+(0xffff5bb1L)+(((((A)) ^ ((B))) & ((D))) ^ ((B)))); C=(((C)<<(17))|(((C)&0xffffffff)>>(32-(17)))); C+=D; };;
        { B+=((X[11])+(0x895cd7beL)+(((((D)) ^ ((A))) & ((C))) ^ ((A)))); B=(((B)<<(22))|(((B)&0xffffffff)>>(32-(22)))); B+=C; };;
        { A+=((X[12])+(0x6b901122L)+(((((C)) ^ ((D))) & ((B))) ^ ((D)))); A=(((A)<<(7))|(((A)&0xffffffff)>>(32-(7)))); A+=B; };;
        { D+=((X[13])+(0xfd987193L)+(((((B)) ^ ((C))) & ((A))) ^ ((C)))); D=(((D)<<(12))|(((D)&0xffffffff)>>(32-(12)))); D+=A; };;
        { C+=((X[14])+(0xa679438eL)+(((((A)) ^ ((B))) & ((D))) ^ ((B)))); C=(((C)<<(17))|(((C)&0xffffffff)>>(32-(17)))); C+=D; };;
        { B+=((X[15])+(0x49b40821L)+(((((D)) ^ ((A))) & ((C))) ^ ((A)))); B=(((B)<<(22))|(((B)&0xffffffff)>>(32-(22)))); B+=C; };;

        { A+=((X[ 1])+(0xf61e2562L)+(((((B)) ^ ((C))) & ((D))) ^ ((C)))); A=(((A)<<(5))|(((A)&0xffffffff)>>(32-(5)))); A+=B; };;
        { D+=((X[ 6])+(0xc040b340L)+(((((A)) ^ ((B))) & ((C))) ^ ((B)))); D=(((D)<<(9))|(((D)&0xffffffff)>>(32-(9)))); D+=A; };;
        { C+=((X[11])+(0x265e5a51L)+(((((D)) ^ ((A))) & ((B))) ^ ((A)))); C=(((C)<<(14))|(((C)&0xffffffff)>>(32-(14)))); C+=D; };;
        { B+=((X[ 0])+(0xe9b6c7aaL)+(((((C)) ^ ((D))) & ((A))) ^ ((D)))); B=(((B)<<(20))|(((B)&0xffffffff)>>(32-(20)))); B+=C; };;
        { A+=((X[ 5])+(0xd62f105dL)+(((((B)) ^ ((C))) & ((D))) ^ ((C)))); A=(((A)<<(5))|(((A)&0xffffffff)>>(32-(5)))); A+=B; };;
        { D+=((X[10])+(0x02441453L)+(((((A)) ^ ((B))) & ((C))) ^ ((B)))); D=(((D)<<(9))|(((D)&0xffffffff)>>(32-(9)))); D+=A; };;
        { C+=((X[15])+(0xd8a1e681L)+(((((D)) ^ ((A))) & ((B))) ^ ((A)))); C=(((C)<<(14))|(((C)&0xffffffff)>>(32-(14)))); C+=D; };;
        { B+=((X[ 4])+(0xe7d3fbc8L)+(((((C)) ^ ((D))) & ((A))) ^ ((D)))); B=(((B)<<(20))|(((B)&0xffffffff)>>(32-(20)))); B+=C; };;
        { A+=((X[ 9])+(0x21e1cde6L)+(((((B)) ^ ((C))) & ((D))) ^ ((C)))); A=(((A)<<(5))|(((A)&0xffffffff)>>(32-(5)))); A+=B; };;
        { D+=((X[14])+(0xc33707d6L)+(((((A)) ^ ((B))) & ((C))) ^ ((B)))); D=(((D)<<(9))|(((D)&0xffffffff)>>(32-(9)))); D+=A; };;
        { C+=((X[ 3])+(0xf4d50d87L)+(((((D)) ^ ((A))) & ((B))) ^ ((A)))); C=(((C)<<(14))|(((C)&0xffffffff)>>(32-(14)))); C+=D; };;
        { B+=((X[ 8])+(0x455a14edL)+(((((C)) ^ ((D))) & ((A))) ^ ((D)))); B=(((B)<<(20))|(((B)&0xffffffff)>>(32-(20)))); B+=C; };;
        { A+=((X[13])+(0xa9e3e905L)+(((((B)) ^ ((C))) & ((D))) ^ ((C)))); A=(((A)<<(5))|(((A)&0xffffffff)>>(32-(5)))); A+=B; };;
        { D+=((X[ 2])+(0xfcefa3f8L)+(((((A)) ^ ((B))) & ((C))) ^ ((B)))); D=(((D)<<(9))|(((D)&0xffffffff)>>(32-(9)))); D+=A; };;
        { C+=((X[ 7])+(0x676f02d9L)+(((((D)) ^ ((A))) & ((B))) ^ ((A)))); C=(((C)<<(14))|(((C)&0xffffffff)>>(32-(14)))); C+=D; };;
        { B+=((X[12])+(0x8d2a4c8aL)+(((((C)) ^ ((D))) & ((A))) ^ ((D)))); B=(((B)<<(20))|(((B)&0xffffffff)>>(32-(20)))); B+=C; };;

        { A+=((X[ 5])+(0xfffa3942L)+(((B)) ^ ((C)) ^ ((D)))); A=(((A)<<(4))|(((A)&0xffffffff)>>(32-(4)))); A+=B; };;
        { D+=((X[ 8])+(0x8771f681L)+(((A)) ^ ((B)) ^ ((C)))); D=(((D)<<(11))|(((D)&0xffffffff)>>(32-(11)))); D+=A; };;
        { C+=((X[11])+(0x6d9d6122L)+(((D)) ^ ((A)) ^ ((B)))); C=(((C)<<(16))|(((C)&0xffffffff)>>(32-(16)))); C+=D; };;
        { B+=((X[14])+(0xfde5380cL)+(((C)) ^ ((D)) ^ ((A)))); B=(((B)<<(23))|(((B)&0xffffffff)>>(32-(23)))); B+=C; };;
        { A+=((X[ 1])+(0xa4beea44L)+(((B)) ^ ((C)) ^ ((D)))); A=(((A)<<(4))|(((A)&0xffffffff)>>(32-(4)))); A+=B; };;
        { D+=((X[ 4])+(0x4bdecfa9L)+(((A)) ^ ((B)) ^ ((C)))); D=(((D)<<(11))|(((D)&0xffffffff)>>(32-(11)))); D+=A; };;
        { C+=((X[ 7])+(0xf6bb4b60L)+(((D)) ^ ((A)) ^ ((B)))); C=(((C)<<(16))|(((C)&0xffffffff)>>(32-(16)))); C+=D; };;
        { B+=((X[10])+(0xbebfbc70L)+(((C)) ^ ((D)) ^ ((A)))); B=(((B)<<(23))|(((B)&0xffffffff)>>(32-(23)))); B+=C; };;
        { A+=((X[13])+(0x289b7ec6L)+(((B)) ^ ((C)) ^ ((D)))); A=(((A)<<(4))|(((A)&0xffffffff)>>(32-(4)))); A+=B; };;
        { D+=((X[ 0])+(0xeaa127faL)+(((A)) ^ ((B)) ^ ((C)))); D=(((D)<<(11))|(((D)&0xffffffff)>>(32-(11)))); D+=A; };;
        { C+=((X[ 3])+(0xd4ef3085L)+(((D)) ^ ((A)) ^ ((B)))); C=(((C)<<(16))|(((C)&0xffffffff)>>(32-(16)))); C+=D; };;
        { B+=((X[ 6])+(0x04881d05L)+(((C)) ^ ((D)) ^ ((A)))); B=(((B)<<(23))|(((B)&0xffffffff)>>(32-(23)))); B+=C; };;
        { A+=((X[ 9])+(0xd9d4d039L)+(((B)) ^ ((C)) ^ ((D)))); A=(((A)<<(4))|(((A)&0xffffffff)>>(32-(4)))); A+=B; };;
        { D+=((X[12])+(0xe6db99e5L)+(((A)) ^ ((B)) ^ ((C)))); D=(((D)<<(11))|(((D)&0xffffffff)>>(32-(11)))); D+=A; };;
        { C+=((X[15])+(0x1fa27cf8L)+(((D)) ^ ((A)) ^ ((B)))); C=(((C)<<(16))|(((C)&0xffffffff)>>(32-(16)))); C+=D; };;
        { B+=((X[ 2])+(0xc4ac5665L)+(((C)) ^ ((D)) ^ ((A)))); B=(((B)<<(23))|(((B)&0xffffffff)>>(32-(23)))); B+=C; };;

        { A+=((X[ 0])+(0xf4292244L)+(((~((D))) | ((B))) ^ ((C)))); A=(((A)<<(6))|(((A)&0xffffffff)>>(32-(6)))); A+=B; };;
        { D+=((X[ 7])+(0x432aff97L)+(((~((C))) | ((A))) ^ ((B)))); D=(((D)<<(10))|(((D)&0xffffffff)>>(32-(10)))); D+=A; };;
        { C+=((X[14])+(0xab9423a7L)+(((~((B))) | ((D))) ^ ((A)))); C=(((C)<<(15))|(((C)&0xffffffff)>>(32-(15)))); C+=D; };;
        { B+=((X[ 5])+(0xfc93a039L)+(((~((A))) | ((C))) ^ ((D)))); B=(((B)<<(21))|(((B)&0xffffffff)>>(32-(21)))); B+=C; };;
        { A+=((X[12])+(0x655b59c3L)+(((~((D))) | ((B))) ^ ((C)))); A=(((A)<<(6))|(((A)&0xffffffff)>>(32-(6)))); A+=B; };;
        { D+=((X[ 3])+(0x8f0ccc92L)+(((~((C))) | ((A))) ^ ((B)))); D=(((D)<<(10))|(((D)&0xffffffff)>>(32-(10)))); D+=A; };;
        { C+=((X[10])+(0xffeff47dL)+(((~((B))) | ((D))) ^ ((A)))); C=(((C)<<(15))|(((C)&0xffffffff)>>(32-(15)))); C+=D; };;
        { B+=((X[ 1])+(0x85845dd1L)+(((~((A))) | ((C))) ^ ((D)))); B=(((B)<<(21))|(((B)&0xffffffff)>>(32-(21)))); B+=C; };;
        { A+=((X[ 8])+(0x6fa87e4fL)+(((~((D))) | ((B))) ^ ((C)))); A=(((A)<<(6))|(((A)&0xffffffff)>>(32-(6)))); A+=B; };;
        { D+=((X[15])+(0xfe2ce6e0L)+(((~((C))) | ((A))) ^ ((B)))); D=(((D)<<(10))|(((D)&0xffffffff)>>(32-(10)))); D+=A; };;
        { C+=((X[ 6])+(0xa3014314L)+(((~((B))) | ((D))) ^ ((A)))); C=(((C)<<(15))|(((C)&0xffffffff)>>(32-(15)))); C+=D; };;
        { B+=((X[13])+(0x4e0811a1L)+(((~((A))) | ((C))) ^ ((D)))); B=(((B)<<(21))|(((B)&0xffffffff)>>(32-(21)))); B+=C; };;
        { A+=((X[ 4])+(0xf7537e82L)+(((~((D))) | ((B))) ^ ((C)))); A=(((A)<<(6))|(((A)&0xffffffff)>>(32-(6)))); A+=B; };;
        { D+=((X[11])+(0xbd3af235L)+(((~((C))) | ((A))) ^ ((B)))); D=(((D)<<(10))|(((D)&0xffffffff)>>(32-(10)))); D+=A; };;
        { C+=((X[ 2])+(0x2ad7d2bbL)+(((~((B))) | ((D))) ^ ((A)))); C=(((C)<<(15))|(((C)&0xffffffff)>>(32-(15)))); C+=D; };;
        { B+=((X[ 9])+(0xeb86d391L)+(((~((A))) | ((C))) ^ ((D)))); B=(((B)<<(21))|(((B)&0xffffffff)>>(32-(21)))); B+=C; };;

        A = c->A += A;
        B = c->B += B;
        C = c->C += C;
        D = c->D += D;
                }
        }






void md5_block_data_order (MD5_CTX *c, const void *data_, size_t num)
        {
        const unsigned char *data=data_;
        register unsigned long A,B,C,D,l;


        unsigned long XX0, XX1, XX2, XX3, XX4, XX5, XX6, XX7,
                                XX8, XX9,XX10,XX11,XX12,XX13,XX14,XX15;






        A=c->A;
        B=c->B;
        C=c->C;
        D=c->D;

        for (;num--;)
                {
        (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX0=l; (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX1=l;

        { A+=((XX0)+(0xd76aa478L)+(((((C)) ^ ((D))) & ((B))) ^ ((D)))); A=(((A)<<(7))|(((A)&0xffffffff)>>(32-(7)))); A+=B; };; (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX2=l;
        { D+=((XX1)+(0xe8c7b756L)+(((((B)) ^ ((C))) & ((A))) ^ ((C)))); D=(((D)<<(12))|(((D)&0xffffffff)>>(32-(12)))); D+=A; };; (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX3=l;
        { C+=((XX2)+(0x242070dbL)+(((((A)) ^ ((B))) & ((D))) ^ ((B)))); C=(((C)<<(17))|(((C)&0xffffffff)>>(32-(17)))); C+=D; };; (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX4=l;
        { B+=((XX3)+(0xc1bdceeeL)+(((((D)) ^ ((A))) & ((C))) ^ ((A)))); B=(((B)<<(22))|(((B)&0xffffffff)>>(32-(22)))); B+=C; };; (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX5=l;
        { A+=((XX4)+(0xf57c0fafL)+(((((C)) ^ ((D))) & ((B))) ^ ((D)))); A=(((A)<<(7))|(((A)&0xffffffff)>>(32-(7)))); A+=B; };; (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX6=l;
        { D+=((XX5)+(0x4787c62aL)+(((((B)) ^ ((C))) & ((A))) ^ ((C)))); D=(((D)<<(12))|(((D)&0xffffffff)>>(32-(12)))); D+=A; };; (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX7=l;
        { C+=((XX6)+(0xa8304613L)+(((((A)) ^ ((B))) & ((D))) ^ ((B)))); C=(((C)<<(17))|(((C)&0xffffffff)>>(32-(17)))); C+=D; };; (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX8=l;
        { B+=((XX7)+(0xfd469501L)+(((((D)) ^ ((A))) & ((C))) ^ ((A)))); B=(((B)<<(22))|(((B)&0xffffffff)>>(32-(22)))); B+=C; };; (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX9=l;
        { A+=((XX8)+(0x698098d8L)+(((((C)) ^ ((D))) & ((B))) ^ ((D)))); A=(((A)<<(7))|(((A)&0xffffffff)>>(32-(7)))); A+=B; };; (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX10=l;
        { D+=((XX9)+(0x8b44f7afL)+(((((B)) ^ ((C))) & ((A))) ^ ((C)))); D=(((D)<<(12))|(((D)&0xffffffff)>>(32-(12)))); D+=A; };; (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX11=l;
        { C+=((XX10)+(0xffff5bb1L)+(((((A)) ^ ((B))) & ((D))) ^ ((B)))); C=(((C)<<(17))|(((C)&0xffffffff)>>(32-(17)))); C+=D; };; (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX12=l;
        { B+=((XX11)+(0x895cd7beL)+(((((D)) ^ ((A))) & ((C))) ^ ((A)))); B=(((B)<<(22))|(((B)&0xffffffff)>>(32-(22)))); B+=C; };; (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX13=l;
        { A+=((XX12)+(0x6b901122L)+(((((C)) ^ ((D))) & ((B))) ^ ((D)))); A=(((A)<<(7))|(((A)&0xffffffff)>>(32-(7)))); A+=B; };; (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX14=l;
        { D+=((XX13)+(0xfd987193L)+(((((B)) ^ ((C))) & ((A))) ^ ((C)))); D=(((D)<<(12))|(((D)&0xffffffff)>>(32-(12)))); D+=A; };; (l =(((unsigned long)(*((data)++))) ), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<<24), l); XX15=l;
        { C+=((XX14)+(0xa679438eL)+(((((A)) ^ ((B))) & ((D))) ^ ((B)))); C=(((C)<<(17))|(((C)&0xffffffff)>>(32-(17)))); C+=D; };;
        { B+=((XX15)+(0x49b40821L)+(((((D)) ^ ((A))) & ((C))) ^ ((A)))); B=(((B)<<(22))|(((B)&0xffffffff)>>(32-(22)))); B+=C; };;

        { A+=((XX1)+(0xf61e2562L)+(((((B)) ^ ((C))) & ((D))) ^ ((C)))); A=(((A)<<(5))|(((A)&0xffffffff)>>(32-(5)))); A+=B; };;
        { D+=((XX6)+(0xc040b340L)+(((((A)) ^ ((B))) & ((C))) ^ ((B)))); D=(((D)<<(9))|(((D)&0xffffffff)>>(32-(9)))); D+=A; };;
        { C+=((XX11)+(0x265e5a51L)+(((((D)) ^ ((A))) & ((B))) ^ ((A)))); C=(((C)<<(14))|(((C)&0xffffffff)>>(32-(14)))); C+=D; };;
        { B+=((XX0)+(0xe9b6c7aaL)+(((((C)) ^ ((D))) & ((A))) ^ ((D)))); B=(((B)<<(20))|(((B)&0xffffffff)>>(32-(20)))); B+=C; };;
        { A+=((XX5)+(0xd62f105dL)+(((((B)) ^ ((C))) & ((D))) ^ ((C)))); A=(((A)<<(5))|(((A)&0xffffffff)>>(32-(5)))); A+=B; };;
        { D+=((XX10)+(0x02441453L)+(((((A)) ^ ((B))) & ((C))) ^ ((B)))); D=(((D)<<(9))|(((D)&0xffffffff)>>(32-(9)))); D+=A; };;
        { C+=((XX15)+(0xd8a1e681L)+(((((D)) ^ ((A))) & ((B))) ^ ((A)))); C=(((C)<<(14))|(((C)&0xffffffff)>>(32-(14)))); C+=D; };;
        { B+=((XX4)+(0xe7d3fbc8L)+(((((C)) ^ ((D))) & ((A))) ^ ((D)))); B=(((B)<<(20))|(((B)&0xffffffff)>>(32-(20)))); B+=C; };;
        { A+=((XX9)+(0x21e1cde6L)+(((((B)) ^ ((C))) & ((D))) ^ ((C)))); A=(((A)<<(5))|(((A)&0xffffffff)>>(32-(5)))); A+=B; };;
        { D+=((XX14)+(0xc33707d6L)+(((((A)) ^ ((B))) & ((C))) ^ ((B)))); D=(((D)<<(9))|(((D)&0xffffffff)>>(32-(9)))); D+=A; };;
        { C+=((XX3)+(0xf4d50d87L)+(((((D)) ^ ((A))) & ((B))) ^ ((A)))); C=(((C)<<(14))|(((C)&0xffffffff)>>(32-(14)))); C+=D; };;
        { B+=((XX8)+(0x455a14edL)+(((((C)) ^ ((D))) & ((A))) ^ ((D)))); B=(((B)<<(20))|(((B)&0xffffffff)>>(32-(20)))); B+=C; };;
        { A+=((XX13)+(0xa9e3e905L)+(((((B)) ^ ((C))) & ((D))) ^ ((C)))); A=(((A)<<(5))|(((A)&0xffffffff)>>(32-(5)))); A+=B; };;
        { D+=((XX2)+(0xfcefa3f8L)+(((((A)) ^ ((B))) & ((C))) ^ ((B)))); D=(((D)<<(9))|(((D)&0xffffffff)>>(32-(9)))); D+=A; };;
        { C+=((XX7)+(0x676f02d9L)+(((((D)) ^ ((A))) & ((B))) ^ ((A)))); C=(((C)<<(14))|(((C)&0xffffffff)>>(32-(14)))); C+=D; };;
        { B+=((XX12)+(0x8d2a4c8aL)+(((((C)) ^ ((D))) & ((A))) ^ ((D)))); B=(((B)<<(20))|(((B)&0xffffffff)>>(32-(20)))); B+=C; };;

        { A+=((XX5)+(0xfffa3942L)+(((B)) ^ ((C)) ^ ((D)))); A=(((A)<<(4))|(((A)&0xffffffff)>>(32-(4)))); A+=B; };;
        { D+=((XX8)+(0x8771f681L)+(((A)) ^ ((B)) ^ ((C)))); D=(((D)<<(11))|(((D)&0xffffffff)>>(32-(11)))); D+=A; };;
        { C+=((XX11)+(0x6d9d6122L)+(((D)) ^ ((A)) ^ ((B)))); C=(((C)<<(16))|(((C)&0xffffffff)>>(32-(16)))); C+=D; };;
        { B+=((XX14)+(0xfde5380cL)+(((C)) ^ ((D)) ^ ((A)))); B=(((B)<<(23))|(((B)&0xffffffff)>>(32-(23)))); B+=C; };;
        { A+=((XX1)+(0xa4beea44L)+(((B)) ^ ((C)) ^ ((D)))); A=(((A)<<(4))|(((A)&0xffffffff)>>(32-(4)))); A+=B; };;
        { D+=((XX4)+(0x4bdecfa9L)+(((A)) ^ ((B)) ^ ((C)))); D=(((D)<<(11))|(((D)&0xffffffff)>>(32-(11)))); D+=A; };;
        { C+=((XX7)+(0xf6bb4b60L)+(((D)) ^ ((A)) ^ ((B)))); C=(((C)<<(16))|(((C)&0xffffffff)>>(32-(16)))); C+=D; };;
        { B+=((XX10)+(0xbebfbc70L)+(((C)) ^ ((D)) ^ ((A)))); B=(((B)<<(23))|(((B)&0xffffffff)>>(32-(23)))); B+=C; };;
        { A+=((XX13)+(0x289b7ec6L)+(((B)) ^ ((C)) ^ ((D)))); A=(((A)<<(4))|(((A)&0xffffffff)>>(32-(4)))); A+=B; };;
        { D+=((XX0)+(0xeaa127faL)+(((A)) ^ ((B)) ^ ((C)))); D=(((D)<<(11))|(((D)&0xffffffff)>>(32-(11)))); D+=A; };;
        { C+=((XX3)+(0xd4ef3085L)+(((D)) ^ ((A)) ^ ((B)))); C=(((C)<<(16))|(((C)&0xffffffff)>>(32-(16)))); C+=D; };;
        { B+=((XX6)+(0x04881d05L)+(((C)) ^ ((D)) ^ ((A)))); B=(((B)<<(23))|(((B)&0xffffffff)>>(32-(23)))); B+=C; };;
        { A+=((XX9)+(0xd9d4d039L)+(((B)) ^ ((C)) ^ ((D)))); A=(((A)<<(4))|(((A)&0xffffffff)>>(32-(4)))); A+=B; };;
        { D+=((XX12)+(0xe6db99e5L)+(((A)) ^ ((B)) ^ ((C)))); D=(((D)<<(11))|(((D)&0xffffffff)>>(32-(11)))); D+=A; };;
        { C+=((XX15)+(0x1fa27cf8L)+(((D)) ^ ((A)) ^ ((B)))); C=(((C)<<(16))|(((C)&0xffffffff)>>(32-(16)))); C+=D; };;
        { B+=((XX2)+(0xc4ac5665L)+(((C)) ^ ((D)) ^ ((A)))); B=(((B)<<(23))|(((B)&0xffffffff)>>(32-(23)))); B+=C; };;

        { A+=((XX0)+(0xf4292244L)+(((~((D))) | ((B))) ^ ((C)))); A=(((A)<<(6))|(((A)&0xffffffff)>>(32-(6)))); A+=B; };;
        { D+=((XX7)+(0x432aff97L)+(((~((C))) | ((A))) ^ ((B)))); D=(((D)<<(10))|(((D)&0xffffffff)>>(32-(10)))); D+=A; };;
        { C+=((XX14)+(0xab9423a7L)+(((~((B))) | ((D))) ^ ((A)))); C=(((C)<<(15))|(((C)&0xffffffff)>>(32-(15)))); C+=D; };;
        { B+=((XX5)+(0xfc93a039L)+(((~((A))) | ((C))) ^ ((D)))); B=(((B)<<(21))|(((B)&0xffffffff)>>(32-(21)))); B+=C; };;
        { A+=((XX12)+(0x655b59c3L)+(((~((D))) | ((B))) ^ ((C)))); A=(((A)<<(6))|(((A)&0xffffffff)>>(32-(6)))); A+=B; };;
        { D+=((XX3)+(0x8f0ccc92L)+(((~((C))) | ((A))) ^ ((B)))); D=(((D)<<(10))|(((D)&0xffffffff)>>(32-(10)))); D+=A; };;
        { C+=((XX10)+(0xffeff47dL)+(((~((B))) | ((D))) ^ ((A)))); C=(((C)<<(15))|(((C)&0xffffffff)>>(32-(15)))); C+=D; };;
        { B+=((XX1)+(0x85845dd1L)+(((~((A))) | ((C))) ^ ((D)))); B=(((B)<<(21))|(((B)&0xffffffff)>>(32-(21)))); B+=C; };;
        { A+=((XX8)+(0x6fa87e4fL)+(((~((D))) | ((B))) ^ ((C)))); A=(((A)<<(6))|(((A)&0xffffffff)>>(32-(6)))); A+=B; };;
        { D+=((XX15)+(0xfe2ce6e0L)+(((~((C))) | ((A))) ^ ((B)))); D=(((D)<<(10))|(((D)&0xffffffff)>>(32-(10)))); D+=A; };;
        { C+=((XX6)+(0xa3014314L)+(((~((B))) | ((D))) ^ ((A)))); C=(((C)<<(15))|(((C)&0xffffffff)>>(32-(15)))); C+=D; };;
        { B+=((XX13)+(0x4e0811a1L)+(((~((A))) | ((C))) ^ ((D)))); B=(((B)<<(21))|(((B)&0xffffffff)>>(32-(21)))); B+=C; };;
        { A+=((XX4)+(0xf7537e82L)+(((~((D))) | ((B))) ^ ((C)))); A=(((A)<<(6))|(((A)&0xffffffff)>>(32-(6)))); A+=B; };;
        { D+=((XX11)+(0xbd3af235L)+(((~((C))) | ((A))) ^ ((B)))); D=(((D)<<(10))|(((D)&0xffffffff)>>(32-(10)))); D+=A; };;
        { C+=((XX2)+(0x2ad7d2bbL)+(((~((B))) | ((D))) ^ ((A)))); C=(((C)<<(15))|(((C)&0xffffffff)>>(32-(15)))); C+=D; };;
        { B+=((XX9)+(0xeb86d391L)+(((~((A))) | ((C))) ^ ((D)))); B=(((B)<<(21))|(((B)&0xffffffff)>>(32-(21)))); B+=C; };;

        A = c->A += A;
        B = c->B += B;
        C = c->C += C;
        D = c->D += D;
                }
        }
