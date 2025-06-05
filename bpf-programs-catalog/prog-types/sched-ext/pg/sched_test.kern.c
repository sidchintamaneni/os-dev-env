#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

char _license[] SEC("license") = "GPL";

SEC(".struct_ops.link")
struct sched_ext_ops sched_test = {
	.name = "sched_test",
};
