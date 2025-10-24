#include "bpf_experimental.h"
//#include <linux/bpf.h>
//#include <linux/types.h>
//#include <bpf/bpf_helpers.h>


SEC("tp/syscalls/sys_enter_socket")
int trigger_syscall_prog(void *ctx) {

//	for (;can_loop;) {
//		bpf_printk("testing: \n");
//	}
//	for (;can_loop;) {
//		bpf_printk("testing: 2 \n");
//	}
//	for (;can_loop;) {
//		bpf_printk("testing: 3 \n");
//	}
//	for (;can_loop;) {
//		bpf_printk("testing: 4 \n");
//	}
	for (;can_loop;) {
		for (;can_loop;) {
			for (;can_loop;) {
				for (;can_loop;) {
					bpf_printk("Testing: \n");
				}
			}
		}
	}
    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

