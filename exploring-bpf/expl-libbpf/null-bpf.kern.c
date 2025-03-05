#include <linux/types.h>
#include <bpf/bpf_helpers.h>

SEC("tp/syscall/sys_enter_execve")
int trigger_syscall_program(void *ctx) {
	return 0;
}

char LISENSE[] SEC("license") = "Dual BSD/GPL";
