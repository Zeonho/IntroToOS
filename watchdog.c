#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int cpid;
void main ( int argc, char *argv[]) { int sec = 10;
int status;
void onTimeout( int );
char *progname;
progname = argv[0];
if ( argc > 1 && argv[1][0] == '-' ) {
sec = atoi(++(argv[1])); printf("Timeout is set to %d\n", sec); argc--;
argv++; }
if (argc < 2 ) {
printf("Usage: %s [-10] command", progname); exit(1);
}
progname = argv[0];
if ( argc > 1 && argv[1][0] == '-' ) {
sec = atoi(++(argv[1])); printf("Timeout is set to %d\n", sec); argc--;
argv++; }
if (argc < 2 ) {
printf("Usage: %s [-10] command", progname); exit(1);
}
if ( WIFEXITED(status) ) {
printf("Program %s exited with status %d\n", argv[1], WEXITSTATUS(status));
exit (WEXITSTATUS(status)); }
}
void onTimeout( int s ) { kill(cpid, SIGKILL);
}
