#define LINUX

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include "mp1_given.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group_17");
MODULE_DESCRIPTION("CS-423 MP1");

#define DEBUG 1
#define FILENAME "status"
#define DIRECTORY "mp1"

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

//structs for proc filesystem
static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_entry;

struct process {
   int pid;
   unsigned long cpu_use;
   struct list_head list;
};

struct list_head processList;

void list_cleanup(void);

static ssize_t mp1_read (struct file *file, char __user *buffer, size_t count, loff_t *data) 
{
   char * tempBuffer = kmalloc(count, GFP_KERNEL);
   struct process *newProcess, *tmp;
   int writeCount = 0;
   int temp;
   if(tempBuffer == NULL)
   {
       printk(KERN_WARN "malloc failed");
       return 0;
   }  
   
   /* Iterate thorugh the list and print the list data into the string */
   /******************************* LOOOOOOOOCK THISSSS *******************/
   list_for_each_entry_safe(thisProcess, tmp, &processList, list) {
		
	writeCount += snprintf(tempBuffer+writeCount,(count- writeCount) , "PID %d: %d", tmp->pid, tmp->cpu_use); 

	/* If done, Null terminate and return */
	if(writeCount >= count) {	
	   tempBuffer[count - 1] = '\0';
	   break;
	}
	/* Else turn the NULL into a newline to continue the string */
	else {
		tempBuffer[writeCount - 1] = '\n';
	}	
		
   }

   /* Copy the string to user and return the right value */
   
   temp = copy_to_user(tempBuffer, buffer, min(count, writeCount));

   kfree(tempBuffer);
   return temp;
}

static ssize_t mp1_write (struct file *file, const char __user *buffer, size_t count, loff_t *data) 
{
   struct process *newProcess, *tmp, *thisProcess;
   newProcess = (process *)kmalloc(sizeof(struct process), GFP_KERNEL);

   // VIB: CHECK IF MALLOC FAILED
  
   newProcess->pid = newPID;     //register new PID

   
   if (list_empty(&processList) == 1){ // VIB: Changed from 0 to 1
      INIT_LIST_HEAD(newProcess->list);
      list_add(newProcess, processList);     //add to new to head

      //TODO   add reading PID from user
      //TODO   add locking
   }
   else {
      list_for_each_entry_safe(thisProcess, tmp, &processList, list){
         //TODO   add locking
         if(thisProcess->pid == newPID){
            // Error PID has already been inserted
         }
         else if(tmp == &processList){
            list_add(newProcess, thisProcess);     //needs Locks
         }
      }
   }
   // FREEEEEEE MEMORY
   return 0;
}

static const struct file_operations mp1_file = {
   .owner = THIS_MODULE,
   // .read = mp1_read,
   .write = mp1_write,
};

// this function safely deletes and frees the linked list
void list_cleanup(void) 
{
   struct process *aProcess, *tmp;

   if (list_empty(&processList) == 0) {
      printk(KERN_INFO "Cleaning up processList\n");
      list_for_each_entry_safe(aProcess, tmp, &processList, list) {
         #ifdef DEBUG
         printk(KERN_INFO "MP1 freeing PID %d\n", aProcess->pid);
         #endif

         list_del(&aProcess->list);
         kfree(aProcess);
      }
   }
}

// mp1_init - Called when module is loaded
int __init mp1_init(void)
{
   #ifdef DEBUG
   printk(KERN_ALERT "MP1 MODULE LOADING\n");
   #endif
   // Insert your code here ...
   printk(KERN_INFO "Hello World!\n");

   INIT_LIST_HEAD(&processList);
   
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

   printk(KERN_INFO "See ya.\n");
   list_cleanup();

   remove_proc_entry(FILENAME, proc_dir);
   proc_remove(proc_dir);

   printk(KERN_ALERT "MP1 MODULE UNLOADED\n");
}

// Register init and exit funtions
module_init(mp1_init);
module_exit(mp1_exit);
