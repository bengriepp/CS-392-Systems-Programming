#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <errno.h>

#include "minishell.h"

//Benjamin Griepp
//I pledge my honor I have abided by the Stevens Honor System

#define MAXLINE 1000

//handle SIGINT (Ctrl+C)
void sigint_handler(int sig) {
    printf("\nCannot terminate the shell with Ctrl+C. Use 'exit' command.\n");
}

//handle SIGCHLD
void sigchld_handler(int sig) {
    pid_t pid;
    int status;
    //reap any child processes while maintaining the value of errno
    int saved_errno = errno;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        //print when process terminates
        if (WIFEXITED(status)) {
            printf("pid %d done\n", pid);
        }
    }
    errno = saved_errno;
}

int main() {
    if (getpgrp() == getpid()) {
        // set process group ID only if not already a member of a process group (allows minishell to run inside itself)
        if (setpgid(0, 0) == -1) {
            perror("Error: setpgid() failed");
            exit(1);
        }
    }

    //set default path environment
    char *default_path = "/bin:/usr/bin:/usr/local/bin";
    setenv("PATH", default_path, 1);

    char cmdline[MAXLINE];

    //install signal handlers, check for errors
    if (signal(SIGINT, sigint_handler) == SIG_ERR || signal(SIGCHLD, sigchld_handler) == SIG_ERR) {
        fprintf(stderr, "Error: Cannot register signal handler. %s.\n", strerror(errno));
        exit(1);
    }

    while (1) {
        //update prompt with current user and working directory
        struct passwd *pwd = getpwuid(getuid());
        char cwd[MAXLINE];

        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            fprintf(stderr, "Error: Cannot get current working directory. %s.\n", strerror(errno));
            continue;
        }
        
        if (!pwd) {
            fprintf(stderr, "Error: Cannot get user information. %s.\n", strerror(errno));
            continue;
        }

        printf("minishell:%s:%s$ ", pwd->pw_name, cwd);

        if (!fgets(cmdline, MAXLINE, stdin)) {
            fprintf(stderr, "Error: Failed to read from stdin. %s.\n", strerror(errno));
            continue;
        }

        cmdline[strlen(cmdline) - 1] = '\0';
        // Remove newline character

        char *token = strtok(cmdline, " ");
        //tokenize cmdline string

        char *argv[MAXLINE];
        int argc = 0;

        while (token) {
            //add tokens to argv
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }

        //reset last arg to null (prevents parameters from previous commands carrying to new commands)
        argv[argc] = NULL;

        if (argc == 0) continue;

        if (strcmp(argv[0], "exit") == 0) {
            //built in exit handle, breaks out of loop to kill processes

            if (argc > 1) {
                fprintf(stderr, "Error: Too many arguments to exit.\n");
            } else {
                break;
            }
        } else if (strcmp(argv[0], "cd") == 0) {
            //built in cd handle

            if (argc == 1 || strcmp(argv[1], "~") == 0) {
            //check if lone cd or cd ~
                struct passwd *pwd = getpwuid(getuid());
                if (pwd) {
                    chdir(pwd->pw_dir);
                } else {
                    fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
                }
            } else if (argc > 2) {
                fprintf(stderr, "Error: Too many arguments to cd.\n");
            } else {
                if (chdir(argv[1]) == -1) {
                    fprintf(stderr, "Error: Cannot change directory to %s. %s.\n", argv[1], strerror(errno));
                }
            }
        } else {
            int background = 0;
            //check if background process
            if (strcmp(argv[argc - 1], "&") == 0) {
                background = 1;
                argv[--argc] = NULL;
            }

            pid_t pid = fork();

            if (pid < 0) {
                fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
                continue;
            }

            if (pid == 0) {
                //child process for executing
                if (execvp(argv[0], argv) == -1) {
                    fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
                    exit(1);
                }
            } else {
                //print background process
                if (background) {
                    printf("pid: %d cmd: %s\n", pid, argv[0]);
                } else{ 
                    int status;
                    if (waitpid(pid, &status, 0) < 0) {
                        fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
                    }
                }
            }
        }
    }
    //kill all processes in process group
    killpg(getpid(), SIGTERM);

    return 0;
}