// Jinhao Chen
// chen4566

// Code form Recitations
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


#define TRUE 1
#define SERVER_PORT 6000
#define BUFFER_SIZE 1024


/* prototypes */
void die(const char *);
void pdie(const char *);

char completeList[1000];

// pthread
typedef struct s
{
    int socket;
}s;


// Helper functions for reading files
FILE **fps;
char fileNames[BUFFER_SIZE][BUFFER_SIZE];
char filePaths[BUFFER_SIZE][BUFFER_SIZE];
int fileIndex = 0;
int readConfig()
{
/* Function:
            1.Read Config.txt
            2.Stores info into array
            3.open all files using filepointer array to stores them(fps)
*/

// init
FILE * fp;
char * line = NULL;
size_t len = 0;
ssize_t read;
fps = malloc(sizeof(FILE*) * (10));

// strtok
char *first;
char *second;

// open config
fp = fopen("config.txt", "r");
if (fp == NULL){
    printf("Missing Config File\n");
    exit(EXIT_FAILURE);
}

// get info from config.txt and stores
while ((read = getline(&line, &len, fp)) != -1) {
    // printf("Retrieved line of length %zu :\n", read);
    // printf("%s", line);
    first = strtok (line,": \n");
    second = strtok (NULL,": \n");
    sprintf(fileNames[fileIndex],"%s",first);
    sprintf(filePaths[fileIndex],"%s",second);
    fileIndex++;
}

// open all files
for (int i = 0; i < fileIndex; i++){
    fps[i] = fopen(filePaths[i], "r");
    if (fps[i] == NULL){
        printf("Error occur in open file, fps[%d] == NULL\n",i);
        // exit(EXIT_FAILURE);
    }
}

fclose(fp);
if (line)
    free(line);


return 0;
}


const char * readLine(FILE * fp)
// void readLine(FILE * fp)
{
// FILE * fp;
char * line = NULL;
size_t len = 0;
ssize_t read;

if (fp == NULL)
    exit(EXIT_FAILURE);

if ((read = getline(&line, &len, fp)) != -1) {
    // printf("%s", line);
    return line;

}
return "line";
}


int getFileNumber(char * selectionName){
selectionName[strcspn(selectionName, "\n")] = 0;
for (int i = 0; i < fileIndex; i++){
    // printf("%s,%s", selectionName,fileNames[i]);
    if (strcmp(selectionName, fileNames[i]) == 0)
        return i;
}
return -1;
}


void *clientHandler(void *sock){
    int msgsock = ((s*)sock)->socket;
    char selection[BUFFER_SIZE]; /* User's input */
    char request[BUFFER_SIZE];   /* String sent to server */
    char response[BUFFER_SIZE];  /* String rec'd from server */
    int rval;                    /* return value from read() */
    memset((void *)selection, '\0', BUFFER_SIZE);
    memset((void *)request, '\0', BUFFER_SIZE);
    memset((void *)response, '\0', BUFFER_SIZE);
    do{
        /* Read from client until it's closed the connection. */
        if ((rval = recv(msgsock, &request, sizeof(request),0)) < 0){
            pdie("Reading stream message");
        }
        if (rval == 0) {
            /* Client has closed the connection */
            fprintf(stderr, "Server: Ending connection\n");
        } else {
            printf("Server: Received msg:\n");
            printf("---reqest: %s\n", request);
            // printf("---params: %s\n", request);

            /* Write back to client. */
            strcpy(response, request);
            strcat(response, "\n");

            int randomIndex;
            int selectIndex;

            if (strcmp(request, "GET: LIST\n") == 0) {
                // strcpy(response, "GET: LIST\n\n");
                strcat(response, completeList);
            }
            else if (strcmp(request, "GET: QUOTE CAT: ANY\n") == 0){
                randomIndex = rand() % fileIndex;
                strcat(response, readLine(fps[randomIndex]));
                strcat(response, readLine(fps[randomIndex]));
            }
            else {/*Get from specific file*/
                // strcpy(response, "GET: QUOTE CAT: %s\n);
                sprintf(selection,"%.*s", 10, request + 16);
                selectIndex = getFileNumber(selection);
                // printf("selectIndex: %d\n", selectIndex);
                if (selectIndex != -1){
                    strcat(response, selection);
                    strcat(response, readLine(fps[selectIndex]));
                    strcat(response, readLine(fps[selectIndex]));
                }else{
                    strcat(response, "Category Not Found");
                }
            }

            if (send(msgsock, &response, sizeof(response),0) < 0){
                pdie("Writing on stream socket");
            }

        }

    } while (rval != 0);

    }

// #include <fcntl.h>
// void set_fl ( int  fd,  int flags ) {
//     int val;
//     if  ( (val =  fcntl(fd, F_GETFL, 0)) < 0 ) {
//         perror("fnctl getfl error");
//     }
//
//     val = val | flags;
//     if  (val =  fcntl(fd, F_SETFL, val) < 0 ) {
//         perror("fnctl setfl error");
//     }
// }


/**********************************************************************
* main
**********************************************************************/

int main(void) {

    readConfig();
    pthread_t thread_id;
    s socketnumber;

    int i=0;
    for(  i=0; i<5; i++){
        strcat(completeList, fileNames[i]);
        strcat(completeList, "\n");
    }

    int sock;                    /* fd for main socket */
    int msgsock;                 /* fd from accept return */
    struct sockaddr_in server;   /* socket struct for server connection */
    struct sockaddr_in client;   /* socket struct for client connection */
    int clientLen;               /* returned length of client from accept() */

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
    // set_fl(sock, O_NONBLOCK);

    /* Loop, waiting for client connections. */
    /* This is an interactive server. */
    while (TRUE) {
        // memset((void *)selection, '\0', BUFFER_SIZE);
        // memset((void *)request, '\0', BUFFER_SIZE);
        // memset((void *)response, '\0', BUFFER_SIZE);
        clientLen = sizeof(client);
        if ((msgsock = accept(sock, (struct sockaddr *) &client, &clientLen)) == -1) {
            pdie("Accept");
        } else {
            // Handle message
            socketnumber.socket = msgsock;
            if( pthread_create( &thread_id , NULL ,  clientHandler , (void*) &msgsock) < 0)
            {
                perror("could not create thread");
                return 1;
            }

            // pthread_create(&thread_id, NULL,clientHandler, (void*) &socketnumber);

        }/* else */
        // close(msgsock);
    }//while

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
