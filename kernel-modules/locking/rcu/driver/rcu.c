#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <asm/io.h>

#define DEBUG_INFO 1
#define DEV_NAME "chardev"
#define MAX_DEV 1

static int dev_major = -1;
static struct class *chardev_class = NULL;
static struct cdev cdev;
static struct device *chardev;

static int chardev_open(struct inode *inode, struct file *file)
{
#ifdef DEBUG_INFO
    pr_info("chardev_open: File opened\n");
#endif

    return 0; 
}

static int chardev_release(struct inode *inode, struct file *file)
{
#ifdef DEBUG_INFO
    pr_info("chardev_release[%d]: File closed\n", current->pid);
#endif

    return 0;
}

static const struct file_operations chardev_fops = {
    .owner = THIS_MODULE,
    .open = chardev_open,
    .release = chardev_release,
};

static int __init chardev_init(void)
{
    dev_t dev;
    int err;
#ifdef DEBUG_INFO
    pr_info("chardev_init: PID %d, starts here\n", current->pid);
#endif

    err = alloc_chrdev_region(&dev, 0, MAX_DEV, DEV_NAME);
    if (err < 0) {
        goto out;
    }

    dev_major = MAJOR(dev);
    chardev_class = class_create(DEV_NAME);
    err = PTR_ERR(chardev_class);
    if (IS_ERR(chardev_class))
        goto err_chardev_class;

    cdev_init(&cdev, &chardev_fops);
    cdev.owner = THIS_MODULE;

    err = cdev_add(&cdev, MKDEV(dev_major, 0), 1);
    if (err < 0)
        goto err_chardev;
    
    chardev = device_create(chardev_class, NULL, MKDEV(dev_major, 0), NULL, DEV_NAME);
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
#ifdef DEBUG_INFO
    pr_info("chardev_exit: PID %d, ends here\n", current->pid);
#endif

	device_destroy(chardev_class, MKDEV(dev_major, 0));

	class_destroy(chardev_class);

	unregister_chrdev(dev_major, DEV_NAME);
	chardev = NULL;
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Siddharth Chintamaneni");
