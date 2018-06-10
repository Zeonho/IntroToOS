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

// fileCopy
#include <fcntl.h>

// time
#include <sys/time.h>
#include <time.h>




void largestFiles(char *dirpath, struct dirent *direntry, struct stat statbuf, char fileArray[][NAMESIZE]){
	if ( atoi(fileArray[2]) == -1 ){
		sprintf(fileArray[0],"%s",dirpath);
		sprintf(fileArray[1],"%s",direntry->d_name);
		sprintf(fileArray[2],"%d",(int) statbuf.st_size);
	} else if ( atoi(fileArray[5]) == -1 ){
		sprintf(fileArray[3],"%s",dirpath);
		sprintf(fileArray[4],"%s",direntry->d_name);
		sprintf(fileArray[5],"%d",(int) statbuf.st_size);
	} else if ( atoi(fileArray[8]) == -1 ){
		sprintf(fileArray[6],"%s",dirpath);
		sprintf(fileArray[7],"%s",direntry->d_name);
		sprintf(fileArray[8],"%d",(int) statbuf.st_size);
	} else if ( (int) statbuf.st_size > atoi(fileArray[2]) ){
		sprintf(fileArray[0],"%s",dirpath);
		sprintf(fileArray[1],"%s",direntry->d_name);
		sprintf(fileArray[2],"%d",(int) statbuf.st_size);
	} else if ( (int) statbuf.st_size > atoi(fileArray[5]) ){
		sprintf(fileArray[3],"%s",dirpath);
		sprintf(fileArray[4],"%s",direntry->d_name);
		sprintf(fileArray[5],"%d",(int) statbuf.st_size);
	} else if ( (int) statbuf.st_size > atoi(fileArray[8]) ){
		sprintf(fileArray[6],"%s",dirpath);
		sprintf(fileArray[7],"%s",direntry->d_name);
		sprintf(fileArray[8],"%d",(int) statbuf.st_size);
	}
}

int zeroFiles(char *dirpath, struct dirent *direntry, struct stat statbuf, char fileArray[][NAMESIZE]) {
	if (statbuf.st_size == 0){
		for (int i = 0; TOKENSIZE; i+=2){
			if (strcmp(fileArray[i],"")==0){
				sprintf(fileArray[i],"%s",dirpath);
				sprintf(fileArray[i+1],"%s",direntry->d_name);
				return 1;
			}
		}
	}
	return 0;
}


int permFiles(char *dirpath, struct dirent *direntry, struct stat statbuf, char fileArray[][NAMESIZE], int octal){
	// DEBUG
	// printf("%o\n", statbuf.st_mode & 0777);
	if ( (statbuf.st_mode & 0777) == octal){
	printf("path:%s;  file name: %s;  ",dirpath, direntry->d_name);
	printf("st_mode: %o\n", statbuf.st_mode);
	return 1;
	}
	return 0;
}


int fileCopy(char *source, char *destination){
	// Modify from filecopy.c
  char  buffer[NAMESIZE];
  ssize_t  count;
  mode_t   perms;
  int		 fdin,  fdout;
  perms   = 0740;
  if  ( (fdin = open ( source,  O_RDONLY))  == -1) {
      perror ( "Error in opening the input file:");
    	exit(2);
	}
  /*    printf( "Input file opened with descriptor %d \n", fdin);  */

  if  ( (fdout = open (destination, (O_WRONLY | O_CREAT | O_TRUNC),  perms)) == -1 ) {
      perror ( "Error in creating the output file:");
      exit(2);
  }
  /*    printf( "Output file opened with descriptor %d \n", fdout);  */

  while ( (count=read(fdin, buffer, NAMESIZE)) > 0 ) {
        if ( write (fdout, buffer, count) != count )
           perror ("Error in writing" );
  }

  if ( count == -1 ) {
     perror ( "Error while reading the input file: ");
  }

  close(fdin);
  close(fdout);


  return 0;
}

int makeLink(char *source, char *dest){
    if ( symlink( source, dest ) == -1) {
       perror("Failed to create a symbolic link");
       exit(2);
    }
		return 0;
}



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


