#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

static __noinline
void func1() {
	unsigned int numa_id = bpf_get_numa_node_id();
	bpf_printk("bpf_prog_trigger_syscall_prog: numa id %d\n", numa_id);
}

SEC("tp/syscalls/sys_enter_socket")
int bpf_prog_trigger_syscall_prog(void *ctx) {

	unsigned int numa_id = bpf_get_numa_node_id();
	bpf_printk("bpf_prog_trigger_syscall_prog: numa id %d\n", numa_id);

	func1();

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

