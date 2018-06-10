/*                          Introduction to Operating Systems
                            CSCI 4061
This program demonstrates the use of stat system call to sum up the file sizes in a directory inside the current directory.
Does not go recursively into sub-directories but sums up the sizes of only "files"*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define NAMESIZE 256
#define DIRSIZE 256
#define THREADSIZE 20

// pthread
#include <pthread.h>
pthread_t tid[THREADSIZE];
pthread_mutex_t mutex1;
typedef struct dirName{char *dirname;}dirName;


// Global Varibles
int scanDirIndex = 1;
char dirArray[DIRSIZE][NAMESIZE];
int totalsum = 0;
// Calculating size
void fileSizePath(char* dirname){
        // init
        struct stat statbuf;
        DIR *dp;
        struct dirent *direntry;

        // Get info From structure


        /* Check if the directory entered exists or not*/
        stat(dirname,&statbuf);
        if(!(S_ISDIR(statbuf.st_mode))){
            printf("The directory name is not valid. Directory does not exist\n");
            exit(0);
        }

        if((dp=opendir(dirname))==NULL){
            perror("Error while opening the directory");
            exit(0);
        }
        /* Loop through the directory structure */
        chdir(dirname); //previously missing
        while( (direntry = readdir(dp)) != NULL )
        {
        	stat(direntry->d_name,&statbuf);
        	if(!(S_ISDIR(statbuf.st_mode)))
        	{
        	    printf("The size of file %s in %s is :%d bytes\n",direntry->d_name, dirname, (int) statbuf.st_size);
                // pthread_mutex_lock(&mutex1);
        	    totalsum += (int) statbuf.st_size;
                // pthread_mutex_unlock(&mutex1);
        	}
        }

        // printf("The total size of the files in %s is : %d bytes\n",dirname,totalsum);
}


// Calculating size
void *fileSize(void *arg){
        pthread_mutex_lock(&mutex1);
        // init
        struct stat statbuf;
        DIR *dp;
        struct dirent *direntry;

        // Get info From structure
        char *dirpath = ((dirName*)arg)->dirname;

        /* Check if the directory entered exists or not*/
        stat(dirpath,&statbuf);
        if(!(S_ISDIR(statbuf.st_mode))){
            printf("The directory name is not valid. Directory does not exist\n");
            exit(0);
        }

        if((dp=opendir(dirpath))==NULL){
            perror("Error while opening the directory");
            exit(0);
        }
        /* Loop through the directory structure */
        chdir(dirpath); //previously missing
        while( (direntry = readdir(dp)) != NULL )
        {
        	stat(direntry->d_name,&statbuf);
        	if(!(S_ISDIR(statbuf.st_mode)))
        	{
        	    printf("The size of file %s in %s is :%d bytes\n",direntry->d_name, dirpath, (int) statbuf.st_size);
        	    totalsum += (int) statbuf.st_size;

        	}
        }
        pthread_mutex_unlock(&mutex1);
        // printf("The total size of the files in %s is : %d bytes\n",dirname,totalsum);
        return NULL;
}

// Scan dir and get all dir into dirArray
void scanDir(char *dirname){
    // init
    struct stat statbuf;
    DIR *dp;
    struct dirent *direntry;
    char *current_dir, *sub_dirpath;


    // allocate memory space for string
	current_dir = (char *) malloc(NAMESIZE * sizeof(char));
	sub_dirpath = (char *) malloc(NAMESIZE * sizeof(char));

    /* Check if the directory entered exists or not*/
    stat(dirname,&statbuf);
    if(!(S_ISDIR(statbuf.st_mode))){
        printf("The directory name is not valid. Directory does not exist\n");
        exit(0);
    }

    if((dp=opendir(dirname))==NULL){
        perror("Error while opening the directory");
        exit(0);
    }

    chdir(dirname);
	getcwd(current_dir, NAMESIZE);

    /* Loop through the directory structure */
    // chdir(dirname); //previously missing
    while( (direntry = readdir(dp)) != NULL )
    {
        stat(direntry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode))
        {
            //  ignore directory entries with  le names “.” and “..”.
			if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0 ){
                // Form absolute path:
                strcpy(sub_dirpath,current_dir);
                strcat(sub_dirpath,"/");
                strcat(sub_dirpath,direntry->d_name);

                // put it in dirArray
                sprintf(dirArray[scanDirIndex],"%s",sub_dirpath);
                scanDirIndex++;
                scanDir(sub_dirpath);
                chdir(current_dir);
            }

        }
    }

    // printf("The total size of the files in %s is : %d bytes\n",dirname,totalsum);
}

int main(int argc, char *argv[])
{
    /* Declarations */
    char *dirname;
    dirname = (char*)malloc(NAMESIZE*sizeof(char));

    /* End of declarations */

    // Prompt Arguments
    if(argc < 2)       /*If the user does not enter any directory name*/
    {
	printf("No directory name specified. Executing the function in the current directory.\n");
	dirname = getcwd(dirname,NAMESIZE);
    }
    else              /* If the user enters a directory name */
    {
	dirname = getcwd(dirname,NAMESIZE);
        strcat(dirname,"/");
        strcat(dirname,argv[1]);
    }

    int i;
    dirName name;



    for (i = 0; i < DIRSIZE; i++){
        strcpy(dirArray[i],"");
    }
    sprintf(dirArray[0],"%s",dirname);
    scanDir(dirname);


    for (int i = 0; i < scanDirIndex ; i++){
        // sprintf(name.dirname,"%s",dirArray[i]);
        name.dirname = dirArray[i];
        printf("Thread [%d] Created, path:%s\n", i, name.dirname);
        pthread_create(&tid[i] , NULL, fileSize,(void*)&name);
    }

    for (int j = 0; j < scanDirIndex; j++)
        pthread_join(tid[j] , NULL);

    // for (i = 0; i < scanDirIndex ; i++){
    //     // sprintf(name.dirname,"%s",dirArray[i]);
    //     fileSizePath(dirArray[i]);
    // }



    // DEBUG Print dir Array
    // for (i = 0; i < scanDirIndex; i++){
    //     printf("dirArray[%d]%s\n", i, dirArray[i]);
    // }
    printf("The total size of the files in %s is : %d bytes\n",dirname,totalsum);
}
