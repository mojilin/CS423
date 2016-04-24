#ifndef HWM_H
#define HWM_H

#include <linux/pid.h>
#include <linux/sched.h>
#include <mutex>

#define find_task_by_pid(nr) pid_task(find_vpid(nr), PIDTYPE_PID)

class HWMonitor {
	public:
		int get_cpu_use(int pid, unsigned long *cpu_use);
		double get_throttle();
		void set_throttle(double new_throttle);

	private:
		std::mutex throttle_lock;
		double throttle;
};

#endif