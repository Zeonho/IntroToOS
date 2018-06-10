/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>	
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10     // how many pending connections queue will hold

struct socket_context
{
    int sock_fd;
    struct sockaddr_storage sock_addr;
};
typedef struct socket_context client_sock_context; 

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void *handle_request(void * arg){
    client_sock_context *client_sock;
    client_sock =(client_sock_context*)arg;
    char s[INET6_ADDRSTRLEN];
    inet_ntop(client_sock->sock_addr.ss_family,
        get_in_addr((struct sockaddr *)&client_sock->sock_addr), 
        s, sizeof s);
    
    printf("%d: got connection from %s\n", pthread_self(),s);
    if (send(client_sock->sock_fd, "Hello, world!", 13, 0) == -1)
        perror("send");
    
    close(client_sock->sock_fd);
    free(client_sock);
    pthread_exit(0);
}

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    int rv;
    fd_set rfds;
    struct timeval tv;
    int retval;

    pthread_t th;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
	FD_ZERO(&rfds);
	FD_SET(sockfd,&rfds);
	tv.tv_sec=5;
	tv.tv_usec=0;
	
	retval = select(sockfd+1,&rfds,NULL,NULL,&tv);
	
	if(retval){
		printf("New connection...\n");
	        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    		
    		if (new_fd == -1) {
        	    perror("accept");
            	    continue;
        	}

		client_sock_context* client_sock = (client_sock_context*) malloc(sizeof(client_sock_context));
		client_sock->sock_fd = new_fd;
		client_sock->sock_addr = their_addr;
		
		//create a new thread to handle this request
		//logic for child is present in handle_request() function defined above
		pthread_create(&th,NULL,handle_request,client_sock);
    	}
    	else{
		printf("No connection request...timeout..set timer again..\n");
	}
    }
    return 0;
}