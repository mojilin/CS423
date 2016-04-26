#ifndef JQ_H
#define JQ_H

#include "job.h"
#define QUEUE_MAX 1024

int enqueue(Job_t job);
Job_t dequeue();
int isEmpty();
int isFull();
void clear();
int get_queue_size();

#endif
