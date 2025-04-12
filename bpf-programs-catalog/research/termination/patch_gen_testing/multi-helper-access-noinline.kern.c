#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>


SEC("tp/syscalls/sys_enter_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) {

	unsigned int pid = bpf_get_current_pid_tgid() >> 32;
	unsigned int numa_id = bpf_get_numa_node_id();

	bpf_printk("bpf_prog_trigger_syscall_prog: pid %d and numa id %d\n", pid, numa_id);

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

