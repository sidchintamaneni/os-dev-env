#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

int iter = 0;

void *fn;

#define LOOPS_CNT 1 << 10

static __noinline
int func1() {
	 return 0;
}

static int callback_fn2(void *ctx) {

	return 0;
}

static int callback_fn(void *ctx) {

	return 0;
}

SEC("fentry/__sys_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) {
	
	if (iter == 0)
		fn = &callback_fn;
	else
		fn = &callback_fn;

	bpf_loop(LOOPS_CNT, fn, NULL, 0);

	func1();

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

