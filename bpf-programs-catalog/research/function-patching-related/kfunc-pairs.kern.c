#include "vmlinux.h"
#include <linux/version.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>


void bpf_preempt_disable(void) __weak __ksym;
void bpf_preempt_enable(void) __weak __ksym;

//struct file *bpf_get_task_exe_file(struct task_struct *task) __weak __ksym;
//void bpf_put_file(struct file *file) __weak __ksym;

extern struct file *bpf_get_task_exe_file(struct task_struct *task) __ksym;
extern void bpf_put_file(struct file *file) __ksym;

__u32 key_id;

SEC("lsm.s/file_open")
int trigger_syscall_prog(struct file *file)
{

    struct file *acquired;

	acquired = bpf_get_task_exe_file(bpf_get_current_task_btf());
	if (!acquired)
		return 0;

    // kptr = dummy_helper(acquired);

    // release(kptr) 

//	bpf_put_file(acquired); 
    //bpf_preempt_disable();
    
    //bpf_printk("Playing with paired kfuncs\n");

    //bpf_preempt_enable();
    return 0;
}

char LISENSE[] SEC("license") = "Dual BSD/GPL";
