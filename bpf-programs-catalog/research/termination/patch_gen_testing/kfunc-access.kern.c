#include "vmlinux.h"
//#include <linux/bpf.h>
//#include <linux/types.h>
#include <bpf/bpf_helpers.h>

struct task_struct *bpf_task_acquire(struct task_struct *p) __ksym;
void bpf_task_release(struct task_struct *p) __ksym;
int bpf_res_spin_lock(struct bpf_res_spin_lock *lock) __ksym;
void bpf_res_spin_unlock(struct bpf_res_spin_lock *lock) __ksym;

struct bpf_res_spin_lock lockA __hidden SEC(".data.A");

SEC("tp_btf/task_newtask")
int bpf_prog_trigger_syscall_prog(void *ctx) {

	struct task_struct *current, *acquired;
	int r;
	
	current = bpf_get_current_task_btf();
	acquired = bpf_task_acquire(current);

	if (acquired)
		bpf_task_release(acquired);

	r = bpf_res_spin_lock(&lockA);
	if(!r)
		bpf_res_spin_unlock(&lockA);


	return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

