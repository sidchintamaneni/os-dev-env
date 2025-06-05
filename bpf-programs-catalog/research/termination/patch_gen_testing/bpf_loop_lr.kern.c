#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

#define LOOPS_CNT 1 << 10

static int callback_fn4(void *ctx) {
	
	bpf_printk("Loop is looping!");

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
int bpf_prog_trigger_syscall_prog(void *ctx) {

	bpf_loop(LOOPS_CNT, callback_fn, NULL, 0);

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

