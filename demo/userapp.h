#ifndef __USERAPP_H__
#define __USERAPP_H__

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

long long fib(int n);
int factorial(int n);

long long fib(int n)
{
	if(n ==0)
		return 0;
	if( n == 1)
		return 1;

	return fib(n-1) + fib(n-2);
}

int factorial(int n)
{
	if(n == 0)
		return 1;
	return n * factorial(n-1);
}


#endif

