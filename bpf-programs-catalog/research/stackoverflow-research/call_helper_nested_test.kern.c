#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

// a new syscall created for testing nesting solution
SEC("fentry/__sys_socket")
int trigger_syscall_prog(void *ctx) {
    bpf_printk("trigger_syscall_prog: At the start\n");
    int val;

    val = bpf_nested_test();

    bpf_printk("trigger_syscall_prog: the value" 
            " returned from the helper (bpf_nested_test): %d\n", val);

    bpf_printk("trigger_syscall_prog: the end\n");
    return 0;
}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

