#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

// Limit of each jmp table is 0 - 89 (90 * 8 = 720 bytes) 
//		- Doesn't have to be 0 - 89 could be from 100 - 189
//		- because they can normalize it!!
//		- but should be seq, why?
// How many jmp tables are allowed in a bpf program?

int a = 0;
int b = 0;

SEC("tp/syscalls/sys_enter_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) 
{
	int ret_user = 0;

	switch (a) {
	    case 100: ret_user = 18; break;
	    case 101: ret_user = 6; break;
	    case 102: ret_user = 9; break;
	    case 103: ret_user = 16; break;
	    case 104: ret_user = 14; break;
	    case 105: ret_user = 2; break;
	    case 106: ret_user = 11; break;
	    case 107: ret_user = 7; break;
	    default: ret_user = 1; break;
	}

	bpf_printk("bpf_prog_tigger_syscall_prog: ret_user %d\n", ret_user);

    return 0;
}

char LISENSE[] SEC("license") = "Dual BSD/GPL";


