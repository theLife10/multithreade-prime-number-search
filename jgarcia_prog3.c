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

  //Setted up parameters
  primeThreadData[0].num = 0;        //thread 1
  primeThreadData[0].low = 1;        //start
  primeThreadData[0].high = 2500000; //choose 2,500,000 since it is half of 5,000,000

  primeThreadData[1].num = 1;        //thread 2
  primeThreadData[1].low = 2500000;  //start at the thresh hold
  primeThreadData[1].high = 5000000; //end at 5 million
  /* Setup a mini shell thread to provide interactivity with the user */

  //this is creating the prethreads
  pthread_create(&tidshell, &attr, mini_shell, NULL);                  //passed thread id for shell, address of attribute , minishell method
  pthread_create(&tid[0], &attr, prime_search, &(primeThreadData[0])); //address of thread 1, attribute, prime_search method,and address
  pthread_create(&tid[1], &attr, prime_search, &(primeThreadData[1])); //address of thread 2, attribute, prime_search method, and address 2

  // this concats the files and deleted the #if 0
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

  sleep(20);

  /* Lastly, kill the interaction thread */
  pthread_kill(tidshell, SIGKILL);
  pthread_kill(tid[0], SIGKILL);
  pthread_kill(tid[1], SIGKILL);

  return 0;
}
