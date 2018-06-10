#include<stdio.h>
#include<stdlib.h>
#include<stdlib.h>
#include<sys/stat.h>
int
main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s <file_name>\n", argv[0]);
		exit(1);
	}
	int             size;

	/* either do this */
	FILE           *f = fopen(argv[1], "r");
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	printf("%d\n", size);

	/* or do this */
	struct stat     st;
	stat(argv[1], &st);
	size = st.st_size;
	printf("%d\n", size);
}
