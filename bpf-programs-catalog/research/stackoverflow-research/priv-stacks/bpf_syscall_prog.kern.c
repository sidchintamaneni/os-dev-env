#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>


SEC("fentry/__sys_socket")
int trigger_syscall_prog(void *ctx) {

    unsigned long pid = bpf_get_current_pid_tgid() >> 32; 
    bpf_printk("trigger_bpf_prog: Printing the PID of the process %ld\n", pid);

    return 0;
}

char LISENSE[] SEC("license") = "Dual BSD/GPL";
