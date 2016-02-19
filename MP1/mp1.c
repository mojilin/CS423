#define LINUX

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include "mp1_given.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group_17");
MODULE_DESCRIPTION("CS-423 MP1");

#define DEBUG 1
#define FILENAME "status"
#define DIRECTORY "mp1"


//structs for proc filesystem
static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_entry;
static struct time_list work_timer;

spinlock_t *list_lock;

typedef struct process 
{
   unsigned int pid;
   unsigned long cpu_use;
   struct list_head list;
} process;

struct list_head processList;

/* Function prototypes */
void list_cleanup(void);
int add_process (int pid);


static ssize_t mp1_read (struct file *file, char __user *buffer, size_t count, loff_t *data) 
{
   char * tempBuffer = kmalloc(count, GFP_KERNEL);
   process *thisProcess;
   int writeCount = 0;
   int temp;
   if(tempBuffer == NULL)
   {
       printk(KERN_WARNING "read malloc failed");
       return 0;
   }  
   
   /* Iterate thorugh the list and print the list data into the string */
   spin_lock(list_lock); 
   list_for_each_entry(thisProcess, &processList, list) 
   {
		
		writeCount += snprintf(tempBuffer+writeCount,(count- writeCount),
					   	"PID %u: %lu", thisProcess->pid, thisProcess->cpu_use); 

		/* If done, Null terminate and return */
		if(writeCount >= count) 
		{	
			tempBuffer[count - 1] = '\0';
			break;
		}	
		/* Else turn the NULL into a newline to continue the string */
		else 
		{
			tempBuffer[writeCount - 1] = '\n';
		}	
				
   }
   spin_unlock(list_lock);
   /* Copy the string to user and return the right value */
   
   temp = copy_to_user(buffer, tempBuffer, min((int)count, writeCount));

   kfree(tempBuffer);
   return temp;
}

int add_process (int pid)
{
	process * newProcess = kmalloc(sizeof(process), GFP_KERNEL);
	if(newProcess == NULL)
	{
		printk(KERN_WARNING "add malloc failed");
		return 0;
	}

	newProcess->pid = pid;
	newProcess->cpu_use = 0;

	/* Add to the linked list */
	INIT_LIST_HEAD(&newProcess->list);


	spin_lock(list_lock);
	list_add( &newProcess->list, &processList);
	spin_unlock(list_lock);

	return 1;

}

static ssize_t mp1_write (struct file *file, const char __user *buffer, size_t count, loff_t *data) 
{
	char * tempBuffer = kmalloc(count, GFP_KERNEL);
	long int temp;	
	if(tempBuffer == NULL)
	{
		printk(KERN_WARNING "write malloc failed");
		return 0;
	}

	temp = copy_from_user(tempBuffer, buffer, count);
	if(temp == 0)
		goto write_fail;

	/* Convert str to int */
	if(kstrtol(tempBuffer, 10, &temp) != 0)
		goto write_fail;

	temp = add_process(temp);

	kfree(tempBuffer);

	return (temp == 0) ? 0 : count;
write_fail:
	printk(KERN_WARNING "write copy failed");
	kfree(tempBuffer);
	return 0;	
}

static const struct file_operations mp1_file = 
{
   .owner = THIS_MODULE,
   .read = mp1_read,
   .write = mp1_write,
};

// this function safely deletes and frees the linked list
void list_cleanup(void) 
{
   process *aProcess, *tmp;

   spin_lock(list_lock);
   if (list_empty(&processList) == 0) 
   {
      printk(KERN_INFO "Cleaning up processList\n");
      list_for_each_entry_safe(aProcess, tmp, &processList, list) 
      {
         #ifdef DEBUG
         printk(KERN_INFO "MP1 freeing PID %d\n", aProcess->pid);
         #endif

         list_del(&aProcess->list);
         kfree(aProcess);
      }
   }

   spin_unlock(list_lock);
}

// mp1_init - Called when module is loaded
int __init mp1_init(void)
{
   #ifdef DEBUG
   printk(KERN_ALERT "MP1 MODULE LOADING\n");
   #endif
   // Insert your code here ...
   printk(KERN_INFO "Hello World!\n");

   // init_timer(&work_timer);   //Initialize timer to wake up work queue
   // work_timer.function = work_time_handler;
   // work_timer.expires = jiffies + 5*HZ;
   // work_timer.data = 0;

   list_lock = kmalloc(sizeof(spinlock_t), GFP_KERNEL);
   if(list_lock == NULL)
   {
		printk(KERN_WARNING "spinlock malloc failed");
		return -1;
   }
   spin_lock_init(list_lock);

   INIT_LIST_HEAD(&processList);
   
   proc_dir = proc_mkdir(DIRECTORY, NULL);
   proc_entry = proc_create(FILENAME, 0666, proc_dir, &mp1_file);  //create entry in proc system
   
   printk(KERN_ALERT "MP1 MODULE LOADED\n");
   // add_timer(&work_timer);
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

   remove_proc_entry(FILENAME, proc_dir);
   proc_remove(proc_dir);
   kfree(list_lock);

   printk(KERN_ALERT "MP1 MODULE UNLOADED\n");
}

// Register init and exit funtions
module_init(mp1_init);
module_exit(mp1_exit);
