#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_helpers.h>

unsigned long cond = 0;


SEC("kprobe.multi")
int trigger_bpf_prog(struct pt_regs *ctx) {

	long x;
	if (cond == 1) {
		x = 2;
		bpf_printk("nested BPF program: Updating the value of x: %ld", x);
		return 0;
	}
	cond = 1;
	x = 1;
    unsigned long pid = bpf_get_current_pid_tgid() >> 32; 
    bpf_printk("trigger_bpf_prog: Printing the PID of the process %ld\n", pid);
	bpf_printk("trigger_bpf_prog: Checking the value of x: %ld, should be %ld", x, 1);
	
	cond = 0;


    return 0;
}

char LISENSE[] SEC("license") = "Dual BSD/GPL";
