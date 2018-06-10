#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define BUFSIZE 10

/* Parent should write a string to the pipe, 
 * which then is to be read by the child.
 * Program does not check for I/O errors.
 */

int main(void) {
   char bufin[BUFSIZE] = "empty";
   char bufout[] = "hello"; 
   int bytesin;
   pid_t childpid;  
   int fd[2]; 
   int pipe_creation_status;
   
   // System call to create a pipe
   pipe_creation_status = pipe(fd);
   
   if ( pipe_creation_status == -1) {
      perror("Failed to create the pipe"); 
      return 1; 
   } 
   
   bytesin = strlen(bufin);
   
   // System call to create a child process
   childpid = fork();
   
   if (childpid == -1) {
      perror("Failed to fork"); 
      return 1;
   }
   
   // Check whether it is parent or child
   if (childpid > 0)  {                                     /* parent code */
      write(fd[1], bufout, 5);
   }
   else	 {                                                 /* child code */
      bytesin = read(fd[0], bufin, BUFSIZE);
   }
   
   fprintf(stderr, "[%ld]:my bufin is {%.*s}, my bufout is {%s}\n",
           (long)getpid(), bytesin, bufin, bufout); 
   return 0;
}
