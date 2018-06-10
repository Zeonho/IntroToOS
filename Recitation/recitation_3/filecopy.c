#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include "check.h"

int main(int argc, char **argv) {
    FILE *fpr, *fpw;
    int letter;
    if(argc != 3) {
        printf("Usage: filecopy <filename> <copy-filename> \n");
		exit (0);
	}

//open the first file for reading
    if( ( fpr = fopen( argv[1], "r")) == NULL ) {
    #ifdef CHECK
        perror("Cannot open the file\n");
    #else
        printf("No error printing enabled!\n");
    #endif
        exit(0);
    }

//open the second file for writing
    if ( (fpw=fopen(argv[2], "w"))==NULL ) {
    #ifdef CHECK
        if (errno == EISDIR)
            printf("Entered name is for a directory\n");
        perror("Cannot open the file\n");
    #else
        printf("No error printing enabled!\n");
    #endif
        exit(0);
    }

//perform file copy 
    while( ( letter = fgetc( fpr ) ) != EOF) {
        fputc(letter, fpw);
    }

//close opened files
    fclose(fpr);
    fclose(fpw);
}

