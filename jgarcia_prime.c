// UT El Paso EE 4374 Assignment 3
// Multi-threaded Prime Number Search
// Author: ???
//
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "jgarcia_prime.h"

/* Global variables that are shared */
sPRIME_THREAD primeThreadData[MAX_THREADS];
int numThreads;

//comment
//write to files
//concat two files togetther

void *prime_search(void *param)
{
	sPRIME_THREAD *p; //pointer to the structure
	p = param;

	for (p->current = p->low; p->current < p->high; p->current++)
	{
		if (test_prime(p->current) == 1)
		{
			//write to two indivual files
		}
	}
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
	int i;
	for (i = 2; i <= n; ++i)
	{
		if (n % i == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}
