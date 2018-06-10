#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>


void main ( void ) {
    int i, stat; int k = 1;
    int n = 3;
    pid_t childpid;
    for ( i=0; i < n; i++ ) {
        printf ( "i = %d k = %d ", i, k);
        childpid = fork();
        if ( childpid != 0 ) { /* I am parent; I just created a child */
            break;
        }else{
            k = k*2;
        }
    } // end of for loop
    // printf("Reach here, Now i == %d\n",i);
    if ( i==n){
        // printf("Reach here, Now exit(k);");
        exit(k);
    }
    else {
        // printf("Reach here, Now wait ( &stat );");
        wait ( &stat );
        if ( WIFEXITED(stat) ) {
            printf ("Exit status %d \n", WEXITSTATUS(stat) );
            printf("exit( k + WEXITSTATUS(stat) ) = %d \n", k + WEXITSTATUS(stat));
            exit( k + WEXITSTATUS(stat) );
        }
    }
    // return 0;
}
