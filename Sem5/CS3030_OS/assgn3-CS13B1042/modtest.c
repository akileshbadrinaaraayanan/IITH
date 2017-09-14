/* C program to test the system call */
#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
int main()
{
         long int var = syscall(360, 123, "akilesh");
         printf("System call sys_hello returned %ld\n", var);
         return 0;
}