#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

static int x(){
	bpf_printk("Yay!");
	//x();
	return 0;
}

SEC("fentry/__sys_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) {
	bpf_printk("Entry..\n");
	x();
    return 0;
}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