int filesBackUp(char *dirpath, struct dirent *direntry, char *back_dirpath, char linkArray[][NAMESIZE] ){
	// init
	char *back_filepath, *filepath, *link_filepath;
	back_filepath = (char *) malloc(NAMESIZE * sizeof(char));
	filepath = (char *) malloc(NAMESIZE * sizeof(char));
	link_filepath = (char *) malloc(NAMESIZE * sizeof(char));
	int retval;
	char info[1024];
	struct stat statbuf;

	// form backup file path
	strcpy(back_filepath,back_dirpath);
	strcat(back_filepath,"/");
	strcat(back_filepath,direntry->d_name);
	// printf("form backup file path: %s\n", back_filepath);
	// form file path
	strcpy(filepath,dirpath);
	strcat(filepath,"/");
	strcat(filepath,direntry->d_name);
	// printf("form file path: %s\n", filepath);

	lstat(filepath, &statbuf);
	if (S_ISLNK(statbuf.st_mode)) {
		// printf("Link: %s\n", direntry->d_name);

		/*  readlink will obtain and store the link information in linkinfo array  */
		if ( (retval = readlink(filepath, info, 1024)) == -1 ) {
		   perror("Error reading symbolic information");
		   exit(3);
		};
		info[retval] = '\0';
		// printf("Link Information:  %s\n", info );

		strcpy(link_filepath,back_dirpath);
		strcat(link_filepath,"/");
		strcat(link_filepath, info);


		//MAKING LINK AFTER ALL THE COPY IS DONE!!!!!!!!!!!!!!!
		// printf("makeLink (back_filepath: %s back_dirpatlh: %s) \n", link_filepath, back_filepath );

		for (int i = 0; i < TOKENSIZE; i+=2){
			if (strcmp(linkArray[i],"") == 0){
				sprintf(linkArray[i],"%s",link_filepath);
				sprintf(linkArray[i+1],"%s",back_filepath);
				return 1;
			}
		}

		// makeLink(info, back_dirpath);
	}else{
		fileCopy(filepath, back_filepath);
	}


	return 0;

}

int filesDelete(char *dirpath, struct dirent *direntry){
	char *deleteFile;
	deleteFile = (char *) malloc(NAMESIZE * sizeof(char));
	strcpy(deleteFile,dirpath);
	strcat(deleteFile,"/");
	strcat(deleteFile,direntry->d_name);
	unlink(deleteFile);
	return 0;
}


int fixLink(char linkArray[][NAMESIZE]){
	for (int i = 0; i < TOKENSIZE; i+=2){
		if (strcmp(linkArray[i],"") != 0){
			makeLink(linkArray[i],linkArray[i+1]);
			// printf("i make link makeLink: (%s, %s) \n", linkArray[i], linkArray[i+1]);
		}
	}
	// Cleaning link list
	for (int i = 0; i < TOKENSIZE; i++){
		strcpy(linkArray[i],"");
	}

	return 0;
}



int dirStat(char *dirpath, char *back_dirpath, struct stat statbuf,char fileArray[][NAMESIZE], char linkArray[][NAMESIZE], int choice, int octal){

	// init
	DIR *dp;
	struct dirent *direntry;
	char *current_dir, *sub_dirpath, *sub_back_dirpath;
	struct stat sub_statbuf;
	current_dir = (char *) malloc(NAMESIZE * sizeof(char));
	sub_dirpath = (char *) malloc(NAMESIZE * sizeof(char));

	sub_back_dirpath = (char *) malloc(NAMESIZE * sizeof(char));



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
				// printf("Dir file: %s\n", direntry->d_name);
				// printf("current_dir: %s\n", current_dir);

				// Form absolute path:
				strcpy(sub_dirpath,current_dir);
				strcat(sub_dirpath,"/");
				strcat(sub_dirpath,direntry->d_name);

				// If program detects sub-directory, create a same sub-directory in .bak
				if (choice == 4){

					strcpy(sub_back_dirpath, back_dirpath);
					strcat(sub_back_dirpath, "/");
					strcat(sub_back_dirpath, direntry->d_name);
					// DEBUG
					// printf("direntry->d_name:%s sub_back_dirpath: %s\n", direntry->d_name, sub_back_dirpath);
					makeDir(sub_back_dirpath);
				}

				dirStat(sub_dirpath, sub_back_dirpath, sub_statbuf, fileArray, linkArray, choice, octal);
				// Changing the Directory back after goes into recursive call
				chdir(dirpath);

				// Cleaning old back_up file
				if (choice == 6)
					rmdir(sub_dirpath);
			}
		}
		else{
			// DEBUG
			// printf("The size of file %s is :%d bytes\n",direntry->d_name,(int) statbuf.st_size);
			if (choice == 1)
				largestFiles(dirpath, direntry, statbuf, fileArray);
			else if (choice == 2)
				zeroFiles(dirpath, direntry, statbuf, fileArray);
			else if (choice == 3)
				permFiles(dirpath, direntry, statbuf, fileArray, octal);
			else if (choice == 4)
				// recursively copying each file to backup dir location
				filesBackUp(dirpath, direntry, back_dirpath, linkArray);
			// Cleaning old back_up file
			else if (choice == 6)
				filesDelete(dirpath, direntry);
		}
	}

	return 0;
}


