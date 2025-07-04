#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 10);
	__type(key, int);
	__type(value, int);
} hash_map SEC(".maps");


struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 10);
	__type(key, int);
	__type(value, int);
} hash_map2 SEC(".maps");

SEC("tp/syscalls/sys_enter_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) {

	const int key = 0;
	const int val = 1;

	bpf_map_update_elem(&hash_map, &key, &val, BPF_ANY);

	bpf_map_update_elem(&hash_map2, &key, &val, BPF_ANY);

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

