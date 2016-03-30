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
#include <linux/vmalloc.h>
#include "mp3_given.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group_17");
MODULE_DESCRIPTION("CS-423 MP3");

#define DEBUG 1
#define FILENAME "status"
#define DIRECTORY "mp3"

 
//structs for proc filesystem
static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_entry;

spinlock_t *list_lock;

struct workqueue_struct *cpu_wq;

static struct kmem_cache *PCB_cache;

//all times are stored as jiffies
typedef struct  
{
   struct task_struct* linux_task; 
   //struct timer_list wakeup_timer; 
   int minor_fault_count;
   int major_fault_count;
   int processor_utilization;
   int pid;
   struct list_head list;
} mp3_task_struct;

static struct list_head processList;

static int read_end;

unsigned char* mem_buf; // memory buffer used between monitor and kernel

/* mp3_read -- Callback function when reading from the proc file
 *
 * Inputs: buffer -- User space buffer to copy information to
 * 			count -- Number of bytes to copy to the buffer
 * Outputs: Copies the string to the user buffer
 * CPU usage time in the following format:
 * PID: xx
 *
 * Side Effects: Holds the list_lock
 * Return Value: Number of bytes written to the buffer.
 */

static ssize_t mp3_read (struct file *file, char __user *buffer, size_t count, loff_t *data) 
{
    char * tempBuffer = kmalloc(count, GFP_KERNEL); 
    mp3_task_struct * cursor;

    if(tempBuffer == NULL)
    {
        printk(KERN_WARNING "read malloc failed");
        return 0;
    }
  
    /* Based on read_end, we decide whether to return 0 (to indicate we are done reading)
	*  If 0 is returned, we reset the boolean */
	if (read_end == 0)
	{
    	int bytes_copied = 0;

    	spin_lock(list_lock);
   
	/* Output the string into tempBuffer for all entries in the list */
		list_for_each_entry(cursor, &processList, list)
		{
			bytes_copied += snprintf(&tempBuffer[bytes_copied], count - bytes_copied, "PID: %d\n", cursor->pid);
		}

		spin_unlock(list_lock);
		read_end = 1;
	// Copy to user buffer
		if (copy_to_user(buffer, tempBuffer, bytes_copied) == 0) // success
		{
			kfree(tempBuffer);
			return bytes_copied;
    	}
    	else
    	{
   			kfree(tempBuffer);
			return 0;
    	}

	} 
	else // read_end == 1 means we just return 0 for formality
	{
    	read_end = 0;
    	kfree(tempBuffer);
    	return 0;	  
	}   
}


/* add_process(int pid)
 * Adds a new process with given pid to the linked list 
 * Inputs:  pid -- The pid of the registered process
 * Return Value: 1 on success, 0 on failure
 * Side Effects: Holds the list_lock lock
 */
int add_process (int pid)
{
   mp3_task_struct * newProcess = kmem_cache_alloc(PCB_cache, GFP_KERNEL);
   if(!newProcess)
   {
      printk(KERN_WARNING "mp3 add malloc failed\n");
      return 0;
   }

   printk(KERN_INFO "Adding pid %d\n", pid);

   /* Add to the linked list */
   INIT_LIST_HEAD(&newProcess->list);

   newProcess->pid = pid;
   newProcess->linux_task = find_task_by_pid(pid);

   // TODO add to work queue? what do we do about the other variables? 

   spin_lock(list_lock);
   list_add_tail( &newProcess->list, &processList);
   spin_unlock(list_lock);

   return 1;
}


/* mp3_write
 * Registers a new process with pid given in the user buffer
 * Inputs: buffer -- User buffer that contains the pid of the new process
 * Return Value: number of bytes copied from the user
 * Side effects: Calls add_process that holds list_lock
 */
