#ifndef HWM_H
#define HWM_H

double get_cpu_use();


typedef struct
{
	int num_jobs;
	float throttle;
	double cpu_use;
} node_state;

#endif
