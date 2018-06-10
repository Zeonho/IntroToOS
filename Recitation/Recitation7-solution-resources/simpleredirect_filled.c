#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
 
/* Program should be able to redirect one commands output to the other
 *  commands input. Eg: ls -l | sort -nr -k5
 */

int main(void) {
   pid_t childpid;
   int fd[2];
   int pipe_creation_status;
   int dup_status;
   int close_desc_status;
	
   // System call to create a pipe
   pipe_creation_status = pipe(fd);
   
   if ( pipe_creation_status == -1) {
      perror("Failed to create the pipe"); 
      return 1; 
   }
   
   // System call to create a child process
   childpid = fork();
   
   if (childpid == -1) {
      perror("Failed to fork"); 
      return 1;
   }
   
   // Check whether it is parent or child   
   if (childpid > 0) {                                  /* ls is the child */
   
	  //Duplicate the file descriptor fd[1] with STDOUT_FILENO
	  dup_status = dup2( fd[1] , STDOUT_FILENO);
	  
      if (dup_status == -1) {
         perror("Failed to redirect stdout of ls");
	  }
	  //Close the file descriptors created by pipe system call
	  
	  //Close descriptor fd[0]
	  close_desc_status = close(fd[0]);
	  if (close_desc_status == -1) {
         perror("Failed to close fd[0] descriptors on ls");
	  }
	  
	  //Close descriptor fd[1]
	  close_desc_status = close(fd[1]);
	  if (close_desc_status == -1) {
         perror("Failed to close fd[1] descriptors on ls");
	  }
	  
	  //Execute the ls command using execl system call
      execl("/bin/ls", "ls", "-l", NULL);
      perror("Failed to exec ls");
   } 
   else {										/* sort is the parent */
	  //Duplicate the file descriptor fd[0] with STDIN_FILENO
	  dup_status = dup2( fd[0] , STDIN_FILENO);
	  
      if (dup_status == -1) {
         perror("Failed to redirect stdout of ls");
	  }
	  //Close the file descriptors created by pipe system call
	  
	  //Close descriptor fd[0]
	  close_desc_status = close(fd[0]);
	  if (close_desc_status == -1) {
         perror("Failed to close fd[0] descriptors on ls");
	  }
	  
	  //Close descriptor fd[1]
	  close_desc_status = close(fd[1]);
	  if (close_desc_status == -1) {
         perror("Failed to close fd[1] descriptors on ls");
	  }
	  
	  //Execute the ls command using execl system call
      execl("/usr/bin/sort", "sort", "-nr", "-k5", NULL);
      perror("Failed to exec wc");
    }
    return 1;    
}
