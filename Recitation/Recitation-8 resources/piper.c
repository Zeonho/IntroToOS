#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define MAX_INPUT_LINE 1024
#define MAX_CMDS_NUM   8
#define MAX_CMD_LENGTH 256
#define MAX_INPUT_LINE_LENGTH 2048
FILE *logfp;
int fildes[2],pipe2[2];



int num_cmds = 0;
char *cmds[MAX_CMDS_NUM];
int cmd_pids[MAX_CMDS_NUM];
int cmd_status[MAX_CMDS_NUM];

int extract_commands(char line[MAX_INPUT_LINE], char* cmds[MAX_CMDS_NUM]){

int  num = 0;
    char delims[] = "|";
    char *result = NULL;
    result = strtok( line, delims );

    printf("Extracting commands from the pipeline...\n");
    while( result != NULL ) {
        cmds[num] = result;
        printf("Extracted : \"%s\"\n", result );
        result = strtok( NULL, delims );
        num++;
    }

   return num;
}

void parse_command(char input[MAX_CMD_LENGTH],
                   char command[MAX_CMD_LENGTH],
                   char *argvector[MAX_CMD_LENGTH]){
  int i = 0;
  char* delim = " \t\n\0";
  argvector[i] = strtok(input, delim);
  strcpy(command, argvector[i]);
  i++;
  while ((argvector[i] = strtok(NULL, delim)) != NULL){
    i++;
  }
}

void fork_child_process_to_execute_command (char cmds[MAX_CMD_LENGTH],   // Command line to be processed
                     int cmd_pids[MAX_CMDS_NUM],          // PIDs of previous pipe processes
                                                          // Insert PID of new command processs
		             int i)                               // commmand line number being processed
{
    int statid,  j=0;
    int valexit,  valsignal, valstop, valcont;

    char just_cmd[MAX_CMD_LENGTH];
    char *argvec[MAX_CMD_LENGTH];

    parse_command(cmds, just_cmd, argvec);  // put command in just_cmd  and all arguments in argvec

    if(cmd_pids[i]=fork()){ /*  Parent process */

	/* ..Close the unwanted pipe descriptors.. */
	close(fildes[0]);
	close(fildes[1]);
    }
    else { /* Newly created child process */

	if(fildes[0] != -1){
	    /* .. indicates that standard input comes from a file .. */
	    dup2(fildes[0],0);
        }
	if(pipe2[1] != -1){
	    /* ..indicates that standard output redirect to a pipe.. */
	    dup2(pipe2[1],STDOUT_FILENO);
	}

	close(fildes[0]);
	close(fildes[1]);
	close(pipe2[0]);
	close(pipe2[1]);

	if(execvp(just_cmd,argvec)){
            exit(1);
        }
    }

    fildes[0] = pipe2[0];
    fildes[1] = pipe2[1];

    pipe2[0]=-1;
    pipe2[1]=-1;
}

void wait_for_pipeline_termination () {
   int finishCount = 0;
   int  signalInterrupt = 0;  // keep track of any iinterrupt due to signal
   int fatalError = 0;
   int statid = 0 ;
   int status = 0 ;
   int i;

     while (finishCount < num_cmds ) {
      //printf("\nwaiting for pipeline termination...");
   	  if((statid=wait(&status))==-1){
            perror("Wait terminated:");
            signalInterrupt = 1;
            break;
	  }

      if (signalInterrupt) {
         continue;
      }
      for (i=0; i<num_cmds; i++) {
         if (cmd_pids[i]==statid)  {
             cmd_status[i] = status;
             finishCount++;   // Only count those processes that belong to the pipeline
          }
      }
      if ( WEXITSTATUS(status) != 0 ) {
           fatalError = 1;
           printf("Terminating pipeline becuase process %d failedto execute\n", statid);
           break;
      }
     }

     if ( fatalError ) {
       for (i=0; i<num_cmds; i++) {
         printf("Terminating process %d \n", cmd_pids[i] );
         kill(cmd_pids[i], 9);
       }
     }
}

int main(int ac, char *av[]){

     int i,  pipcount;

     /* Set up signal handler for CNTRL-C to kill only the pipeline processes  */
     signal(SIGINT, SIG_DFL );
     pipcount = 0;

     //initializations
     fildes[0]=-1;
     fildes[1]=-1;
     pipe2[0] = -1;
     pipe2[1] = -1;

     char pipeCommand[] = "find /Users/jinhaochen -name *.c | wc";

    //  char pipeCommand[MAX_INPUT_LINE_LENGTH];
     //
    //  fflush(stdout);
    //  printf("Give a list of pipe commands: ");
    //  gets(pipeCommand);
    //  char* terminator = "quit";
    //  printf("You entered : list of pipe commands  %s\n", pipeCommand);
    //  if ( strcmp(pipeCommand, terminator) == 0  ) {
    //     fflush(logfp);
    //     fclose(logfp);
    //     printf("Goodbye!\n");
    //     exit(0);
    //  }


     printf("Pipe commands : %s\n", pipeCommand);

     /*  num_cmds indicates the number of commands in the line */
     num_cmds = extract_commands( pipeCommand, cmds);
     printf("Number of commands in the line : %d\n", num_cmds );

     printf("cmds: %s\n", *cmds);

     for(i=0;i<num_cmds;i++){
         printf("\nCommand %d info: %s", i,  cmds[i]);
         if(pipcount < num_cmds-1) { //we do not need pipe for the last command. So, num_cmds-1
	          pipe(pipe2);
            pipcount++;
         }
         fork_child_process_to_execute_command (cmds[i], cmd_pids, i);
     }

    printf("\nFinished creating pipeline. Result of pipe commands : \n");

    wait_for_pipeline_termination();

}
