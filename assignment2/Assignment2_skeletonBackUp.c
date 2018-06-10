/***********************************************************************************************

 CSci 4061 Fall 2017
 Assignment# 2:   I/O Programming on UNIX/LINUX

 Student name: Jinhao Chen
 Student ID:   5200636
 X500 id: chen4566

 Operating system on which you tested your code: Linux, Unix, MacOS
 CSELABS machine: <machine you tested on eg: xyz.cselabs.umn.edu>

 GROUP INSTRUCTION:  Please make only ONLY one  submission when working in a group.

***********************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#define NAMESIZE 256
#define TOKENSIZE 100



void largestFiles(struct dirent *direntry, struct stat statbuf, char fileArray[][NAMESIZE]){
	if ( atoi(fileArray[1]) == -1 ){
		sprintf(fileArray[0],"%s",direntry->d_name);
		sprintf(fileArray[1],"%d",(int) statbuf.st_size);
	} else if ( atoi(fileArray[3]) == -1 ){
		sprintf(fileArray[2],"%s",direntry->d_name);
		sprintf(fileArray[3],"%d",(int) statbuf.st_size);
	} else if ( atoi(fileArray[5]) == -1 ){
		sprintf(fileArray[4],"%s",direntry->d_name);
		sprintf(fileArray[5],"%d",(int) statbuf.st_size);
	} else if ( (int) statbuf.st_size > atoi(fileArray[1]) ){
		sprintf(fileArray[0],"%s",direntry->d_name);
		sprintf(fileArray[1],"%d",(int) statbuf.st_size);
	} else if ( (int) statbuf.st_size > atoi(fileArray[3]) ){
		sprintf(fileArray[2],"%s",direntry->d_name);
		sprintf(fileArray[3],"%d",(int) statbuf.st_size);
	} else if ( (int) statbuf.st_size > atoi(fileArray[5]) ){
		sprintf(fileArray[4],"%s",direntry->d_name);
		sprintf(fileArray[5],"%d",(int) statbuf.st_size);
	}
}

int zeroFiles(char *dirpath, struct dirent *direntry, struct stat statbuf, char fileArray[][NAMESIZE]) {
	if (statbuf.st_size == 0){
		for (int i = 0; TOKENSIZE; i+=2){
			if (strcmp(fileArray[i],"")==0){
				sprintf(fileArray[i+1],"%s",direntry->d_name);
				sprintf(fileArray[i],"%s",dirpath);
				return 1;
			}
		}
	}
	return 0;
}


int dirStat(char *dirpath, struct stat statbuf,char fileArray[][NAMESIZE],int choice){
	DIR *dp;
	struct dirent *direntry;
	int totalsum = 0;
	char *current_dir, *sub_dirpath;
	struct stat sub_statbuf;
	current_dir = (char *) malloc(NAMESIZE * sizeof(char));
	sub_dirpath = (char *) malloc(NAMESIZE * sizeof(char));

	if((dp=opendir(dirpath))==NULL){
			perror("Error while opening the directory");
			exit(0);
	}

	/* Loop through the directory structure */
	chdir(dirpath); //previously missing
	getcwd(current_dir, NAMESIZE*sizeof(char));
	// DEBUG
	// printf("Current Directory is: %s\n", current_dir);
	// printf ("Current Directory is : %s \n",dirpath);
	while( (direntry = readdir(dp)) != NULL )
	{
		stat(direntry->d_name,&statbuf);
		if (S_ISDIR(statbuf.st_mode))
		{
			if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0 ){
				// DEBUG
				// printf("Dir file: %s\n", direntry->d_name);
				// printf("current_dir: %s\n", current_dir);
				strcpy(sub_dirpath,current_dir);
				strcat(sub_dirpath,"/");
				strcat(sub_dirpath,direntry->d_name);
				dirStat(sub_dirpath,sub_statbuf, fileArray, choice);
				chdir(dirpath);
			}
		}
		else{
			// printf("The size of file %s is :%d bytes\n",direntry->d_name,(int) statbuf.st_size);
			printf("%s\n", );
			totalsum += (int) statbuf.st_size;
			if (choice == 1)
				largestFiles(direntry, statbuf, fileArray);
			else if (choice == 2)
				zeroFiles(dirpath, direntry, statbuf, fileArray);

		}
	}
	// printf("The total size of the files in %s is : %d bytes\n",dirpath,totalsum);

	return 0;
}


