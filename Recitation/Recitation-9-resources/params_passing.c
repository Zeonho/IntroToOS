#include<pthread.h>
#include<string.h>
#include <stdio.h>

typedef struct item
{
    char Parameter1[100];
    int Parameter2;
}item;

void *thread_function(void *arg)
{
    //Deliver two parameter to the thread and print them out
}

int main()
{
    pthread_t tid[5],t;
    int i=0;
    item *allitems,val;

    strcpy(val.Parameter1,"ParamOne");
	val.Parameter2 = 2;
    /* initialize all the items */
    
    printf("Creating the thread...\n");
    pthread_create(&t,NULL,thread_function,(void*)&val);
    pthread_join(t,NULL);
    return 0;
}

