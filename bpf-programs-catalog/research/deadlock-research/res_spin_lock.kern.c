#include "vmlinux.h"
#include <bpf/bpf_helpers.h>

int bpf_res_spin_lock(struct bpf_res_spin_lock *lock) __ksym;

struct arr_elem {
	struct bpf_res_spin_lock lock;
};

struct {
	__uint(type, BPF_MAP_TYPE_ARRAY);
	__uint(max_entries, 1);
	__type(key, int);
	__type(value, struct arr_elem);
} arrmap SEC(".maps");

SEC("fentry/bpf_res_spin_unlock")
int trigger_syscall_prog2(void *ctx)
{
	struct arr_elem *elem;

	bpf_printk("trigger_syscall_prog2\n");
	elem = bpf_map_lookup_elem(&arrmap, &(int){0});
	if (!elem)
		return 0;
	int r = bpf_res_spin_lock(&elem->lock);
	if (!r) {
		bpf_res_spin_unlock(&elem->lock);
	}
    
    return 0;
}

SEC("fentry/__sys_socket")
int trigger_syscall_prog(void *ctx)
{
	struct arr_elem *elem;

	bpf_printk("trigger_syscall_prog\n");
	elem = bpf_map_lookup_elem(&arrmap, &(int){0});
	if (!elem)
		return 0;
	int r = bpf_res_spin_lock(&elem->lock);
	if (!r) {
		bpf_res_spin_unlock(&elem->lock);
		bpf_printk("trigger_syscall_prog: unlocking\n");
	}
    
    return 0;
}

char _license[] SEC("license") = "GPL";
