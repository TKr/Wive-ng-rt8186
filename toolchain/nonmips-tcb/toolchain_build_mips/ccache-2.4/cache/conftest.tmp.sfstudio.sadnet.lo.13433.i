# 1 "conftest.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "conftest.c"
# 40 "conftest.c"
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

# 41 "conftest.c" 2




# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/stat.h" 1 3 4
# 96 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/stat.h" 3 4


# 1 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/stat.h" 1 3 4
# 37 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/stat.h" 3 4
struct stat
  {
    unsigned long int st_dev;
    long int st_pad1[3];

    __ino_t st_ino;



    __mode_t st_mode;
    __nlink_t st_nlink;
    __uid_t st_uid;
    __gid_t st_gid;
    unsigned long int st_rdev;

    long int st_pad2[2];
    __off_t st_size;

    long int st_pad3;
# 64 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/bits/stat.h" 3 4
    __time_t st_atime;
    long int __reserved0;
    __time_t st_mtime;
    long int __reserved1;
    __time_t st_ctime;
    long int __reserved2;
    __blksize_t st_blksize;

    __blkcnt_t st_blocks;




    long int st_pad5[14];
  };
# 99 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/stat.h" 2 3 4
# 200 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/stat.h" 3 4
extern int stat (__const char *__restrict __file,
                 struct stat *__restrict __buf) ;



extern int fstat (int __fd, struct stat *__buf) ;
# 228 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/stat.h" 3 4
extern int lstat (__const char *__restrict __file,
                  struct stat *__restrict __buf) ;
# 249 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/stat.h" 3 4
extern int chmod (__const char *__file, __mode_t __mode) ;



extern int fchmod (int __fd, __mode_t __mode) ;





extern __mode_t umask (__mode_t __mask) ;
# 268 "/opt/Wive/toolchain/mips-linux-uclibc/sys-include/sys/stat.h" 3 4
extern int mkdir (__const char *__path, __mode_t __mode) ;





extern int mknod (__const char *__path, __mode_t __mode, __dev_t __dev)
     ;



extern int mkfifo (__const char *__path, __mode_t __mode) ;


# 46 "conftest.c" 2


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


extern int wctomb (