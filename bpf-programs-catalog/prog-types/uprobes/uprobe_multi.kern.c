#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>


SEC("uprobe.multi//proc/self/exe:uprobe_multi_func_*")
int uprobe(struct pt_regs *ctx) 
{
	bpf_printk("uprobe: is it working?\n");
	
    return 0;
}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

