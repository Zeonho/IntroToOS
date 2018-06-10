#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>


int main (int argc,  char* argv[] ) {
DIR   *dpntr;
struct  dirent   *dentry;

   if ( argc > 2 ) {
      printf ("Usage:  %s  [directory-name]\n", argv[0] );
      exit( 1 );
   }

   
}

