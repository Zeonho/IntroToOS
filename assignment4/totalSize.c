// chen4566
// Jinhao Chen



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include <pthread.h>

#define NAMESIZE 256
#define THREADNUMBER 20

// DEBUG SET
#define DEBUG 1
#define DEBUGDIR 1

// Globalize
int totalsum = 0;
pthread_t tid[THREADNUMBER],t;
int i = 0;


typedef struct dirInfo
{
    char *dirpath;
    struct stat statbuf;
}dirInfo;




void *dirStat(void *arg){

	// init
	DIR *dp;
	struct dirent *direntry;
	char *current_dir, *sub_dirpath;

    // allocate memory space for string
	current_dir = (char *) malloc(NAMESIZE * sizeof(char));
	sub_dirpath = (char *) malloc(NAMESIZE * sizeof(char));

    dirInfo sub_val;

    // Get info From structure
    char *dirpath = ((dirInfo*)arg)->dirpath;
    struct stat statbuf = ((dirInfo*)arg)->statbuf;



	if((dp=opendir(dirpath))==NULL){
            printf("%s ",dirpath);
			perror("Error while opening the directory");
			exit(0);
	}


	/* Loop through the directory structure */
	chdir(dirpath);
	getcwd(current_dir, NAMESIZE*sizeof(char));


	// DEBUG
	// printf("Current Directory is: %s\n", current_dir);
	// printf ("Current Directory is : %s \n",dirpath);
    int partial_sum;
    partial_sum = 0;
	while( (direntry = readdir(dp)) != NULL )
	{
		stat(direntry->d_name,&statbuf);

		// If it is a directory, call dirStat on sub directory recursively
		if (S_ISDIR(statbuf.st_mode))
		{
			//  ignore directory entries with  le names “.” and “..”.
			if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0 ){


				// Form absolute path:
				strcpy(sub_dirpath,current_dir);
				strcat(sub_dirpath,"/");
				strcat(sub_dirpath,direntry->d_name);

                sub_val.dirpath = sub_dirpath;
                sub_val.statbuf = statbuf;

				pthread_create(&t,NULL,dirStat,(void*)&sub_val);
                // printf("Thread Created i: %d\n",i );
                i++;
                pthread_join(t,NULL);


                // DEBUG
                if (DEBUG){
				    printf("DEBUG: Dir file: %s, partial_sum: %d\n", sub_dirpath, partial_sum);
                }

				// Changing the Directory back after goes into recursive call
				chdir(dirpath);

			}
		}
		else{
			// DEBUG
            if(DEBUG){
                // printf("DEBUG The size of file %s/%s is :%d bytes\n",current_dir,direntry->d_name,(int) statbuf.st_size);
				// printf("Total sum: %d\n", *totalsum );
            }
            partial_sum += (int) statbuf.st_size;
    	    totalsum += (int) statbuf.st_size;

		}
	}



    return NULL;

}



int main(int argc, char *argv[])
{
    // Stat Part
    /* Declarations */
    char *dirname;
    struct stat statbuf;
    dirname = (char*)malloc(NAMESIZE*sizeof(char));

    /* End of declarations */

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
    /* Check if the directory entered exists or not*/
    stat(dirname,&statbuf);
    if(!(S_ISDIR(statbuf.st_mode))){
        printf("The directory name is not valid. Directory does not exist\n");
        exit(0);
    }

    // init struct dirino
    dirInfo val;
    val.dirpath = dirname;
    val.statbuf = statbuf;

    // pthread_create(&tid[i],NULL,dirStat,(void*)&val);
    // i++;
    // pthread_join(tid[0],NULL);
    //
    // for (int j = 1 ; j < i; j++){
    //     pthread_join(tid[j],NULL);
    // }
    pthread_create(&t,NULL,dirStat,(void*)&val);
    i++;
    pthread_join(t,NULL);
    // Print Result
    printf("The total size of the files in %s is : %d bytes\n",dirname,totalsum);
    return 0;
}
