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

#include <pthread.h>

#define NAMESIZE 256
#define THREADNUMBER 20

// DEBUG SET
#define DEBUG 1
#define DEBUGDIR 0

// Globalize
int totalsum = 0;




int dirStat(char *dirpath, struct stat statbuf){

	// init
	DIR *dp;
	struct dirent *direntry;
	char *current_dir, *sub_dirpath;
	// struct stat sub_statbuf;
	current_dir = (char *) malloc(NAMESIZE * sizeof(char));
	sub_dirpath = (char *) malloc(NAMESIZE * sizeof(char));

	if((dp=opendir(dirpath))==NULL){
			perror("Error while opening the directory");
			exit(0);
	}


	/* Loop through the directory structure */
	chdir(dirpath);
	getcwd(current_dir, NAMESIZE*sizeof(char));




	// DEBUG
	// printf("Current Directory is: %s\n", current_dir);
	// printf ("Current Directory is : %s \n",dirpath);
	while( (direntry = readdir(dp)) != NULL )
	{
		stat(direntry->d_name,&statbuf);

		// If it is a directory, call dirStat on sub directory
		if (S_ISDIR(statbuf.st_mode))
		{
			//  ignore directory entries with  le names “.” and “..”.
			if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0 ){


				// Form absolute path:
				strcpy(sub_dirpath,current_dir);
				strcat(sub_dirpath,"/");
				strcat(sub_dirpath,direntry->d_name);
				dirStat(sub_dirpath, statbuf);


                // DEBUG
                if (DEBUGDIR){
				    printf("Dir file: %s\n", direntry->d_name);
				    printf("current_dir: %s\n", current_dir);

                }

				// Changing the Directory back after goes into recursive call
				chdir(dirpath);

			}
		}
		else{
			// DEBUG
            if(DEBUG){
                printf("DEBUG The size of file %s/%s is :%d bytes\n",current_dir,direntry->d_name,(int) statbuf.st_size);
				// printf("Total sum: %d\n", totalsum );
            }
    	    totalsum += (int) statbuf.st_size;

		}
	}

	return 0;
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


    dirStat(dirname, statbuf);
    printf("The total size of the files in %s is : %d bytes\n",dirname,totalsum);
}
