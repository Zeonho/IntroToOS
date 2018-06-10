/**********************************************************************
 * struct_server.c --- Demonstrate the core workings of a simple server
 *              - compile/run on linux
 *              - note that server does not exit on its own, you 
 *                must kill it yourself.
 *              - Do NOT leave this running on itlabs machines
 *              - Note: some code borrowed from web, source unknown
 **********************************************************************/


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TRUE 1
#define SERVER_PORT 6000
#define BUFFER_SIZE 1024


/* prototypes */
void die(const char *);
void pdie(const char *);
					
struct request_type {
  char req_command[16];
  int first_operand;
  int second_operand;
  int result;
};

/**********************************************************************
 * main
 **********************************************************************/

int main(void) {

  int sock;                    /* fd for main socket */
  int msgsock;                 /* fd from accept return */
  struct sockaddr_in server;   /* socket struct for server connection */
  struct sockaddr_in client;   /* socket struct for client connection */
  int clientLen;               /* returned length of client from accept() */
  int rval;                    /* return value from read() */


  struct request_type req;
  struct request_type response;

  /* Open a socket */
  /*<Write your code here>*/

  /* Fill out inetaddr struct */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(SERVER_PORT);
  
  /* Bind */
  /*<Write your code here>*/

  printf("Server: Socket has port %hu\n", ntohs(server.sin_port));

  /* Listen w/ max queue of 5 */
  /*<Write your code here>*/

  /* Loop, waiting for client connections. */
  /* This is an interactive server. */
  while (TRUE) {

    clientLen = sizeof(client);
    if ((msgsock = accept(sock, (struct sockaddr *) &client, &clientLen)) == -1) {
      pdie("Accept");
    } else {
     
      do {   
	/* Read from client until it's closed the connection. */

	if ((rval = recv(msgsock, &req, sizeof(req),0)) < 0){
	  pdie("Reading stream message");
	}

	if (rval == 0) {
	  /* Client has closed the connection */
	  fprintf(stderr, "Server: Ending connection\n");
	} else {
	  printf("Server: Rec'd msg:\n");
	  printf("---req_command: %s\n", req.req_command);
	  printf("---first operand: %d\n", req.first_operand);
	  printf("---second_operand: %d\n", req.second_operand);
	  
	  /* Write back to client. */
	  strcpy(response.req_command, req.req_command);
	  
	  /*Perform the required operation from the operands operand based on the req_command*/
	  /*<Write your code here>*/
	  
	  if (send(msgsock, &response, sizeof(response),0) < 0){
	    pdie("Writing on stream socket");
	  }
	}
      } while (rval != 0);
    }   /* else */
    close(msgsock);
  }

  exit(0);

}


/**********************************************************************
 * pdie --- Call perror() to figure out what's going on and die.
 **********************************************************************/

void pdie(const char *mesg) {

  perror(mesg);
  exit(1);
}


/**********************************************************************
 * die --- Print a message and die.
 **********************************************************************/

void die(const char *mesg) {

  fputs(mesg, stderr);
  fputc('\n', stderr);
  exit(1);
}
