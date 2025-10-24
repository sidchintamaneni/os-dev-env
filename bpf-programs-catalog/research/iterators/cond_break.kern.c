#include "bpf_experimental.h"


SEC("tp/syscalls/sys_enter_socket")
int trigger_syscall_prog(void *ctx) {
	
	for (;;){
		bpf_printk("iter\n");
	    __cond_break(break);  // may_goto will force break after N iterations
	}    

	return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

