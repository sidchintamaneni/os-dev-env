#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

extern int sample_kfunc(void) __ksym;

SEC("fentry/__sys_socket")
int trigger_syscall_prog(void *ctx) {

    bpf_printk("trigger_syscall_prog:\n");
    int ret = sample_kfunc();
    bpf_printk("Return from kfunc: %x\n", ret);

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

