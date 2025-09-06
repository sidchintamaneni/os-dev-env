#include "vmlinux.h"
#include <bpf/bpf_helpers.h>

int bpf_res_spin_lock(struct bpf_res_spin_lock *lock) __ksym;
void bpf_res_spin_unlock(struct bpf_res_spin_lock *lock) __ksym;

struct bpf_res_spin_lock lockA __hidden SEC(".data.A");

SEC("fexit/bpf_res_spin_lock")
int bpf_prog_trigger_syscall_prog2(void *ctx) 
{
	
	int r =	bpf_res_spin_lock((struct bpf_res_spin_lock *)&lockA);
	
	if (!r)
		bpf_res_spin_unlock((struct bpf_res_spin_lock *)&lockA); 
	else
		bpf_printk("bpf_prog_trigger_syscall_prog: failed to acquire the lock\n");

	bpf_printk("bpf_prog_trigger_syscall_prog2: ends here\n");

    return 0;

}

SEC("fentry/__sys_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) 
{
	
	int r =	bpf_res_spin_lock((struct bpf_res_spin_lock *)&lockA);
	
	if (!r)
		bpf_res_spin_unlock((struct bpf_res_spin_lock *)&lockA); 
	else
		bpf_printk("bpf_prog_trigger_syscall_prog: failed to acquire the lock\n");

	bpf_printk("bpf_prog_trigger_syscall_prog: ends here\n");

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

