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
/*****************************/

/* Your code here*/

/*****************************/
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
  
     char pipeCommand[] = "ls -l | wc";
     printf("Pipe commands : %s\n", pipeCommand);

     /*  num_cmds indicates the number of commands in the line */
     num_cmds = extract_commands( pipeCommand, cmds);
     printf("Number of commands in the line : %d\n", num_cmds ); 

     for(i=0;i<num_cmds;i++){
          /*****************************/

          /* Your code here*/

          /*****************************/
     }

    printf("\nFinished creating pipeline. Result of pipe commands : \n");
    
    wait_for_pipeline_termination();

}

