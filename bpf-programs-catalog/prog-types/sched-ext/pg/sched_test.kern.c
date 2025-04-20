#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

#define BPF_STRUCT_OPS(name, args...)				\
SEC("struct_ops/"#name)								\
BPF_PROG(name, ##args)

char _license[] SEC("license") = "GPL";

void BPF_STRUCT_OPS(enable_test, struct task_struct *p, 
		struct scx_enable_args *args)
{
	bpf_printk("enable_test: Task %s enabled in example scheduler\n", p->comm);
}

SEC(".struct_ops.link")
struct sched_ext_ops sched_test = {
	.enable = (void *) enable_test,
	.name = "sched_test",
};
