#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE  256

void  main (int argc, char * argv[])  {
char  buffer[BUFSIZE];
ssize_t  count;
mode_t   perms;
int		 fdin,  fdout;

    perms   = 0777;

    if  ( argc !=3 ) {
        printf( "Incorrect use: Usage: %s  source-file-name  target-copy-name\n",  argv[0] );
        exit( 1 );
    } 

    
}
