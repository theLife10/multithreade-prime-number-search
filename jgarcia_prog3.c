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

#define BUFFER_SIZE 1000000

unsigned char buffer[BUFFER_SIZE + 1];
unsigned char fileName[100];
FILE *primeFile;
FILE *primeThreadFile;

int main(int argc, char *argv[])
{
  int i, bytesRead, bytesWritten;
  pthread_t tid[MAX_THREADS];
  pthread_t tidshell;
  pthread_attr_t attr;
  time_t before, after;

  /* Record time at start */
  before = time(NULL);

  /* Setup threads to find prime numbers */
  pthread_attr_init(&attr);
  numThreads = 2;

  //put it here
  //learn how to offload both threads do the same amount of work
  //
  primeThreadData[0].num = 0;
  primeThreadData[0].low = 1;
  primeThreadData[0].high = 2500000; //thresh hold

  primeThreadData[1].num = 1;
  primeThreadData[1].low = 2500000;  //thresh hold
  primeThreadData[1].high = 5000000; //40
  /* Setup a mini shell thread to provide interactivity with the user */
  pthread_create(&tidshell, &attr, mini_shell, NULL);
  pthread_create(&tid[0], &attr, prime_search, &(primeThreadData[0]));
  pthread_create(&tid[1], &attr, prime_search, &(primeThreadData[1]));
  //#if 0

  /* Create primes output file */
  primeFile = fopen("primest", "w");
  fclose(primeFile);

  /* Wait for the prime search threads to complete and combine their data */
  for (i = 0; i < numThreads; i++)
  {
    /* Wait for the next thread to complete */
    pthread_join(tid[i], NULL);
    /* On thread completion, write its data to "primest" */
    fileName[0] = '\0';
    sprintf(fileName, "primes%d", i + 1); // Open the thread's data file
    if ((primeThreadFile = fopen(fileName, "r")) == NULL)
    {
      printf("Failed to open file: %s\n", fileName);
    }
    else
    {
      if ((primeFile = fopen("primest", "a")) == NULL) // Open "primest"
      {
        printf("Failed to open file: primest\n");
      }
      else
      {
        while (feof(primeThreadFile) == 0)
        {
          /* Read from the thread's data file */
          bytesRead = fread(buffer, 1, BUFFER_SIZE, primeThreadFile);
          /* Write to the combined file */
          bytesWritten = fwrite(buffer, 1, bytesRead, primeFile);
        }
        fclose(primeFile);
      }
      fclose(primeThreadFile);
    }
  }

  /* Record execution time */
  after = time(NULL);
  printf("\nPrime search done after %ld seconds\n", after - before);
  //#endif

  sleep(20);

  /* Lastly, kill the interaction thread */
  pthread_kill(tidshell, SIGKILL);
  pthread_kill(tid[0], SIGKILL);
  pthread_kill(tid[1], SIGKILL);

  return 0;
}
