#define BUFFER_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FILE **fps;
// char fileNames[BUFFER_SIZE][BUFFER_SIZE];
// char filePaths[BUFFER_SIZE][BUFFER_SIZE];
int fileIndex = 0;
//
//
// int readConfig()
// {
//     /* Function:
//                 1.Read Config.txt
//                 2.Stores info into array
//                 3.open all files using filepointer array to stores them(fps)
//     */
//
//     // init
//     FILE * fp;
//     char * line = NULL;
//     size_t len = 0;
//     ssize_t read;
//     fps = malloc(sizeof(FILE*) * (10));
//
//     // strtok
//     char *first;
//     char *second;
//
//     // open config
//     fp = fopen("config.txt", "r");
//     if (fp == NULL)
//         exit(EXIT_FAILURE);
//
//     // get info from config.txt and stores
//     while ((read = getline(&line, &len, fp)) != -1) {
//         // printf("Retrieved line of length %zu :\n", read);
//         // printf("%s", line);
//         first = strtok (line,": \n");
//         second = strtok (NULL,": \n");
//         sprintf(fileNames[fileIndex],"%s",first);
//         sprintf(filePaths[fileIndex],"%s",second);
//         fileIndex++;
//     }
//
//     // open all files
//     for (int i = 0; i < fileIndex; i++){
//         fps[i] = fopen(filePaths[i], "r");
//         if (fps[i] == NULL){
//             printf("Error occur in open file, fps[%d] == NULL\n",i);
//             // exit(EXIT_FAILURE);
//         }
//     }
//
//     fclose(fp);
//     if (line)
//         free(line);
//
//
//     return 0;
// }

char somename[1024];
int getFileNumber(char *openname){
    // fileIndex = 3;

    openname[strcspn(openname, "\n")] = 0;
    printf("%s\n", openname);

    // for (int i = 0; i < fileIndex; i++){
    //     if (strcmp(name, fileNames[i]) == 0)
    //         printf("%d\n", i);
    //         // return i;
    // }
    return 20;
}


// const char * readLine(FILE * fp)
// // void readLine(FILE * fp)
// {
//     // FILE * fp;
//     char * line = NULL;
//     size_t len = 0;
//     ssize_t read;
//
//     if (fp == NULL)
//         exit(EXIT_FAILURE);
//
//     if ((read = getline(&line, &len, fp)) != -1) {
//         // printf("%s", line);
//         return line;
//
//     }
//     return "";
// }


int main() {
    // readConfig();

    // printf("%s", readLine(fps[1]) );
    // printf("%s", readLine(fps[1]) );
    // readLine(fps[1]);
    // fclose(fps[0]);
    strcpy(somename, "Computers\n");
     getFileNumber(somename);
    // printf("getFileNumber:%d\n", getFileNumber("GET: QUOTE CAT: Computers\n"));
    return 0;
}
