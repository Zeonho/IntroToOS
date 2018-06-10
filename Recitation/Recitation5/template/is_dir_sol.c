#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s <file_name>\n", argv[0]);
		exit(1);
	}
	struct stat st;
	// Write your code here
	// DIR *dip;
	// dip = opendir(argv[1]);
	if (!stat(argv[1], &st)) {
		printf("DIR Exists\n");
		// Write your code here
	} else {
		perror("DIR Does Not Exists\n");
	}

}
