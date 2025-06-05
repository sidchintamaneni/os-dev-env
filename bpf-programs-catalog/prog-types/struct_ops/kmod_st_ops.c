#include <linux/bpf.h>
#include <linux/btf.h>
#include <linux/btf_ids.h>
#include <linux/delay.h>
#include <linux/error-injection.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/percpu-defs.h>
#include <linux/sysfs.h>
#include <linux/tracepoint.h>
#include <linux/net.h>
#include <linux/socket.h>
#include <linux/nsproxy.h>
#include <linux/inet.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/un.h>
#include <linux/filter.h>
#include <net/sock.h>
#include <linux/namei.h>

struct bpf_test_ops_state {
	int val;
};
struct bpf_test_st_ops {
	int (*test_st_1) (struct bpf_test_ops_state *cb);
	int (*test_st_2) (struct bpf_test_ops_state *cb, int arg1, 
						unsigned long arg2);
	int (*test_st_sleepable) (struct bpf_test_ops_state *cb);
};

static int bpf_test_st_sleepable(struct bpf_test_ops_state *cb)
{
	pr_info("bpf_test_st_sleepable: default behaviour\n");
	return 0;
}
static int bpf_test_st_2(struct bpf_test_ops_state *cb, int arg1, unsigned long arg2)
{
	pr_info("bpf_test_st_2: default behaviour\n");
	return 0;
}


static int bpf_test_st_1(struct bpf_test_ops_state *cb__nullable)
{
	pr_info("bpf_test_st_1: default behaviour\n");
	return 0;
}


static struct bpf_test_st_ops _bpf_test_st_ops = {
	.test_st_1 = bpf_test_st_1,
	.test_st_2 = bpf_test_st_2,
	.test_st_sleepable = bpf_test_st_sleepable,
};

static struct bpf_struct_ops bpf_bpf_test_st_ops = {
	.name = "bpf_test_st_ops",
	.cfi_stubs = &_bpf_test_st_ops,
	.owner = THIS_MODULE,
};

static int __init kmod_st_ops_init(void)
{
#ifdef DEBUG_INFO
    pr_info("kmod_st_ops_init: PID %d, starts here\n", current->pid);
#endif

	int ret;

	ret = register_bpf_struct_ops(&bpf_bpf_test_st_ops, bpf_test_st_ops);
	if (ret < 0) {
		pr_info("kmod_st_ops_init: PID %d, register_btf_struct_ops failed\n", current->pid);
		return ret;
	}

	return 0;
}

static void __exit kmod_st_ops_exit(void)
{
#ifdef DEBUG_INFO
    pr_info("kmod_st_ops_exit: PID %d, starts here\n", current->pid);
#endif
}


module_init(kmod_st_ops_init);
module_exit(kmod_st_ops_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Siddharth Chintamaneni");
