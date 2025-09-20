#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

#define LOOPS_CNT 1 << 23

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

SEC("tracepoint/syscalls/sys_enter_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) {

	bpf_loop(LOOPS_CNT, callback_fn, NULL, 0);

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

