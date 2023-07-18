/*****************************************************************************
 * server-c.c                                                                 
 * Name:	
 * NetId:	
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <fcntl.h>

#define QUEUE_LENGTH 10
#define RECV_BUFFER_SIZE 2048

int server(char* server_port)
{
    int MySocket = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[RECV_BUFFER_SIZE];
    ssize_t num;
    if (MySocket < 0)
    {
        perror("MySocket");
        return -1;
    }
    struct sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(atoi(server_port));
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(MySocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)) < 0)
    {
        perror("bind");
        close(MySocket);
        return -1;
    }
    if (listen(MySocket, QUEUE_LENGTH) < 0)
    {
        perror("listen");
        close(MySocket);
        return -1;
    }
    while (1)
    {
        struct sockaddr_in ClientAddr;
        socklen_t client_addr_len = sizeof(ClientAddr);
        int fd = accept(MySocket, (struct sockaddr*)&ClientAddr, &client_addr_len);
        if (fd < 0)
        {
            perror("accept");
            continue;
        }
        while ((num = recv(fd, buffer, RECV_BUFFER_SIZE, 0)) > 0)
        {
            fwrite(buffer, 1, num, stdout);
            fflush(stdout);
        }
        close(fd);
    }
    close(MySocket);
    return 0;
}

/*
 * main():
 * Parse command-line arguments and call server function
*/
int main(int argc, char **argv) 
{
  char *server_port;

  if (argc != 2) 
  {
    fprintf(stderr, "Usage: ./server-c [server port]\n");
    exit(EXIT_FAILURE);
  }

  server_port = argv[1];
  return server(server_port);
}
