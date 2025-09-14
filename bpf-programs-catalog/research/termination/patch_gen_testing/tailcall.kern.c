#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

#define LOOPS_CNT 1 << 10

static int callback_fn4(void *ctx) {
	
	unsigned int pid = bpf_get_current_pid_tgid() >> 32;

	bpf_printk("callback_fn4: pid %d", pid);

	return 0;
}

static int callback_fn3(void *ctx) {

	bpf_loop(LOOPS_CNT, callback_fn4, NULL, 0);

	return 0;
}


static int callback_fn2(void *ctx) {

	bpf_loop(LOOPS_CNT, callback_fn3, NULL, 0);

	return 0;
}

static int callback_fn(void *ctx) {

	bpf_loop(LOOPS_CNT, callback_fn2, NULL, 0);

	return 0;
}

SEC("fentry/__sys_socket")
int bpf_prog_testing_tail_func(void *ctx) {

	bpf_loop(LOOPS_CNT, callback_fn, NULL, 0);

    return 0;

}

struct {
	__uint(type, BPF_MAP_TYPE_PROG_ARRAY);
	__uint(max_entries, 10);
	__uint(key_size, sizeof(__u32));
	__array(values, int (void *));
} prog_array_init SEC(".maps") = {
	.values = {
		[1] = (void *)&bpf_prog_testing_tail_func,
	},
};

SEC("fentry/__sys_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) {

    bpf_printk("trigger_syscall_prog: BPF program attached to __sys_socket\n");

	bpf_tail_call(ctx, &prog_array_init, 1);

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

