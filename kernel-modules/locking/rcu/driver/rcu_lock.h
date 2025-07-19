#include <linux/preempt.h>
#include <linux/compiler_types.h>

/*
 * Basic RCU API
 *
 * rcu_read_lock()					-> Begin an RCU critical section
 * rcu_read_unlock()		  		-> Complete an RCU critical section
 * synchronize_rcu()		  		-> Wait for exiting RCU critical sections to complete
 * call_rcu(callback, args..) 		-> Call the callback when exiting RCU critical sections complete
 * rcu_dereference(pointer)	  		-> Signal the intent to dereference a pointer in an RCU critical section	
 * rcu_assign_pointer(ptr_addr, ptr)-> Assign a value to a pointer that is read in RCU critical seciton
 */ 

static inline void __rcu_read_lock(void) {
	/*
	 * In case of non-preemption mode preempt_{enable, disable}
	 * mean just barrier()
	 */
	preempt_disable();
}

static inline void __rcu_read_unlock(void)
{
	preempt_enable();
}

static __always_inline void rcu_read_lock(void)
{

	__rcu_read_lock();

	/*
	 * Some kinda __CHECKER__
	 * looks like use for static analysis
	 */
//	__acquire(RCU);

	/*
	 * Even this is for lockdep analysis
	 */
//	rcu_lock_acquire(&rcu_lock_map);
}

static inline void rcu_read_unlock(void)
{
	/*
	 * Commenting these two for similar reasons as above
	 */
//	rcu_lock_release(&rcu_lock_map);
//	__release(RCU);
	
	__rcu_read_unlock();
}
