#include <errno.h>
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

struct dummy {
	int val;
}

struct {
	__uint(type, BPF_MAP_TYPE_PERCPU_CGROUP_STORAGE);
	__type(key, struct bpf_cgroup_storage_key);
	__type(value, struct dummy);
} percpu_map SEC(".maps");

SEC("cgroup/skb")
int bpf_test(struct __sk_buff *skb)
{
	int ret = 0;

	struct percpu_map *map = bpf_get_local_storage(&percpu_map, 0);

	__sync_fetch_and_add(&map->val, 1);
	
	bpf_printk("bpf_test: map->val %d\n", map->val);

	return 0;
}

char _license[] SEC("license") = "GPL";