int main(int argc, char *argv[])
{
	int choice = -1;
	char *input_dir_name, *dirpath;
	struct stat statbuf;

	char *back_dirpath;
	back_dirpath = (char *) malloc(NAMESIZE * sizeof(char));

	char linkArray[TOKENSIZE][NAMESIZE];
	for (int i = 0; i < TOKENSIZE; i++){
		strcpy(linkArray[i],"");
	}

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

		char fileArray[9][NAMESIZE] = {"","","-1","","","-1","","","-1"};
		dirStat(dirpath, back_dirpath, statbuf,fileArray, linkArray, choice, 0);

		for (int i = 0; i < 9; i+=3){
			printf("Path: %s; Largest File: %s; Size: %s; \n", fileArray[i] ,fileArray[i+1], fileArray[i+2]);
		}



	}

	else if(choice == 2){
		printf("\nEXECUTING \"2. List all zero length files in a directory\"\n");
		/********************************************************/
		/**************Function to perform choice 2**************/
		/********************************************************/
		char fileArray[TOKENSIZE][NAMESIZE];
		for (int i = 0; i < TOKENSIZE; i++){
			strcpy(fileArray[i],"");
		}
		// DEBUG
		// for (int i; i < TOKENSIZE; i++){
		// 	printf("Number:%d Content: %s \n" ,i ,fileArray[i]);
		// }
		dirStat(dirpath, back_dirpath, statbuf, fileArray, linkArray, choice, 0);
		for (int i = 0; i < TOKENSIZE; i+=2){
			if (strcmp(fileArray[i],"") != 0)
				printf("Path: %s  Zero File: %s \n", fileArray[i], fileArray[i+1]);
		}

	}

	else if(choice == 3){
		printf("\nEXECUTING \"3. In a directory tree,find all the files that have permission specified by the user as 3 octal digits\"\n");
		/********************************************************/
		/**************Function to perform choice 3**************/
		/********************************************************/

		int input_octal;
		// File Array is not need in case 3
		char fileArray[1][NAMESIZE];
		fflush(NULL);
		printf("Enter 3 octal digits: ");
		scanf("%3o", &input_octal);
		// printf("You Entered: %3o\n", input_octal);
		// if (input_octal == 0777)
		// 	printf("input equals to 777\n");
		dirStat(dirpath, back_dirpath, statbuf,fileArray, linkArray, choice, input_octal);

	}

	else if(choice == 4){
		printf("\nEXECUTING \"4. Create a backup of a directory\"\n");
		/********************************************************/
		/**************Function to perform choice 4**************/
		/********************************************************/
		// File Array is not need in case 4

		// init
		char fileArray[1][NAMESIZE];

		// init back up file path for dirs and individual files
		strcpy(back_dirpath,dirpath);
		strcat(back_dirpath,".bak");
		// dealing with .bak file already exist
    if (makeDir(back_dirpath) == -1){
			char *time_dirpath;
			time_dirpath = (char *) malloc(NAMESIZE * sizeof(char));
			// modified from makedir.c and timeinfo.c
			char  buffer[NAMESIZE];
			// char* datetime;
			time_t  clocktime;
			struct tm  *timeinfo;
			time (&clocktime);
			timeinfo = localtime( &clocktime );
			strftime(buffer, 64, "%b-%d-%Y-%H-%M-%S", timeinfo);
			// printf("Current date and time is: %s\n", buffer);
			strcpy(time_dirpath,dirpath);
			strcat(time_dirpath,".bak.");
			strcat(time_dirpath,buffer);
			// printf("back_dirpath: %s, time_dirpath: %s\n",back_dirpath, time_dirpath);
			makeDir(time_dirpath);
			// copying all the file from back_up to back_up.bak with time
			dirStat(back_dirpath, time_dirpath, statbuf,fileArray, linkArray, choice, 0);
			// fixLink(linkArray);
			// Cleaning old back_up files and dir ... choice == 6 is used to delteFile
			dirStat(back_dirpath, time_dirpath, statbuf,fileArray, linkArray, 6, 0);
	}



		dirStat(dirpath, back_dirpath, statbuf,fileArray, linkArray, choice, 0);
		fixLink(linkArray);


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

	for (int i = 0; i < TOKENSIZE; i++){
		if (strcmp(linkArray[i],"") != 0)
			printf("linkArray: %s \n", linkArray[i]);
	}

	free(input_dir_name);
	free(dirpath);
	return 0;
}
