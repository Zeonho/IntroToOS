/***********************************************************************************************

 CSci 4061 Fall 2017
 Assignment# 3: Piper program for executing pipe commands

 Student name: Jinhao Chen
 Student ID:   5200636

 Student name: <full name of second student>
 Student ID:   <Second student's ID>

 X500 id: chen4566, <id2 (optional)>


 Operating system on which you tested your code: Linux, MacOS
 CSELABS machine: <machine you tested on eg: xyz.cselabs.umn.edu>

 GROUP INSTRUCTION:  Please make only ONLY one  submission when working in a group.
***********************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define DEBUG

#define MAX_INPUT_LINE_LENGTH 2048 // Maximum length of the input pipeline command
                                   // such as "ls -l | sort -d +4 | cat "
#define MAX_CMDS_NUM   8           // maximum number of commands in a pipe list
                                   // In the above pipeline we have 3 commands
#define MAX_CMD_LENGTH 4096         // A command has no more than 4098



FILE *logfp;
int fildes[2],pipe2[2];

int num_cmds = 0;
char *cmds[MAX_CMDS_NUM];
int cmd_pids[MAX_CMDS_NUM];
int cmd_status[MAX_CMDS_NUM];


/*******************************************************************************/
/*   The function parse_command_line will take a string such as
     ls -l | sort -d +4 | cat | wc
     given in the char array commandLine, and it will separate out each pipe
     command and store pointer to the command strings in array "cmds"
     For example:
     cmds[0]  will pooint to string "ls -l"
     cmds[1] will point to string "sort -d +4"
     cmds[2] will point to string "cat"
     cmds[3] will point to string "wc"

     This function will write to the LOGFILE above information.
*/
/*******************************************************************************/

int parse_command_line(char line[MAX_INPUT_LINE_LENGTH], char* cmds[MAX_CMDS_NUM]){

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

/*******************************************************************************/
/*  parse_command takes command such as
    sort -d +4
    It parses a string such as above and puts command program name "sort" in
    argument array "cmd" and puts pointers to ll argument string to argvector
    It will return  argvector as follows
    command will be "sort"
    argvector[0] will be "sort"
    argvector[1] will be "-d"
    argvector[2] will be "+4"
/
/*******************************************************************************/

// void parse_command(char input[MAX_CMD_LENGTH],
//                    char command[MAX_CMD_LENGTH],
//                    char *argvector[MAX_CMD_LENGTH]){
// }
// Code From Recitation

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

/*******************************************************************************/
/*  The function print_info will print to the LOGFILE information about all    */
/*  processes  currently executing in the pipeline                             */
/*  This printing should be enabled/disabled with a DEBUG flag                 */
/*******************************************************************************/

void print_info(char* cmds[MAX_CMDS_NUM],
		int cmd_pids[MAX_CMDS_NUM],
		int cmd_stat[MAX_CMDS_NUM],
		int num_cmds) {

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

/********************************************************************************/
/*   The function waitPipelineTermination waits for all of the pipeline         */
/*   processes to terminate.                                                    */
/********************************************************************************/


void waitPipelineTermination () {
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

/********************************************************************************/
/*  This is the signal handler function. It should be called on CNTRL-C signal  */
/*  if any pipeline of processes currently exists.  It will kill all processes  */
/*  in the pipeline, and the piper program will go back to the beginning of the */
/*  control loop, asking for the next pipe command input.                       */
/********************************************************************************/

void killPipeline( int signum ) {

}

/********************************************************************************/


int main(int ac, char *av[]){

     int i,  pipcount;



     //check usage
     if (ac > 1){
       printf("\nIncorrect use of parameters\n");
       printf("USAGE: %s \n", av[0]);
       exit(1);
     }



     /* Set up signal handler for CNTRL-C to kill only the pipeline processes  */
     while (1) {

     logfp =  fopen("LOGFILE", "a");

     signal(SIGINT, SIG_DFL );
     pipcount = 0;

     //initializations
     fildes[0]=-1;
     fildes[1]=-1;
     pipe2[0] = -1;
     pipe2[1] = -1;

    //  char pipeCommand[] = "ls -l | wc | wc";

     char pipeCommand[MAX_INPUT_LINE_LENGTH];
     logfp =  fopen("LOGFILE", "a");

     fflush(stdout);
     printf("Give a list of pipe commands: ");
     gets(pipeCommand);
     char* terminator = "quit";
     printf("You entered : list of pipe commands  %s\n", pipeCommand);
     if ( strcmp(pipeCommand, terminator) == 0  ) {
        fflush(logfp);
        fclose(logfp);
        printf("Goodbye!\n");
        exit(0);
     }


     printf("Pipe commands : %s\n", pipeCommand);

     /*  num_cmds indicates the number of commands in the line */
     num_cmds = parse_command_line( pipeCommand, cmds);
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

    waitPipelineTermination();

  } //while
}// main
