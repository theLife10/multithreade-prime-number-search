// UT El Paso EE 4374 Assignment 3
// Multi-threaded Prime Number Search
// Author: Jesus Garcia
//
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "jgarcia_prime.h"
#include <math.h>
#include <errno.h>

/* Global variables that are shared */
sPRIME_THREAD primeThreadData[MAX_THREADS];
int numThreads;


void *prime_search(void *param)
{
	//this creates the strut
	sPRIME_THREAD *p; 
	p = param;
	//init files f1 and f2
	FILE *f1;
	FILE *f2;
	//opens files f1 and f2
	f1 = fopen("/home/jesusgarcia/jgarcia_prog3/primes1", "w+");
	f2 = fopen("/home/jesusgarcia/jgarcia_prog3/primes2", "w+");

	/*
		p->current starts at low and ends at p->high
		p->current updates
	*/
	for (p->current = p->low; p->current < p->high; p->current++)
	{
		/*
			if test_prime returns 1 meaning it is a prime execute the if-else statement
			inside the block
		*/
		if (test_prime(p->current) == 1)
		{
			/*
				First thread will go throuth the p->low till it hits 2,500,000
				and will print too primes1 file
			*/
			if (p->current <= 2500000)
			{
				fprintf(f1, "%d\n", p->current);
			}
			else
			{
				/*
					After 2,500,000 thread 2 will print to primes2
				*/
				fprintf(f2, "%d\n", p->current);
			}
		}
	}
	//closes f1
	fclose(f1);
	//closes f2
	fclose(f2);
	pthread_exit(0);
}

void *mini_shell(void *param)
{
	int inputSize, i;
	int threadNum;
	char buff[128]; // Character buffer

	while (1)
	{
		// Request a line of input
		write(1, " -> ", 4);
		// Read the line
		inputSize = read(0, buff, 128);
		// Strip off carriage return
		buff[inputSize - 1] = '\0';

		if ((buff[0] >= '1') && (buff[0] <= '9'))
		{
			// An integer was entered
			threadNum = buff[0] - '0';
			if (threadNum <= numThreads)
			{
				printf("Thread %d progress: %d\n", threadNum, primeThreadData[threadNum - 1].current);
			}
			else
			{
				printf("Thread %d does not exist\n", threadNum);
			}
		}
		else
		{
			if (buff[0] == 'a')
			{
				// Print status for all threads
				for (i = 0; i < numThreads; i++)
				{
					printf("Thread %d progress: %d\n", i + 1, primeThreadData[i].current);
				}
			}
			else
			{
				printf("Unrecognized input\n");
			}
		}

		printf("\n");
		fflush(NULL);
	}

	pthread_exit(0);
}

int test_prime(int n)
{
	// 1 is not a prime number so return 0
	if (n < 2)
	{
		return 0;
	}
	// 2 is a prime number return 2
	//if i don't put this statement the primes don't print right
	if (n % 2 == 0)
	{
		return n == 2;
	}
	//optimizing the iteration and getting the sqrt of n and turning it into the limit
	int limit = sqrt(n);
	//will start at 3 finish up to the limit and jumps by 2
	for (int i = 3; i <= limit; i += 2)
	{
		// returns 0 if it is not prime
		if (n % i == 0)
		{
			return 0;
		}
	}
	//this it is a prime
	return 1;
}
