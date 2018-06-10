#include <stdio.h>

int
main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("Usage: %s <srcfile> <dstfile>\n", argv[0]);
		return 1;
	}
	if (rename(argv[1], argv[2]))
		perror("rename");
	return 0;
}
