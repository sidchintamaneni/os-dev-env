#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>


SEC("tp/syscalls/sys_enter_socket")
int trigger_syscall_prog(void *ctx) {
	
	int i;
	bpf_for(i, 5, 10) {
		bpf_printk("iters: val %d\n", i);
	}

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

