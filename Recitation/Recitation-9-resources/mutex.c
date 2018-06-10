#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;  // Mutex Id
int totalsize = 0;  // A global shared variable

void *thread_function(void *arg)
{
    char *sbName;

    sbName = (char *)arg;
    printf("The value of totalsize before increment from %s = %d\n", sbName, totalsize);
    IncrementX();
    printf("The value of totalsize after increment from %s = %d\n", sbName, totalsize);
}    

void IncrementX()
{
    int Temp; /* local variable */
    FILE *fp;

	//Lock the shared variable when you use thread
	//Put you code here

    Temp = totalsize;
    fp = fopen("output.txt", "a");
    Temp = Temp + 1;
    totalsize = Temp;
    fprintf(fp ,"%d\n",totalsize);
    fclose(fp);
	
	//Release the shared variable when you are done using it
	//Put you code here
}    

main()
{
    pthread_t idA, idB; /* ids of threads */
    void *thread_function(void *);

	//Initial the mutex lock
	//Put your code here

    if (pthread_create(&idA, NULL, thread_function, (void *)"Subdirectory1") != 0) 
    {
        perror("pthread_create");
        exit(1);
    }
    if (pthread_create(&idB, NULL, thread_function, (void *)"Subdirectory2") != 0)  
    {
        perror("pthread_create");
        exit(1);
    }
    
    pthread_join(idA, NULL);
    pthread_join(idB, NULL);
    
    //Destroy the mutex lock
	//Put your code here
}        
