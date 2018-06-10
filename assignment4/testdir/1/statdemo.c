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

int makeDir(char *dir){
	mode_t   perms;
	perms   = 0740;
	if ( (mkdir(dir, perms)) == 0 ) {
    //  printf( "New directory created successfully.\n");
		 return 0;
  }
  else  {
    //  printf("Error in directory creation\n ");
		 return -1;
  }
}


int dirStat(char *dirpath, struct stat statbuf, int totalsum){

	// init
	DIR *dp;
	struct dirent *direntry;
	char *current_dir, *sub_dirpath;
	// struct stat sub_statbuf;
	current_dir = (char *) malloc(NAMESIZE * sizeof(char));
	sub_dirpath = (char *) malloc(NAMESIZE * sizeof(char));
    int partialsum = totalsum;




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
				// DEBUG
				printf("Dir file: %s\n", direntry->d_name);
				printf("current_dir: %s\n", current_dir);

				// Form absolute path:
				strcpy(sub_dirpath,current_dir);
				strcat(sub_dirpath,"/");
				strcat(sub_dirpath,direntry->d_name);

				partialsum += dirStat(sub_dirpath, statbuf, partialsum);
				// Changing the Directory back after goes into recursive call
				chdir(dirpath);

			}
		}
		else{
			// DEBUG
            printf("The size of file %s is :%d bytes\n",direntry->d_name,(int) statbuf.st_size);
    	    partialsum += (int) statbuf.st_size;
		}
	}

	return partialsum;
}



int main(int argc, char *argv[])
{
    /* Declarations */
    char *dirname;
    struct stat statbuf;
    DIR *dp;
    struct dirent *direntry;
    dirname = (char*)malloc(NAMESIZE*sizeof(char));
    int totalsum = 0;
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

    totalsum = dirStat(dirname, statbuf, totalsum);
    printf("The total size of the files in %s is : %d bytes\n",dirname,totalsum);
}
