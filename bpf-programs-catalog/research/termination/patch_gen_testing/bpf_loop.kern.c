#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

#define LOOPS_CNT 3

static int callback_fn(void *ctx) {

	bpf_printk("Printing inside the loop\n");

	return 0;
}

SEC("tp/syscalls/sys_enter_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) {

	bpf_loop(LOOPS_CNT, callback_fn, NULL, 0);

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

