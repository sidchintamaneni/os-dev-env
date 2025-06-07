#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

struct {
	__uint(type, BPF_MAP_TYPE_PROG_ARRAY);
	__uint(max_entries, 10);
	__uint(key_size, sizeof(__u32));
	__array(values, int (void *));
} prog_array_init SEC(".maps");

SEC("fentry/__sys_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) {

    bpf_printk("trigger_syscall_prog: BPF program attached to __sys_socket\n");

	bpf_tail_call(ctx, &prog_array_init, 0);

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

