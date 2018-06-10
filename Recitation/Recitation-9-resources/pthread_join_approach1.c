#include<pthread.h>
#include <stdio.h>

int sum = 0;

void *thread_function(void *arg)
{
	int i;
    	for (i = 0; i < 100; i++)
	{
		sum = sum + i;
	} 
}

int main()
{
	pthread_t tid;
	pthread_create(&tid,NULL,thread_function,NULL);
	//wait until the thread finish, put your code below

	printf("sum equals to %d\n", sum);
	return 0;
}
