#ifndef JQ_H
#define JQ_H

#include "job.h"
#define QUEUE_MAX ((A_SIZE)/2) 

int enqueue(Job_t job);
Job_t dequeue();
int isEmpty();
int isFull();
void clear();
int get_queue_size();

#endif
