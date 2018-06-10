/*
 * ex2.c:  recitation demo
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void *printme()
{
  printf("Hello world. I am %u\n",   (unsigned)  pthread_self());
  return NULL;
}

void main()
{
  pthread_t thread_id;
  void *status;

  if (pthread_create(&thread_id, NULL, printme, NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }
  printf("I am %u. I created %u\n", (unsigned) pthread_self(),  (unsigned) thread_id);
  if (pthread_join(thread_id, &status) != 0) { perror("pthread_join"); exit(1); }

}
