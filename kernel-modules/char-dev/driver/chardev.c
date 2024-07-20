#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/delay.h>

#include "chardev.h"

#define MAX_DEV 2
#define DEV_NAME "chardev"

static int dev_major = -1;
static struct class *chardev_class = NULL;
static struct device *chardev;

typedef int chardev_ioctl_t(struct file *file, unsigned int cmd, unsigned long arg);

struct chardev_ioctl_desc {
	unsigned int cmd;
	int flags;
	chardev_ioctl_t *func;
	const char *name;
};

#define CHARDEV_IOCTL_DEF(ioctl, _func, _flags)		\
	[_IOC_NR(ioctl)] = {.cmd = ioctl, 		\
		            .func = _func,		\
		            .flags = _flags,		\
		            .name = #ioctl }		\

static int chardev_create(struct file *file, unsigned int cmd, unsigned long arg)
{

	pr_info("Current %d, chardev_create function"
			" starts here..\n", current->pid);
	
	if (signal_pending(current)) {
		goto err_out;	
	}

	if (signal_pending(current)) {
		goto err_out;	
	}

	if (signal_pending(current)) {
		goto err_out;	
	}
	pr_info("Current %d, chardev_create function"
			" ends here..\n", current->pid);

	return 0;
err_out:
	
	pr_info("Current %d, chardev_create function"
			" ends with error %d\n", current->pid, -EINTR);
	return -EINTR;
}


static struct chardev_ioctl_desc chardev_ioctls[] = {
	CHARDEV_IOCTL_DEF(CHARDEV_IOCTL_INIT,
				chardev_create, 0),
};
#define CHARDEV_IOCTL_COUNT ARRAY_SIZE(chardev_ioctls)

static long chardev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	pr_info("Current %d, chardev_ioctl function"
			" starts here..\n", current->pid);

	int ret = -EINVAL;
	const struct chardev_ioctl_desc *ioctl = NULL;
	chardev_ioctl_t *func;

	unsigned int nr = _IOC_NR(cmd);
	pr_info("Current %d, chardev_ioctl function"
			" nr value %d\n", current->pid, nr);

	if (nr >= CHARDEV_IOCTL_COUNT)
		goto err_out;

	ioctl = &chardev_ioctls[nr];
	func = ioctl->func;

	ret = func(file, cmd, arg);
	if (ret < 0) {
		goto err_out;
	}

	return ret;
err_out:
	return ret;
}

static int chardev_open(struct inode *inode, struct file *file)
{
	pr_info("Current %d, chardev_open function" 
			" starts here..\n", current->pid);
	return 0;
}



static int chardev_release(struct inode *inode, struct file *file)
{
	pr_info("Current %d, chardev_release function"
			" starts here..\n", current->pid);
	return 0;
}


static const struct file_operations chardev_fops = {
	.owner = THIS_MODULE,
	.open = chardev_open,
	.release = chardev_release,
	.unlocked_ioctl = chardev_ioctl,
	.compat_ioctl = compat_ptr_ioctl
	// planning to implement mmap component as well
	// .mmap = chardev_mmap
};

static int __init chardev_init(void) 
{
	pr_info("Current %d, chardev_init function" 
			" starts here..\n", current->pid);

	int err = 0;

	dev_major = register_chrdev(0, DEV_NAME, &chardev_fops);

	if (dev_major < 0) {
		err = dev_major;
		goto out;
	}

	chardev_class = class_create(DEV_NAME);
	err = PTR_ERR(chardev_class);
	if (IS_ERR(chardev_class))
		goto err_chardev_class;

	chardev = device_create(chardev_class, NULL,
				MKDEV(dev_major, 0), NULL,
				DEV_NAME);

	err = PTR_ERR(chardev);
	if (IS_ERR(chardev))
		goto err_chardev;

	return 0;

err_chardev:
	class_destroy(chardev_class);
err_chardev_class:
	unregister_chrdev(dev_major, DEV_NAME);
out:
	return err;

}

static void __exit chardev_exit(void) 
{

	pr_info("Current %d, chardev_exit function" 
			" starts here..\n", current->pid);
	
	device_destroy(chardev_class, MKDEV(dev_major, 0));

	class_unregister(chardev_class);
	class_destroy(chardev_class);

	unregister_chrdev(dev_major, DEV_NAME);
	chardev = NULL;
}



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Siddharth Chintamaneni");

module_init(chardev_init);
module_exit(chardev_exit);
