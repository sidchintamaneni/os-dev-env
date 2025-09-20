#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

struct {
    __uint(type, BPF_MAP_TYPE_STACK);
    __uint(max_entries, 32);
    __uint(map_flags, 0);
    __uint(key_size, 0);
    __uint(value_size, sizeof(int));
} map SEC(".maps");

SEC("fentry/__sys_socket")
int raw_spin_unlock_prog(void *ctx)
{
    bpf_printk("raw_spin_unlock_prog: starts here\n");

    int err;
    int val;

    err = bpf_map_pop_elem(&map, &val);
    if (err)
        bpf_printk("raw_spin_unlock_prog: pop operation failed");

    bpf_printk("raw_spin_unlock_prog: ends here\n");

    return 0;
}

SEC("fentry/__sys_socket")
int trigger_syscall_prog(void *ctx)
{
    
    bpf_printk("trigger_syscall_prog: starts here\n");

    int err;
    int val = 10;

    err = bpf_map_push_elem(&map, &val, 0);
    if (err)
        bpf_printk("trigger_syscall_prog: push operation failed");

    bpf_printk("trigger_syscall_prog: ends here\n");
    
    return 0;
}

char _license[] SEC("license") = "GPL";

