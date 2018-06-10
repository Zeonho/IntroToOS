#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char* argv[]) {

  if ( argc != 2) {
     printf( "Usage: %s Directory-Path-Name\n ", argv[0] );
     exit(1);
  }

}
