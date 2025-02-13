#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

#define ARR_SIZE 480

// a new syscall created for testing nesting solution
SEC("fentry/bpf_get_current_pid_tgid")
int trigger_helper_prog(void *ctx) {
    bpf_printk("trigger_helper_prog: BPF program attached to bpf_get_current_pid_tgid\n");

    char arr[ARR_SIZE];
    for(int i=0; i<ARR_SIZE; i++){
        arr[i] = i%255;
    }

    for(int i=0; i<ARR_SIZE; i++){
          bpf_printk("value of arr[%d]: %d\n", i, arr[i]);
    }

    // Call helper function
    unsigned long pid = bpf_get_current_pid_tgid(); 
    bpf_printk("trigger_helper_prog: printing the PID of the process %ld\n", pid);
    bpf_printk("trigger_helper_prog: the end\n");
    return 0;
}

SEC("fentry/__sys_socket")
int trigger_syscall_prog(void *ctx) {

    bpf_printk("trigger_syscall_prog: BPF program attached to __sys_socket\n");
    // checking the pointer access
    int family = *((int *) ctx);
    int type = *((int *) ctx + 2);
    int protocol = *((int *) ctx + 4);

    bpf_printk("family: %d", family);
    bpf_printk("type: %d", type);
    bpf_printk("protocol: %d", protocol);

    // Call helper function
    unsigned long pid = bpf_get_current_pid_tgid(); 
    bpf_printk("trigger_syscall_prog: printing the PID of the process %ld\n", pid);
    bpf_printk("trigger_syscall_prog: the end\n");

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

