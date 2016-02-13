#define LINUX

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include "mp1_given.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group_17");
MODULE_DESCRIPTION("CS-423 MP1");

#define DEBUG 1

struct process {
   int pid;
   unsigned long cpu_use;
   struct list_head list;
};

struct process processList;

static ssize_t mp1_read (struct file *file, char user *buffer, size_t count, loff_t*data){
   // implementation goes here...
}

static ssize_t mp1_write (struct file *file, const char user *buffer, size_t count, loff_t*data){
   // implementation goes here...
}

static const struct file_operations mp1_file = {
   .owner = THIS_MODULE,
   .read = mp1_read,
   .write = mp1_write,
};

// mp1_init - Called when module is loaded
int __init mp1_init(void)
{
   #ifdef DEBUG
   printk(KERN_ALERT "MP1 MODULE LOADING\n");
   #endif
   // Insert your code here ...
   printk(KERN_INFO "Hello World!\n");
   
   
   
   printk(KERN_ALERT "MP1 MODULE LOADED\n");
   return 0;   
}

// mp1_exit - Called when module is unloaded
void __exit mp1_exit(void)
{
   #ifdef DEBUG
   printk(KERN_ALERT "MP1 MODULE UNLOADING\n");
   #endif
   // Insert your code here ...
   printk(KERN_INFO "See ya.\n");
   list_cleanup();
   

   printk(KERN_ALERT "MP1 MODULE UNLOADED\n");
}

// this function safely deletes and frees the linked list
void list_cleanup() {
   struct process *aProcess, *tmp;

   printk(KERN_INFO "Cleaning up processList\n");
   list_for_each_entry_safe(aProcess, tmp, &processList.list, list) {
      printk(KERN_INFO "MP1 freeing PID %d\n", aProcess->pid);
      list_del(&aProcess->list);
      kfree(aProcess);
   }
}

// Register init and exit funtions
module_init(mp1_init);
module_exit(mp1_exit);
