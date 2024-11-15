#include <linux/types.h>
#include <bpf/bpf_helpers.h> // include SEC macro

SEC("kprobe/__x64_sys_socket")
int trigger_syscall_program(void *ctx) {
	return 0;
}

//char LISENSE[] SEC("license") = "Dual BSD/GPL";
