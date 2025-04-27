#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

static int x(){
	bpf_printk("Yay!");
	x();
	return 0;
}

SEC("tp/syscalls/sys_enter_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) {

	bpf_printk("Recursion");
	x();
    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

