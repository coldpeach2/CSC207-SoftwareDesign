#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 256

#define SUCCESS "Password verified\n"
#define INVALID "Invalid password\n"
#define NO_USER "No such user\n"

int main(void) {
  char user_id[MAXLINE];
  char password[MAXLINE];
  int status;
  /* The user will type in a user name on one line followed by a password 
     on the next.
     DO NOT add any prompts.  The only output of this program will be one 
   of the messages defined above.

   Your task is to complete checkpasswd.c, which reads a user id and password from stdin, 
   creates a new process to run the validate program, sends it the user id and password, 
   and prints a message to stdout reporting whether the validation is successful.

The user will type in a user name on one line followed by a password 
     on the next.*/

  if(fgets(user_id, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }
  if(fgets(password, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }
  int fd[2];
   //Create the pipe 
  if ((pipe(fd)) == -1) {
    perror("pipe");
    exit(1);
  }

  // call fork
  int r = fork();
  if (r < 0) {
    perror("fork");
    exit(1);   
  } 
  // now focus on the child
else if (r == 0) {
  //redirect stdoutput using dup2
  //Reset stdin so it reads from the pipe? 
  //Here we connect stdin to the read end of the pipe, 
  //so that when the process reads from stdin the data comes from the pipe
    close(fd[1]);
    if ((dup2(fd[0], fileno(stdin))) == -1) {
      perror("dup2");
      exit(1);
    perror("read to pipe");
  }

  close(fd[0]); // closes read file discriptor 
    execl("./validate", "validate", NULL);
}
  else{
    close(fd[0]); // closes read file discriptor
    write(fd[1], user_id, 10);
    write(fd[1], password, 10);
    close(fd[1]);
    waitpid(r, &status, 0);
    if(WIFEXITED(status)){
      int value = WEXITSTATUS(status);
      if(value == 3){
            printf("%s", NO_USER);  

      }
      else if(value == 2){
            printf("%s", INVALID);  

      }
      else if(value == 0){
            printf("%s", SUCCESS);  

      }
    }
        
    }
    
return 0;

}