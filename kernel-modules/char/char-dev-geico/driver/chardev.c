#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <asm/io.h>

#include "chardev_ioctl.h"

//#define DEBUG_INFO

#define DEV_NAME "chardev"
#define MAX_DEV 1
//#define BUFFER_SIZE 65536
#define BUFFER_SIZE 1024

static int dev_major = -1;
static struct class *chardev_class = NULL;
static struct cdev cdev;
static struct device *chardev;

struct internal_buffer {
    char *start;
    char *end;
    struct mutex buffer_lock;
};

struct chardev_process {
    struct internal_buffer *device_buffer;
    bool clear_on_read;
};

static ssize_t chardev_read(struct file *file, char __user *buf,
			                size_t count, loff_t *ppos) {
    struct internal_buffer *device_buffer;
    struct chardev_process *cdev_proc;
    size_t bytes_read;
#ifdef DEBUG_INFO
    pr_info("chardev_read: read syscall\n");
    pr_info("chardev_read: init values - count %ld and ppos %lld\n", count, *ppos);
#endif
    cdev_proc = file->private_data;
    if (!cdev_proc)
        return -EFAULT;

    device_buffer = cdev_proc->device_buffer;
    if (!device_buffer)
        return -EFAULT;

    mutex_lock(&device_buffer->buffer_lock);

    /*
     * If read past the internal buffer capacity then
     * read returns data from ppos till buffer size
     */ 
    if (count + *ppos >= BUFFER_SIZE) {
        bytes_read = BUFFER_SIZE - *ppos;
    } else {
        bytes_read = count + *ppos;
    }
    
    if (copy_to_user((char __user *)buf, device_buffer->start + *ppos, bytes_read) != 0) {
        mutex_unlock(&device_buffer->buffer_lock);
        return -EFAULT;
    }

    if (cdev_proc->clear_on_read == true) {
        memset(device_buffer->start + *ppos, 0, bytes_read);
    }

#ifdef DEBUG_INFO
    pr_info("chardev_read: user buffer memory %lx\n", *((unsigned long *)device_buffer->start + *ppos));
#endif

    if (count + *ppos >= BUFFER_SIZE) {
        *ppos = 0;
    } else {
        *ppos += bytes_read;
    }

#ifdef DEBUG_INFO
    pr_info("chardev_read: exit values - bytes_read %ld and ppos %lld\n", bytes_read, *ppos);
#endif
    mutex_unlock(&device_buffer->buffer_lock);
    return bytes_read;

}

static ssize_t chardev_write(struct file *file, const char __user *buf,
            			     size_t count, loff_t *ppos)
{
    struct internal_buffer *device_buffer;
    struct chardev_process *cdev_proc;
    size_t bytes_written = 0;
#ifdef DEBUG_INFO
    pr_info("chardev_write: write syscall\n");
    pr_info("chardev_write: init values - count %ld and ppos %lld\n", count, *ppos);
#endif
    cdev_proc = file->private_data;
    if (!cdev_proc)
        return -EFAULT;

    device_buffer = cdev_proc->device_buffer;
    if (!device_buffer)
        return -EFAULT;

    mutex_lock(&device_buffer->buffer_lock);
    /*
     *  If buffer is size is exceeded write will
     *  overwrite the contents at the start
     */ 
    while (count + *ppos >= BUFFER_SIZE) {
        size_t bytes_to_write = BUFFER_SIZE - *ppos;
        if (copy_from_user(device_buffer->start + *ppos, buf + bytes_written, bytes_to_write) != 0) {
            goto err_copy_from_user;
        }
        *ppos = 0;
        count -= bytes_to_write;
        bytes_written += bytes_to_write;
    }

    if (count > 0) {
        if (copy_from_user(device_buffer->start + *ppos, buf + bytes_written, count) != 0) {
            goto err_copy_from_user;
        }
        *ppos = count;
        bytes_written += count;
        count = 0;
    }

#ifdef DEBUG_INFO
    pr_info("chardev_write: kernel buffer memory %lx\n", *((unsigned long *)device_buffer->start));
    pr_info("chardev_write: exit values - bytes_write %ld and ppos %lld\n", bytes_written, *ppos);
#endif
    mutex_unlock(&device_buffer->buffer_lock);
    return bytes_written;

err_copy_from_user:
    mutex_unlock(&device_buffer->buffer_lock);
    return -EFAULT;
}

