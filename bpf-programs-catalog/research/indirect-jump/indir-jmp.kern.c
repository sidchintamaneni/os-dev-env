#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>


SEC("tp/syscalls/sys_enter_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) 
{

	__label__ l1, l2;
	void * tgt;
   
	int ret = 0;
   
	if (ctx)
		tgt = &&l1; // synthetic jump table generated here
	else
		tgt = &&l2; // another synthetic jump table
   
	goto *tgt;

	l1: ret += 1;
	
	l2: ret += 2;

    return 0;
}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

