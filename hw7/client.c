#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return -1;
    }

    int sock = 0;
    struct sockaddr_in serv_addr;
    double number, sum;
    char input[256];


    //create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

    //send and receive data until user types 'exit'
    while (1) {
        printf("Enter a number to add or type 'exit' to stop: ");
        fgets(input, sizeof(input), stdin);

        if (strncmp(input, "exit", 4) == 0) {
            break;
        }

        sscanf(input, "%lf", &number);

        send(sock, &number, sizeof(number), 0);
        read(sock, &sum, sizeof(sum));
        printf("Current sum: %.2lf\n", sum);
    }

    close(sock);
    return 0;
}