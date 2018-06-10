#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char* argv[]){
	int fd;
	fd = open(argv[1],O_WRONLY);
	
	if(fd==-1){
		printf("cannot open %s ",argv[1]);
		perror("open:");
	}
	else {
		write(fd,strcat(argv[2],"\n"),strlen(argv[2])+2);
	}
	
	return 0;
}
