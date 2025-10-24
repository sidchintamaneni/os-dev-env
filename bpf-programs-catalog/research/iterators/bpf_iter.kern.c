#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

#define INT_MAX 2147483647

SEC("tp/syscalls/sys_enter_socket")
int trigger_syscall_prog(void *ctx) {
	
	struct bpf_iter_num it;
	int *v;

	bpf_iter_num_new(&it, 0, INT_MAX);
	for (v = bpf_iter_num_next(&it); v; v = bpf_iter_num_next(&it)) {
		bpf_printk("iters: val %d\n", *v);
	}
	bpf_iter_num_destroy(&it);

    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";
