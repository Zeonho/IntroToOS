#include <unistd.h>
#include <sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>

void sig_handler(int signo)
{
  if (signo == SIGINT)
    printf("received SIGINT and exiting\n");
	exit(1);
}

void main ( void ) {

    int i;
    int status;
	pid_t waitreturn;
    pid_t first_childpid;
	pid_t second_childpid;
	
	signal(SIGINT, sig_handler);
	
	//Parent creating first child process
	first_childpid = fork();
	
	if( first_childpid == 0) { // Inside the first child process
		
		printf("Child 1: Sleeping for 10000 seconds\n");
		sleep(10000); //Kind of sleep indefinately
		
	} else { //Inside the parent
		
		printf("Parent: Created child 1 with process ID %d \n", first_childpid);
		second_childpid = fork();
		
		if( second_childpid == 0) {  // Inside the second child process
			
			printf("Child 2: Sleeping for 5 seconds\n");
			sleep(5); // Sleep for 5 seconds
			printf("Child 2: Woke up and exit with status 3\n");
			exit(3);
		
		} else { //Inside the parent
				
			printf("Parent: Created child 2 with process ID %d\n", second_childpid);
		}
		
	}
	
	waitreturn = wait(&status);
	if( WEXITSTATUS(status) != 0 ) {
		
		printf("Parent: Received exit status %d \n", WEXITSTATUS(status));
		printf("Parent: Sending kill signal to first child\n");
		kill(first_childpid, SIGINT);
		printf("Parent: Sending kill signal to second child\n");
		kill(second_childpid, SIGINT);
	}
	
    return;
}

