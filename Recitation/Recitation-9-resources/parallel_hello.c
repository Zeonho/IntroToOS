#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS     10

/* ===================================
 * Our own thread.
 * ===================================*/

void *PrintHello (void *threadid)
{
  int tid;
  tid = (int) threadid;
  printf ("Hello World! (Thread #%d)\n", tid);
  pthread_exit (NULL);
}

/* ============================
 * Function main as coordinator
 * ============================*/

int
main (int argc, char *argv[])
{
  pthread_t threads[NUM_THREADS];
  int rc, t;
  for (t = 0; t < NUM_THREADS; t++)
    {
      int thread_id = t;
      printf ("In main: creating thread %d\n", t);
      rc = pthread_create (&threads[t], NULL, PrintHello, (void *) thread_id);
      if (rc)
	{
	  printf ("ERROR; return code from pthread_create() is %d\n", rc);
	  exit (-1);
	}
    }
}
