/* Exercise 3.7 Text book Page no. 66 */ 


#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
	pid_t childpid;

        childpid = fork();
	if (childpid == -1)
	{
		perror("fork() failed");
		return 1;
	}
	if (childpid == 0)
		printf("I am a child with id %ld\n", (long)getpid());
	else
		printf("I am a parent with id %ld\n", (long)getpid());
	return 0;
}
