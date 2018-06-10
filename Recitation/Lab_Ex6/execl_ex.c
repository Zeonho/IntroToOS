/* Program 3.4 Text Book Page no 79 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(void)
{
	pid_t childpid;
        childpid = fork();
        if(childpid==-1){
                perror("Failed to fork");
                return 1;
        }
        //child code
        if(childpid == 0){
								// execl("/bin/ps", "ps", "-af",NULL);
								execlp( "ps", "-a”,”f",NULL);
                perror("child failed to exec all_ids");
                return 1;
        }
        if(childpid != wait(NULL)){
                perror("parent failed to wait due to signal or error");
                return 1;

        }


}
