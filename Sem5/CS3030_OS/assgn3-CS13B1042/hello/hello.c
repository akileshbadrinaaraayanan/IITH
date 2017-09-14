/* Implementing system call */
#include <linux/kernel.h>

 asmlinkage long sys_hello(void)
{
        printk("hello world\n");
        return 0;
 }