int main(int argc, char *argv[])
{
	int choice = -1;
	char *input_dir_name, *dirpath, *chptr;
	struct stat statbuf;

	input_dir_name = (char *) malloc(NAMESIZE * sizeof(char));
	dirpath = (char *) malloc(NAMESIZE * sizeof(char));
	printf("SELECT THE FUNCTION YOU WANT TO EXECUTE:\n");
	printf("1. Find the 3 largest files in a directory\n");
	printf("2. List all zero length files in a directory\n");
	printf("3. Find all files with permission 777 in a directory\n");
	printf("4. Create a backup of a directory\n");
	printf("5. Exit\n");
	printf("\n");
	printf("ENTER YOUR CHOICE: ");
	scanf("%d", &choice);
	printf("Enter a directory name in the current directory: ");
	scanf("%s", input_dir_name);
	/**********************************************************/
	/*Form a full path to the directory and check if it exists*/
	/**********************************************************/
	// DEBUG
	// printf("First Charater: %c\n", input_dir_name[0]);


	if (input_dir_name[0] == '/'){
		printf ("DIR:%s is a absolute path beacause it contains /  in the first char\n", input_dir_name);
		strcat(dirpath,input_dir_name);
		printf("This is the FULL PATH: %s\n", dirpath);
	}
	else{
		printf ("DIR:%s is a relative path beacause it does not contain / in the first char\n", input_dir_name);
		dirpath = getcwd(dirpath,NAMESIZE);
    strcat(dirpath,"/");
    strcat(dirpath,input_dir_name);
		printf("-----------------------------------------------------------------------------------\n");
		printf("This is the FULL PATH: %s\n", dirpath);
	}

	/* Check if the directory entered exists or not*/
	stat(dirpath,&statbuf);
	if(!(S_ISDIR(statbuf.st_mode))){
			printf("The directory path is not valid. Directory does not exist\n");
			exit(0);
	} else
			printf("The directory path is valid. Now proceeding to next step...\n");



	if(choice == 1){
		printf("\nEXECUTING \"1. Find the 3 largest files in a directory\"\n");
		/********************************************************/
		/**************Function to perform choice 1**************/
		/********************************************************/

		char fileArray[6][NAMESIZE] = {"","-1","","-1","","-1"};
		dirStat(dirpath, statbuf,fileArray, choice);

		for (int i; i < 6; i+=2){
			printf("Largest File: %s Size: %s \n", fileArray[i],fileArray[i+1]);
		}



	}

	else if(choice == 2){
		printf("\nEXECUTING \"2. List all zero length files in a directory\"\n");
		/********************************************************/
		/**************Function to perform choice 2**************/
		/********************************************************/
		char fileArray[TOKENSIZE][NAMESIZE];
		for (int i; i < TOKENSIZE; i++){
			strcpy(fileArray[i],"");
		}
		// DEBUG
		// for (int i; i < TOKENSIZE; i++){
		// 	printf("Number:%d Content: %s \n" ,i ,fileArray[i]);
		// }
		dirStat(dirpath, statbuf,fileArray, choice);
		for (int i; i < TOKENSIZE; i+=2){
			if (strcmp(fileArray[i],"") != 0)
				printf("Path: %s  Zero File: %s \n", fileArray[i], fileArray[i+1]);
		}

	}

	else if(choice == 3){
		printf("\nEXECUTING \"3. In a directory tree,find all the files that have permission specified by the user as 3 octal digits\"\n");
		/********************************************************/
		/**************Function to perform choice 3**************/
		/********************************************************/

		char input_octal[3];

		printf("Enter 3 octal digits: ");
		scanf("%3s", input_octal);
		// printf("You Entered: %s size:%d \n", input_octal, (int)sizeof(input_octal));

	}

	else if(choice == 4){
		printf("\nEXECUTING \"4. Create a backup of a directory\"\n");
		/********************************************************/
		/**************Function to perform choice 4**************/
		/********************************************************/
	}

	else if(choice == 5){
		printf("\nEXIT\n");
		/********************************************************/
		/**************Function to perform choice 5**************/
		/********************************************************/
	}

	else{
		printf("Invalid choice\n");
		exit(1);
	}
	free(input_dir_name);
	free(dirpath);
	return 0;
}