static loff_t chardev_llseek(struct file *file, loff_t off, int whence)
{
    struct internal_buffer *device_buffer;
    struct chardev_process *cdev_proc;
    loff_t new_pos;

#ifdef DEBUG_INFO
    pr_info("chardev_llseek: llseek syscall\n");
    pr_info("chardev_llseek: init values - off %lld and whence %d\n", off, whence);
#endif
    cdev_proc = file->private_data;
    if (!cdev_proc)
        return -EFAULT;

    device_buffer = cdev_proc->device_buffer;
    if (!device_buffer)
        return -EFAULT;

    mutex_lock(&device_buffer->buffer_lock);
    switch(whence) {
        case SEEK_SET:
            new_pos = off;
            break;
        case SEEK_CUR:
            new_pos = file->f_pos + off;
            break;
        case SEEK_END:
            new_pos = BUFFER_SIZE + off;
            break;
        default:
            goto err_inval_pos;
    }
    /*
     * If updated position is < 0 or > Internal buffer size
     * lseek will return -EINVAL
     */
    if (new_pos < 0) { 
            goto err_inval_pos;
    }
    else if (new_pos >= BUFFER_SIZE) {
            goto err_inval_pos;
    }

    file->f_pos = new_pos;
#ifdef DEBUG_INFO
    pr_info("chardev_llseek: new position %lld\n", new_pos);
#endif
    mutex_unlock(&device_buffer->buffer_lock);
    return new_pos;

err_inval_pos:
    mutex_unlock(&device_buffer->buffer_lock);
    return -EINVAL;
}

static long chardev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct internal_buffer *device_buffer;
    struct chardev_process *cdev_proc;
#ifdef DEBUG_INFO
    pr_info("chardev_ioctl: ioctl syscall\n");
    pr_info("chardev_ioctl: init values - cmd %d\n", cmd);
#endif
    cdev_proc = file->private_data;
    if (!cdev_proc)
        return -EFAULT;

    device_buffer = cdev_proc->device_buffer;
    if (!device_buffer)
        return -EFAULT;
    mutex_lock(&device_buffer->buffer_lock);
    /*
     * ioctl supports
     *      - CHARDEV_IOC_CLEAR_ON_READ
     *          - clears the buffer memory which has been read
     *      - CHARDEV_IOC_NO_CLEAR
     */
    switch (cmd) {
        case CHARDEV_IOC_CLEAR_ON_READ:
#ifdef DEBUG_INFO
            pr_info("chardev_ioctl: triggered CLEAR_ON_READ\n");
#endif
            cdev_proc->clear_on_read = true;
            break;
        case CHARDEV_IOC_NO_CLEAR:
#ifdef DEBUG_INFO
            pr_info("chardev_ioctl: triggered NO_CLEAR\n");
#endif
            cdev_proc->clear_on_read = false;
            break;
        default:
            mutex_unlock(&device_buffer->buffer_lock);
            return -EINVAL;
    }
    mutex_unlock(&device_buffer->buffer_lock);
    return 0;
}

static int chardev_open(struct inode *inode, struct file *file)
{
    int err;
    struct internal_buffer *device_buffer;
    struct chardev_process *cdev_proc;
#ifdef DEBUG_INFO
    pr_info("chardev_open: File opened\n");
#endif
    cdev_proc = kzalloc(sizeof(struct chardev_process), GFP_KERNEL); 
    if (!cdev_proc)
        return -ENOMEM;

    device_buffer = kzalloc(sizeof(struct internal_buffer), GFP_KERNEL);
    if (!device_buffer) {  
        err = -ENOMEM;
        goto err_device_buffer;
    }

    device_buffer->start = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!device_buffer->start) {
        err = -ENOMEM;
        goto err_buffer;
    }

    device_buffer->end = device_buffer->start + BUFFER_SIZE;
    mutex_init(&device_buffer->buffer_lock);

    cdev_proc->device_buffer = device_buffer;
    cdev_proc->clear_on_read = false;

    file->private_data = cdev_proc;

    return 0; 

err_buffer:
    kfree(device_buffer);
err_device_buffer:
    kfree(cdev_proc);

    return err;
}

static int chardev_release(struct inode *inode, struct file *file)
{
    struct chardev_process *cdev_proc;
#ifdef DEBUG_INFO
    pr_info("chardev_open: File closed\n");
#endif
    cdev_proc = file->private_data;
    if (!cdev_proc)
        return 0;
    
    kfree(cdev_proc->device_buffer->start);
    kfree(cdev_proc->device_buffer);
    kfree(cdev_proc);


    return 0;
}

static const struct file_operations chardev_fops = {
    .owner = THIS_MODULE,
    .open = chardev_open,
    .release = chardev_release,
    .unlocked_ioctl = chardev_ioctl,
    .read = chardev_read,
    .write = chardev_write,
    .llseek  = chardev_llseek,
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
    chardev_class = class_create(THIS_MODULE, DEV_NAME);
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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Siddharth Chintamaneni");

module_init(chardev_init);
module_exit(chardev_exit);
