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
#include <linux/workqueue.h>
#include "mp2_given.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group_17");
MODULE_DESCRIPTION("CS-423 MP2");

#define DEBUG 1
#define FILENAME "status"
#define DIRECTORY "mp2"

//

task_struct * kernel_thread;

typedef struct  {
struct task_struct* linux_task; 
struct timer_list wakeup_timer; 
int period;  //p
int computation; //c
int pid;
enum {READY, RUNNING, SLEEPING} status;
} mp2_task_struct;


//structs for proc filesystem
static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_entry;

/* Function prototypes */
void timer_handler(unsigned long task);
int kernel_thread_fn(void *data);

int kernel_thread_fn(void *data)
{
	//dispatcher
return 0;
}
void timer_handler(unsigned long task)
{
	mp2_task_struct * the_task = (mp2_task_struct *) task;
	the_task -> status = READY;

	//needs to wake up dispatcher thread, but thats pretty much it
	
}
/* mp2_read -- Callback function when reading from the proc file
 *
 * Inputs: buffer -- User space buffer to copy information to
 * 			count -- Number of bytes to copy to the buffer
 * Outputs: Copies the string to the user buffer
 * CPU usage time in the following format:
 * PID: xx | CPU Use: xxxxx
 * PID: xx | CPU Use: xxxxx
 *
 * Side Effects: Holds the list_lock
 * Return Value: Number of bytes written to the buffer.
 */

static ssize_t mp2_read (struct file *file, char __user *buffer, size_t count, loff_t *data) 
{
 //   char * tempBuffer = kmalloc(count, GFP_KERNEL); 
 //   process * cursor;

 //   if(tempBuffer == NULL)
 //   {
 //       printk(KERN_WARNING "read malloc failed");
 //       return 0;
 //   }
	
 //   /* Based on read_end, we decide whether to return 0 (to indicate we are done reading)
	// *  If 0 is returned, we reset the boolean */
 //   if (read_end == 0)
 //   {
	//   int bytes_copied = 0;

 //      spin_lock(list_lock);
   
	//   /* Output the string into tempBuffer for all entries in the list */
	//   list_for_each_entry(cursor, &processList, list)
	//   {
	// 	 bytes_copied += snprintf(&tempBuffer[bytes_copied], count - bytes_copied, "PID: %d | CPU Use: %lu\n", cursor->pid, cursor->cpu_use);
	//   }

	//   spin_unlock(list_lock);
	//   read_end = 1;
	//   // Copy to user buffer
	//   if (copy_to_user(buffer, tempBuffer, bytes_copied) == 0) // success
	//   {
 //         kfree(tempBuffer);
	//      return bytes_copied;
	//   }
	//   else
	//   {
	// 	 kfree(tempBuffer);
	// 	 return 0;
	//   }

 //   } 
 //   else // read_end == 1 means we just return 0 for formality
 //   {
 //      read_end = 0;
	//   kfree(tempBuffer);
 //      return 0;	  
 //   }   
}

/* mp2_write
 * Registers a new process with pid given in the user buffer
 * Inputs: buffer -- User buffer that contains the pid of the new process
 * Return Value: number of bytes copied from the user
 * Side effects: Calls add_process that holds list_lock
 */
static ssize_t mp2_write (struct file *file, const char __user *buffer, size_t count, loff_t *data) 
{
	char * tempBuffer = kmalloc(count+1, GFP_KERNEL);
	long int temp;
   int PID, period, comp_time;
   char op;
	if(tempBuffer == NULL)
	{
		printk(KERN_WARNING "write malloc failed\n");
		return 0;
	}

	/* Get pid from user and convert to int */
	temp = copy_from_user(tempBuffer, buffer, count);
	if(temp != 0)
		goto write_fail;

	/* NULL terminate string */
	tempBuffer[count] = '\0';

	printk(KERN_INFO "%s", tempBuffer);
   sscanf(tempBuffer, "%c, %d, %d, %d", &op, &PID, &period, &comp_time);
	/* Convert str to int */
   switch (op){
      case 'R':
         printk(KERN_INFO "MP2 Registration\n");
         break;

      case 'Y':
         printk(KERN_INFO "MP2 Yield\n");
         break;

      case 'D':
         printk(KERN_INFO "MP2 De-Registration\n");
         break;

      default:
         printk(KERN_WARNING "MP2 write fail\n");
         goto write_fail;
         break;
   }

	kfree(tempBuffer);

	return (temp == 0) ? 0 : count;
write_fail:
	printk(KERN_WARNING "write failed\n");
	kfree(tempBuffer);
	return 0;	
}

static const struct file_operations mp2_file = 
{
   .owner = THIS_MODULE,
   .read = mp2_read,
   .write = mp2_write,
};

// mp2_init - Called when module is loaded
int __init mp2_init(void)
{
   #ifdef DEBUG
   printk(KERN_ALERT "mp2 MODULE LOADING\n");
   #endif
   //create kthread
   kernel_thread = kthread_create(kernel_thread_fn,NULL,"MP2 dispatcher");
   // Insert your code here ...
   printk(KERN_INFO "Hello World!\n");
   
   proc_dir = proc_mkdir(DIRECTORY, NULL);
   proc_entry = proc_create(FILENAME, 0666, proc_dir, &mp2_file);  //create entry in proc system
   
   printk(KERN_ALERT "mp2 MODULE LOADED\n");
   return 0;   
}

// mp2_exit - Called when module is unloaded
void __exit mp2_exit(void)
{
   #ifdef DEBUG
   printk(KERN_ALERT "mp2 MODULE UNLOADING\n");
   #endif
   // Insert your code here ...
kthread_create()
   remove_proc_entry(FILENAME, proc_dir);
   proc_remove(proc_dir);

   printk(KERN_ALERT "MP2 MODULE UNLOADED\n");
}

// Register init and exit funtions
module_init(mp2_init);
module_exit(mp2_exit);
