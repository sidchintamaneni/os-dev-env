#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/kthread.h>
#include <asm/mwait.h>
#include <asm/io.h>

#include "chardev.h"

#define MAX_DEV 2
#define DEV_NAME "chardev"


#define CHARDEV_USER_CTXT 0xdeadbeef
#define CHARDEV_KERNEL_CTXT 0xdeaddead

#define CHARDEV_MMAP_TYPE_SHIFT 40
#define CHARDEV_MMAP_MEMORY (0x1ULL << CHARDEV_MMAP_TYPE_SHIFT)

static int dev_major = -1;
static struct class *chardev_class = NULL;
static struct device *chardev;

struct chardev_init_args {
        __u32 init_type;
        __u64 chardev_mmap_memory_base;
};

struct chardev_process {
	struct chardev_init_args *chardev_init_ptr;
	void *chardev_mmap_ptr;
	struct task_struct *lead_thread;
};

typedef int chardev_ioctl_t(struct file *file, unsigned int cmd, void *data);

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

static int kthread_function(void *args)
{
	pr_info("Current %d, kthread_function function"
			" starts here..\n", current->pid);
	unsigned long *test = (unsigned long *) args;
        msleep(30000);

        *test = 0x0;
	pr_info("Current %d, kthread_function function"
			" ends here..\n", current->pid);
	return 0;
}

static int chardev_create(struct file *file, unsigned int cmd, void *data)
{

	pr_info("Current %d, chardev_create function"
			" starts here..\n", current->pid);
	struct chardev_init_args *chardev_kernel_args = data;
	struct chardev_process *chardev_proc = file->private_data;

	pr_info("Current %d, chardev_create function"
			" receiving data from userspace %x..\n", current->pid, 
			chardev_kernel_args->init_type);

	// Do MONITOR and MWAIT Instruction Testing
	unsigned long *test_mwait = kzalloc(sizeof(unsigned long), GFP_KERNEL);
        if (test_mwait) {
                *test_mwait = (unsigned long) 0xdeadbeef;
                kthread_run(kthread_function, (void *)test_mwait, "kthread_function");
                while (*test_mwait != 0) {
			pr_info("Current %d, chardev_create function"
				"Monitor/Mwait interface starts here\n", current->pid);
                        mb();
                        __monitor((void *)test_mwait, 0, 0);
                        mb();
			pr_info("Current %d, chardev_create function"
				"the value of test_mwait %lx before mwait\n", current->pid, *test_mwait);
                        if (*test_mwait != 0) {
                                __mwait(1, 0);
                                mb();
                        }
			pr_info("Current %d, chardev_create function"
				"the value of test_mwait %lx after mwait\n", current->pid, *test_mwait);
			pr_info("Current %d, chardev_create function"
				"Monitor/Mwait interface ends here\n", current->pid);
                }
        }
	// Multi threading syscall interruption testing
	if (signal_pending(current)) {
		goto err_out;	
	}

	if (signal_pending(current)) {
		goto err_out;	
	}

	if (signal_pending(current)) {
		goto err_out;	
	}
	// change init type
	chardev_kernel_args->init_type = CHARDEV_KERNEL_CTXT;
	
	chardev_proc->chardev_mmap_ptr = kzalloc(4096, GFP_KERNEL);
	if (!chardev_proc->chardev_mmap_ptr) {
		pr_info("Current %d, chardev_create function"
			" chardev_mmap_memory_base kzalloc failed\n", current->pid);
	}
	chardev_kernel_args->chardev_mmap_memory_base = CHARDEV_MMAP_MEMORY;
	*((__u32 *)chardev_proc->chardev_mmap_ptr) = 0xbaadbaad;
	chardev_proc->chardev_init_ptr = chardev_kernel_args;
	pr_info("Current %d, chardev_create function"
		" chardev_mmap_memory_base 0x%llx before \n", current->pid,
			chardev_kernel_args->chardev_mmap_memory_base);

	pr_info("Current %d, chardev_create function"
		" chardev_mmap_ptr value %x\n", current->pid,
			*(__u32 *)chardev_proc->chardev_mmap_ptr);
	pr_info("Current %d, chardev_create function"
			" ends here..\n", current->pid);

	return 0;
err_out:
	
	pr_info("Current %d, chardev_create function"
			" ends with error %d\n", current->pid, -EINTR);
	return -EINTR;
}

