/*Client.c --- Demonstrates the core functions of a simple client.
 *               - compile/run on linux
 *               - Note: some code borrowed from web, source unknown
 **********************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA "Client says: Hello Server, Client here!"
#define SERVER_PORT 6000
#define BUFFER_SIZE 1024

/* prototypes */
void die(const char *);
void pdie(const char *);

/**********************************************************************
 * main
 **********************************************************************/

int main(int argc, char *argv[]) {

  int sock;                    /* fd for socket connection */
  struct sockaddr_in server;   /* Socket info. for server */
  char buf[BUFFER_SIZE];       /* Received data buffer */
  int i;                       /* loop counter */

  if (argc != 2) {
    die("Usage: client ip_address\n");
  }
  
  /* Create socket */
  sock = socket(AF_INET, SOCK_STREAM, 0);
   
  /* Fill out sockaddr struct */
  server.sin_family = AF_INET;
  inet_aton(argv[1],&server.sin_addr);
  server.sin_port = htons((u_short) SERVER_PORT);
  
  /* Try to connect */
  if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0){
    pdie("Connecting stream socket");
  }

  /* Write out message. */
  if (send(sock, DATA, sizeof(DATA),0) < 0) {
    pdie("Writing on stream socket");
  }

  /* Prepare our buffer for a read and then read. */
  if (recv(sock, buf, BUFFER_SIZE,0) < 0){
    pdie("Reading stream message");
  }
  
  printf("Client: Rec'd msg: %s\n", buf);
  
  /* Close this connection. */
  close(sock);

  exit(0);

}


/**********************************************************************
 * pdie --- Call perror() to figure out what's going on and die.
 **********************************************************************/

void pdie(const char *msg) {

  perror(msg);
  exit(1);
}


/**********************************************************************
 * die --- Print a message and die.
 **********************************************************************/

void die(const char *msg) {

  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}





