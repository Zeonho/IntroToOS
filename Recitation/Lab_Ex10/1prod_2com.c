#include<pthread.h>
#include<stdio.h>
#include<string.h>

#define NUMTHREAD 3      /* number of threads */

const int N = 5;
int Buffer[5];
int in = 0;
int out = 0;
int count = 0;

pthread_mutex_t  lock;
pthread_cond_t  SpaceAvailable, ItemAvailable;
int thread_id[NUMTHREAD]  = {0,1,2};
/* define the type to be pthread */
pthread_t thread[NUMTHREAD];

void * producer (int *id)
{ int i;

  for ( i = 0; i< 50; i++) {

      pthread_mutex_lock ( &lock);  /* Enter critical section  */  

      while ( count == N )          /* Make sure that buffer is NOT full   */
         while (  pthread_cond_wait( &SpaceAvailable, &lock) != 0 ) ; /* Sleep using a cond variable */
       /*  printf( "Producer %d \n", i);  */

       /* count must be less than N    */

       Buffer[in] = i;             /* Put item in the buffer using "in"  */
       in = (in + 1) % N;
       count++;                    /* Increment the count of items in the buffer */
       printf("Deposited item = %d\n", i);

       pthread_mutex_unlock ( &lock);  

       pthread_cond_signal( &ItemAvailable ); /* Wakeup consumer, if waiting */
 }


  /* Put -1 in the buffer to indicate finish to the consumer  */  
  pthread_mutex_lock ( &lock);
  while ( count == N )
       while (  pthread_cond_wait( &SpaceAvailable, &lock) != 0 ) ;

  Buffer[in] = -1;
  in = (in + 1) % N;
  count++;

  pthread_mutex_unlock ( &lock );
  pthread_cond_signal( &ItemAvailable ); /* Wakeup consumer, if waiting */
}

void * consumer (int *id)
{ int i;

   i = 0;
   do {

         pthread_mutex_lock ( &lock);    /* Enter critical section  */
         while ( count == 0 )     /* Make sure that buffer is NOT empty */
             while (  pthread_cond_wait( &ItemAvailable, &lock) != 0 ) ; /* Sleep using a cond variable *

         /* count must be > 0    */
         if  ( count > 0 ) {
               i = Buffer[out] ;  /* Remove item form the buffer using "out"  */
               out = (out + 1) % N; 
               count--;           /* Decrement the count of items in the buffer */
               printf( "Thread %d Removed %d \n", id, i);
	       if(i==-1){
		printf("END LINE \n");
			pthread_cancel(thread[0]);
			if(id==1){
			pthread_cancel(thread[2]);
			pthread_cancel(thread[1]);
			}else{
			pthread_cancel(thread[1]);
			pthread_cancel(thread[2]);
			}		
		
		//exit(1);		
		}
         }
         pthread_mutex_unlock ( &lock);   /* exit critical seciton  */

         pthread_cond_signal( &SpaceAvailable );  /* Wakeup prodcuer, if waiting */
        //sleep to allow 2 consumers to compute
 	if (rand()%100 < 30)
	  sleep(rand()%3);

   } while ( i != -1 );
}


int main(void)
{
  int i;
  
  pthread_attr_t attr;              /*attribute object*/
  int n;

  pthread_mutex_init( &lock, NULL);
  pthread_cond_init( &SpaceAvailable, NULL);
  pthread_cond_init( &ItemAvailable, NULL);

  pthread_create(&thread[0], NULL, (void *)producer, &thread_id[0]);
  pthread_create(&thread[1], NULL, (void *)consumer, &thread_id[1]);
  pthread_create(&thread[2], NULL, (void *)consumer, &thread_id[2]);
  
  for(i=0; i< NUMTHREAD ; i++)
    {
      pthread_join(thread[i], NULL);
    }
  printf("Finished execution \n" );

}



