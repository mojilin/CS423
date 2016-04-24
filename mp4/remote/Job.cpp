#include "job.h"

Job::Job() 
{
	// init everything to 1.111111
	for (int i = 0; i < A_SIZE; i++) 
	{
		A[i] = 1.111111;
	}
}

void Job::compute() 
{
	for (int i = 0; i < A_SIZE; i++) 
	{
		for(int j = 0; j < 6000; j++)
		{
			A[i] += 1.111111;
		}
	}
}