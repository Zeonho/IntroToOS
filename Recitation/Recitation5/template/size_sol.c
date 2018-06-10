#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include <dirent.h>


#define NAMESIZE 256

int main(int argc, char *argv[])
{

	if (argc != 2) {
		printf("Usage: %s <directory_name>\n", argv[0]);
		exit(1);
	}

	char *dirname;
	DIR *dp;
	struct stat st;
	struct dirent *direntry;
	int totalsum = 0;

	if((dp=opendir(argv[1]))==NULL){
		perror("Error while opening the directory");
		exit(0);
    	}

 	/* Write your code here*/

				// struct stat st;
				int size;
				stat(argv[1], &st);
				size = st.st_size;
				printf("%d\n", size);
				totalsum = size
        closedir (dp);

        printf("The total size of the files is : %d bytes\n",totalsum);

}
