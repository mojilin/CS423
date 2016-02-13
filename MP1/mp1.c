#define LINUX

#include <linux/module.h>
#include <linux/kernel.h>
#include "mp1_given.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group_ID");
MODULE_DESCRIPTION("CS-423 MP1");

#define DEBUG 1
#define FILENAME "status"
#define DIRECTORY "mp1"

//structs for proc filesystem
static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_entry;

static ssize_t mp1_read(struct file *file, char__user *buffer, size_t count, loff_t *data){
   //TODO
}
static ssize_t mp1_write(struct file *file, const char__user *buffer, size_t count, loff_t *data){

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
   
   proc_dir = proc_mkdir(DIRECTORY, NULL);
   proc_entry = proc_create(FILENAME, 0666, proc_dir, &mp1_file);  //create entry in proc system
   
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
   
   remove_proc_entry(FILENAME, proc_dir);
   proc_remove(proc_dir);

   printk(KERN_ALERT "MP1 MODULE UNLOADED\n");
}

// Register init and exit funtions
module_init(mp1_init);
module_exit(mp1_exit);
