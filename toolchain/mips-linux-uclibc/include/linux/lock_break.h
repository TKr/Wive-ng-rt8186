/*
 * include/linux/lock_break.h - lock breaking routines
 *
 * since in-kernel preemption can not occur while a lock is
 * held, we can drop and reacquire long-held locks when they are
 * in a natural quiescent state to further lower system latency.
 *
 * (C) 2001 Robert Love
 *
 */

#ifndef _LINUX_LOCK_BREAK_H
#define _LINUX_LOCK_BREAK_H

#include <linux/compiler.h>

/*
 * setting this to 1 will instruct debug_lock_break to
 * note when the expected lock count does not equal the
 * actual count. if the lock count is higher than expected,
 * we aren't dropping enough locks.  if it is 0, we are
 * wasting our time since the system is already preemptible.
 */
#ifndef DEBUG_LOCK_BREAK
#define DEBUG_LOCK_BREAK 0
#endif

#define unconditional_schedule()
#define conditional_schedule()
#define conditional_schedule_needed() 0
#define break_spin_lock(n)
#define break_spin_lock_and_resched(n)
#define debug_lock_break(n)
#define DEFINE_LOCK_COUNT()
#define TEST_LOCK_COUNT(n) 0
#define RESET_LOCK_COUNT()

#endif /* _LINUX_LOCK_BREAK_H */
