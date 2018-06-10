#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
int main(int argc, char const *argv[]) {
    int fdA[2];
    int fdB[2];
    pipe( fdA );
    pipe( fdB );
    if ( fork() ) {
        // close(fdB[0]);
        // close((fdB[1]));
        /* ..indicates that standard output redirect to a pipe.. */
        dup2(fdA[1], STDOUT_FILENO);
        /* .. indicates that standard input comes from a file .. */
        dup2(fdB[0], 0);
    } else {

    }
    return 0;
}
