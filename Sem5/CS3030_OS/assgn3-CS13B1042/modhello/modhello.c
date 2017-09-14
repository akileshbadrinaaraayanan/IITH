/* defining new system call */
#include <linux/kernel.h>
#include <linux/linkage.h>

 asmlinkage long sys_modhello(int a, char* b)
{
	printk(KERN_ALERT "hello world\n");
    printk(KERN_ALERT "integer is %d\n", a);
	printk(KERN_ALERT "string is %s\n", b);
        return 0;
 }