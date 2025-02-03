#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

SEC("fentry/__sys_socket")
int trigger_bpf_prog(void *ctx) {

    bpf_printk("trigger_bpf_prog: BPF program attached to __sys_socket\n");
	
    return 0;
}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

