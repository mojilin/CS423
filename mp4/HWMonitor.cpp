#include "HWMonitor.h"

// from MP1
int HWMonitor::get_cpu_use(int pid, unsigned long *cpu_use)
{
   struct task_struct* task;
   rcu_read_lock();
   task = find_task_by_pid(pid);

   if (task != NULL)
   {  
		*cpu_use = task->utime;
      rcu_read_unlock();

      return 0;
   }
   else 
   {
     rcu_read_unlock();

     return -1;
   }
}

double HWMonitor::get_throttle() {
  return throttle;
}

void HWMonitor::set_throttle(double new_throttle) {
  std::lock(throttle_lock);
  this.throttle = new_throttle;
  throttle_lock.unlock();
}