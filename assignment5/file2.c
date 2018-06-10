#define BUFFER_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE **fps;

int readConfig()
{


    // init
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    fps = malloc(sizeof(FILE*) * (10));


    // open config
    fps[2] = fopen("computers.txt", "r");
    if (fps[2] == NULL)
        exit(EXIT_FAILURE);

    // get info from config.txt and stores
    while ((read = getline(&line, &len, fps[2])) != -1) {
        printf("Retrieved line of length %zu :\n", read);
        printf("%s", line);
    }

    fclose(fps[2]);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);


    return 0;
}


// const char * readLine(FILE * fp)
void readLine(FILE * fp)
{
    // FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    if (fp == NULL)
        exit(EXIT_FAILURE);

    if ((read = getline(&line, &len, fp)) != -1) {
        printf("%s", line);

    }
    // return line;
}


int main() {
    readConfig();
    return 0;
}
