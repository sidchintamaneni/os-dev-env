#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

int iter = 0;

void *fn;

#define LOOPS_CNT 1 << 10

unsigned long loop_cnt = 0;

static int callback_fn5(void *ctx) {
	
	bpf_printk("Lopping..");
	return 0;
}

static int callback_fn4(void *ctx) {
	bpf_printk("Lopping..");
	return 0;
}

static int callback_fn3(void *ctx) {

	if (iter == 0)
		fn = &callback_fn4;
	else
		fn = &callback_fn5;

	bpf_loop(LOOPS_CNT, fn, NULL, 0);

	return 0;
}


static int callback_fn2(void *ctx) {

	if (iter == 0)
		fn = &callback_fn3;
	else
		fn = &callback_fn4;

	bpf_loop(LOOPS_CNT, fn, NULL, 0);

	return 0;
}

static int callback_fn(void *ctx) {

	if (iter == 0)
		fn = &callback_fn2;
	else
		fn = &callback_fn3;

	bpf_loop(LOOPS_CNT, fn, NULL, 0);

	return 0;
}

SEC("fentry/__sys_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) {
	
	if (iter == 0)
		fn = &callback_fn;
	else
		fn = &callback_fn2;

	bpf_loop(LOOPS_CNT, fn, NULL, 0);

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

