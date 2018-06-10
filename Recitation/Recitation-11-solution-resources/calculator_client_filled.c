/**********************************************************************
 * struct_client.c --- Client to connect to quote_server
 *                     - change SERVER_PORT value to that of your server
 **********************************************************************/

/* Requisite includes */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_PORT 6000 /* CHANGE THIS TO THE PORT OF YOUR SERVER */
#define BUFFER_SIZE 1024

struct request_type {
  char req_command[16];
  int first_operand;
  int second_operand;
  int result;
};

/**********************************************************************
 * main
 **********************************************************************/

int main(int argc, char *argv[]) {

  int sock;                    /* fd for socket connection */
  struct sockaddr_in server;   /* Socket info. for server */
  struct hostent *host;        /* Server info */  
  int selection;
  int finished = 0;  

  struct request_type req;
  struct request_type response;
    
  if (argc != 2) {
    printf("Usage: %s address\n", argv[0]);
  }
     
  /* Fill out sockaddr struct */
  server.sin_family = AF_INET;
  if ((host = gethostbyname(argv[1])) == NULL){
    printf("%s: unknown host '%s'\n", argv[0], argv[1]);
    exit(1);
  }
  server.sin_family = host->h_addrtype;
  memcpy((char *) &server.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
  server.sin_port = htons((u_short) SERVER_PORT);
  
  /* Create socket */
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    printf("%s: Unable to open socket\n",argv[0]);
    perror("Aborting");
    exit(1);
  }

  /* Try to connect */
  if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0){
    printf("%s: Unable to connect to %s\n", argv[0], argv[1]);
    perror("Aborting");
    exit(1);
  } else {
    printf("\nConnection to the quote server established\n");
  }

  while (!finished){

    /* Get input from user */
    printf("Options:\n");
    printf("  1. for ADDITION\n");
    printf("  2. for SUBTRACTION\n");
    printf("  3. for MULTIPLICATION\n");
	printf("  4. for DIVISION\n");
	printf("  5. for Exiting\n");
	
	printf("Enter your selection: ");
	scanf("%d",&selection);
	
	if (selection != 5){
		printf("Enter first operands: ");
		scanf("%d",&req.first_operand);
		 
		printf("Enter second operands: ");
		scanf("%d",&req.second_operand);    
	}
	
    printf("\n");
    /* Parse user's selection */
    if (selection == 5){
      sprintf(req.req_command,"EXIT");
      finished = 1;
    } else if (selection == 1) {
      sprintf(req.req_command,"ADD");
    } else if (selection == 2) {
       sprintf(req.req_command,"SUBTRACT");
    } else if (selection == 3){
       sprintf(req.req_command,"MULTIPLY");
    } else if (selection == 4){
       sprintf(req.req_command,"DIVIDE");
    }
    
    /* Send user's request to server. */
    if (send(sock, &req, sizeof(req),0) < 0) {
      printf("%s: Error writing on stream socket", argv[0]);
      perror("Aborting");
      close(sock);
      exit(1);
    }
    
    if (!finished){
      /* Get response from server */
      if (recv(sock, &response, sizeof(response),0) < 0){
		printf("%s: Error reading from stream socket", argv[0]);
		perror("Aborting");
		close(sock);
		exit(1);
      }
      
      /* Print response */
      printf("Response from server:\n");
      printf("---Result: %d\n\n",response.result);
    }

  } /* end while(!finished) */
  
  close(sock);
  printf("Connection to server terminated. Exiting...\n\n");
  exit(0);
 
} /* End of main() */

/*********************************************************************/





