#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/filter.h>
#include <linux/bpf.h>

static int __init tiny_init(void)
{
	pr_info("tiny_init: \n");
	
    return 0;  
}

static void __exit hello_exit(void)
{

}

module_init(tiny_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Siddharth Chintamaneni");
MODULE_DESCRIPTION("Doesn't do much");
