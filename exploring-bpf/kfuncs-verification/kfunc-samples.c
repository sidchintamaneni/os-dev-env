#include <linux/module.h>
#include <linux/btf.h>
#include <linux/btf_ids.h>
#include <linux/filter.h>


__bpf_kfunc_start_defs();

__bpf_kfunc int sample_kfunc(void)
{
    return 0xdeaddead;
}

__bpf_kfunc_end_defs();


BTF_KFUNCS_START(sample_kfunc_ids)
BTF_ID_FLAGS(func, sample_kfunc)
BTF_KFUNCS_END(sample_kfunc_ids)


static const struct btf_kfunc_id_set sample_kfunc_set = {
	.owner = THIS_MODULE,
	.set   = &sample_kfunc_ids,
};

static int __init sample_kfunc_init(void)
{
    int err;
    
    pr_info("sample_kfunc_entry: Module is loaded\n");
    err = register_btf_kfunc_id_set(BPF_PROG_TYPE_TRACING, &sample_kfunc_set);
    if (err) {
        pr_info("Failed!!\n");
        return 0;
    }
    pr_info("kfuncs are registed and can be called from helper function\n");
    return 0;
}

static void __exit sample_kfunc_exit(void)
{
    // Do nothing
    pr_info("sample_kfunc_exit: Module is unloaded\n");
}

MODULE_AUTHOR("Siddharth Chintamaneni");
MODULE_LICENSE("GPL");

module_init(sample_kfunc_init);
module_exit(sample_kfunc_exit);
