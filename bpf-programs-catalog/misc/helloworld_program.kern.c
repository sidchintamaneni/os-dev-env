#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

SEC("fexit/__sys_socket")
int trigger_syscall_prog(void *ctx) {

//    bpf_printk("trigger_syscall_prog: BPF program attached to __sys_socket\n");
      bpf_printk("trigger_syscall_prog: Hello World\n");
//    bpf_printk("trigger_syscall_prog: the end\n");

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

