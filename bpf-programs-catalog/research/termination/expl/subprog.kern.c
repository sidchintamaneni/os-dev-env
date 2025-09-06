#include <linux/bpf.h>
#include <linux/types.h>
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_helpers.h>

extern void bpf_throw(long cookie) __ksym;
extern void *bpf_obj_new_impl(__u64 local_type_id, void *meta) __ksym;
extern void bpf_obj_drop_impl(void *kptr, void *meta) __ksym;

#define bpf_obj_new(type) ((type *)bpf_obj_new_impl(bpf_core_type_id_local(type), NULL))
#define bpf_obj_drop(kptr) bpf_obj_drop_impl(kptr, NULL)


__noinline int throwing_global_subprog(struct __sk_buff *ctx)
{
	bpf_printk("throwing_global_subprog: subprog starts here...\n");
	if (ctx->len)
		bpf_throw(0);
	return 0;
}

 
SEC("tc")
int bpf_prog_trigger_syscall_prog(struct __sk_buff *ctx)
{
	bpf_printk("bpf_prog_trigger_syscall_prog: tc starts here...\n");
	struct { long a; } *p = bpf_obj_new(typeof(*p));

	if (!p)
		return 0;
	if (ctx->protocol)
		throwing_global_subprog(ctx);
	bpf_obj_drop(p);
	return 0;
}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

