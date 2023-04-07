#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


int main(int argc, char *argv[]) {

    /*
      TODO:
      Use fork() and pipe(), but NOT popen().

      Create a pipe through which a child process will send two integers to 
      the parent. The parent will add the integers and print the result to 
      the terminal. 

      OPTIONAL:
      Make a second child, which will do the addition. In this case, the parent
      waits for the children to terminate. (See Lab 6.)
    */
    int pid;
    int fd[2];

    int x, y, sum;
    
    if (pipe(fd) < 0) {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        close(fd[0]);

        int x = 42;
        int y = 69;

        write(fd[1], &x, sizeof(x));
        write(fd[1], &y, sizeof(y));

        close(fd[1]);

        pid_t pid2 = fork();
        if(pid2 == 0){
          read(fd[0], &x, sizeof(x));
          read(fd[0], &y, sizeof(y));

          sum = x + y;

          printf("Result sum: %d\n", sum);

          close(fd[0]);
        }
        return 0;
    }
    else {
        wait(NULL);
        close(fd[1]);
        close(fd[0]);
        return 0;
    }

    return 0;
}
