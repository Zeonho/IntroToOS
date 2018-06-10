#include<stdio.h>

#define CHECK

int main(int argc,char** argv) {
#ifdef CHECK
    printf("CHECK is defined\n");
#else
	printf("CHECK is not defined\n");
#endif
}		
