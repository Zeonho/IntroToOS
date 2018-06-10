#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include<sys/wait.h>
void main ( void ) {

    int i;
    int n = 4;
    int waitstat;
    pid_t childpid;
    pid_t second_childpid;
    for ( i=0;  i < n; i++ )  {

  	    childpid = fork();
        
        if (i == 1)
          second_childpid = childpid;



	      if ( childpid == 0){
		        /* I just created a child */
		          break;
	  }
    }
    waitpid(second_childpid, &waitstat, 0);
    printf( "Process-ID: %-8ld, Parent-Process-ID: %-8ld\n",  (long)getpid(), (long)getppid() );

}
