#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

SEC("fentry/__sys_socket")
int basic_prog(void *ctx) {
	
    bpf_printk("trigger_bpf_prog: \n");
    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

