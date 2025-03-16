// SPDX-License-Identifier: GPL-2.0-or-later
#include <linux/types.h>
// Not sure about these two
#include <linux/jump_label.h>
#include <asm/cpufeature.h>

#include <asm-generic/qspinlock_types.h>

// LOCK_PREFIX in queued_fetch_set_pending_acquire
#include <asm/rmwcc.h>


#define _Q_PENDING_LOOPS	(1 << 9)

typedef struct qspinlock sqspinlock_t;

void squeued_spin_lock_slowpath(struct qspinlock *lock, u32 val);

static __always_inline u32 squeued_fetch_set_pending_acquire(struct qspinlock *lock)
{
	u32 val;

	/*
	 * We can't use GEN_BINARY_RMWcc() inside an if() stmt because asm goto
	 * and CONFIG_PROFILE_ALL_BRANCHES=y results in a label inside a
	 * statement expression, which GCC doesn't like.
	 */
	val = GEN_BINARY_RMWcc(LOCK_PREFIX "btsl", lock->val.counter, c,
			       "I", _Q_PENDING_OFFSET) * _Q_PENDING_VAL;
	val |= atomic_read(&lock->val) & ~_Q_PENDING_MASK;

	return val;
}

static __always_inline int squeued_spin_trylock(struct qspinlock *lock)
{
	// This one looks fishy
	int val = atomic_read(&lock->val);

	if (unlikely(val))
		return 0;

	return likely(atomic_try_cmpxchg_acquire(&lock->val, &val, _Q_LOCKED_VAL));
}

static __always_inline void squeued_spin_lock(struct qspinlock *lock)
{
	int val = 0;
	pr_info("squeued_spin_lock: Called\n");
	
	if (likely(atomic_try_cmpxchg_acquire(&lock->val, &val, _Q_LOCKED_VAL))) {
		pr_info("squeued_spin_lock: lock value - %d\n", lock->val.counter);
		return;
	}

	pr_info("squeued_spin_lock: entered slowpath\n");
	squeued_spin_lock_slowpath(lock, val);
}

static __always_inline void squeued_spin_unlock(struct qspinlock *lock)
{
	smp_store_release(&lock->locked, 0);
}
