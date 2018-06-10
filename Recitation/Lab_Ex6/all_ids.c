#include<stdio.h>
#include<unistd.h>
int main()
{
	printf("I am a process with an id %ld\n", (long)getpid());
	printf("The Id of my parent is %ld\n", (long)getppid());
	printf("My real user id is %ld\n", (long)getuid());
	printf("My effective user id is %ld\n", (long)geteuid());
	printf("My real group id is %ld\n", (long)getgid());
	printf("My effective group id is %ld\n", (long)getegid());
	
}
