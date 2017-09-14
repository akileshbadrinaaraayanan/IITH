// Assignment 4, Linux Kernel Modules. Written by Akilesh B, CS13B1042
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/printk.h>
#include <linux/slab.h>

// struct birthday containing day, month and year as elements
   struct birthday {
   	int day;
   	int month;
   	int year;
   	struct list_head list; //it holds next and prev, to point to corresponding entries in the list.
   };

   static LIST_HEAD(birthday_list);    //macro which initialized birthday_list of type list_head struct

    int yr = 1995, dy = 2, mnth = 8, i;

// module entry point, which is invoked when module is loaded into the kernel.
int init_linkedlist(void) {
   printk(KERN_INFO "Linked list: Module loaded successfully\n");

   struct birthday *person, *ptr;
   person = kmalloc(sizeof(*person), GFP_KERNEL);     //kernel memory is being allocated
   person->day = dy;
   person->month = mnth;
   person->year = yr;
   
   INIT_LIST_HEAD(&person->list);      //initializes the list head

   list_add_tail(&person->list, &birthday_list);   //add this element to the end of the linked list

   dy++;
   mnth++;
   yr++;

   for (i=0;i<4;i++)   // adding other elements of the linked list
   {
   person = kmalloc(sizeof(*person), GFP_KERNEL);
   person->day = dy;
   person->month = mnth;
   person->year = yr;
   
   list_add_tail(&person->list, &birthday_list);  

   dy++;
   mnth++;
   yr++; 	
   }

// Traversing the linked list and printing the elements to log
   list_for_each_entry(ptr, &birthday_list, list)     
   {
   		printk(KERN_INFO "day is %d\n",ptr->day);
   		printk(KERN_INFO "month is %d\n", ptr->month);
   		printk(KERN_INFO "year is %d\n", ptr->year);
   }
   return 0;
}

// module exit point, this function is called when the module is removed from the kernel.
void unload_linkedlist(void) {
   printk(KERN_INFO "Linked list: Module unloaded successfully\n");

   struct birthday *ptr, *next;
   // on each iteration, ptr points to the next birthday struct
   list_for_each_entry_safe(ptr, next, &birthday_list, list)
   {
   	list_del(&ptr->list);     //deleting the element
   	kfree(ptr);      //free the memory allocated to it
   }
	printk(KERN_INFO "Linked list: Module Deleted successfully\n");   
}

// macros for registering module entry and exit points with the kernel.
module_init(init_linkedlist);
module_exit(unload_linkedlist);

// Standard software details. 
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linked list module");
MODULE_AUTHOR("akilesh");