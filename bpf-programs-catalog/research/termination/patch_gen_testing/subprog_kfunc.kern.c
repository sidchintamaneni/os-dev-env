#include "vmlinux.h"
#include <bpf/bpf_helpers.h>

struct task_struct *bpf_task_acquire(struct task_struct *p) __ksym;
void bpf_task_release(struct task_struct *p) __ksym;
int bpf_res_spin_lock(struct bpf_res_spin_lock *lock) __ksym;
void bpf_res_spin_unlock(struct bpf_res_spin_lock *lock) __ksym;

struct bpf_res_spin_lock lockA __hidden SEC(".data.A");

static __noinline
int func1() {
	unsigned int numa_id = bpf_get_numa_node_id();
	struct task_struct *current, *acquired;
	int r;

	current = bpf_get_current_task_btf(); // HELPER
	acquired = bpf_task_acquire(current); // KFUNC: KF_RET_NULL

	if (acquired)
		bpf_task_release(acquired); // KFUNC

	r = bpf_res_spin_lock(&lockA); // KFUNC: KF_RET_NULL
	if(!r)
		bpf_res_spin_unlock(&lockA); // KFUNC

	bpf_printk("bpf_prog_trigger_syscall_prog: numa id %d\n", numa_id);
	
	return 0;

}

SEC("fentry/__sys_socket")
//SEC("tp_btf/task_newtask")
//SEC("tp/syscalls/sys_enter_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) {

	unsigned int numa_id = bpf_get_numa_node_id();
	bpf_printk("bpf_prog_trigger_syscall_prog: numa id %d\n", numa_id);

	func1();

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

