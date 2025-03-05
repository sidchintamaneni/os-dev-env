#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

int bpf_prog(void *ctx)
{

	return 0;
}


char LISENSE[] SEC("license") = "Dual BSD/GPL";
