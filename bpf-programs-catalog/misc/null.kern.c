#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

int a = 10;
int b = 20;

SEC("tp/syscalls/sys_enter_socket")
int trigger_syscall_prog(void *ctx) {

	int x = a;
	int y = b;
	int z = x + y;

	bpf_printk("%d\n", z);

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