static ssize_t mp3_write (struct file *file, const char __user *buffer, size_t count, loff_t *data) 
{
	char * tempBuffer = kmalloc(count+1, GFP_KERNEL);
	int PID;
	char op;
	if(tempBuffer == NULL)
	{
		printk(KERN_WARNING "mp3 write malloc failed\n");
		return 0;
	}

	/* Get info from user */
	  
	if(copy_from_user(tempBuffer, buffer, count) != 0)
		goto write_fail;

	/* NULL terminate string */
	tempBuffer[count] = '\0';

	printk(KERN_INFO "mp3 Command: %s\n", tempBuffer);
   	sscanf(tempBuffer, "%c", &op);

   	printk(KERN_INFO "mp3 OP: %c\n", op);
	/* parse string */
   	switch (op){
    	case 'R':
	        printk(KERN_INFO "mp3 Registration\n");
	   		sscanf(tempBuffer, "%c, %d", &op, &PID);
	        add_process(PID);
	        break;

    	case 'U':
        	printk(KERN_INFO "mp3 Unregistration\n");
        	sscanf(tempBuffer, "%c, %d", &op, &PID);
        	// TODO
        	break;

      	default:
			printk(KERN_WARNING "mp3 write fail\n");
			goto write_fail;
			break;
   }

	kfree(tempBuffer);

	return  count;
write_fail:
	printk(KERN_WARNING "write failed\n");
	kfree(tempBuffer);
	return 0;	
}

static const struct file_operations mp3_file = 
{
   .owner = THIS_MODULE,
   .read = mp3_read,
   .write = mp3_write,
};


/* list_cleanup
 * Helper function to delete the linked list upon kernel module removal
 * Side Effects: Holds list lock and deletes the entire processList
 */
void list_cleanup(void) 
{
   mp3_task_struct *aProcess, *tmp;

   spin_lock(list_lock);
   if (list_empty(&processList) == 0) 
   {
	#ifdef DEBUG
      printk(KERN_INFO "Cleaning up processList\n");
    #endif
	  // Delete all entry
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


// mp3_init - Called when module is loaded
int __init mp3_init(void)
{
   #ifdef DEBUG
   printk(KERN_ALERT "MP3 MODULE LOADING\n");
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

   //Init spin lock
   spin_lock_init(list_lock);

   //Init workqueue
   cpu_wq = alloc_workqueue("cpu", (unsigned int) 0, 1);
   if(cpu_wq == NULL)
   {
      printk(KERN_WARNING "MP3 workqueue init failed");
      return -1;
   }

   // Init entry to work queue
   // INIT_WORK(&update_cpu_times, cpu_time_updater_work);

   INIT_LIST_HEAD(&processList);

   read_end = 0;
   
   proc_dir = proc_mkdir(DIRECTORY, NULL);
   proc_entry = proc_create(FILENAME, 0666, proc_dir, &mp3_file);  //create entry in proc system

   mem_buf = (unsigned char*)vmalloc(128 * 4000); // 128 * 4 KB in Step 5

   if (!mem_buf) {
		   printk(KERN_WARNING "Memory buffer allocation fail.\n");
		   return -1;
   } 

   // TODO not sure if we need to do anything explicit with PG_reserved
   
   printk(KERN_ALERT "MP3 MODULE LOADED\n");
   // add_timer(&work_timer);
   return 0;   
}

// mp3_exit - Called when module is unloaded
void __exit mp3_exit(void)
{
#ifdef DEBUG
   printk(KERN_ALERT "MP3 MODULE UNLOADING\n");
   #endif
   // Insert your code here ...
   // del_timer(&work_timer);
   
   flush_workqueue(cpu_wq);
   destroy_workqueue(cpu_wq);

   printk(KERN_INFO "See ya.\n");
   list_cleanup();

   remove_proc_entry(FILENAME, proc_dir);
   proc_remove(proc_dir);
   kfree(list_lock);

   printk(KERN_ALERT "MP3 MODULE UNLOADED\n");
}

// Register init and exit funtions
module_init(mp3_init);
module_exit(mp3_exit);
