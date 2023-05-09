#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define MSGSIZE 60

//Benjamin Griepp

void parent(int readPipe[], int writePipe[]);
void child(int readPipe[], int writePipe[]);

void parent(int readPipe[], int writePipe[]) {
    //set seed
    srand(time(0));
    int chosen_number = (rand() % 32) + 1;
    char message[MSGSIZE];

    //send initial message
    snprintf(message, MSGSIZE, "welcome to the number guessing game U・ᴥ・U\n");
    write(writePipe[1], message, MSGSIZE);

    //wait for response
    while (1) {
        read(readPipe[0], message, MSGSIZE);
        int guess = atoi(message);

        //check guess
        if (guess == chosen_number) {
            snprintf(message, MSGSIZE, "bingo");
        } else if (guess < chosen_number) {
            snprintf(message, MSGSIZE, "too low");
        } else {
            snprintf(message, MSGSIZE, "too high");
        }
        write(writePipe[1], message, MSGSIZE);

        if (guess == chosen_number) {
            //wait for child to exit
            wait(NULL);
            exit(0);
        }
    }
}

void child(int readPipe[], int writePipe[]) {
    char message[MSGSIZE];
    int guess;

    //print initial message
    read(readPipe[0], message, MSGSIZE);
    printf("%s", message);

    while (1) {
        printf("Enter your guess: ");
        scanf("%d", &guess);

        //send guess
        snprintf(message, MSGSIZE, "%d", guess);
        write(writePipe[1], message, MSGSIZE);

        //print response
        read(readPipe[0], message, MSGSIZE);
        printf("%s\n", message);

        if (strcmp(message, "bingo") == 0) {
            exit(0);
        }
    }
}

int main() {
    // create pipes
    int sendResponse[2], sendGuess[2];

    if (pipe(sendResponse) == -1 || pipe(sendGuess) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) { 
        // child process
        close(sendResponse[1]);
        close(sendGuess[0]);
        child(sendResponse, sendGuess);
    } else { 
        // parent process
        close(sendResponse[0]);
        close(sendGuess[1]);
        parent(sendGuess, sendResponse);
    }

    return 0;
}