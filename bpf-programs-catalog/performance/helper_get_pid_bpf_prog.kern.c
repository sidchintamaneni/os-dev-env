#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 32);
    __type(key, int);
    __type(value, __u32);
} map SEC(".maps");

SEC("fentry/bpf_get_current_pid_tgid")
int trigger_helper_prog(void *ctx)
{
    __u32 pid = bpf_get_current_pid_tgid() >> 32;
    int init_val = 1;

    __u32 *val = bpf_map_lookup_elem(&map, &pid);
    if (val) {
       *val += 1; 
    } else {
        bpf_map_update_elem(&map, &pid, &init_val, BPF_ANY);
    }

    return 0;
}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

