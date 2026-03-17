#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void server(int readfd, int writefd);
void client(int writefd, int readfd);

int main() 
{
    int p1[2], p2[2];
    pid_t pid;

    // Create two pipes
    if (pipe(p1) == -1 || pipe(p2) == -1)
    {
        perror("Pipe failed");
        exit(1);
    }

    pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0)
    {
        // Child process (Server)
        close(p1[1]);   // Close write end of pipe1
        close(p2[0]);   // Close read end of pipe2

        server(p1[0], p2[1]);
        exit(0);
    }
    else
    {
        // Parent process (Client)
        close(p1[0]);   // Close read end of pipe1
        close(p2[1]);   // Close write end of pipe2

        client(p1[1], p2[0]);

        wait(NULL);
    }

    return 0;
}

// Server function
void server(int readfd, int writefd)
{
    char buffer[100];

    // Read from client
    read(readfd, buffer, sizeof(buffer));
    printf("Server received: %s\n", buffer);

    // Send response
    char *msg = "Hello from server";
    write(writefd, msg, strlen(msg) + 1);
}

// Client function
void client(int writefd, int readfd)
{
    char buffer[100];

    // Send message to server
    char *msg = "Hello from client";
    write(writefd, msg, strlen(msg) + 1);

    // Read response
    read(readfd, buffer, sizeof(buffer));
    printf("Client received: %s\n", buffer);
}
