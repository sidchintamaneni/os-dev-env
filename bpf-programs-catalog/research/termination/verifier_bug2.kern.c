#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

#define LOOPS_CNT 2

int iter = 0;

int (*fn)(void *);


static int __attribute__ ((noinline)) callback_fn2(void *ctx) {

	bpf_printk("callback_fn2\n");

	return 0;
}

static int __attribute__ ((noinline)) callback_fn(void *ctx) {

	bpf_printk("callback_fn\n");
	
	return 0;
}

SEC("tp/syscalls/sys_enter_socket")
int trigger_syscall_prog(void *ctx) {

	fn = &callback_fn;
//	callback_fn2(NULL);
//	bpf_loop(2, fn, 0, 0);
	fn(NULL);
	fn(NULL);
	fn(NULL);

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

