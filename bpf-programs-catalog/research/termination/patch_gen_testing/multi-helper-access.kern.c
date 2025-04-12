#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>


SEC("tp/syscalls/sys_enter_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) {

	unsigned int pid = bpf_get_current_pid_tgid() >> 32;
	unsigned int cpu_id = bpf_get_smp_processor_id();

	bpf_printk("bpf_prog_trigger_syscall_prog: pid %d and cpu id %d\n", pid, cpu_id);

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

