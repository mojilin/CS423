#ifndef JOB_H
#define JOB_H

#define A_SIZE 1024 * 1024 * 4

typedef struct {
	int id; // this doubles as the index for the thread to operate on
	double data;
} Job_t;

void compute(Job_t j);


#endif
