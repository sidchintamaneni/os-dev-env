#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/bpf.h>
#include <asm/io.h>


#define DEV_NAME "kmod_st_ops"
#define MAX_DEV 1

static int dev_major = -1;
static struct class *kmod_st_ops_class = NULL;
static struct cdev cdev;
static struct device *kmod_st_ops;

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

static int kmod_st_ops_open(struct inode *inode, struct file *file)
{
	pr_info("kmod_st_ops_open[%d]: starts here\n", current->pid);

	_bpf_test_st_ops.test_st_1(NULL);
	_bpf_test_st_ops.test_st_2(NULL, 1, 2);
	_bpf_test_st_ops.test_st_sleepable(NULL);

    return 0; 
}

static const struct file_operations kmod_st_ops_fops = {
    .owner = THIS_MODULE,
    .open = kmod_st_ops_open,
};

static int __init kmod_st_ops_init(void)
{
    dev_t dev;
    int err;
#ifdef DEBUG_INFO
    pr_info("kmod_st_ops_init: PID %d, starts here\n", current->pid);
#endif

    err = alloc_chrdev_region(&dev, 0, MAX_DEV, DEV_NAME);
    if (err < 0) {
        goto out;
    }

    dev_major = MAJOR(dev);
    kmod_st_ops_class = class_create(DEV_NAME);
    err = PTR_ERR(kmod_st_ops_class);
    if (IS_ERR(kmod_st_ops_class))
        goto err_kmod_st_ops_class;

    cdev_init(&cdev, &kmod_st_ops_fops);
    cdev.owner = THIS_MODULE;

    err = cdev_add(&cdev, MKDEV(dev_major, 0), 1);
    if (err < 0)
        goto err_kmod_st_ops;
    
    kmod_st_ops = device_create(kmod_st_ops_class, NULL, MKDEV(dev_major, 0), NULL, DEV_NAME);
    err = PTR_ERR(kmod_st_ops);
    if (IS_ERR(kmod_st_ops))
		goto err_kmod_st_ops;

	register_bpf_struct_ops(&bpf_bpf_test_st_ops, bpf_test_st_ops);

    return 0;

err_kmod_st_ops:
    class_destroy(kmod_st_ops_class);
err_kmod_st_ops_class:
    unregister_chrdev(dev_major, DEV_NAME);
out:
    return err;
}

static void __exit kmod_st_ops_exit(void)
{
#ifdef DEBUG_INFO
    pr_info("kmod_st_ops_exit: PID %d, starts here\n", current->pid);
#endif

	device_destroy(kmod_st_ops_class, MKDEV(dev_major, 0));

	class_destroy(kmod_st_ops_class);

	unregister_chrdev(dev_major, DEV_NAME);
	kmod_st_ops = NULL;
}


module_init(kmod_st_ops_init);
module_exit(kmod_st_ops_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Siddharth Chintamaneni");
