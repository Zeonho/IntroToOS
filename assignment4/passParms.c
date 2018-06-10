// test if all the info pass into function



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include <pthread.h>

#define NAMESIZE 256

typedef struct dirInfo
{
    char *dirpath;
    struct stat statbuf;
	int *totalsum_ptr;
}dirInfo;

void *thread_function(void *arg)
{
    struct dirent *direntry;
    DIR *dp;

    int *total_sum_ptr = (((dirInfo*)arg)->totalsum_ptr);
    char *dirpath = ((dirInfo*)arg)->dirpath;
    struct stat statbuf = ((dirInfo*)arg)->statbuf;

    // (*total_sum_ptr)++;
    // DEBUG
    printf("Inside Thread\n");
    printf("dirpath is %s\n", dirpath);
    printf("statbuf is %d\n", (int)statbuf.st_size);
    printf("totalsum is %d\n",*total_sum_ptr);

    if((dp=opendir(dirpath))==NULL){
        perror("Error while opening the directory");
        exit(0);
    }

    chdir(dirpath); //previously missing
    while( (direntry = readdir(dp)) != NULL )
    {
	stat(direntry->d_name,&statbuf);
	if(!(S_ISDIR(statbuf.st_mode)))
	{
	    printf("The size of file %s is :%d bytes\n",direntry->d_name,(int) statbuf.st_size);
	    *total_sum_ptr += (int) statbuf.st_size;
	}
    }

    printf("The total size of the files in %s is : %d bytes\n",dirpath,*total_sum_ptr);


    return NULL;

}


int main(int argc, char *argv[])
{

    /* Declarations */
    char *dirname;
    struct stat statbuf;
    DIR *dp;
    // struct dirent *direntry;
    dirname = (char*)malloc(NAMESIZE*sizeof(char));
    int totalsum = 0;
    /* End of declarations */

    if(argc < 2)       /*If the user does not enter any directory name*/
    {
	    printf("No directory name specified. Executing the function in the current directory.\n");
    	dirname = getcwd(dirname,NAMESIZE);
    }
    else              /* If the user enters a directory name */
    {
	    dirname = getcwd(dirname,NAMESIZE);
        strcat(dirname,"/");
        strcat(dirname,argv[1]);

    }
    /* Check if the directory entered exists or not*/
    stat(dirname,&statbuf);
    if(!(S_ISDIR(statbuf.st_mode))){
        printf("The directory name is not valid. Directory does not exist\n");
        exit(0);
    }

    // PASS_PRAMS
    pthread_t tid[5];
    dirInfo val;
    int i=0;

    val.dirpath = dirname;
    val.statbuf = statbuf;
	val.totalsum_ptr = &totalsum;
    /* initialize all the items */

    // for (i = 0; i < 5; i++){
    //     printf("Creating the thread...\n");
    //     pthread_create(&tid[i],NULL,thread_function,(void*)&val);
    //
    // }
    // for (i = 0; i < 5; i++){
    //     pthread_join(tid[i],NULL);
    // }

    printf("Creating the thread...\n");
    pthread_create(&tid[i],NULL,thread_function,(void*)&val);
    pthread_join(tid[i],NULL);



    printf("Parent Thread\n");
    printf("dirpath is %s\n", dirname);
    printf("statbuf is %d\n", (int)statbuf.st_size);
    printf("totalsum is %d\n",totalsum);


    return 0;
}
