#include <stdio.h>
#include <string.h>
#include<sys/stat.h>

int
main(int argc, char *argv[])
{
	char buf[256];
	if (argc != 3) {
		printf("Usage: %s <srcfile> <dstfile>\n", argv[0]);
		return 1;
	}
	struct stat st;
	// Write your code here
	if (!stat(argv[2], &st)) {
		printf("DIR Exists\n");

		if (rename(argv[2], strcat(argv[2],".bak") ) ) {
			perror("rename source file");
		}
	} else {
		perror("DIR Does Not Exists\n");
		if (rename(argv[1], argv[2])) {
			perror("rename source file");
		}

	}
	return 0;
}
