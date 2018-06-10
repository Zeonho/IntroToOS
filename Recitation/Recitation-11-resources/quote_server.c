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


#define DATA1 "Server says:"
#define DATA2 "All your base are belong to us..."
#define TRUE 1
#define SERVER_PORT 6000
#define BUFFER_SIZE 1024


/* prototypes */
void die(const char *);
void pdie(const char *);

char quotes[5][200] = {"We are what we repeatedly do. Excellence, therefore, is not an act but a habit. – Aristotle \n",
					"Do not wait to strike till the iron is hot; but make it hot by striking. – William B. Sprague \n",
					"Great spirits have always encountered violent opposition from mediocre minds. – Albert Einstein \n",
					"You must be the change you want to see in the world. - Gandhi \n",
					"The best way to cheer yourself up is to try to cheer somebody else up. – Mark Twain \n"};

char completeList[1000];

struct request_type {
  char req_command[16];
  char params[1000];
};

/**********************************************************************
 * main
 **********************************************************************/

int main(void) {

  int i=0;
  for(  i=0; i<5; i++){
	  strcat(completeList, quotes[i]);
  }




  int sock;                    /* fd for main socket */
  int msgsock;                 /* fd from accept return */
  struct sockaddr_in server;   /* socket struct for server connection */
  struct sockaddr_in client;   /* socket struct for client connection */
  int clientLen;               /* returned length of client from accept() */
  int rval;                    /* return value from read() */


  struct request_type req;
  struct request_type response;

  /* Open a socket */
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    pdie("Opening stream socket");
  }

  /* Fill out inetaddr struct */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(SERVER_PORT);

  /* Bind */
  if (bind(sock, (struct sockaddr *) &server, sizeof(server))){
    pdie("Binding stream socket");
  }

  printf("Server: Socket has port %hu\n", ntohs(server.sin_port));

  /* Listen w/ max queue of 5 */
  listen(sock, 5);
  /* set "sock" into non-blocking mode */
  set_fl(sock, O_NONBLOCK);

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
	  printf("---params: %s\n", req.params);

	  /* Write back to client. */
	  strcpy(response.req_command, req.req_command);

	  if (strcmp(req.params, "LIST") == 0) {
		  strcpy(response.params, completeList);
	  }
	  else {
		  strcpy(response.params, quotes[rand() % 5]);
	  }

	  //strcpy(response.params, DATA2);
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
