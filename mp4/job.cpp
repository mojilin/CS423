#include "job.h"

void compute(Job job) 
{
	for (int i = 0; i < A_SIZE; i++) 
	{
		for(int j = 0; j < 6000; j++)
		{
			job.A[i] += 1.111111;
		}
	}
}