static int chardev_mmap(struct file *file, struct vm_area_struct *vma) {


	pr_info("Current %d, chardev_mmap function"
			" starts here..\n", current->pid);
	
	struct chardev_process *chardev_proc = file->private_data;
	unsigned long offset;
	unsigned long pfn;
	void *ptr = chardev_proc->chardev_mmap_ptr;
	int ret;
	
	// Incase there are more memory addresses to mmap
	offset = vma->vm_pgoff << PAGE_SHIFT;
	pr_info("Current %d, chardev_mmap function"
			" verifying offset %lx..\n", current->pid, offset);

	vm_flags_set(vma, VM_IO | VM_DONTCOPY | VM_DONTEXPAND | VM_NORESERVE |
                     VM_DONTDUMP | VM_PFNMAP);		

	pfn = virt_to_phys(ptr)>>PAGE_SHIFT;

	pr_info("Current %d, chardev_mmap function"
			" pfn %lx..\n", current->pid, pfn);
	pr_info("Current %d, chardev_mmap function"
			" vma->vm_start before remap_pfn_range %lx and mmap_ptr %lx..\n", current->pid, vma->vm_start, (long unsigned int)ptr);
	// Map Physical Pages to userspace process
	ret = remap_pfn_range(vma, vma->vm_start, pfn, 4096, vma->vm_page_prot);

	pr_info("Current %d, chardev_mmap function"
			" vma->vm_start after remap_pfn_range %lx and mmap_ptr %lx..\n", current->pid, vma->vm_start, (long unsigned int)ptr);
	pr_info("Current %d, chardev_mmap function"
		" chardev_mmap_ptr value %x\n", current->pid,
			*(__u32 *)ptr);
	return ret;
}

static int chardev_flush(struct file *file, fl_owner_t id) {
	
	pr_info("Current %d, chardev_flush function"
			" starts here..\n", current->pid);
	return 0;
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

	struct chardev_init_args *chardev_kernel_args;

	unsigned int nr = _IOC_NR(cmd);
	pr_info("Current %d, chardev_ioctl function"
			" nr value %d\n", current->pid, nr);

	if (nr >= CHARDEV_IOCTL_COUNT)
		goto err_out;

	ioctl = &chardev_ioctls[nr];
	func = ioctl->func;
	
	chardev_kernel_args = kzalloc(sizeof(struct chardev_init_args), GFP_KERNEL);
	if (!chardev_kernel_args) {
		pr_info("Current %d, chardev_ioctl function"
			" chardev_kernel_args memory allocation failed\n", current->pid);
	} // copy the arguments from user
	if (copy_from_user(chardev_kernel_args, (void __user *)arg, sizeof(struct chardev_init_args))) {
		pr_info("Current %d, chardev_ioctl function"
			" copy_from_user failed\n", current->pid);
	}

	ret = func(file, cmd, chardev_kernel_args);
	if (ret < 0) {
		goto err_out;
	}

	pr_info("Current %d, chardev_create function"
		" chardev_mmap_memory_base before copy_to_user 0x%llx\n", current->pid,
			chardev_kernel_args->chardev_mmap_memory_base);

	if (copy_to_user((void __user *)arg, chardev_kernel_args, sizeof(struct chardev_init_args))) {
		pr_info("Current %d, chardev_ioctl function"
			" copy_to_user failed\n", current->pid);
	}
	// copy the arguments to the user

	return ret;
err_out:
	return ret;
}

static int chardev_open(struct inode *inode, struct file *file)
{
	pr_info("Current %d, chardev_open function" 
			" starts here..\n", current->pid);
	
	struct chardev_process *chardev_proc = kzalloc(sizeof(struct chardev_process), GFP_KERNEL);
	chardev_proc->lead_thread = current->group_leader;
	get_task_struct(chardev_proc->lead_thread);

	file->private_data = chardev_proc;
		
	return 0;
}



static int chardev_release(struct inode *inode, struct file *file)
{
	pr_info("Current %d, chardev_release function"
			" starts here..\n", current->pid);
	struct chardev_process *chardev_proc = file->private_data;

	if (!chardev_proc)
		return 0;

	if (!chardev_proc->chardev_init_ptr) {
		kfree(chardev_proc);
		return 0;
	}

	if (chardev_proc->chardev_init_ptr->init_type == CHARDEV_KERNEL_CTXT) {
		pr_info("Current %d, chardev_release function"
			" releasing chardev_mmap_ptr..\n", current->pid);
		kfree(chardev_proc->chardev_mmap_ptr);	
	}
	put_task_struct(chardev_proc->lead_thread);

	kfree(chardev_proc);
	pr_info("Current %d, chardev_release function"
		" releasing chardev_proc..\n", current->pid);
	return 0;
}


static const struct file_operations chardev_fops = {
	.owner = THIS_MODULE,
	.open = chardev_open,
	.release = chardev_release,
	.unlocked_ioctl = chardev_ioctl,
	.compat_ioctl = compat_ptr_ioctl,
	.mmap = chardev_mmap,
	.flush = chardev_flush
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
