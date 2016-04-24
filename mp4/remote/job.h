#ifdef JOB_H
#define JOB_H

#define A_SIZE 1024 * 1024 * 4

typedef struct {
	int id;
	double A[A_SIZE] = {1.111111};
} Job;

void compute();


#endif