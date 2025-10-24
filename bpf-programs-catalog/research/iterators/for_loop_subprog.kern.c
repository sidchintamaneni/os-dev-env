#include "bpf_experimental.h"


static __noinline
void func1() {
	for (;can_loop;) {
		bpf_printk("Testing: \n");
	}
}

SEC("tp/syscalls/sys_enter_socket")
int trigger_syscall_prog(void *ctx) {

	for (;can_loop;) {
		bpf_printk("Testing: \n");
	}

	func1();
	func1();

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

