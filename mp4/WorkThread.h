#ifndef WT_H
#define WT_H

typedef struct {
	// Job_t job;
	double throttle;
} arg_t;

void worker_thread(void *arg);

#endif