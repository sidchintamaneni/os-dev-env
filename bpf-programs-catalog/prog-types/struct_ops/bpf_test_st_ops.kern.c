#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

struct bpf_test_ops_state {
	int val;
};

struct bpf_test_st_ops {
	int (*test_st_1) (struct bpf_test_ops_state *cb);
	int (*test_st_2) (struct bpf_test_ops_state *cb, int arg1, 
						unsigned long arg2);
	int (*test_st_sleepable) (struct bpf_test_ops_state *cb);
};


SEC("struct_ops/test_st_sleepable")
int bpf_test_st_sleepable(struct bpf_test_ops_state *state)
{
	//bpf_printk("bpf_test_st_sleepable: running inside a BPF program\n");
	return 0;
}


SEC("struct_ops/test_st_2")
int bpf_test_st_2(struct bpf_test_ops_state *state, int arg1, unsigned long arg2)
{
	//bpf_printk("bpf_test_st_2: running inside a BPF program\n");
	return 0;
}

SEC("struct_ops/test_st_1")
int bpf_test_st_1(struct bpf_test_ops_state *state)
{
	//bpf_printk("bpf_test_st_1: running inside a BPF program\n");
	return 0;
}

SEC(".struct_ops")
struct bpf_test_st_ops bpf_test_st_ops = {
	.test_st_1 = bpf_test_st_1,
	.test_st_2 = bpf_test_st_2,
	.test_st_sleepable = bpf_test_st_sleepable,
};

char _license[] SEC("license") = "GPL";
