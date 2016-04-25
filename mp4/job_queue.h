#ifndef JQ_H
#define JQ_H

#define QUEUE_MAX 512

int enqueue(Job_t job);
Job_t dequeue();
int isEmpty();
int isFull();
void clear();

#endif