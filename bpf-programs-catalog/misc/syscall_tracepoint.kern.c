
#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

SEC("tp/syscalls/sys_enter_socket")
int trigger_syscall_prog(void *ctx) {

      bpf_printk("Triggering socket syscall:\n");

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

